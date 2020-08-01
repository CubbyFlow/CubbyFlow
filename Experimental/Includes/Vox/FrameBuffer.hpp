/*************************************************************************
> File Name: FrameBuffer.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL FrameBufferObject abstraction
> Created Time: 2020/07/30
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_FRAMEBUFFER_HPP
#define CUBBYFLOW_VOX_FRAMEBUFFER_HPP

#include <Vox/GLTypes.hpp>

namespace Vox {

    /**
     * OpenGL FrameBufferObject
     */
    class FrameBuffer 
    {
    public:
        //! Default Constructor
        FrameBuffer();
        //! Default Destructor
        ~FrameBuffer();

        //! Bind Framebuffer to current opengl context.
        void BindFrameBuffer(GLenum target);
        //! Set color texture attachment to framebuffer 
        void SetColorAttachment(GLenum index, GLuint texture);
        //! Set Render buffer attachment to framebuffer.
        void SetRenderBufferAttachment(GLuint rbo);
        //! Assertion for framebuffer completeness.
        bool AssertFramebufferStatus() const;
    protected:
    private:
        GLuint _fbo { 0 };
    };

};

#endif