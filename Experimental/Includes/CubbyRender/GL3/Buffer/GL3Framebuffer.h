/*************************************************************************
> File Name: GL3Framebuffer.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Modern opengl framebuffer object wrapper class.
> Created Time: 2020/05/02
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GL3_FRAME_BUFFER_H
#define CUBBYFLOW_GL3_FRAME_BUFFER_H

#include <Framework/Buffer/Framebuffer.h>
#include <memory>

#ifdef CUBBYFLOW_USE_GL

#include <GL3/Utils/GL3Common.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Modern opengl framebuffer object wrapper class.
    //!
    class GL3Framebuffer : public Framebuffer
    {
    public:
        //! Default constructor.
        GL3Framebuffer();

        //! Default destructor.
        virtual ~GL3Framebuffer();

        //! configure framebuffer with currently binded color and depth textures.
        void configure(RendererPtr renderer) override;

    protected:
        //! implementation of bind method
        void onBind(RendererPtr renderer) override;
        
        //! implementation of unbind method
        void onUnbind(RendererPtr renderer) override;

        //! implementation of destroy method
        void onDestroy() override;

        //! implementation of allocate framebuffer
        void onAllocateBuffer(RendererPtr renderer) override;
    private:
        GLuint _fboID; //! framebuffer  object id.
        GLuint _rboID; //! renderbuffer object id.
    };

    using GL3FramebufferPtr = std::shared_ptr<GL3Framebuffer>;
} 
}

#endif

#endif