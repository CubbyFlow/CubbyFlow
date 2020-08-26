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

#include <Vox/FileSystem.hpp>
#include <Vox/PixelFmt.hpp>
#include <Vox/GLTypes.hpp>
#include <Vox/DebugUtils.hpp>
#include <memory>
#include <string>

namespace Vox {

    namespace Renderer {

        //! Check whether if given extensions are supported or not.
        bool CheckExtensionsSupported(const std::initializer_list<const char*>& exts);

        //! Create Texture with pixel format
        GLuint CreateTexture(GLsizei width, GLsizei height, const PixelFmt pf, const void* data, bool multisample=false);

        //! Create Render Buffer
        GLuint CreateRenderBuffer(GLsizei width, GLsizei height, const PixelFmt pf, bool multisample=false);

        //! Create Shader from glsl source data.
        GLuint CreateShaderFromSource(const char* source, GLenum shaderType);

        //! Create Shader with glsl shader file.
        GLuint CreateShaderFromFile(const Path& path, GLenum shaderType);

        //! Create Program with pre-compiled shaders.
        GLuint CreateProgram(GLuint vs, GLuint gs, GLuint fs);

        //! Read the pixel data from the current framebuffer and return by pointer.
        void ReadFrameBuffer(int width, int height, int mips, const PixelFmt pf, void* data);
    };


    //! Reference from OpenGL Insights code.
    struct PixelFmtDesc {
        int internal;			//< OpenGL internal format (GL_RGBA8)
        int format;			    //< OpenGL format (GL_RGBA)
        int type;				//< OpenGL component type (GL_UNSIGNED_BYTE)
        unsigned int size;		//< byte size of one pixel (4)
        int components;		    //< number of components (4)
        bool rt;				//< true if it can be used as render target
        PixelFmt sRGB;				//< sRGB pixel format alternative
        const char *txt;		//< readable description
        bool compressed;		//< true if it is compressed format
    };

    //! Must be inmodifiable.
    extern const PixelFmtDesc __pfds[];

    inline const PixelFmtDesc* GetPixelFmtDesc(const PixelFmt pf) 
    {
        VoxAssert(PixelFmt::PF_UNKNOWN < pf && pf < PixelFmt::PF_END, CURRENT_SRC_PATH_TO_STR, "Unknown Pixel Format");
        return __pfds + static_cast<unsigned int>(pf);
    }

};

#endif