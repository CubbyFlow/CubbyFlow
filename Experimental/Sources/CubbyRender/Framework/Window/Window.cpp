/*************************************************************************
> File Name: Window.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for WIndow instance.
> Created Time: 2020/02/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/Window/Window.h>
#include <Framework/Renderer/Renderer.h>
#include <Framework/Utils/Common.h>
#include <Framework/View/CameraController.h>
#include <Framework/Simulation/Simulation.h>
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
        _numRequestedRenderFrames = std::max(_numRequestedRenderFrames, numFrames);
    }

    void Window::setSwapInterval(int interval)
    {
        _swapInterval = interval;
    }

    const RendererPtr& Window::getRenderer() const
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

    void Window::render()
    {
        _renderer->render();
    }

    void Window::update()
    {
        if (_simulation)
        {
            _simulation->advanceSimulation();
        }
        onUpdate();
    }

    void Window::setViewport(int x, int y, size_t width, size_t height)
    {
        _renderer->setViewport(x, y, width, height);
    }

    void Window::registerSimulation(SimulationPtr simulation)
    {
        _simulation = simulation;
        _simulation->setup(shared_from_this());
    }

    const CameraControllerPtr& Window::getCameraController() const
    {
        return _camController;
    }

    void Window::setCameraController(CameraControllerPtr camController)
    {
        _camController = camController;
        _renderer->setCamera(_camController->getCamera());
    }

    unsigned int Window::getNumRequestedRenderFrames() const
    {
        return _numRequestedRenderFrames;
    }
    
    unsigned int& Window::getNumRequestedRenderFrames()
    {
        return _numRequestedRenderFrames;
    }
    
    ArrayAccessor1<unsigned char> Window::getCurrentScreen(Size2 size) const
    {
        return _renderer->getCurrentFramebuffer(size);
    }
} 
}