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

    void Simulation::setup(WindowPtr window)
    {
        
        _frame = Frame(0, 1.0 / _fps);
        onResetView(window);
        onSetup(window);
    }

    void Simulation::resetSimulation()
    {
        _frame = Frame(0, 1.0 / _fps);
        onResetSimulation();
    }

    void Simulation::advanceSimulation()
    {
        std::cout << "\33[2K\rFrame index : " << _frame.index << std::flush;
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