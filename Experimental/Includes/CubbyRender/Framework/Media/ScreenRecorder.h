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

#include <memory>
#include <string>

#include <Core/Size/Size3.h>

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

        //! Default Destructor.
        virtual ~ScreenRecorder();

        //! Store given frame to _buffers
        //! void storeFrame(Buffer buffer);

        //! Save the frame with given frameIndex as tga format.
        int saveScreenShot(const std::string& path, int frameIndex);

        //! Save the stored frames as video.
        int saveVideo(const std::string& path);

        //! Set dimension of the screen(frame)
        void setFrameDiemsnion(Size3 dim);

    protected:
        // std::vector<Buffer> _buffers;
        Size3 _frameDimension;
        int _frameCnt = 0;
    private:
    };

    using ScreenRecorderPtr = ScreenRecorder*;
} 
}

#endif