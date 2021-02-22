#version 430 core

precision highp float;
precision highp int;
precision highp sampler2D;
precision highp samplerCube;
precision highp isampler2D;
precision highp sampler2DArray;

uniform sampler2D ScreenTexture;
out vec4 fragColor;

void main() {
    vec3 color = texelFetch(ScreenTexture, ivec2(gl_FragCoord.xy), 0).rgb;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    fragColor = vec4(color, 1.0);
}