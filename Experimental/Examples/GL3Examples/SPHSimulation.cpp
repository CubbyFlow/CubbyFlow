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
#include <Framework/View/CameraController.h>
#include <Framework/View/PerspectiveCamera.h>
#include <Framework/View/CameraState.h>
#include <Framework/View/Viewport.h>
#include <Framework/Window/Window.h>
#include <Framework/Renderer/Renderer.h>
#include <Core/Collider/RigidBodyCollider3.h>
#include <Core/Emitter/VolumeParticleEmitter3.h>
#include <Core/Geometry/Box3.h>
#include <Core/Geometry/Cylinder3.h>
#include <Core/Geometry/Plane3.h>
#include <Core/Geometry/Sphere3.h>
#include <Core/Particle/ParticleSystemData3.h>
#include <Core/Solver/Particle/PCISPH/PCISPHSolver3.h>
#include <Core/Solver/Particle/SPH/SPHSolver3.h>
#include <Core/Surface/ImplicitSurfaceSet3.h>
#include <Core/Utils/Constants.h>
#include <random>


using namespace CubbyFlow;
using namespace CubbyRender;

SPHSimulation::SPHSimulation()
{
    //! Do nothing
}

SPHSimulation::SPHSimulation(double fps)
    : Simulation(fps)
{

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

    auto particles = _solver->GetParticleSystemData();
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
    Size2 framebufferSize = window->getFramebufferSize();
    Viewport viewport;
    viewport.leftTop     = Vector2F(0.0f, framebufferSize.y);
    viewport.rightBottom = Vector2F(framebufferSize.x, 0.0f);

    CameraState camState;
    camState.viewport = viewport;
    camState.origin = Vector3F(0, 1, 1);
    camState.lookAt = Vector3F(0, 0, -1);

    window->setCameraController(std::make_shared<CameraController>(
        std::make_shared<PerspectiveCamera>(camState, HALF_PI_FLOAT)
    ));
}

void SPHSimulation::onResetSimulation()
{
    BoundingBox3D domain(Vector3D(-0.2f, -1.0f, -0.2f), Vector3D(0.2f, 1.0f, 0.2f));

	_solver = PCISPHSolver3::GetBuilder()
		.WithTargetDensity(1000.0)
		.WithTargetSpacing(_spacing)
		.MakeShared();

	_solver->SetPseudoViscosityCoefficient(0.0);

	// Build emitter
	BoundingBox3D sourceBound(domain);
	sourceBound.Expand(-_spacing);

	const auto plane = Plane3::GetBuilder()
		.WithNormal({ 0, 1, 0 })
		.WithPoint({ 0, 0.25 * domain.GetHeight(), 0 })
		.MakeShared();

	const auto sphere = Sphere3::GetBuilder()
		.WithCenter(domain.MidPoint())
		.WithRadius(0.15 * domain.GetWidth())
		.MakeShared();

	const auto surfaceSet = ImplicitSurfaceSet3::GetBuilder()
		.WithExplicitSurfaces({ plane, sphere })
		.MakeShared();

	const auto emitter = VolumeParticleEmitter3::GetBuilder()
		.WithSurface(surfaceSet)
		.WithSpacing(_spacing)
		.WithMaxRegion(sourceBound)
		.WithIsOneShot(true)
		.MakeShared();

	_solver->SetEmitter(emitter);

	// Build collider
	const auto box = Box3::GetBuilder()
		.WithIsNormalFlipped(true)
		.WithBoundingBox(domain)
		.MakeShared();

	const auto collider = RigidBodyCollider3::GetBuilder()
		.WithSurface(box)
		.MakeShared();

	_solver->SetCollider(collider);
}

void SPHSimulation::onAdvanceSimulation()
{
    _solver->Update(_frame);
}

void SPHSimulation::onUpdateRenderables()
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

    std::mt19937 rng(0);
    std::uniform_real_distribution<float> dist2(0.0f, 1.0f);
    for (size_t i = oldNumParticles; i < newNumParticles; ++i) {
        _colors[i] = Vector4F(dist2(rng), dist2(rng), dist2(rng), 1.0f);
    }
    _renderable->update(_positions, _colors);
}
