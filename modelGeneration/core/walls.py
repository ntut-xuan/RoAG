import numpy
import shapely
from shapely import LinearRing
from shapely.geometry import MultiPolygon, Polygon
from core.wall import Wall
from util import offset_polygon

class Walls:
    wall_objects: list[Wall] = []
    wall_bottoms: list[Polygon] = []
    def _walls_from_polygon (self, polygon: Polygon, floor_z: float, height: float):
        ring = polygon.exterior
        for i in range(len(ring.coords) - 1):
            start = ring.coords[i]
            end = ring.coords[i + 1]
            wall = Wall(start[0], start[1], end[0], end[1], floor_z, height)
            self.wall_objects.append(wall)

        for int_ring in polygon.interiors:
            for i in range(len(int_ring.coords) - 1):
                start = int_ring.coords[i]
                end = int_ring.coords[i + 1]
                wall = Wall(start[0], start[1], end[0], end[1], floor_z, height)
                self.wall_objects.append(wall)
        

    def wall_bottom_generation_from_ring (self, rings: LinearRing, wall_thickness: float):
        offset_dist = wall_thickness / 2
        outer_verticies = offset_polygon(rings.coords, offset_dist)[0]
        inner_verticies = offset_polygon(rings.coords, -offset_dist)[0]
        generated_wall_bottom = Polygon(outer_verticies, [inner_verticies])
        self.wall_bottoms.append(generated_wall_bottom)

    def wall_generations_from_bottoms (self, floor_z: float, height: float) -> list[Wall]:
        wall_bottom = shapely.set_operations.union_all(self.wall_bottoms)
        unioned_wall_bottoms: list[Polygon] = []
        if wall_bottom.geom_type == "MultiPolygon":
            assert(type(wall_bottom) is shapely.geometry.MultiPolygon)
            # if there are multiple polygons, we need to merge them
            for wall in wall_bottom.geoms:
                unioned_wall_bottoms.append(wall)
        elif wall_bottom.geom_type == "Polygon":
            assert(type(wall_bottom) is shapely.geometry.Polygon)
            unioned_wall_bottoms.append(wall_bottom)
        else:
            raise ValueError("Invalid geometry type: {}".format(wall_bottom.geom_type))

        self.wall_objects = []
        for wall_bottom in unioned_wall_bottoms:
            self._walls_from_polygon(wall_bottom, floor_z, height)
        return self.wall_objects


