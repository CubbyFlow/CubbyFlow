/*************************************************************************
> File Name: Renderer.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Renderere class managing whole context and states.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_RENDERER_HPP
#define CUBBYFLOW_VOX_RENDERER_HPP

#include <Vox/Utils/FileSystem.hpp>
#include <Vox/Core/PixelFmt.hpp>
#include <Vox/Utils/GLTypes.hpp>
#include <Vox/Utils/DebugUtils.hpp>
#include <memory>
#include <string>

namespace Vox {

    namespace Renderer {

        //! Check whether if given extensions are supported or not.
        bool CheckExtensionsSupported(const std::initializer_list<const char*>& exts);

        //! Create Texture with pixel format
        GLuint CreateTexture(GLsizei width, GLsizei height, const PixelFmt pf, const void* data, bool multisample=false);

        //! Create Volume Texture (3D Texture)
        GLuint CreateVolumeTexture(GLsizei width, GLsizei height, GLsizei depth, const PixelFmt pf, const void* data);

        //! Create Render Buffer
        GLuint CreateRenderBuffer(GLsizei width, GLsizei height, const PixelFmt pf, bool multisample=false);

        //! Create Shader from glsl source data.
        GLuint CreateShaderFromSource(const char* source, GLenum shaderType);

        //! Create Shader with glsl shader file.
        GLuint CreateShaderFromFile(const Path& path, GLenum shaderType);

        //! Create Program with pre-compiled shaders.
        GLuint CreateProgram(GLuint vs, GLuint gs, GLuint fs);

        //! Create Frame Buffer resource.
        GLuint CreateFrameBuffer(); 

        //! Read the pixel data from the current framebuffer and return by pointer.
        void ReadFrameBuffer(int width, int height, int mips, const PixelFmt pf, void* data);
    };

};

#endif