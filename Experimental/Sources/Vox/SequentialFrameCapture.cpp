/*************************************************************************
> File Name: SequentialFrameCapture.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Video capture class using Texture Compression technique
> Created Time: 2020/08/23
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/SequentialFrameCapture.hpp>
#include <Vox/TextureIO.hpp>
#include <Vox/Renderer.hpp>
#include <cstring>

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

    bool SequentialFrameCapture::WriteCurrentCaptureToDDS(const char* format)
    {
        //! ReadPixels must be implemented with Asynchronous features.
        //! Note this reference : http://http.download.nvidia.com/developer/Papers/2005/Fast_Texture_Transfers/Fast_Texture_Transfers.pdf
        char baseName[256];
        snprintf(baseName, sizeof(baseName), format, _frame);
        return DDS::WriteTexture(baseName, _width, _height, _mips, _pf, _buffer);
    }

    bool SequentialFrameCapture::WriteCurrentCaptureToTGA(const char* format)
    {
        char baseName[256];
        snprintf(baseName, sizeof(baseName), format, _frame);
        return TGA::WriteTexture(baseName, _width, _height, _mips, _pf, _buffer);
    }

};