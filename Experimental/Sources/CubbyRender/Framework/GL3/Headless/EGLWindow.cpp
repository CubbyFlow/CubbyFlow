/*************************************************************************
> File Name: EGLWindow.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: EGL Wrapper window class
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifdef CUBBYFLOW_USE_GL

#include <Framework/GL3/Headless/EGLWindow.h>
#include <Framework/GL3/GL3Renderer.h>
#include <Framework/Common.h>
#include <Framework/GL3/Headless/EGLCommon.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    EGLWindow::EGLWindow(EGLDisplay display, const std::string& title, int width, int height)
        : Window(title, width, height), _display(display)
    {
        constexpr EGLint configAttributes[] = {
            EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
            EGL_NONE
        };  
        static const EGLint pbufferAttribs[] = {
            EGL_WIDTH, width,
            EGL_HEIGHT, height,
            EGL_NONE,
        };
        EGLint numConfigs;
        EGLConfig config;
        eglChooseConfig(_display, configAttributes, &config, 1, &numConfigs);

        _surface = eglCreatePbufferSurface(_display, config, pbufferAttribs);
        EGL_ASSERT(_surface != EGL_NO_SURFACE, "Create Pbuffer surface");

        EGLContext context = eglCreateContext(_display, config, EGL_NO_CONTEXT, NULL);
        EGL_ASSERT(context != EGL_NO_CONTEXT, "Create Context with current Pbuffer surface");

        EGL_ASSERT(eglMakeCurrent(_display, _surface, _surface, context), "Binding current Context to render thread");
        CUBBYFLOW_INFO << "Create EGL Surface with " << width << "x" << height << " complete.";
    }

    EGLWindow::~EGLWindow()
    {
        eglDestroySurface(_display, _surface);
    }

    Size2 EGLWindow::framebufferSize() const
    {
        EGLint width, height;
        eglQuerySurface(_display, _surface, EGL_WIDTH, &width);
        eglQuerySurface(_display, _surface, EGL_HEIGHT, &height);

        return Size2(width, height);
    }

    Size2 EGLWindow::windowSize() const
    {
        return _size;
    }
}
}


#endif 