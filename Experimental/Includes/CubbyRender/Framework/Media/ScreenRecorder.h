/*************************************************************************
> File Name: ScreenRecorder.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Recorder class for storing screenshot or simulation video.
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_SCREEN_RECORDER_H
#define CUBBYFLOW_SCREEN_RECORDER_H

#ifdef CUBBYFLOW_RECORDING

#include <Core/Size/Size2.h>
#include <Core/Array/ArrayAccessor2.h>
#include <Core/Vector/Vector4.h>
#include <memory>
#include <string>
#include <cstdio>

class AVCodecContext;
class AVFrame;
class AVPacket;
class SwsContext;

namespace CubbyFlow {
namespace CubbyRender {

    //!
    //! \brief Recorder class for storing rendering result as image or video.
    //!
    class ScreenRecorder final
    {
    public: 
        //! Default Constructor.
        ScreenRecorder();

        //! Default Constructor.
        ScreenRecorder(size_t width, size_t height);

        //! Default Constructor.
        ScreenRecorder(Size2 frameDimension);

        //! Default Destructor. 
        virtual ~ScreenRecorder();

        //! Start encoding
        bool StartEncode(const std::string& filename, int fps);

        //! Finish encoding
        bool FinishEncode();

        //! Encode frame into context.
        bool EncodeFrame(const ConstArrayAccessor2<Vector4UB>& frame);

        //! Get frame dimension
        Size2 size() const;

        //! Get width of the frame
        size_t Width() const;

        //! Get height of the frame
        size_t Height() const;

        //! Set dimension of the screen(frame)
        void SetSize(Size2 dim);

        //! Set dimension of the screen(frame)
        void SetSize(size_t width, size_t height);

    protected:
    private:
        Size2 _size;

        AVCodecContext *_codecContext = nullptr;
        AVFrame *_avFrame = nullptr;
        AVPacket *_pkt = nullptr;
        SwsContext *_swsContext = nullptr;
        FILE* _file = nullptr;
    };

    using ScreenRecorderPtr = std::shared_ptr<ScreenRecorder>;
} 
}

#endif
#endif