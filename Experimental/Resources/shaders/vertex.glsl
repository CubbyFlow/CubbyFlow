#version 430 core

#include common/globals.glsl
#include common/uniforms.glsl

precision highp float;
precision highp int;
precision highp sampler2D;
precision highp samplerCube;
precision highp isampler2D;
precision highp sampler2DArray;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out VSOUT {
     vec3 normal;
     vec3 worldPos;
} vs_out;

void main()
{
    gl_Position = camera.projection * camera.view * camera.model * vec4(position, 1.0);
    vs_out.worldPos = position;
    vs_out.normal = normal;
}