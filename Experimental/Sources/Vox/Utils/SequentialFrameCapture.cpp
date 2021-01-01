/*************************************************************************
> File Name: SequentialFrameCapture.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Video capture class using Texture Compression technique
> Created Time: 2020/08/23
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Utils/SequentialFrameCapture.hpp>
#include <Vox/Core/Renderer.hpp>
#include <Vox/Core/Texture.hpp>
#include <cstring>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace Vox {

    SequentialFrameCapture::SequentialFrameCapture()
    {
        //! Do nothing.
    }

    SequentialFrameCapture::~SequentialFrameCapture()
    {
        free(reinterpret_cast<unsigned char*>(_buffer));
    }

    void SequentialFrameCapture::CaptureFrameBuffer(int width, int height, int mips, const PixelFmt pf)
    {
        const size_t prevNumBytes = GetByteSize(_width, _height, _mips, _pf);
        const size_t curNumBytes = GetByteSize(width, height, mips, pf);
        if (prevNumBytes != curNumBytes)
        {
            _buffer = ::realloc(_buffer, curNumBytes);
        }
        //! Store the current frame capture informations.
        _width = width; _height = height; _mips = mips; _pf = pf;
        //! Read the pixel data from the current framebuffer.
        Renderer::ReadFrameBuffer(width, height, mips, pf, _buffer);
        //! Advance the frame index.
        ++_frame;
    }

    bool SequentialFrameCapture::WriteCurrentCapture(const std::string& format)
    {
        char baseName[256];
        snprintf(baseName, sizeof(baseName), format.c_str(), _frame);

        size_t dotPos = format.find_last_of('.');
        const std::string extension = format.substr(dotPos + 1);

        stbi_flip_vertically_on_write(true);
        if (extension == "png")
            stbi_write_png(baseName, _width, _height, 4, _buffer, 0);
        else if (extension == "jpg")
            stbi_write_jpg(baseName, _width, _height, 4, _buffer, 30);
        else if (extension == "tga")
            stbi_write_tga(baseName, _width, _height, 4, _buffer);
        else
        {
            
            VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown image extension (only support png, jpg and tga)");
        }

        return true;
    }

};