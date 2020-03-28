/*************************************************************************
> File Name: Material.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Material object.
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Material.h>
#include <Framework/Renderer.h>
#include <Framework/Shader.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    Material::Material()
    {
        //! Do nothing
    }

    Material::Material(ShaderPtr shader)
        : _shader(shader)
    {
        //! Do nothing
    }

    Material::~Material()
    {
        destroy();
    }

    ShaderPtr Material::getShader()
    {
        return _shader;
    }

    void Material::destroy()
    {
        _shader.reset();
    }

} 
}