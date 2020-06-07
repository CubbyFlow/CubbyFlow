/*************************************************************************
> File Name: OfflineSimulator.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: A fluid simulator with prepared offline simulated particle files.
> Created Time: 2020/04/17
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_OFFLINE_SIMULATION_H
#define CUBBYFLOW_OFFLINE_SIMULATION_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/Renderable/PointsRenderable.h>
#include <Core/Vector/Vector3.h>
#include <Core/Animation/Frame.h>
#include <memory>

//!
//! \brief  An implemenation of SPH derived by Simulation base class.. 
//!
class OfflineSimulation
{
public:
    //! Default constructor.
    OfflineSimulation();

    //! Constructor with fps.
    OfflineSimulation(double fps);

    //! Constructor with origin and fps.
    OfflineSimulation(double fps, CubbyFlow::Vector3F origin);

    //! Default destructor.
    ~OfflineSimulation();

    //! Set offline particle file parser.
    void setParticleParser(CubbyFlow::CubbyRender::ParticleParserPtr parser);

    //! Setup the offline simualtor.
    void setup(CubbyFlow::CubbyRender::RendererPtr renderer);

    //! update renderables vertex buffer with result of the solver.
    void updateRenderable();
protected:
    //! implementation of the reset simulation method.
    void resetSimulation();

    //! implementation of the advance simulation.
    void advanceSimulation();
private:
    CubbyFlow::CubbyRender::ParticleParserPtr _particleParser;
    CubbyFlow::CubbyRender::PointsRenderablePtr _renderable;
    CubbyFlow::Vector3F _simulationOrigin;
    CubbyFlow::Frame _frame;
    double _fps = 60.0;
};

using OfflineSimulationnPtr = std::shared_ptr<OfflineSimulation>;

#endif