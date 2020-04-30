/*************************************************************************
> File Name: Material.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Material object.
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Renderable/Material.h>
#include <Framework/Renderer/Renderer.h>
#include <Framework/Shader/Shader.h>
#include <Framework/Textures/Texture.h>

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
        //! Do nothing.
    }

    void Material::addTexture(unsigned int slotID, TexturePtr texture)
    {
        _textures.emplace_back(slotID, texture);
    }

    void Material::setShader(ShaderPtr shader)
    {
        _shader = shader;
    }

    ShaderPtr Material::getShader()
    {
        return _shader;
    }

    void Material::bind(RendererPtr renderer)
    {
        _shader->bind(renderer);
        for (auto iter = _textures.begin(); iter < _textures.end(); ++iter)
        {
            unsigned int slotID = iter->first;
            const TexturePtr& texture = iter->second;
            texture->bind(renderer, slotID);
        }
    }

    void Material::unbind(RendererPtr renderer)
    {
        _shader->unbind(renderer);
        for (auto iter = _textures.begin(); iter < _textures.end(); ++iter)
        {
            unsigned int slotID = iter->first;
            const TexturePtr& texture = iter->second;
            texture->unbind(renderer, slotID);
        }
    }

    void Material::release()
    {
        _shader.reset();
        for (auto pair : _textures)
            pair.second.reset();
        _textures.clear();
    }
} 
}