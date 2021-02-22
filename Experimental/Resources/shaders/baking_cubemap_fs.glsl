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

uniform sampler2D equirectangularMap;

out vec4 fragColor;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

void main()
{
	vec2 uv = SampleSphericalMap(normalize(fs_in.localPos));
	vec3 color = texture(equirectangularMap, uv).rgb;

	fragColor = vec4(color, 1.0);
}