#version 430 core

#include common/globals.glsl
#include common/uniforms.glsl

precision highp float;
precision highp int;
precision highp sampler2D;
precision highp samplerCube;
precision highp isampler2D;
precision highp sampler2DArray;

in VSOUT{
	vec3 localPos;
} fs_in;

uniform samplerCube cubeMap;

out vec4 fragColor;

void main()
{
	fragColor = vec4(texture(cubeMap, fs_in.localPos).rgb, 1.0);
}