/*************************************************************************
> File Name: Framebuffer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for FrameBufferObject
> Created Time: 2020/04/30
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_FRAME_BUFFER_H
#define CUBBYFLOW_FRAME_BUFFER_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Buffer/Buffer.h>
#include <memory>
#include <vector>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Abstract base class for Frame Buffer Object
    //!
    class Framebuffer : public Buffer
    {
    public:
        //! Default constructor.
        Framebuffer();

        //! Default destructor.
        virtual ~Framebuffer();

        //! allocate framebuffer with already configured textures.
        void allocateBuffer(RendererPtr renderer, const std::vector<TexturePtr>& colorTextures, const std::vector<TexturePtr>& depthTextures);

        //! Get color texture pointer with index 
        TexturePtr getColorTexture(size_t index);

        //! Get depth texture pointer with index 
        TexturePtr getDepthTexture(size_t index);

        //! configure framebuffer with currently binded color and depth textures.
        virtual void configure() = 0;

    protected:
        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) override = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) override = 0;

        //! implementation of destroy method
        virtual void onDestroy() override = 0;

        //! implementation of allocate framebuffer
        virtual void onAllocateBuffer(RendererPtr renderer) = 0;

        std::vector<TexturePtr> _colorTextures;
        std::vector<TexturePtr> _depthTextures;
    private:
    };

    using FramebufferPtr = std::shared_ptr<Framebuffer>;
} 
}

#endif