/*************************************************************************
> File Name: SPHSimulation.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: An implemenation of SPH derived by Simulation base class.
> Created Time: 2020/04/17
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include "SPHSimulation.h"

#include <Framework/View/Camera.h>
#include <Framework/View/Viewport.h>
#include <Framework/Window/Window.h>
#include <Framework/Renderer/Renderer.h>
#include <Core/Particle/ParticleSystemData2.h>
#include <Core/Vector/Vector3.h>
#include <Core/Vector/Vector4.h>
#include <Core/Utils/Constants.h>
#include <random>

using namespace CubbyFlow;
using namespace CubbyRender;

SPHSimulation::SPHSimulation()
{
    //! Do nothing
}

SPHSimulation::~SPHSimulation()
{
    //! Do nothing
}

std::string SPHSimulation::name() const
{
    return "SPH Simulation";
}

void SPHSimulation::onSetup(WindowPtr window)
{
    onResetSimulation();

    auto particles = _solver.GetParticleSystemData();
    auto positions = particles->GetPositions();
    _positions.Resize(particles->GetNumberOfParticles());
    _colors.Resize(particles->GetNumberOfParticles());

    std::mt19937 rng(0);
    std::uniform_real_distribution<float> dist2(0.0f, 1.0f);
    ParallelFor(ZERO_SIZE, particles->GetNumberOfParticles(), [&](size_t i){
        _positions[i] = Vector3F(positions[i].x, positions[i].y, 0.0f);
        _colors[i] = Vector4F(dist2(rng), dist2(rng), dist2(rng), 1.0f);
    });

    _renderable = std::make_shared<PointsRenderable>(
        _positions, _colors, 0.5f
    );
    window->getRenderer()->addRenderable(_renderable);
    window->getRenderer()->setBackgroundColor(Vector4F(0.1f, 0.1f, 0.1f, 1.0f));
    window->setSwapInterval(1);
}

void SPHSimulation::onResetView(WindowPtr window)
{
    //Viewport viewport(0, 0, window->framebufferSize().x,
    //                  window->framebufferSize().y);
    //CameraState camera{.origin = Vector3F(0, 1, 1),
    //                   .lookAt = Vector3F(0, 0, -1),
    //                   .viewport = viewport};
    //window->setViewController(std::make_shared<OrthoViewController>(
    //                          std::make_shared<OrthoCamera>(camera, 0, 1, -1, 1)));
}

void SPHSimulation::onResetSimulation()
{

}

void SPHSimulation::onAdvanceSimulation()
{
    _solver.Update(_frame);
}

void SPHSimulation::onUpdateRenderables()
{

}
