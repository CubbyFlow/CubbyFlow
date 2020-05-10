/*************************************************************************
> File Name: Simulation.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The abstract base class for the fluid simulation.  
> Created Time: 2020/04/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Simulation/Simulation.h>
#include <memory>
#include <iostream>

namespace CubbyFlow {
namespace CubbyRender {
    
    Simulation::Simulation()
    {
        //! Do nothing.
    }

    Simulation::Simulation(double fps)
        : _fps(fps)
    {

    }

    Simulation::~Simulation()
    {
        //! Do nothing.
    }

    void Simulation::setup(RendererPtr renderer, DockerPtr docker)
    {
        
        _frame = Frame(0, 1.0 / _fps);
        onResetView(docker);
        onSetup(renderer);
    }

    void Simulation::resetSimulation()
    {
        _frame = Frame(0, 1.0 / _fps);
        onResetSimulation();
    }

    void Simulation::advanceSimulation()
    {
        onAdvanceSimulation();
        onUpdateRenderables();
        _frame++;
    }

    const Frame& Simulation::getCurrentFrame() const
    {
        return _frame;
    }
} 
}