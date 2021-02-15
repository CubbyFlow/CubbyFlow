#version 330 core

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
	vec3 envColor = texture(cubeMap, fs_in.localPos).rgb;

	envColor = envColor / (envColor + vec3(1.0));
	envColor = pow(envColor, vec3(1.0 / 2.2));

	fragColor = vec4(envColor, 1.0);
}