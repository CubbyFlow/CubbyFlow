/*************************************************************************
> File Name: Docker.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The docker class, simulate registered solver and 
           generate result texture with a shared renderer context.
> Created Time: 2020/05/06
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Window/Docker.h>
#include <Framework/Simulation/Simulation.h>
#include <Framework/Renderer/Renderer.h>
#include <Framework/View/CameraController.h>
#include <Framework/Buffer/Framebuffer.h>

namespace CubbyFlow {
namespace CubbyRender {

    Docker::Docker()
    {
        //! Do nothing.
    }

    Docker::Docker(const std::string& title)
        : _dockerTitle(title)
    {
        //! Do nothing.
    }

    Docker::~Docker()
    {
        _framebuffer.reset();
        _simulation.reset();
        _camController.reset();
    }

    void Docker::setDockerTitle(const std::string& title)
    {
        _dockerTitle = title;
    }

    std::string Docker::title() const
    {
        return _dockerTitle;
    }

    void Docker::setCameraController(CameraControllerPtr camController)
    {
        _camController = camController;
    }

    const CameraControllerPtr& Docker::getCameraController() const
    {
        return _camController;
    }

    void Docker::registerSimulation(RendererPtr renderer, SimulationPtr simulation)
    {
        _simulation = simulation;
        _simulation->setup(renderer, shared_from_this());
    }

    void Docker::setFramebuffer(FramebufferPtr framebuffer)
    {
        _framebuffer = framebuffer;
    }

    const FramebufferPtr& Docker::getFramebuffer() const
    {
        return _framebuffer;
    }

    void Docker::updateDocker()
    {
        //! Do nothing.
    }

    void Docker::renderDocker(RendererPtr renderer)
    {
        //! Do nothing.
    }


} 
}
