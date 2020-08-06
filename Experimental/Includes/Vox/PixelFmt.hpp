/*************************************************************************
> File Name: PixemFmt.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Simple filesystem class
> Created Time: 2020/07/29
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_PIXEL_FMT_HPP
#define CUBBYFLOW_VOX_PIXEL_FMT_HPP

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
};

#endif