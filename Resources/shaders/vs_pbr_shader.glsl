#version 330 core
in vec3 position;
in vec3 normal;

uniform mat4 ViewProjection;

out VSOUT {
     vec3 normal;
     vec3 worldPos;
} vs_out;

void main() 
{
    vs_out.normal = normal;
    vs_out.worldPos = position;
    gl_Position =  ViewProjection * vec4(position, 1.0);
}