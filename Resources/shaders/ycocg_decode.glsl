#version 330 core
precision highp float;
precision highp int;

uniform sampler2D ScreenTexture;
out vec4 fragColor;

void main() {
    const float offset = 128.0 / 255.0;
    vec4 v = texelFetch(ScreenTexture, ivec2(gl_FragCoord.xy), 0);

    float Y = v.a;
    float Co = v.r - offset;
    float Cg = v.g - offset;

    float R = Y + Co - Cg;
    float G = Y + Cg;
    float B = Y - Co - Cg;

    fragColor = vec4(R, G, B, v.b);
}