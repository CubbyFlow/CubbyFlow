/*************************************************************************
> File Name: Docker.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The docker class, simulate registered solver and generate result texture in an isolated env.
> Created Time: 2020/05/06
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_DOCKER_H
#define CUBBYFLOW_DOCKER_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Window/Window.h>
#include <Core/Size/Size2.h>
#include <string>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief Abstract base class for Window.
    //!
    class Docker : public std::enable_shared_from_this<Docker>
    {
    public: 
        //! Default Constructor.
        Docker();

        //! Construct with title and window size.
        Docker(const std::string& title);

        //! Default Destructor.
        ~Docker();

        //! Set the title of docker.
        void setDockerTitle(const std::string& title);

        //! Get the title of docker.
        std::string title() const;

        //! Set camera controller instance;
        void setCameraController(CameraControllerPtr camController);

        //! Get camera controller instance.
        const CameraControllerPtr& getCameraController() const;

        //! create docker and register simulation to it.
        void registerSimulation(RendererPtr renderer, SimulationPtr simulation);

        //! set framebuffer object
        void setFramebuffer(FramebufferPtr framebuffer);

        //! get framebuffer object.
        const FramebufferPtr& getFramebuffer() const;

        //! Update the framebuffer and simulation.
        void updateDocker();

        //! Render the simulation renderables to given renderer context.
        void renderDocker(RendererPtr renderer);

    protected:
    private:
        FramebufferPtr _framebuffer;
        SimulationPtr _simulation;
        CameraControllerPtr _camController;
        std::string _dockerTitle;
    };

} 
}

#endif
