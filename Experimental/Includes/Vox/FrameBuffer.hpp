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

        void BindFrameBuffer(GLenum target);

        void SetColorAttachment(GLenum index, GLuint texture);
        void SetRenderBufferAttachment(GLuint rbo);
        bool AssertFramebufferStatus() const;
    protected:
    private:
        GLuint _fbo { 0 };
    };

};

#endif