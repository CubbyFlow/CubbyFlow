/*************************************************************************
> File Name: ShaderPreset.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: shader presets collection.
> Created Time: 2020/07/30
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifndef CUBBYFLOW_VOX_SHADER_PRESET_HPP
#define CUBBYFLOW_VOX_SHADER_PRESET_HPP

namespace Vox {

    const char* kSimpleColorShaders[2] = 
    {
        // Vertex shader
        R"glsl(
            #version 330 core
            in vec3 position;
            in vec3 normal;
            uniform mat4 View;
            uniform mat4 Projection;
            out VSOUT {
                vec4 color;
            } vs_out;
            void main() {
                vs_out.color = vec4(normal, 1.0);
                gl_Position = Projection * View * vec4(position,1.0);
            }
        )glsl",
    
        // Fragment shader
        R"glsl(
            #version 330 core
            in VSOUT {
                vec4 color;
            } fs_in;
            out vec4 fragColor;
            void main() {
                fragColor = fs_in.color;
            }
        )glsl"
    };
    
    const char* kPointsShaders[2] = 
    {
        // Vertex shader
        R"glsl(
            #version 330 core
            in vec3 position;
            uniform mat4 ViewProjection;
            #define RADIUS 8.0
            void main() {
                gl_PointSize = RADIUS * (1.0 - ViewProjection[0][0] * 0.00001);
                gl_Position =  vec4(position, 1.0);
            }
        )glsl",
    
        // Fragment shader
        R"glsl(
            #version 330 core
            //struct DirectionalLight {
            //    vec3 lookAt;
            //};
            //#define MAX_NUM_LIGHTS 4
            //uniform DirectionalLight directionalLights[MAX_NUM_LIGHTS];
            //uniform int NumLights;
            //uniform vec3 Color;
            out vec4 fragColor;
            void main() {
                vec3 N;
                N.xy = gl_PointCoord* 2.0 - vec2(1.0);
                float mag = dot(N.xy, N.xy);
                if(mag > 1.0) discard; // kill pixels outside circle
                N.z = sqrt(1.0-mag);
                //float diffuse = 0.0f;
                //for (int i = 0; i < NumLights; ++i)
                //{
                //    vec3 lightDir = normalize(directionalLights[i].lookAt);
                //    diffuse = diffuse + max(0.0, dot(lightDir, N));
                //}
                //fragColor = vec4((N * Color) * diffuse, 1.0f);
                fragColor = vec4(N, 1.0f);
            }
        )glsl"
    };
    
    const char* kScreenShaders[2] = 
    {
        // Vertex shader
        R"glsl(
            #version 330 core
            precision highp float;
            precision highp int;

            void main() {
                gl_Position = vec4(vec2(gl_VertexID & 1, (gl_VertexID & 2) >> 1) * 2 - 1), 0, 1);
            }
        )glsl",
    
        // Fragment shader
        R"glsl(
            #version 330 core
            uniform sampler2D screenTexture;
            in VertexData {
                vec2 texCoord;
            } inData;
            out vec4 fragColor;
            void main() {
                vec3 color = texture(screenTexture, inData.texCoord).rgb;
                fragColor = vec4(color, 1.0);
            }
        )glsl"
    };
    
} 

#endif 