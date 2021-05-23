#include "gtest/gtest.h"

#include <ManualTests.hpp>

#include <Core/Emitter/VolumeParticleEmitter3.hpp>
#include <Core/Geometry/Box.hpp>
#include <Core/Geometry/ImplicitSurfaceSet.hpp>
#include <Core/Geometry/Plane.hpp>
#include <Core/Geometry/RigidBodyCollider.hpp>
#include <Core/Geometry/Sphere.hpp>
#include <Core/Geometry/SurfaceToImplicit.hpp>
#include <Core/Solver/Particle/PCISPH/PCISPHSolver3.hpp>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(PCISPHSolver3);

CUBBYFLOW_BEGIN_TEST_F(PCISPHSolver3, SteadyState)
{
    PCISPHSolver3 solver;
    solver.SetViscosityCoefficient(0.1);
    solver.SetPseudoViscosityCoefficient(10.0);

    SPHSystemData3Ptr particles = solver.GetSPHSystemData();
    particles->SetTargetDensity(1000.0);
    const double targetSpacing = particles->TargetSpacing();

    BoundingBox3D initialBound(Vector3D(), Vector3D(1, 0.5, 1));
    initialBound.Expand(-targetSpacing);

    auto emitter = std::make_shared<VolumeParticleEmitter3>(
        std::make_shared<SurfaceToImplicit3>(
            std::make_shared<Sphere3>(Vector3D(), 10.0)),
        initialBound, targetSpacing, Vector3D());
    emitter->SetJitter(0.0);
    solver.SetEmitter(emitter);

    Box3Ptr box = std::make_shared<Box3>(Vector3D(), Vector3D(1, 1, 1));
    box->isNormalFlipped = true;
    RigidBodyCollider3Ptr collider = std::make_shared<RigidBodyCollider3>(box);
    solver.SetCollider(collider);

    SaveParticleDataXY(particles, 0);

    for (Frame frame(0, 1.0 / 60.0); frame.index < 100; ++frame)
    {
        solver.Update(frame);

        SaveParticleDataXY(particles, frame.index);
    }
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(PCISPHSolver3, WaterDrop)
{
    const double targetSpacing = 0.02;

    BoundingBox3D domain(Vector3D(), Vector3D(1, 2, 0.5));

    // Initialize solvers
    PCISPHSolver3 solver;
    solver.SetPseudoViscosityCoefficient(0.0);

    SPHSystemData3Ptr particles = solver.GetSPHSystemData();
    particles->SetTargetDensity(1000.0);
    particles->SetTargetSpacing(targetSpacing);

    // Initialize source
    ImplicitSurfaceSet3Ptr surfaceSet = std::make_shared<ImplicitSurfaceSet3>();
    surfaceSet->AddExplicitSurface(std::make_shared<Plane3>(
        Vector3D(0, 1, 0), Vector3D(0, 0.25 * domain.Height(), 0)));
    surfaceSet->AddExplicitSurface(
        std::make_shared<Sphere3>(domain.MidPoint(), 0.15 * domain.Width()));

    BoundingBox3D sourceBound(domain);
    sourceBound.Expand(-targetSpacing);

    auto emitter = std::make_shared<VolumeParticleEmitter3>(
        surfaceSet, sourceBound, targetSpacing, Vector3D());
    solver.SetEmitter(emitter);

    // Initialize boundary
    Box3Ptr box = std::make_shared<Box3>(domain);
    box->isNormalFlipped = true;
    RigidBodyCollider3Ptr collider = std::make_shared<RigidBodyCollider3>(box);
    solver.SetCollider(collider);

    SaveParticleDataXY(particles, 0);

    for (Frame frame(0, 1.0 / 60.0); frame.index < 100; ++frame)
    {
        solver.Update(frame);

        SaveParticleDataXY(particles, frame.index);
    }
}
CUBBYFLOW_END_TEST_F