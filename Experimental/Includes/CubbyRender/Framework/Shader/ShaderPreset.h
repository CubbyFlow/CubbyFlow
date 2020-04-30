/*************************************************************************
> File Name: GL3ShaderPreset.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: shader presets collection.
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifndef CUBBYFLOW_GL3SHADERPRESET_H
#define CUBBYFLOW_GL3SHADERPRESET_H

#ifdef CUBBYFLOW_USE_GL

#include <GL3/Utils/GL3Common.h>

namespace CubbyFlow {
namespace CubbyRender {

const GLchar* kSimpleColorShaders[2] = {
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
    )glsl"};

const GLchar* kPointsShaders[2] = {
    // Vertex shader
    R"glsl(
    #version 330 core
    in vec3 position;
    in vec4 color;
    uniform float Radius;
    uniform mat4 View;
    uniform mat4 Projection;
    out VertexData {
        vec4 color;
    } outData;
    void main() {
        outData.color = color;
        gl_PointSize = 2.0 * Radius * Projection[0][0];
        gl_Position = View * vec4(position, 1.0);
    }
    )glsl",

    // Fragment shader
    R"glsl(
    #version 330 core
    in VertexData {
    	 vec4 color;
    } inData;
    out vec4 fragColor;
    void main() {
    	 fragColor = inData.color;
    }
    )glsl"};

} 
}
#elif CUBBYFLOW_USE_METAL
const char* kSimpleColorShaders[2] = {
    // Vertex shader
    R"metal(
    )metal",

    // Fragment shader
    R"metal(
    )metal"};
const char* kPointsShaders[2] = {
    // Vertex shader
    R"metal(
    )metal",

    // Fragment shader
    R"metal(
    )metal"};
#else //! If any graphics api not exists. 
const char* kSimpleColorShaders[2]  = { nullptr, nullptr };
const char* kPointsShaders[2]       = { nullptr, nullptr };
#endif  

#endif 
