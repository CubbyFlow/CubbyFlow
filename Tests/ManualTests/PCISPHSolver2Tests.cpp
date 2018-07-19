#include "pch.h"

#include <ManualTests.h>

#include <Collider/RigidBodyCollider2.h>
#include <Emitter/VolumeParticleEmitter2.h>
#include <Geometry/Box2.h>
#include <Geometry/Plane2.h>
#include <Geometry/Sphere2.h>
#include <Solver/Particle/PCISPH/PCISPHSolver2.h>
#include <Surface/ImplicitSurfaceSet2.h>
#include <Surface/SurfaceToImplicit2.h>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(PCISPHSolver2);

CUBBYFLOW_BEGIN_TEST_F(PCISPHSolver2, SteadyState)
{
	PCISPHSolver2 solver;
	solver.SetViscosityCoefficient(0.1);
	solver.SetPseudoViscosityCoefficient(10.0);

	SPHSystemData2Ptr particles = solver.GetSPHSystemData();
	particles->SetTargetDensity(1000.0);
	const double targetSpacing = particles->GetTargetSpacing();

	BoundingBox2D initialBound(Vector2D(), Vector2D(1, 0.5));
	initialBound.Expand(-targetSpacing);

	auto emitter = std::make_shared<VolumeParticleEmitter2>(
		std::make_shared<SurfaceToImplicit2>(std::make_shared<Sphere2>(Vector2D(), 10.0)),
		initialBound,
		targetSpacing,
		Vector2D());
	emitter->SetJitter(0.0);
	solver.SetEmitter(emitter);

	Box2Ptr box = std::make_shared<Box2>(Vector2D(), Vector2D(1, 1));
	box->isNormalFlipped = true;
	RigidBodyCollider2Ptr collider = std::make_shared<RigidBodyCollider2>(box);
	solver.SetCollider(collider);

	SaveParticleDataXY(particles, 0);

	for (Frame frame(0, 1.0 / 60.0); frame.index < 100; ++frame)
	{
		solver.Update(frame);

		SaveParticleDataXY(particles, frame.index);
	}
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(PCISPHSolver2, WaterDrop)
{
	const double targetSpacing = 0.02;

	BoundingBox2D domain(Vector2D(), Vector2D(1, 2));

	// Initialize solvers
	PCISPHSolver2 solver;
	solver.SetPseudoViscosityCoefficient(0.0);

	SPHSystemData2Ptr particles = solver.GetSPHSystemData();
	particles->SetTargetDensity(1000.0);
	particles->SetTargetSpacing(targetSpacing);

	// Initialize source
	ImplicitSurfaceSet2Ptr surfaceSet = std::make_shared<ImplicitSurfaceSet2>();
	surfaceSet->AddExplicitSurface(std::make_shared<Plane2>(Vector2D(0, 1), Vector2D(0, 0.25 * domain.GetHeight())));
	surfaceSet->AddExplicitSurface(std::make_shared<Sphere2>(domain.MidPoint(), 0.15 * domain.GetWidth()));

	BoundingBox2D sourceBound(domain);
	sourceBound.Expand(-targetSpacing);

	auto emitter = std::make_shared<VolumeParticleEmitter2>(
		surfaceSet,
		sourceBound,
		targetSpacing,
		Vector2D());
	solver.SetEmitter(emitter);

	// Initialize boundary
	Box2Ptr box = std::make_shared<Box2>(domain);
	box->isNormalFlipped = true;
	RigidBodyCollider2Ptr collider = std::make_shared<RigidBodyCollider2>(box);
	solver.SetCollider(collider);

	SaveParticleDataXY(particles, 0);

	for (Frame frame(0, 1.0 / 60.0); frame.index < 120; ++frame)
	{
		solver.Update(frame);

		SaveParticleDataXY(particles, frame.index);
	}
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(PCISPHSolver2, RotatingTank)
{
	const double targetSpacing = 0.02;

	// Build solver
	auto solver = PCISPHSolver2::Builder()
		.WithTargetSpacing(targetSpacing)
		.MakeShared();

	solver->SetViscosityCoefficient(0.01);

	// Build emitter
	auto box = Box2::Builder()
		.WithLowerCorner({ 0.25 + targetSpacing, 0.25 + targetSpacing })
		.WithUpperCorner({ 0.75 - targetSpacing, 0.50 })
		.MakeShared();

	auto emitter = VolumeParticleEmitter2::Builder()
		.WithSurface(box)
		.WithSpacing(targetSpacing)
		.WithIsOneShot(true)
		.MakeShared();

	solver->SetEmitter(emitter);

	// Build collider
	auto tank = Box2::Builder()
		.WithLowerCorner({ -0.25, -0.25 })
		.WithUpperCorner({ 0.25,  0.25 })
		.WithTranslation({ 0.5, 0.5 })
		.WithOrientation(0.0)
		.WithIsNormalFlipped(true)
		.MakeShared();

	auto collider = RigidBodyCollider2::Builder()
		.WithSurface(tank)
		.WithAngularVelocity(2.0)
		.MakeShared();

	collider->SetOnBeginUpdateCallback([](Collider2* col, double t, double)
	{
		if (t < 1.0)
		{
			col->GetSurface()->transform.SetOrientation(2.0 * t);
			static_cast<RigidBodyCollider2*>(col)->angularVelocity = 2.0;
		}
		else
		{
			static_cast<RigidBodyCollider2*>(col)->angularVelocity = 0.0;
		}
	});

	solver->SetCollider(collider);

	for (Frame frame; frame.index < 120; ++frame)
	{
		solver->Update(frame);

		SaveParticleDataXY(solver->GetParticleSystemData(), frame.index);
	}
}
CUBBYFLOW_END_TEST_F