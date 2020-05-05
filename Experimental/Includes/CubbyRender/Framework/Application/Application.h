/*************************************************************************
> File Name: Application.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Application.
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_APPLICATION_H
#define CUBBYFLOW_APPLICATION_H

#include <Framework/Window/Window.h>
#include <Media/ScreenRecorder.h>
#include <Core/Array/ArrayAccessor1.h>
#include <Core/Vector/Vector4.h>
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
    class Application
    {
    public: 
        using EncodingCallback = std::function<bool(Size2, const ArrayAccessor1<unsigned char>&)>;
        //! Default Constructor.
        Application();

        //! Default Destructor.
        virtual ~Application();

        //! Initializes the application.
        virtual int initialize() = 0;

        //! Run the application with limited frames and fps.
        virtual int run(int numberOfFrames, EncodingCallback makeScreenshot = nullptr) = 0;

        //! Create window with window title, width, height.
        virtual void createMainWindow(const std::string& title, int width, int height) = 0;

        //! Return Main window of the application.
        virtual WindowPtr getMainWindow() = 0;

        //! Destroy the application.
        virtual void terminate() = 0;
    protected:
        //! Validate the application whether the app can run simulation or not.
        //! If this method return true, the application can run any simulation.
        virtual bool validateApplication() = 0;
    private:
    
    };

    using ApplicationPtr = std::shared_ptr<Application>;
} 
}

#endif