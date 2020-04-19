/*************************************************************************
> File Name: SPHSimulation.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: An implemenation of SPH derived by Simulation base class.
> Created Time: 2020/04/17
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_SPH_SIMULATION_H
#define CUBBYFLOW_SPH_SIMULATION_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Simulation/Simulation.h>
#include <Framework/Renderable/PointsRenderable.h>
#include <Core/Solver/Particle/PCISPH/PCISPHSolver3.h>
#include <memory>

//!
//! \brief  An implemenation of SPH derived by Simulation base class.. 
//!
class SPHSimulation : public CubbyFlow::CubbyRender::Simulation
{
public:
    //! Default constructor.
    SPHSimulation();
    
    //! Constructor with fps.
    SPHSimulation(double fps);

    //! Default destructor.
    virtual ~SPHSimulation();

    std::string name() const override;

protected:
    //! implementation of the setup method.
    void onSetup(CubbyFlow::CubbyRender::WindowPtr window) override;

    //! Reset camera view of the window 
    void onResetView(CubbyFlow::CubbyRender::WindowPtr window) override;

    //! implementation of the reset simulation method.
    void onResetSimulation() override;

    //! implementation of the advance simulation.
    void onAdvanceSimulation() override;

    //! update renderables vertex buffer with result of the solver.
    void onUpdateRenderables() override;

private:
    CubbyFlow::PCISPHSolver3Ptr _solver;
    CubbyFlow::Array1<CubbyFlow::Vector3F> _positions;
    CubbyFlow::Array1<CubbyFlow::Vector4F> _colors;
    CubbyFlow::CubbyRender::PointsRenderablePtr _renderable;
    float _spacing = 0.01f;
};
    
using SPHSimulationPtr = std::shared_ptr<SPHSimulation>;

#endif