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

    Simulation::~Simulation()
    {
        //! Do nothing.
    }

    void Simulation::setup(WindowPtr window)
    {
        _frame = Frame(0, 1.0 / 60.0);
        onResetView(window);
        onSetup(window);
    }

    void Simulation::resetSimulation()
    {
        _frame = Frame(0, 1.0 / 60.0);
        onResetSimulation();
    }

    void Simulation::advanceSimulation()
    {
        std::cout << "Frame index : " << _frame.index << std::endl;
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