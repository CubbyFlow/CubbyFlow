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
#include <cassert>

namespace CubbyFlow {
namespace CubbyRender {
    
    Framebuffer::Framebuffer()
    {
        //! Do nothing
    }

    Framebuffer::~Framebuffer()
    {
        //! Do nothing
    }

    void Framebuffer::allocateBuffer(RendererPtr renderer, const std::vector<TexturePtr>& colorTextures, const std::vector<TexturePtr>& depthTextures)
    {
        _colorTextures = colorTextures;
        _depthTextures = depthTextures;

        onAllocateBuffer(renderer);
    }

    TexturePtr Framebuffer::getColorTexture(size_t index)
    {
        assert(size_t(0) <= index && index < _colorTextures.size());
        return _colorTextures[index];
    }

    TexturePtr Framebuffer::getDepthTexture(size_t index)
    {
        assert(size_t(0) <= index && index < _depthTextures.size());
        return _depthTextures[index];
    }

} 
}