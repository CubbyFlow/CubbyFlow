/*************************************************************************
> File Name: PICSimulation.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: An implemenation of PIC derived by Simulation base class.
> Created Time: 2020/05/02
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_PIC_SIMULATION_H
#define CUBBYFLOW_PIC_SIMULATION_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Simulation/Simulation.h>
#include <Framework/Renderable/PointsRenderable.h>
#include <Core/Solver/Hybrid/PIC/PICSolver3.h>
#include <Core/Size/Size2.h>
#include <memory>

//!
//! \brief  An implemenation of PIC derived by Simulation base class
//!
class PICSimulation : public CubbyFlow::CubbyRender::Simulation
{
public:
    //! Default constructor.
    PICSimulation();
    
    //! Constructor with fps.
    PICSimulation(double fps);

    //! Constructor with resolution and fps.
    PICSimulation(size_t resolutionX, double fps);

    //! Default destructor.
    virtual ~PICSimulation();

    std::string name() const override;

protected:
    //! implementation of the setup method.
    void onSetup(CubbyFlow::CubbyRender::RendererPtr renderer) override;

    //! Reset camera view of the window 
    void onResetView(CubbyFlow::CubbyRender::DockerPtr docker) override;

    //! implementation of the reset simulation method.
    void onResetSimulation() override;

    //! implementation of the advance simulation.
    void onAdvanceSimulation() override;

    //! update renderables vertex buffer with result of the solver.
    void onUpdateRenderables() override;

private:
    CubbyFlow::PICSolver3Ptr _solver;
    CubbyFlow::Array1<CubbyFlow::Vector3F> _positions;
    CubbyFlow::Array1<CubbyFlow::Vector4F> _colors;
    CubbyFlow::CubbyRender::PointsRenderablePtr _renderable;
    size_t _resolutionX { 60 };
};
    
using PICSimulationPtr = std::shared_ptr<PICSimulation>;

#endif