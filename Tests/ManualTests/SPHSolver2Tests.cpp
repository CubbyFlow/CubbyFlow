#include "gtest/gtest.h"

#include <ManualTests.hpp>

#include <Core/Emitter/VolumeParticleEmitter2.hpp>
#include <Core/Geometry/Box.hpp>
#include <Core/Geometry/ImplicitSurfaceSet.hpp>
#include <Core/Geometry/Plane.hpp>
#include <Core/Geometry/RigidBodyCollider.hpp>
#include <Core/Geometry/Sphere.hpp>
#include <Core/Geometry/SurfaceToImplicit.hpp>
#include <Core/Solver/Particle/SPH/SPHSolver2.hpp>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(SPHSolver2);

CUBBYFLOW_BEGIN_TEST_F(SPHSolver2, SteadyState)
{
    SPHSolver2 solver;
    solver.SetViscosityCoefficient(0.1);
    solver.SetPseudoViscosityCoefficient(10.0);

    SPHSystemData2Ptr particles = solver.GetSPHSystemData();
    const double targetSpacing = particles->TargetSpacing();

    BoundingBox2D initialBound(Vector2D(), Vector2D(1, 0.5));
    initialBound.Expand(-targetSpacing);

    auto emitter = std::make_shared<VolumeParticleEmitter2>(
        std::make_shared<SurfaceToImplicit2>(
            std::make_shared<Sphere2>(Vector2D(), 10.0)),
        initialBound, particles->TargetSpacing(), Vector2D());
    emitter->SetJitter(0.0);
    solver.SetEmitter(emitter);

    Box2Ptr box = std::make_shared<Box2>(Vector2D(), Vector2D(1, 1));
    box->isNormalFlipped = true;
    RigidBodyCollider2Ptr collider = std::make_shared<RigidBodyCollider2>(box);

    solver.SetCollider(collider);

    SaveParticleDataXY(particles, 0);

    for (Frame frame(0, 1.0 / 60.0); frame.index < 100; frame.Advance())
    {
        solver.Update(frame);

        SaveParticleDataXY(particles, frame.index);
    }
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(SPHSolver2, WaterDrop)
{
    const double targetSpacing = 0.02;

    BoundingBox2D domain(Vector2D(), Vector2D(1, 2));

    // Initialize solvers
    SPHSolver2 solver;
    solver.SetPseudoViscosityCoefficient(0.0);

    SPHSystemData2Ptr particles = solver.GetSPHSystemData();
    particles->SetTargetDensity(1000.0);
    particles->SetTargetSpacing(targetSpacing);

    // Initialize source
    ImplicitSurfaceSet2Ptr surfaceSet = std::make_shared<ImplicitSurfaceSet2>();
    surfaceSet->AddExplicitSurface(std::make_shared<Plane2>(
        Vector2D(0, 1), Vector2D(0, 0.25 * domain.Height())));
    surfaceSet->AddExplicitSurface(
        std::make_shared<Sphere2>(domain.MidPoint(), 0.15 * domain.Width()));

    BoundingBox2D sourceBound(domain);
    sourceBound.Expand(-targetSpacing);

    auto emitter = std::make_shared<VolumeParticleEmitter2>(
        surfaceSet, sourceBound, targetSpacing, Vector2D());
    solver.SetEmitter(emitter);

    // Initialize boundary
    Box2Ptr box = std::make_shared<Box2>(domain);
    box->isNormalFlipped = true;
    RigidBodyCollider2Ptr collider = std::make_shared<RigidBodyCollider2>(box);

    // Setup solver
    solver.SetCollider(collider);

    SaveParticleDataXY(particles, 0);

    for (Frame frame(0, 1.0 / 60.0); frame.index < 120; frame.Advance())
    {
        solver.Update(frame);

        SaveParticleDataXY(particles, frame.index);
    }
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(SPHSolver2, WaterDropLargeDt)
{
    const double targetSpacing = 0.02;

    BoundingBox2D domain(Vector2D(), Vector2D(1, 2));

    // Initialize solvers
    SPHSolver2 solver;
    solver.SetPseudoViscosityCoefficient(0.0);

    SPHSystemData2Ptr particles = solver.GetSPHSystemData();
    particles->SetTargetDensity(1000.0);
    particles->SetTargetSpacing(targetSpacing);

    // Initialize source
    ImplicitSurfaceSet2Ptr surfaceSet = std::make_shared<ImplicitSurfaceSet2>();
    surfaceSet->AddExplicitSurface(std::make_shared<Plane2>(
        Vector2D(0, 1), Vector2D(0, 0.25 * domain.Height())));
    surfaceSet->AddExplicitSurface(
        std::make_shared<Sphere2>(domain.MidPoint(), 0.15 * domain.Width()));

    BoundingBox2D sourceBound(domain);
    sourceBound.Expand(-targetSpacing);

    auto emitter = std::make_shared<VolumeParticleEmitter2>(
        surfaceSet, sourceBound, targetSpacing, Vector2D());
    solver.SetEmitter(emitter);

    // Initialize boundary
    Box2Ptr box = std::make_shared<Box2>(domain);
    box->isNormalFlipped = true;
    RigidBodyCollider2Ptr collider = std::make_shared<RigidBodyCollider2>(box);

    // Setup solver - allow up to 10x from suggested time-step
    solver.SetCollider(collider);
    solver.SetTimeStepLimitScale(10.0);

    SaveParticleDataXY(particles, 0);

    for (Frame frame(0, 1.0 / 60.0); frame.index < 120; frame.Advance())
    {
        solver.Update(frame);

        SaveParticleDataXY(particles, frame.index);
    }
}
CUBBYFLOW_END_TEST_F