#version 330 core

// https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows

layout (triangles) in;
// for each triangle, emit each vertex coorsponded to each face of cubemap
layout (triangle_strip, max_vertices=18) out;

uniform mat4 lightSpaceMatrix[6];

out vec4 FragPos;

void main()
{
  for(int cubeFace = 0; cubeFace < 6; cubeFace++)
  {
    gl_Layer = cubeFace; // built-in variable that specifies to which face of cubemap we will render to.
    for(int i = 0; i < 3; i++)
    {
      FragPos = gl_in[i].gl_Position;
      gl_Position = lightSpaceMatrix[cubeFace] * FragPos;
      EmitVertex();
    }    
    EndPrimitive();
  }
}  
