#version 330 core

#include common/globals.glsl
#include common/uniforms.glsl

precision highp float;
precision highp int;
precision highp sampler2D;
precision highp samplerCube;
precision highp isampler2D;
precision highp sampler2DArray;

layout (location = 0) in vec3 position;

out VSOUT {
     vec3 worldPos;
} vs_out;

void main()
{
    gl_PointSize = 12;
    gl_Position = camera.projection * camera.view * camera.model * vec4(position, 1.0);
    vs_out.worldPos = position;
}