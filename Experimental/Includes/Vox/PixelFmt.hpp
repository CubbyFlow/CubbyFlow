/*************************************************************************
> File Name: PixemFmt.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Pixel format enumeration for texture and buffer attributes.
> Created Time: 2020/07/29
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_PIXEL_FMT_HPP
#define CUBBYFLOW_VOX_PIXEL_FMT_HPP

#include <Vox/DebugUtils.hpp>
#include <Vox/FileSystem.hpp>

namespace Vox {
    
    enum class PixelFmt : unsigned int
    {
        PF_UNKNOWN = 0,
        PF_R8,
        PF_R16,
        PF_RG8,
        PF_RG16,
        PF_RGB8,
        PF_RGBA8,
        PF_RGBA16,
        PF_SRGB8,
        PF_SRGB8_ALPHA8,
        PF_R16F,
        PF_RG16F,
        PF_RGB16F,
        PF_RGBA16F,
        PF_R32F,
        PF_RG32F,
        PF_RGB32F,
        PF_RGBA32F,
        PF_R8I,
        PF_R8UI,
        PF_R16I,
        PF_R16UI,
        PF_R32I,
        PF_R32UI,
        PF_RG8I,
        PF_RG8UI,
        PF_RG16I,
        PF_RG16UI,
        PF_RG32I,
        PF_RG32UI,
        PF_RGB8I,
        PF_RGB8UI,
        PF_RGB16I,
        PF_RGB16UI,
        PF_RGB32I,
        PF_RGB32UI,
        PF_RGBA8I,
        PF_RGBA8UI,
        PF_RGBA16I,
        PF_RGBA16UI,
        PF_RGBA32I,
        PF_RGBA32UI,
        PF_BGR8,
        PF_BGRA8,
        PF_SBGR8_ALPHA8,
        PF_DEPTH_COMPONENT16,
        PF_DEPTH_COMPONENT24,
        PF_DEPTH_COMPONENT24_STENCIL8,
        PF_DEPTH_COMPONENT32,
        PF_DEPTH_COMPONENT32F,
        PF_DEPTH_COMPONENT32F_STENCIL8,
        PF_RGB565,
        PF_DXT1,
        PF_DXT3,
        PF_DXT5,
        PF_DXT1_SRGB,
        PF_DXT3_SRGB,
        PF_DXT5_SRGB,
        PF_END,
    };

    inline bool operator<(const PixelFmt pf1, const PixelFmt pf2)
    {
        return static_cast<unsigned int>(pf1) < static_cast<unsigned int>(pf2);
    }
    inline bool operator>(const PixelFmt pf1, const PixelFmt pf2)
    {
        return static_cast<unsigned int>(pf1) > static_cast<unsigned int>(pf2);
    }

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