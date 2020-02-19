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

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Abstract base class for Application.
    //!
    //! This class contains basic interface for application. 
    //! GLFW wrapper class or EGL wrapper class must override
    //! this class and implement pure methods.
    //!
    class ScreenRecorder final
    {
    public: 
        //! Default Constructor.
        ScreenRecorder();

        //! Default Destructor.
        ~ScreenRecorder();

        //!
        //! 
        int saveScreenShot(const std::string& path, int frameIndex);

        //!
        int saveVideo(const std::string& path);

    protected:
    private:
    };

    using ScreenRecorderPtr = std::shared_ptr<ScreenRecorder>;
} 
}

#endif