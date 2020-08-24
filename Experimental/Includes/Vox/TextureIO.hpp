/*************************************************************************
> File Name: TextureIO.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> References: OpenInsights sample codes.
> Purpose: Texture write and load functions for dds and tga file.
> Created Time: 2020/08/23
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_TEXTURE_IO_HPP
#define CUBBYFLOW_VOX_TEXTURE_IO_HPP
#include <Vox/PixelFmt.hpp>

namespace Vox {

    //! Returns the total number of the bytes with given arguments.
    unsigned int GetByteSize(int w, int h, int mips, Vox::PixelFmt fmt);

    namespace DDS
    {
        void* LoadTextureData(const char* f, int* w, int* h, int* mips, Vox::PixelFmt* pixmft);
        bool WriteTexture(const char* file, int w, int h, int mips, Vox::PixelFmt fmt, const void* data);
    };

    namespace TGA
    {
        void* LoadTextureData(const char* f, int* w, int* h, int* mips, Vox::PixelFmt* pixmft);
        bool WriteTexture(const char* file, int w, int h, int mips, Vox::PixelFmt fmt, const void* data);
    };

;}

#endif