/*************************************************************************
> File Name: FrameBuffer.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL FrameBuffer wrapper class
> Created Time: 2020/08/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_FRAMEBUFFER_HPP
#define CUBBYFLOW_VOX_FRAMEBUFFER_HPP

#include <Vox/Core/NonCopyable.hpp>
#include <Vox/Core/PixelFmt.hpp>
#include <Vox/Utils/GLTypes.hpp>
#include <memory>

namespace Vox {

    class Texture;

    /**
     * OpenGL FrameBuffer wrapper class 
     */
    class FrameBuffer : public NonCopyable
    {
    public:
        //! Default Constructor
        FrameBuffer(GLuint framebufferID);
        //! Default Destructor
        ~FrameBuffer();

        //! Bind texture to the gpu.
        void BindFrameBuffer(GLuint target);

        //! Attach Color Texture to the framebuffer.
        void AttachTexture(GLsizei index, const std::shared_ptr<Texture>& texture, bool bMultisample=false);

        //! Attach Render Buffer Object to the framebuffer.
        void AttachRenderBuffer(GLuint rbo);

        //! Validate framebuffer status
        bool ValidateFrameBufferStatus();

    private:
        GLuint _id;
        std::vector<std::shared_ptr<Texture>> _textures;
        GLuint _rbo;
    };

};

#endif