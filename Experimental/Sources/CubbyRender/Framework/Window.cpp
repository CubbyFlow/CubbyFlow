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
#include <Core/Point/Point2.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    Window::Window()
    {
        //! Do nothing.
    }

    Window::Window(const std::string& title, int width, int height)
        : _title(title), _windowSize(width, height)
    {
        //! Do nothing.
    }

    Window::~Window()
    {
        //! Do nothing.
    }

    Vector2F Window::displayScalingFactor() const
    {
        Point2F factor = (getFramebufferSize().CastTo<float>() / getWindowSize().CastTo<float>());
        return Vector2F { factor.x, factor.y };
    }

    void Window::requestRender(unsigned int numFrames)
    {
        //! Do nothing.
        UNUSED_VARIABLE(numFrames);
    }

    void Window::setSwapInterval(int interval)
    {
        _swapInterval = interval;
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

    RendererPtr Window::getRenderer()
    {
        return _renderer;
    }

    void Window::update()
    {
        onUpdate();
    }
} 
}