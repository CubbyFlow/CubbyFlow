/*************************************************************************
> File Name: Texture.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for texture/
> Created Time: 2020/02/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Textures/Texture.h>

namespace CubbyFlow {
namespace CubbyRender {

    Texture::Texture()
    {
        //! Do nothing.
    }

    Texture::~Texture()
    {
        //! Do nothing.
    }

    const TextureSamplingMode& Texture::samplingMode() const
    {
        return _samplingMode;
    }

    void Texture::setSamplingMode(const TextureSamplingMode& mode)
    {
        _samplingMode = mode;
    }

    void Texture::bind(RendererPtr renderer, unsigned int slotID)
    {
        onBind(renderer, slotID);
    }

    void Texture::unbind(RendererPtr renderer, unsigned int slotID)
    {
        onUnbind(renderer, slotID);
    }

    void Texture::destroy()
    {
        onDestroy();    
    }
    
} 
}