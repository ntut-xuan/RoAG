#version 330 core

// https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows

in vec4 FragPos; // from GS

uniform vec3 lightPos;
uniform float farPlane;

void main() {
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - lightPos);
    
    // map to [0:1] range
    lightDistance = lightDistance / farPlane;
    
    // write as depth
    gl_FragDepth = lightDistance;
}  
