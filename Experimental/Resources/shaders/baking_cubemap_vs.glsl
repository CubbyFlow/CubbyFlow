#version 330 core
layout(location = 0) in vec3 position;

#include common/globals.glsl
#include common/uniforms.glsl

precision highp float;
precision highp int;
precision highp sampler2D;
precision highp samplerCube;
precision highp isampler2D;
precision highp sampler2DArray;

out VSOUT{
	vec3 localPos;
} vs_out;

void main()
{
	vs_out.localPos = position;
	gl_Position = camera.projection * camera.view * vec4(vs_out.localPos, 1.0);
}