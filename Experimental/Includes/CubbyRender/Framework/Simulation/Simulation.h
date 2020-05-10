/*************************************************************************
> File Name: Simulation.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The abstract base class for the fluid simulation.  
> Created Time: 2020/04/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_SIMULATION_H
#define CUBBYFLOW_SIMULATION_H

#include <Framework/Utils/Prerequisites.h>
#include <Core/Animation/Frame.h>
#include <memory>
#include <string>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief he abstract base class for the fluid simulation. 
    //!
    //! This class contains method for update solver and renderable efficiently.
    //!
    class Simulation 
    {
    public:
        //! Default constructor.
        Simulation();

        //! Constructor with fps
        Simulation(double fps);

        //! Default destructor.
        virtual ~Simulation();

        virtual std::string name() const = 0;

        //! Setup simulation.
        void setup(RendererPtr renderer, DockerPtr docker);

        //! Reset simulation to beginning state without modifying parameters.
        void resetSimulation();

        //! Advance simulation with one frame.
        void advanceSimulation();

        //! Return current simulation frame.
        const Frame& getCurrentFrame() const;

    protected:
        //! implementation of the setup method.
        virtual void onSetup(RendererPtr renderer) = 0;

        //! Reset camera view of the window 
        virtual void onResetView(DockerPtr docker) = 0;

        //! implementation of the reset simulation method.
        virtual void onResetSimulation() = 0;

        //! implementation of the advance simulation.
        virtual void onAdvanceSimulation() = 0;

        //! update renderables vertex buffer with result of the solver.
        virtual void onUpdateRenderables() = 0;

        Frame _frame;
        double _fps = 60.0;
    private:
    };

    using SimulationPtr = std::shared_ptr<Simulation>;
} 
}

#endif