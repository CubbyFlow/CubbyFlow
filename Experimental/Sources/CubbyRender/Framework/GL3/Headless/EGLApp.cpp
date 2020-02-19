/*************************************************************************
> File Name: EGLApp.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: EGL application implementation for offline rendering in headless.
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/GL3/Headless/EGLApp.h>

//! For using EGL extension, define extra macro.
#define EGL_EGLEXT_PROTOTYPES
#include <EGL/eglext.h>
#include <Framework/Window.h>
#include <Framework/GL3/Headless/EGLWindow.h>
#include <Framework/GL3/GL3Renderer.h>
#include <cassert>

namespace CubbyFlow {
namespace CubbyRender {

    
    EGLApp::EGLApp()
        : _display(nullptr)
    {
        //! Do nothing.
    }

    EGLApp::~EGLApp()
    {
        terminate();
    }

    int EGLApp::initialize()
    {
        constexpr int kMaxDevices = 4;
        EGLDeviceEXT devices[kMaxDevices];
        EGLint numDevices;

        //! Get additional egl extension for querying devices.
        PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT = 
            (PFNEGLQUERYDEVICESEXTPROC)eglGetProcAddress("eglQueryDevicesEXT");
        
        //! Query available devices from the hardware where this program is running.
        eglQueryDevicesEXT(kMaxDevices, devices, &numDevices);

        //! Get additional egl extension for selecting device for display.
        PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT =
            (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");

        //! Select device for operating egl display.
        _display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, devices[0], 0);
        
        EGLint major, minor;
        eglInitialize(_display, &major, &minor);
        eglBindAPI(EGL_OPENGL_API);

        return 0;
    }

    WindowPtr& EGLApp::createWindow(const std::string& title, int width, int height)
    {
        _window = std::make_shared<EGLWindow>(_display, title, width, height);

        RendererPtr renderer = std::make_shared<GL3Renderer>();
        assert( renderer->initializeGL() == 0 );

        _window->setRenderer(renderer);
        
        return _window;
    }

    int EGLApp::run(ScreenRecorderPtr recorder)
    {
        if (recorder)
        {
            //! Do nothing.
        }

        assert( validateApplication() );

        while (_window != nullptr)
        {
            eglWaitGL();

            if (_window->isUpdateEnabled())
            {
                //! Do nothing.
            }
        }

        terminate();

        return 0;
    }

    int EGLApp::runWithLimitedFrames(int numberOfFrames, double fps, ScreenRecorderPtr recorder)
    {
        if(recorder)
        {
            //! Do nothing
        }

        return 0;
    }

    void EGLApp::terminate()
    {   
        if (_display) 
            eglTerminate(_display);
    }

    bool EGLApp::validateApplication() 
    {
        return (_display != nullptr) && 
               (_window  != nullptr);
    }

} 
}