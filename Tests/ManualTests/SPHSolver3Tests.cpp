#include "pch.h"

#include <ManualTests.h>

#include <Collider/RigidBodyCollider3.h>
#include <Emitter/VolumeParticleEmitter3.h>
#include <Geometry/Box3.h>
#include <Geometry/Plane3.h>
#include <Geometry/Sphere3.h>
#include <Solver/Particle/SPH/SPHSolver3.h>
#include <Surface/ImplicitSurfaceSet3.h>
#include <Surface/SurfaceToImplicit3.h>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(SPHSolver3);

CUBBYFLOW_BEGIN_TEST_F(SPHSolver3, SteadyState)
{
	SPHSolver3 solver;
	solver.SetViscosityCoefficient(0.1);
	solver.SetPseudoViscosityCoefficient(10.0);

	SPHSystemData3Ptr particles = solver.GetSPHSystemData();
	const double targetSpacing = particles->GetTargetSpacing();

	BoundingBox3D initialBound(Vector3D(), Vector3D(1, 0.5, 1));
	initialBound.Expand(-targetSpacing);

	auto emitter = std::make_shared<VolumeParticleEmitter3>(
		std::make_shared<SurfaceToImplicit3>(std::make_shared<Sphere3>(Vector3D(), 10.0)),
		initialBound,
		particles->GetTargetSpacing(),
		Vector3D());
	emitter->SetJitter(0.0);
	solver.SetEmitter(emitter);

	Box3Ptr box = std::make_shared<Box3>(Vector3D(), Vector3D(1, 1, 1));
	box->isNormalFlipped = true;
	RigidBodyCollider3Ptr collider = std::make_shared<RigidBodyCollider3>(box);

	solver.SetCollider(collider);

	SaveParticleDataXY(particles, 0);

	for (Frame frame(0, 1.0 / 60.0); frame.index < 100; frame.Advance())
	{
		solver.Update(frame);

		SaveParticleDataXY(particles, frame.index);
	}
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(SPHSolver3, WaterDrop)
{
	const double targetSpacing = 0.02;

	BoundingBox3D domain(Vector3D(), Vector3D(1, 2, 0.5));

	// Initialize solvers
	SPHSolver3 solver;
	solver.SetPseudoViscosityCoefficient(0.0);

	SPHSystemData3Ptr particles = solver.GetSPHSystemData();
	particles->SetTargetDensity(1000.0);
	particles->SetTargetSpacing(targetSpacing);

	// Initialize source
	ImplicitSurfaceSet3Ptr surfaceSet = std::make_shared<ImplicitSurfaceSet3>();
	surfaceSet->AddExplicitSurface(std::make_shared<Plane3>(Vector3D(0, 1, 0), Vector3D(0, 0.25 * domain.GetHeight(), 0)));
	surfaceSet->AddExplicitSurface(std::make_shared<Sphere3>(domain.MidPoint(), 0.15 * domain.GetWidth()));

	BoundingBox3D sourceBound(domain);
	sourceBound.Expand(-targetSpacing);

	auto emitter = std::make_shared<VolumeParticleEmitter3>(
		surfaceSet,
		sourceBound,
		targetSpacing,
		Vector3D());
	solver.SetEmitter(emitter);

	// Initialize boundary
	Box3Ptr box = std::make_shared<Box3>(domain);
	box->isNormalFlipped = true;
	RigidBodyCollider3Ptr collider = std::make_shared<RigidBodyCollider3>(box);
	solver.SetCollider(collider);

	// Make it fast, but stable
	solver.SetViscosityCoefficient(0.01);
	solver.SetTimeStepLimitScale(5.0);

	SaveParticleDataXY(particles, 0);

	for (Frame frame(0, 1.0 / 60.0); frame.index < 100; frame.Advance())
	{
		solver.Update(frame);

		SaveParticleDataXY(particles, frame.index);
	}
}
CUBBYFLOW_END_TEST_F