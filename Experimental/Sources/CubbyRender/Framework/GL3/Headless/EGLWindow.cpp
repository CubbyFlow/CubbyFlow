/*************************************************************************
> File Name: EGLWindow.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: EGL Wrapper window class
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/GL3/Headless/EGLWindow.h>
#include <Framework/GL3/GL3Renderer.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    EGLWindow::EGLWindow(EGLDisplay display, const std::string& title, int width, int height)
        : _display(display)
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
        EGLContext context = eglCreateContext(_display, config, EGL_NO_CONTEXT, NULL);
        eglMakeCurrent(_display, _surface, _surface, context);
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
