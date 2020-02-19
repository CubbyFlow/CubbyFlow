/*************************************************************************
> File Name: EGLApp.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: EGL application implementation for offline rendering in headless.
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_EGLAPP_H
#define CUBBYFLOW_EGLAPP_H

#include <Framework/Application.h>
#include <EGL/egl.h>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {

    class EGLWindow;
    using EGLWindowPtr = std::shared_ptr<EGLWindow>;

    //!
    //! \brief Abstract base class for Application.
    //!
    //! This class contains basic interface for application. 
    //! GLFW wrapper class or EGL wrapper class must override
    //! this class and implement pure methods.
    //!
    class EGLApp final : public Application
    {
    public: 
        //! Default Constructor.
        EGLApp();

        //! Default Destructor.
        ~EGLApp() override;

        //! Initializes the application.
        int initialize() override;

        //! Run the application.
        int run(ScreenRecorderPtr recorder = nullptr) override;

        //! Run the application with limited time and fps.
        int runWithLimitedTime(double time, double fps, ScreenRecorderPtr recorder = nullptr) override;
        
        //! Create window and return it.
        WindowPtr& createWindow(const std::string& title, int width, int height) override;

        //! Destroy the application.
        void terminate() override;
        
    protected:
        //! Validate the application whether the app can run simulation or not.
        bool validateApplication() override;

    private:
        EGLDisplay _display;
    };

    using EGLAppPtr = std::shared_ptr<EGLApp>;
} 
}

#endif