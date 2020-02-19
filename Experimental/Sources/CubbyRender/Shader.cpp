/*************************************************************************
> File Name: Shader.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Shader object.
> Created Time: 2020/02/19
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Shader.h>
#include <Framework/Renderer.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    Shader::Shader()
    {

    }

    Shader::Shader(const std::string& shaderName)
        : _shaderName(shaderName)
    {

    }

    Shader::~Shader()
    {

    }

    void Shader::setShaderName(const std::string& shaderName)
    {
        _shaderName = shaderName;
    }

    std::string Shader::getShaderName()
    {
        return _shaderName;
    }

} 
}