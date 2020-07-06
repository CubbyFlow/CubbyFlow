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
#include <Framework/View/PerspectiveCamera.h>
#include <Framework/View/CameraController.h>
#include <Framework/View/Pivot.h>
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
#include <Core/Solver/Particle/PCISPH/PCISPHSolver3.h>
#include <Core/Solver/Particle/SPH/SPHSolver3.h>
#include <Core/Surface/ImplicitSurfaceSet3.h>
#include <Core/Utils/Constants.h>
#include <random>
#include <limits>

using namespace CubbyFlow;
using namespace CubbyRender;

SPHSimulation::SPHSimulation()
{
    //! Do nothing
}

SPHSimulation::SPHSimulation(double fps)
    : Simulation(fps)
{
    //! Do nothing
}

SPHSimulation::SPHSimulation(float spacing, double fps)
    : Simulation(fps), _spacing(spacing)
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

void SPHSimulation::onSetup(RendererPtr renderer)
{
    onResetSimulation();

    auto particles = _solver->GetParticleSystemData();
    auto positions = particles->GetPositions();
    
    Array1<float> vertices(particles->GetNumberOfParticles() * 3);
    positions.ParallelForEachIndex([&](size_t index){
        const size_t baseIndex = index * 3;
        vertices[  baseIndex  ] = static_cast<float>(positions[index].x);
        vertices[baseIndex + 1] = static_cast<float>(positions[index].y);
        vertices[baseIndex + 2] = 0.0f;
    });

    _renderable = std::make_shared<PointsRenderable>(vertices, Vector3F(221 / 255.0f, 145 / 255.0f, 48 / 255.0f));

    renderer->addRenderable(_renderable);
    renderer->setBackgroundColor(Vector4F(0.1f, 0.1f, 0.1f, 1.0f));
}

void SPHSimulation::onResetView(DockerPtr docker)
{
    Size2 framebufferSize = docker->getFramebuffer()->getViewportSize();
    Viewport viewport;
    viewport.leftTop     = Vector2F(0.0f, framebufferSize.y);
    viewport.rightBottom = Vector2F(framebufferSize.x, 0.0f);

    Pivot pivot;
    pivot.origin = Vector3F(0, 0, 4);
    pivot.lookAt = Vector3F(0, 0, -1);
    pivot.viewport = viewport;
    
    CameraPtr camera = std::make_shared<PerspectiveCamera>(pivot, 60.0f);
    
    docker->setCameraController(std::make_shared<CameraController>(camera));
}

void SPHSimulation::onResetSimulation()
{
    BoundingBox3D domain(Vector3D(-0.1f, -0.1f, -0.1f), Vector3D(0.1f, 0.1f, 0.1f));

	_solver = SPHSolver3::GetBuilder()
		.WithTargetDensity(1000.0)
		.WithTargetSpacing(_spacing)
		.MakeShared();

	_solver->SetPseudoViscosityCoefficient(0.0);

	// Build emitter
	BoundingBox3D sourceBound(domain);
	sourceBound.Expand(-_spacing * 8);

	const auto plane = Plane3::GetBuilder()
		.WithNormal({ 0, 1, 0 })
		.WithPoint({ 0, 0.25 * domain.GetHeight(), 0 })
		.MakeShared();

	const auto sphere = Sphere3::GetBuilder()
		.WithCenter(domain.MidPoint())
		.WithRadius(0.25 * domain.GetWidth())
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
    auto positions = particles->GetPositions();
    
    Array1<float> vertices(particles->GetNumberOfParticles() * 3);
    positions.ParallelForEachIndex([&](size_t index){
        const size_t baseIndex = index * 3;
        vertices[  baseIndex  ] = static_cast<float>(positions[index].x);
        vertices[baseIndex + 1] = static_cast<float>(positions[index].y);
        vertices[baseIndex + 2] = 0.0f;
    });

    _renderable->update(vertices);
}