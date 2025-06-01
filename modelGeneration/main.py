import json

import numpy
from shapely.geometry import LinearRing, Polygon

from core.model import Model
from core.ground import Ground
from core.door import Door
from core.window import Window
from core.walls import Walls
from util import convert_doors_to_door_objects, convert_windows_to_window_objects, is_full_overlap

import os
import argparse

def handle_args():
    dirname = os.path.dirname(__file__)
    pathname = os.path.join(dirname, '..', 'assets', 'obj')
    cwd = os.getcwd()
    pathname = os.path.relpath(pathname, cwd)
    parser = argparse.ArgumentParser(description="Generate a 3D model from a JSON file.")
    parser.add_argument("input_json", type=str, help="Path to the model JSON file.")
    parser.add_argument("--output_path", type=str, default=pathname, help="Output OBJs file path, should be a isolated folder \n (default: %(default)s)")
    args = parser.parse_args()
    [input_json, output_path] = [args.input_json, args.output_path]
    if not os.path.isfile(input_json):
        parser.print_help()
        raise FileNotFoundError(f"[ERROR]: Input JSON file '{input_json}' does not exist.")
    if not os.path.isdir(output_path):
        parser.print_help()
        raise NotADirectoryError(f"[ERROR]: Output path '{output_path}' is not a directory.")
    return input_json, output_path

if __name__ == "__main__":
    try:
        [input_json, output_path] = handle_args()
    except (FileNotFoundError, NotADirectoryError) as e:
        print(e)
        exit(1)

     
    floor_z = 0
    offset_dist = 0.5 / 2

    with open(input_json) as f:
        model_json = json.loads(f.read())

    basePolygon = model_json["basePolygon"]

    outer: list[tuple[float, float]] = [[point[0], point[1]] for point in basePolygon["outer"]]
    inter: list[list[tuple[float, float]]] = []
    if "inter" in basePolygon:
        inter = [
            [[point[0], point[1]] for point in inter] for inter in basePolygon["inter"]
        ]


    floor_level = 0
    for floor in model_json["floors"]:
        floor_level += 1
        ground_model_name = f"{floor['name']}_ground"
        wall_model_name = f"{floor['name']}_wall"
        if ground_model_name is None:
            ground_model_name = f"{floor_level}F_ground"
        if wall_model_name is None:
            wall_model_name = f"{floor_level}F_wall"

        ground_model = Model()
        wall_model = Model()
        print(f"Processing floor: {floor['name']}")
        height: float = floor["height"]
        door_in_input = floor.get("door", [])
        window_in_input = floor.get("window", [])
        doors = convert_doors_to_door_objects(door_in_input, height, floor_z)
        windows = convert_windows_to_window_objects(window_in_input, floor_z)

        if not floor["enable"]:
            continue

        ground = Ground(outer, inter, floor_z, height)
        print("ground")
        assert type(ground.base_polygon) is Polygon
        ground_model.add_ground(ground)

        walls = Walls()
        print ("ext")
        walls.wall_bottom_generation_from_ring(ground.base_polygon.exterior, offset_dist)

        print ("int")
        for interiors in ground.base_polygon.interiors:
            walls.wall_bottom_generation_from_ring(interiors, offset_dist)

        print ("room")
        for room in floor["rooms"]:
            ring = LinearRing(room)
            walls.wall_bottom_generation_from_ring(ring, offset_dist)

        walls_obj = walls.wall_generations_from_bottoms(floor_z, height)

        for wall in walls_obj:
            wall_coord = wall.get_non_offset_coord()
            offset_diff = numpy.array(wall_coord) - numpy.array(wall.get_offset_coord())
            for door in doors:
                if(not None in wall_coord and is_full_overlap(wall_coord[0], wall_coord[1], wall_coord[2], wall_coord[3], door.x1, door.y1, door.x2, door.y2)):
                    new_door = Door(door.x1 - offset_diff[0], door.y1 - offset_diff[1], door.x2 - offset_diff[2], door.y2 - offset_diff[3], height * 0.5, floor_z)
                    wall.dig_a_hole(new_door.to_3D_coordinates())
                    
            for window in windows:
                if(not None in wall_coord and is_full_overlap(wall_coord[0], wall_coord[1], wall_coord[2], wall_coord[3], window.x1, window.y1, window.x2, window.y2)):
                    new_window = Window(window.x1 - offset_diff[0], window.y1 - offset_diff[1], window.x2 - offset_diff[2], window.y2 - offset_diff[3], window.y_offset, window.height, floor_z)
                    wall.dig_a_hole(new_window.to_3D_coordinates())

            wall_model.add_wall(wall)
            
        floor_z += height + 0.1
        ground_model_name = os.path.join(output_path, f"{ground_model_name}.obj")
        ground_model.export_obj(ground_model_name)
        wall_model_name = os.path.join(output_path, f"{wall_model_name}.obj")
        wall_model.export_obj(wall_model_name)

        print(f"Exported as {ground_model_name} and {wall_model_name}")
