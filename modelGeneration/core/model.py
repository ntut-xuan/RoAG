import numpy 
from core.wall import Wall
from core.ground import Ground
from trimesh import Trimesh
from util import generate_uvs_box_projection_with_tiling

class Model:
    def __init__(self):
        self.vertices = numpy.empty((0, 3), dtype=float)
        self.faces = numpy.empty((0, 3), dtype=int)
    
    def add_ground(self, ground: Ground):
        size = self.vertices.shape[0]
        self.vertices = numpy.vstack([self.vertices, ground.vertices])
        self.faces = numpy.vstack([self.faces, numpy.array(ground.faces) + size])
    
    def add_wall(self, wall: Wall):
        size = self.vertices.shape[0]
        self.vertices = numpy.vstack([self.vertices, wall.vertices])
        
        # Append faces and adjust the indices of faces.
        self.faces = numpy.vstack([self.faces, numpy.array(wall.faces) + size])
        
    def export_obj(self, filename: str):
        self.vertices[:, [1, 2]] = self.vertices[:, [2, 1]]
        mesh = Trimesh(vertices=self.vertices, faces=self.faces, process=True)
        mesh.visual = generate_uvs_box_projection_with_tiling(mesh, tile_scale=(1.0, 1.0))
        mesh.export(filename, include_normals=True, include_texture=True, include_color=False)
