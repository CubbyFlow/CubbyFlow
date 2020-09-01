/*************************************************************************
> File Name: SequentialFrameCapture.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Video capture class using Texture Compression technique
> Created Time: 2020/08/23
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_SEQUENTIAL_FRAME_CAPTURE_HPP
#define CUBBYFLOW_VOX_SEQUENTIAL_FRAME_CAPTURE_HPP

#include <Vox/GLTypes.hpp>
#include <Vox/PixelFmt.hpp>
#include <string>
#include <memory>

namespace Vox {
    /**
     * OpenGL Context wrapper which will be used for rendering one frame.
     */
    class SequentialFrameCapture 
    {
    public:
        //! Default Constructor
        SequentialFrameCapture();
        //! Default Destructor
        ~SequentialFrameCapture();

        //! Capture screen shot of the current framebuffer.
        void CaptureFrameBuffer(int width, int height, int mips, const PixelFmt pf);
        //! Write current frame capture as dds file.
        bool WriteCurrentCaptureToDDS(const char* format);
        //! Write current frame capture as tga file.
        bool WriteCurrentCaptureToTGA(const char* format);

        //! Returns the current frame index.
        inline size_t GetCurrentFrameIndex() const
        {
            return _frame;
        }
        
    private:
        size_t _frame { 0 };
        void* _buffer { nullptr };
        int _width { 0 };
        int _height { 0 };
        int _mips { 0 };
        PixelFmt _pf { PixelFmt::PF_RGB8 };
    };

};

#endif