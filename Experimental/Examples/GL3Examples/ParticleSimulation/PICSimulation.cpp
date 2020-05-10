/*************************************************************************
> File Name: PICSimulation.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: An implemenation of PIC derived by Simulation base class
> Created Time: 2020/04/17
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include "PICSimulation.h"

#include <Framework/View/Camera.h>
#include <Framework/View/CameraController.h>
#include <Framework/View/PerspectiveCamera.h>
#include <Framework/View/CameraState.h>
#include <Framework/View/Viewport.h>
#include <Framework/Window/Docker.h>
#include <Framework/Renderer/Renderer.h>
#include <Framework/Buffer/Framebuffer.h>
#include <Core/Collider/RigidBodyCollider3.h>
#include <Core/Emitter/VolumeParticleEmitter3.h>
#include <Core/Geometry/Box3.h>
#include <Core/Geometry/Cylinder3.h>
#include <Core/Geometry/Plane3.h>
#include <Core/Geometry/Sphere3.h>
#include <Core/Particle/ParticleSystemData3.h>
#include <Core/Surface/ImplicitSurfaceSet3.h>
#include <Core/PointGenerator/GridPointGenerator3.h>
#include <Core/Utils/Constants.h>
#include <random>
#include <limits>

using namespace CubbyFlow;
using namespace CubbyRender;

PICSimulation::PICSimulation()
{
    //! Do nothing
}

PICSimulation::PICSimulation(double fps)
    : Simulation(fps)
{
    //! Do nothing
}

PICSimulation::PICSimulation(size_t resolutionX, double fps)
    : Simulation(fps), _resolutionX(resolutionX)
{
    //! Do nothing
}

PICSimulation::~PICSimulation()
{
    //! Do nothing
}

std::string PICSimulation::name() const
{
    return "PIC Simulation";
}

void PICSimulation::onSetup(RendererPtr renderer)
{
    onResetSimulation();

    auto particles = _solver->GetParticleSystemData();
    auto positions = particles->GetPositions();
    _positions.Resize(particles->GetNumberOfParticles());
    _colors.Resize(particles->GetNumberOfParticles());

    ParallelFor(ZERO_SIZE, particles->GetNumberOfParticles(), [&](size_t i){
        _positions[i] = Vector3F(positions[i].x, positions[i].y, 0.0f);
        _colors[i] = Vector4F(0.0f, 0.0f, 0.8f, 1.0f);
    });

    _renderable = std::make_shared<PointsRenderable>(
        _positions, _colors, 0.5f
    );

    renderer->addRenderable(_renderable);
    renderer->setBackgroundColor(Vector4F(0.1f, 0.1f, 0.1f, 1.0f));
}

void PICSimulation::onResetView(DockerPtr docker)
{
    Size2 framebufferSize = docker->getFramebuffer()->getViewportSize();
    Viewport viewport;
    viewport.leftTop     = Vector2F(0.0f, framebufferSize.y);
    viewport.rightBottom = Vector2F(framebufferSize.x, 0.0f);

    CameraState camState;
    camState.viewport = viewport;
    camState.origin = Vector3F(1, -1, -3);
    camState.lookAt = Vector3F(0, 0,  1);

    docker->setCameraController(std::make_shared<CameraController>(
        std::make_shared<PerspectiveCamera>(camState, 60.0f)
    ));
}

void PICSimulation::onResetSimulation()
{
    // Build solver
    const Size3 resolution{ 3 * _resolutionX, 2 * _resolutionX, (3 * _resolutionX) / 2 };
    _solver = PICSolver3::Builder()
        .WithResolution(resolution)
        .WithDomainSizeX(3.0)
        .MakeShared();
    _solver->SetUseCompressedLinearSystem(true);

    const auto grids = _solver->GetGridSystemData();
    const double dx = grids->GetGridSpacing().x;
    const BoundingBox3D domain = grids->GetBoundingBox();
    const double lz = domain.GetDepth();

    // Build emitter
    const auto box1 = Box3::Builder()
        .WithLowerCorner({ 0, 0, 0 })
        .WithUpperCorner({ 0.5 + 0.001, 0.75 + 0.001, 0.75 * lz + 0.001 })
        .MakeShared();

    const auto box2 = Box3::Builder()
        .WithLowerCorner({ 2.5 - 0.001, 0, 0.25 * lz - 0.001 })
        .WithUpperCorner({ 3.5 + 0.001, 0.75 + 0.001, 1.5 * lz + 0.001 })
        .MakeShared();

    const auto boxSet = ImplicitSurfaceSet3::Builder()
        .WithExplicitSurfaces({ box1, box2 })
        .MakeShared();

    auto emitter = VolumeParticleEmitter3::Builder()
        .WithSurface(boxSet)
        .WithMaxRegion(domain)
        .WithSpacing(0.5 * dx)
        .MakeShared();

    emitter->SetPointGenerator(std::make_shared<GridPointGenerator3>());
    _solver->SetParticleEmitter(emitter);

    // Build collider
    const auto cyl1 = Cylinder3::Builder()
        .WithCenter({ 1, 0.375, 0.375 })
        .WithRadius(0.1)
        .WithHeight(0.75)
        .MakeShared();

    const auto cyl2 = Cylinder3::Builder()
        .WithCenter({ 1.5, 0.375, 0.75 })
        .WithRadius(0.1)
        .WithHeight(0.75)
        .MakeShared();

    const auto cyl3 = Cylinder3::Builder()
        .WithCenter({ 2, 0.375, 1.125 })
        .WithRadius(0.1)
        .WithHeight(0.75)
        .MakeShared();

    const auto cylSet = ImplicitSurfaceSet3::Builder()
        .WithExplicitSurfaces({ cyl1, cyl2, cyl3 })
        .MakeShared();

    const auto collider = RigidBodyCollider3::Builder()
        .WithSurface(cylSet)
        .MakeShared();

    _solver->SetCollider(collider);
}

void PICSimulation::onAdvanceSimulation()
{
    _solver->Update(_frame);
}

void PICSimulation::onUpdateRenderables()
{
    auto particles = _solver->GetParticleSystemData();
    auto pos2D = particles->GetPositions();

    size_t oldNumParticles = _positions.size();
    size_t newNumParticles = particles->GetNumberOfParticles();
    _positions.Resize(newNumParticles);
    _colors.Resize(newNumParticles);

    for (size_t i = 0; i < newNumParticles; ++i) {
        Vector3F pt((float)pos2D[i].x, (float)pos2D[i].y, 0);
        _positions[i] = pt;
    }

    for (size_t i = oldNumParticles; i < newNumParticles; ++i) {
        _colors[i] = Vector4F(0.0f, 0.0f, 0.8f, 1.0f);
    }
    _renderable->update(_positions, _colors);
}