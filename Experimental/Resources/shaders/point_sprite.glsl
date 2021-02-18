#version 330 core

#include common/globals.glsl
#include common/uniforms.glsl
#include common/brdf.glsl

in VSOUT {
    vec3 worldPos;
} fs_in;

out vec4 fragColor;

void main() {
    vec2 cxy = 2.0f * gl_PointCoord - 1.0f;
    float radius2 = dot(cxy, cxy);
    if (radius2 > 1.0) discard;
    vec3 N = vec3(cxy, sqrt(1.0f - radius2));
    vec3 V = normalize(camera.position - fs_in.worldPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, material.albedo, material.metallic);

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < numLights; ++i)
    {
        //! Calculate per-light radiance
        vec3 L = normalize(light[i].position - fs_in.worldPos);
        vec3 H = normalize(L + V);
        float distance = length(light[i].position - fs_in.worldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = light[i].color * attenuation;

        //! Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, material.roughness);
        float G = GeometrySmith(N, V, L, material.roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - material.metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = numerator / max(denominator, EPS);

        //! Add to outgoing radiance Lo.
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * material.albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * material.albedo * material.ao;
    vec3 color = ambient + Lo;

    float delta = fwidth(radius2);
    float alpha = 1.0f - smoothstep(1.0f - delta, 1.0f + delta, radius2);

    fragColor = vec4(color, alpha);
}