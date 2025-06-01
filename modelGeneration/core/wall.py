import numpy
import triangle

from util.dimension_converter import convert_2d_to_3d, convert_3d_to_2d
from shapely import Polygon, MultiPolygon

class Wall:
    def __init__(self, x1, y1, x2, y2, z1, h):
        self.x1 = x1
        self.x2 = x2
        self.y1 = y1
        self.y2 = y2
        self.z1 = z1
        self.z2 = z1 + h
        
        self.wall_coord_3d = numpy.array([[self.x1, self.y1, self.z1], [self.x2, self.y2, self.z1], [self.x2, self.y2, self.z2], [self.x1, self.y1, self.z2]])
        
        self.origin = self.wall_coord_3d[0]
        
        u_vec = self.wall_coord_3d[1] - self.origin
        u_vec = u_vec / numpy.linalg.norm(u_vec)

        w_vec = self.wall_coord_3d[2] - self.origin

        n_vec = numpy.cross(u_vec, w_vec)
        n_vec = n_vec / numpy.linalg.norm(n_vec)

        v_vec = numpy.cross(n_vec, u_vec)
        v_vec = v_vec / numpy.linalg.norm(v_vec)

        wall_coord_2d = convert_3d_to_2d(self.origin, self.wall_coord_3d, u_vec, v_vec)
        
        self.wall_coord_2d = wall_coord_2d
        self.u_vec = u_vec
        self.v_vec = v_vec
        
        self.polygon = Polygon(shell=wall_coord_2d)
        self.holes = None
        
        self._process_vertices_and_faces()
        
    def _fix_offset(self, c):
        if c - int(c) == 0.875:
            return c + 0.125
        elif c - int(c) == -0.875:
            return c + 0.875
        elif c - int(c) == 0.125:
            return c - 0.125
        elif c - int(c) == -0.125:
            return c + 0.125
        else:
            return c
        
        
    def get_offset_coord(self):
        return [self.x1, self.y1, self.x2, self.y2]

    def get_non_offset_coord(self):
        return [self._fix_offset(self.x1), self._fix_offset(self.y1), self._fix_offset(self.x2), self._fix_offset(self.y2)]
        
    def _process_vertices_and_faces(self):
        vertices = []
        segments = []
        holes = []
        vertex_index = 0

        shell_coords = list(self.polygon.exterior.coords)[:-1]
        for i, pt in enumerate(shell_coords):
            vertices.append(pt)
            segments.append([i, (i + 1) % len(shell_coords)])
        vertex_index += len(shell_coords)

        for hole in self.polygon.interiors:
            hole_coords = list(hole.coords)[:-1]
            hole_start = vertex_index
            for i, pt in enumerate(hole_coords):
                vertices.append(pt)
                segments.append([hole_start + i, hole_start + (i + 1) % len(hole_coords)])
            holes.append(list(Polygon(hole_coords).representative_point().coords)[0])
            vertex_index += len(hole_coords)

        if len(holes) == 0:
            A = {
                'vertices': numpy.array(vertices),
                'segments': numpy.array(segments),
            }
        else:
            A = {
                'vertices': numpy.array(vertices),
                'segments': numpy.array(segments),
                'holes': numpy.array(holes),
            }
        
        t = triangle.triangulate(A, 'pqa3000.0')

        clipped_wall_3d = convert_2d_to_3d(self.origin, t["vertices"], self.u_vec, self.v_vec)

        self.vertices = clipped_wall_3d
        self.faces = t["triangles"]
        
        if 'holes' in t:
            self.holes = t["holes"]

    def dig_a_hole(self, hole_coord_3d: numpy.ndarray, debug=False):
        hole_coord_2d = convert_3d_to_2d(self.origin, hole_coord_3d, self.u_vec, self.v_vec)
        
        hole_polygon = Polygon(shell=hole_coord_2d)

        self.polygon: Polygon = self.polygon.difference(hole_polygon)
        assert type(self.polygon) is Polygon
        
        if debug:
            print(self.wall_coord_2d)
            print(hole_coord_2d)
        
        self._process_vertices_and_faces()
