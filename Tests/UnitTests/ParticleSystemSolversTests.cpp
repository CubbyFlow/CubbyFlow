#include "gtest/gtest.h"

#include <Core/Solver/Particle/ParticleSystemSolver2.hpp>
#include <Core/Solver/Particle/ParticleSystemSolver3.hpp>

using namespace CubbyFlow;

TEST(ParticleSystemSolver2, Constructor)
{
    ParticleSystemSolver2 solver;

    auto data = solver.GetParticleSystemData();
    EXPECT_EQ(0u, data->NumberOfParticles());

    auto wind = solver.GetWind();
    EXPECT_TRUE(wind != nullptr);

    auto collider = solver.GetCollider();
    EXPECT_EQ(nullptr, collider);
}

TEST(ParticleSystemSolver2, BasicParams)
{
    ParticleSystemSolver2 solver;

    solver.SetDragCoefficient(6.0);
    EXPECT_DOUBLE_EQ(6.0, solver.GetDragCoefficient());

    solver.SetDragCoefficient(-7.0);
    EXPECT_DOUBLE_EQ(0.0, solver.GetDragCoefficient());

    solver.SetRestitutionCoefficient(0.5);
    EXPECT_DOUBLE_EQ(0.5, solver.GetRestitutionCoefficient());

    solver.SetRestitutionCoefficient(8.0);
    EXPECT_DOUBLE_EQ(1.0, solver.GetRestitutionCoefficient());

    solver.SetRestitutionCoefficient(-8.0);
    EXPECT_DOUBLE_EQ(0.0, solver.GetRestitutionCoefficient());

    solver.SetGravity(Vector2D(2, -10));
    EXPECT_EQ(Vector2D(2, -10), solver.GetGravity());
}

TEST(ParticleSystemSolver2, Update)
{
    ParticleSystemSolver2 solver;
    solver.SetGravity(Vector2D(0, -10));

    ParticleSystemData2Ptr data = solver.GetParticleSystemData();
    ParticleSystemData2::VectorData positions(10);
    data->AddParticles(positions);

    Frame frame(0, 1.0 / 60.0);
    solver.Update(frame);

    for (size_t i = 0; i < data->NumberOfParticles(); ++i)
    {
        EXPECT_DOUBLE_EQ(0.0, data->Positions()[i].x);
        EXPECT_NE(0, data->Positions()[i].y);

        EXPECT_DOUBLE_EQ(0.0, data->Velocities()[i].x);
        EXPECT_NE(0, data->Velocities()[i].y);
    }
}

TEST(ParticleSystemSolver3, Constructor)
{
    ParticleSystemSolver3 solver;

    auto data = solver.GetParticleSystemData();
    EXPECT_EQ(0u, data->NumberOfParticles());

    auto wind = solver.GetWind();
    EXPECT_TRUE(wind != nullptr);

    auto collider = solver.GetCollider();
    EXPECT_EQ(nullptr, collider);
}

TEST(ParticleSystemSolver3, BasicParams)
{
    ParticleSystemSolver3 solver;

    solver.SetDragCoefficient(6.0);
    EXPECT_DOUBLE_EQ(6.0, solver.GetDragCoefficient());

    solver.SetDragCoefficient(-7.0);
    EXPECT_DOUBLE_EQ(0.0, solver.GetDragCoefficient());

    solver.SetRestitutionCoefficient(0.5);
    EXPECT_DOUBLE_EQ(0.5, solver.GetRestitutionCoefficient());

    solver.SetRestitutionCoefficient(8.0);
    EXPECT_DOUBLE_EQ(1.0, solver.GetRestitutionCoefficient());

    solver.SetRestitutionCoefficient(-8.0);
    EXPECT_DOUBLE_EQ(0.0, solver.GetRestitutionCoefficient());

    solver.SetGravity(Vector3D(3, -10, 7));
    EXPECT_EQ(Vector3D(3, -10, 7), solver.GetGravity());
}

TEST(ParticleSystemSolver3, Update)
{
    ParticleSystemSolver3 solver;
    solver.SetGravity(Vector3D(0, -10, 0));

    ParticleSystemData3Ptr data = solver.GetParticleSystemData();
    ParticleSystemData3::VectorData positions(10);
    data->AddParticles(positions);

    Frame frame(0, 1.0 / 60.0);
    solver.Update(frame);

    for (size_t i = 0; i < data->NumberOfParticles(); ++i)
    {
        EXPECT_DOUBLE_EQ(0.0, data->Positions()[i].x);
        EXPECT_NE(0, data->Positions()[i].y);
        EXPECT_DOUBLE_EQ(0.0, data->Positions()[i].z);

        EXPECT_DOUBLE_EQ(0.0, data->Velocities()[i].x);
        EXPECT_NE(0, data->Velocities()[i].y);
        EXPECT_DOUBLE_EQ(0.0, data->Velocities()[i].z);
    }
}