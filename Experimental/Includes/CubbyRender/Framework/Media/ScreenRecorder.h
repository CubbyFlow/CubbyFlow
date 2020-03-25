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

#include <Core/Size/Size3.h>
#include <Core/Array/Array2.h>
#include <Core/Array/ArrayAccessor2.h>
#include <Core/Vector/Vector3.h>
#include <memory>
#include <string>
#include <vector>

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
        ScreenRecorder(Size2 frameDimension);

        //! Default Destructor. 
        virtual ~ScreenRecorder();

        //! Store given frame to _buffers
        void storeFrame(Array2<Vector3B>&& frame);

        //! Save the frame with given frameIndex as tga format.
        int saveFrame(const std::string& path, size_t frameIndex) const;

        //! Save the frames with given period
        int saveMultiFrames(const std::string& path, size_t period) const;

        //! Save the all stored frames in given path.
        int saveAllFrames(const std::string& path) const;

        //! Get frame dimension
        Size2 getFrameDimension() const;

        //! Set dimension of the screen(frame)
        void setFrameDimension(Size2 dim);

        //! Get the count of the frames.
        size_t getFrameCount() const;

    protected:
        std::vector<Array2<Vector3B>> _frames;
        Size2 _frameDimension;
    private:
    };

    using ScreenRecorderPtr = std::shared_ptr<ScreenRecorder>;
} 
}

#endif