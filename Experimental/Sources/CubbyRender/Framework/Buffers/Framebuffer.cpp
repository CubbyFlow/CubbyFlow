/*************************************************************************
> File Name: Framebuffer.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for FrameBufferObject
> Created Time: 2020/04/30
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Buffer/Framebuffer.h>
#include <Framework/Texture/Texture2D.h>
#include <cassert>

namespace CubbyFlow {
namespace CubbyRender {
    
    Framebuffer::Framebuffer()
    {
        //! Do nothing
    }

    Framebuffer::~Framebuffer()
    {
        _colorTextures.clear();
        _depthTexture.reset();
    }

    void Framebuffer::allocateBuffer(RendererPtr renderer, const std::vector<Texture2DPtr>& colorTextures, Texture2DPtr depthTexture)
    {
        _colorTextures = colorTextures;
        _depthTexture = depthTexture;

        onAllocateBuffer(renderer);
    }

    Texture2DPtr Framebuffer::getColorTexture(size_t index)
    {
        assert(size_t(0) <= index && index < _colorTextures.size());
        return _colorTextures[index];
    }

    Texture2DPtr Framebuffer::getDepthTexture()
    {
        return _depthTexture;
    }
} 
}