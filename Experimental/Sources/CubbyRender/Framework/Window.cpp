/*************************************************************************
> File Name: Window.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for WIndow instance.
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Window.h>
#include <Framework/Common.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    Window::Window()
    {
        //! Do nothing.
    }

    Window::Window(const std::string& title, int width, int height)
        : _title(title), _size(width, height)
    {
        //! Do nothing.
    }

    Window::~Window()
    {
        //! Do nothing.
    }

    Size2 Window::displayScalingFactor() const
    {
        return framebufferSize() / windowSize();
    }

    void Window::requestRender(unsigned int numFrames)
    {
        //! Do nothing.
        UNUSED_VARIABLE(numFrames);
    }

    void Window::setSwapInterval(int interval)
    {
        //! Do nothing.
        UNUSED_VARIABLE(interval);
    }

    const RendererPtr& Window::renderer() const
    {
        return _renderer;
    }

    bool Window::isUpdateEnabled() const
    {
        return _isUpdateEnabled;
    }

    void Window::setIsUpdateEnabled(bool enabled)
    {
        _isUpdateEnabled = enabled;
    }

    std::string Window::getTitle()
    {
        return _title;
    }

    void Window::setRenderer(RendererPtr renderer)
    {
        _renderer = renderer;
    }

} 
}