/*************************************************************************
> File Name: ScreenRecorder.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Recorder class for storing screenshot or simulation video.
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

//! reference  : https://github.com/cirosantilli/cpp-cheat/blob/70b22ac36f92e93c94f951edb8b5af7947546525/opengl/offscreen.c
#ifdef CUBBYFLOW_RECORDING

#include <Framework/Media/ScreenRecorder.h>
#include <Core/Utils/Logging.h>
#include <Core/Math/MathUtils.h>
#include <Core/Utils/Logging.h>
#include <pystring/pystring.h>
#include <fstream>

#include <png.h>

namespace CubbyFlow {
namespace CubbyRender {
    
        ScreenRecorder::ScreenRecorder()
        {
            //! Do nothing
        }

        ScreenRecorder::ScreenRecorder(size_t width, size_t height)
        {
            SetSize(width, height);
        }

        ScreenRecorder::ScreenRecorder(Size2 frameDimension)
        {
            SetSize(frameDimension);
        }

        ScreenRecorder::~ScreenRecorder()
        {
        }

        bool ScreenRecorder::StartEncode(const std::string& filename, int fps)
        {
            UNUSED_VARIABLE(filename);
            UNUSED_VARIABLE(fps);
            return true;
        }

        bool ScreenRecorder::FinishEncode()
        {
            return true;
        }

        bool ScreenRecorder::EncodeFrame(const ConstArrayAccessor2<Vector4UB>& frame)
        {
            UNUSED_VARIABLE(frame);
            return true;
        }

        Size2 ScreenRecorder::size() const
        {
            return _size;
        }

        size_t ScreenRecorder::Width() const
        {
            return _size[0];
        }

        size_t ScreenRecorder::Height() const
        {
            return _size[1];
        }
        
        void ScreenRecorder::SetSize(Size2 dim)
        {
            _size = dim;
        }

        void ScreenRecorder::SetSize(size_t width, size_t height)
        {
            SetSize(Size2(width, height));
        }
} 
}

#endif