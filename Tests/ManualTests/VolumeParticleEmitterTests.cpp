#include "pch.hpp"

#include <ManualTests.hpp>

#include <Core/Emitter/VolumeParticleEmitter2.hpp>
#include <Core/Emitter/VolumeParticleEmitter3.hpp>
#include <Core/Geometry/Sphere2.hpp>
#include <Core/Geometry/Sphere3.hpp>
#include <Core/Geometry/SurfaceToImplicit2.hpp>
#include <Core/Geometry/SurfaceToImplicit3.hpp>
#include <Core/Solver/Particle/ParticleSystemSolver2.hpp>
#include <Core/Solver/Particle/ParticleSystemSolver3.hpp>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(VolumeParticleEmitter2);

CUBBYFLOW_BEGIN_TEST_F(VolumeParticleEmitter2, EmitContinuousNonOverlapping)
{
    ParticleSystemSolver2 solver;

    ParticleSystemData2Ptr particles = solver.GetParticleSystemData();
    auto emitter = std::make_shared<VolumeParticleEmitter2>(
        std::make_shared<SurfaceToImplicit2>(
            std::make_shared<Sphere2>(Vector2D(), 1.0)),
        BoundingBox2D(Vector2D(-1, -1), Vector2D(1, 1)), 0.2);
    emitter->SetIsOneShot(false);
    emitter->SetAllowOverlapping(false);
    solver.SetEmitter(emitter);

    SaveParticleDataXY(particles, 0);

    for (Frame frame(0, 1.0 / 60.0); frame.index < 120; ++frame)
    {
        solver.Update(frame);

        SaveParticleDataXY(particles, frame.index);
    }
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_TESTS(VolumeParticleEmitter3);

CUBBYFLOW_BEGIN_TEST_F(VolumeParticleEmitter3, EmitContinuousNonOverlapping)
{
    ParticleSystemSolver3 solver;

    ParticleSystemData3Ptr particles = solver.GetParticleSystemData();
    auto emitter = std::make_shared<VolumeParticleEmitter3>(
        std::make_shared<SurfaceToImplicit3>(
            std::make_shared<Sphere3>(Vector3D(), 1.0)),
        BoundingBox3D(Vector3D(-1, -1, -1), Vector3D(1, 1, 1)), 0.2);
    emitter->SetIsOneShot(false);
    emitter->SetAllowOverlapping(false);
    solver.SetEmitter(emitter);

    SaveParticleDataXY(particles, 0);

    for (Frame frame(0, 1.0 / 60.0); frame.index < 120; ++frame)
    {
        solver.Update(frame);

        SaveParticleDataXY(particles, frame.index);
    }
}
CUBBYFLOW_END_TEST_F