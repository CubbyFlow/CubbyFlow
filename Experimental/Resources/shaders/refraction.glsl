#version 430 core

#include common/globals.glsl
#include common/uniforms.glsl
#include common/brdf.glsl

precision highp float;
precision highp int;
precision highp sampler2D;
precision highp samplerCube;
precision highp isampler2D;
precision highp sampler2DArray;

in VSOUT {
    vec3 normal;
    vec3 worldPos;
} fs_in;

uniform samplerCube cubeMap;

out vec4 fragColor;

void main() {
    float ratio = 1.0f / material.refractionRatio;

    vec3 I = normalize(fs_in.worldPos - camera.position);
    vec3 R = refract(I, normalize(fs_in.normal), ratio);
    fragColor = vec4(texture(cubeMap, R).rgb, 1.0);
}