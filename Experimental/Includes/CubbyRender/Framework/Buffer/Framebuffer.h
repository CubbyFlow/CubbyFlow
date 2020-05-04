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

        //! allocate framebuffer with pre-configured textures.
        void allocateBuffer(RendererPtr renderer, const std::vector<Texture2DPtr>& colorTextures, Texture2DPtr depthTexture);

        //! Get color texture pointer with index 
        Texture2DPtr getColorTexture(size_t index);

        //! Get depth texture pointer with index 
        Texture2DPtr getDepthTexture();

        //! configure framebuffer with currently binded color and depth textures.
        virtual void configure(RendererPtr renderer) = 0;

    protected:
        //! implementation of bind method
        virtual void onBind(RendererPtr renderer) override = 0;
        
        //! implementation of unbind method
        virtual void onUnbind(RendererPtr renderer) override = 0;

        //! implementation of destroy method
        virtual void onDestroy() override = 0;

        //! implementation of allocate framebuffer
        virtual void onAllocateBuffer(RendererPtr renderer) = 0;

        std::vector<Texture2DPtr> _colorTextures;
        Texture2DPtr _depthTexture { nullptr };
    private:
    };

    using FramebufferPtr = std::shared_ptr<Framebuffer>;
} 
}

#endif