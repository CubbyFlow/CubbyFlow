#include "pch.hpp"

#include <ManualTests.hpp>

#include <Core/Emitter/PointParticleEmitter3.hpp>
#include <Core/Field/ConstantVectorField3.hpp>
#include <Core/Geometry/Plane3.hpp>
#include <Core/Geometry/RigidBodyCollider3.hpp>
#include <Core/Solver/Particle/ParticleSystemSolver3.hpp>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(ParticleSystemSolver3);

CUBBYFLOW_BEGIN_TEST_F(ParticleSystemSolver3, PerfectBounce)
{
    Plane3Ptr plane = std::make_shared<Plane3>(Vector3D(0, 1, 0), Vector3D());
    RigidBodyCollider3Ptr collider =
        std::make_shared<RigidBodyCollider3>(plane);

    ParticleSystemSolver3 solver;
    solver.SetCollider(collider);
    solver.SetDragCoefficient(0.0);
    solver.SetRestitutionCoefficient(1.0);

    ParticleSystemData3Ptr particles = solver.GetParticleSystemData();
    particles->AddParticle({ 0.0, 3.0, 0.0 }, { 1.0, 0.0, 0.0 });

    Array1<double> x(1000);
    Array1<double> y(1000);
    char fileName[256];
    snprintf(fileName, sizeof(fileName), "data.#line2,0000,x.npy");
    SaveData(x.View(), 0, fileName);
    snprintf(fileName, sizeof(fileName), "data.#line2,0000,y.npy");
    SaveData(y.View(), 0, fileName);

    Frame frame;
    frame.timeIntervalInSeconds = 1.0 / 300.0;
    for (; frame.index < 1000; frame.Advance())
    {
        solver.Update(frame);

        x[frame.index] = particles->Positions()[0].x;
        y[frame.index] = particles->Positions()[0].y;
        snprintf(fileName, sizeof(fileName), "data.#line2,%04d,x.npy",
                 frame.index);
        SaveData(x.View(), frame.index, fileName);
        snprintf(fileName, sizeof(fileName), "data.#line2,%04d,y.npy",
                 frame.index);
        SaveData(y.View(), frame.index, fileName);
    }
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(ParticleSystemSolver3, HalfBounce)
{
    Plane3Ptr plane = std::make_shared<Plane3>(Vector3D(0, 1, 0), Vector3D());
    RigidBodyCollider3Ptr collider =
        std::make_shared<RigidBodyCollider3>(plane);

    ParticleSystemSolver3 solver;
    solver.SetCollider(collider);
    solver.SetDragCoefficient(0.0);
    solver.SetRestitutionCoefficient(0.5);

    ParticleSystemData3Ptr particles = solver.GetParticleSystemData();
    particles->AddParticle({ 0.0, 3.0, 0.0 }, { 1.0, 0.0, 0.0 });

    Array1<double> x(1000);
    Array1<double> y(1000);
    char fileName[256];
    snprintf(fileName, sizeof(fileName), "data.#line2,0000,x.npy");
    SaveData(x.View(), 0, fileName);
    snprintf(fileName, sizeof(fileName), "data.#line2,0000,y.npy");
    SaveData(y.View(), 0, fileName);

    Frame frame;
    frame.timeIntervalInSeconds = 1.0 / 300.0;
    for (; frame.index < 1000; frame.Advance())
    {
        solver.Update(frame);

        x[frame.index] = particles->Positions()[0].x;
        y[frame.index] = particles->Positions()[0].y;
        snprintf(fileName, sizeof(fileName), "data.#line2,%04d,x.npy",
                 frame.index);
        SaveData(x.View(), frame.index, fileName);
        snprintf(fileName, sizeof(fileName), "data.#line2,%04d,y.npy",
                 frame.index);
        SaveData(y.View(), frame.index, fileName);
    }
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(ParticleSystemSolver3, HalfBounceWithFriction)
{
    Plane3Ptr plane = std::make_shared<Plane3>(Vector3D(0, 1, 0), Vector3D());
    RigidBodyCollider3Ptr collider =
        std::make_shared<RigidBodyCollider3>(plane);
    collider->SetFrictionCoefficient(0.04);

    ParticleSystemSolver3 solver;
    solver.SetCollider(collider);
    solver.SetDragCoefficient(0.0);
    solver.SetRestitutionCoefficient(0.5);

    ParticleSystemData3Ptr particles = solver.GetParticleSystemData();
    particles->AddParticle({ 0.0, 3.0, 0.0 }, { 1.0, 0.0, 0.0 });

    Array1<double> x(1000);
    Array1<double> y(1000);
    char fileName[256];
    snprintf(fileName, sizeof(fileName), "data.#line2,0000,x.npy");
    SaveData(x.View(), 0, fileName);
    snprintf(fileName, sizeof(fileName), "data.#line2,0000,y.npy");
    SaveData(y.View(), 0, fileName);

    Frame frame;
    frame.timeIntervalInSeconds = 1.0 / 300.0;
    for (; frame.index < 1000; frame.Advance())
    {
        solver.Update(frame);

        x[frame.index] = particles->Positions()[0].x;
        y[frame.index] = particles->Positions()[0].y;
        snprintf(fileName, sizeof(fileName), "data.#line2,%04d,x.npy",
                 frame.index);
        SaveData(x.View(), frame.index, fileName);
        snprintf(fileName, sizeof(fileName), "data.#line2,%04d,y.npy",
                 frame.index);
        SaveData(y.View(), frame.index, fileName);
    }
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(ParticleSystemSolver3, NoBounce)
{
    Plane3Ptr plane = std::make_shared<Plane3>(Vector3D(0, 1, 0), Vector3D());
    RigidBodyCollider3Ptr collider =
        std::make_shared<RigidBodyCollider3>(plane);

    ParticleSystemSolver3 solver;
    solver.SetCollider(collider);
    solver.SetDragCoefficient(0.0);
    solver.SetRestitutionCoefficient(0.0);

    ParticleSystemData3Ptr particles = solver.GetParticleSystemData();
    particles->AddParticle({ 0.0, 3.0, 0.0 }, { 1.0, 0.0, 0.0 });

    Array1<double> x(1000);
    Array1<double> y(1000);
    char fileName[256];
    snprintf(fileName, sizeof(fileName), "data.#line2,0000,x.npy");
    SaveData(x.View(), 0, fileName);
    snprintf(fileName, sizeof(fileName), "data.#line2,0000,y.npy");
    SaveData(y.View(), 0, fileName);

    Frame frame;
    frame.timeIntervalInSeconds = 1.0 / 300.0;
    for (; frame.index < 1000; frame.Advance())
    {
        solver.Update(frame);

        x[frame.index] = particles->Positions()[0].x;
        y[frame.index] = particles->Positions()[0].y;
        snprintf(fileName, sizeof(fileName), "data.#line2,%04d,x.npy",
                 frame.index);
        SaveData(x.View(), frame.index, fileName);
        snprintf(fileName, sizeof(fileName), "data.#line2,%04d,y.npy",
                 frame.index);
        SaveData(y.View(), frame.index, fileName);
    }
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(ParticleSystemSolver3, Update)
{
    auto plane = Plane3::Builder()
                     .WithNormal({ 0, 1, 0 })
                     .WithPoint({ 0, 0, 0 })
                     .MakeShared();

    auto collider =
        RigidBodyCollider3::Builder().WithSurface(plane).MakeShared();

    auto wind =
        ConstantVectorField3::Builder().WithValue({ 1, 0, 0 }).MakeShared();

    auto emitter = PointParticleEmitter3::Builder()
                       .WithOrigin({ 0, 3, 0 })
                       .WithDirection({ 0, 1, 0 })
                       .WithSpeed(5)
                       .WithSpreadAngleInDegrees(45.0)
                       .WithMaxNumberOfNewParticlesPerSecond(300)
                       .MakeShared();

    auto solver = ParticleSystemSolver3::Builder().MakeShared();
    solver->SetCollider(collider);
    solver->SetEmitter(emitter);
    solver->SetWind(wind);
    solver->SetDragCoefficient(0.0);
    solver->SetRestitutionCoefficient(0.5);

    for (Frame frame(0, 1.0 / 60.0); frame.index < 360; ++frame)
    {
        solver->Update(frame);

        SaveParticleDataXY(solver->GetParticleSystemData(), frame.index);
    }
}
CUBBYFLOW_END_TEST_F