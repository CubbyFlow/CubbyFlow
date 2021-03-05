#include "pch.hpp"

#include <Core/Solver/Grid/GridFluidSolver3.hpp>

using namespace CubbyFlow;

TEST(GridFluidSolver3, Constructor)
{
    GridFluidSolver3 solver;

    // Check if the sub-step solvers are present
    EXPECT_TRUE(solver.GetAdvectionSolver() != nullptr);
    EXPECT_TRUE(solver.GetDiffusionSolver() != nullptr);
    EXPECT_TRUE(solver.GetPressureSolver() != nullptr);

    // Check default parameters
    EXPECT_GE(solver.GetViscosityCoefficient(), 0.0);
    EXPECT_GT(solver.GetMaxCFL(), 0.0);
    EXPECT_EQ(DIRECTION_ALL, solver.GetClosedDomainBoundaryFlag());

    // Check grid system data
    EXPECT_TRUE(solver.GetGridSystemData() != nullptr);
    EXPECT_EQ(1u, solver.GetGridSystemData()->GetResolution().x);
    EXPECT_EQ(1u, solver.GetGridSystemData()->GetResolution().y);
    EXPECT_EQ(1u, solver.GetGridSystemData()->GetResolution().z);
    EXPECT_EQ(solver.GetGridSystemData()->GetVelocity(), solver.GetVelocity());

    // Collider should be null
    EXPECT_TRUE(solver.GetCollider() == nullptr);
}

TEST(GridFluidSolver3, ResizeGridSystemData)
{
    GridFluidSolver3 solver;

    solver.ResizeGrid(Vector3UZ(1, 2, 3), Vector3D(4.0, 5.0, 6.0),
                      Vector3D(7.0, 8.0, 9.0));

    EXPECT_EQ(1u, solver.GetResolution().x);
    EXPECT_EQ(2u, solver.GetResolution().y);
    EXPECT_EQ(3u, solver.GetResolution().z);
    EXPECT_EQ(4.0, solver.GetGridSpacing().x);
    EXPECT_EQ(5.0, solver.GetGridSpacing().y);
    EXPECT_EQ(6.0, solver.GetGridSpacing().z);
    EXPECT_EQ(7.0, solver.GetGridOrigin().x);
    EXPECT_EQ(8.0, solver.GetGridOrigin().y);
    EXPECT_EQ(9.0, solver.GetGridOrigin().z);
}

TEST(GridFluidSolver3, MinimumResolution)
{
    GridFluidSolver3 solver;

    solver.ResizeGrid(Vector3UZ(1, 1, 1), Vector3D(1.0, 1.0, 1.0), Vector3D());
    solver.GetVelocity()->Fill(Vector3D());

    Frame frame(0, 1.0 / 60.0);
    frame.timeIntervalInSeconds = 0.01;
    solver.Update(frame);
}

TEST(GridFluidSolver3, GravityOnly)
{
    GridFluidSolver3 solver;
    solver.SetGravity(Vector3D(0, -10, 0.0));
    solver.SetAdvectionSolver(nullptr);
    solver.SetDiffusionSolver(nullptr);
    solver.SetPressureSolver(nullptr);

    solver.ResizeGrid(Vector3UZ(3, 3, 3),
                      Vector3D(1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0), Vector3D());
    solver.GetVelocity()->Fill(Vector3D());

    Frame frame(0, 1.0 / 60.0);
    frame.timeIntervalInSeconds = 0.01;
    solver.Update(frame);

    solver.GetVelocity()->ForEachUIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_NEAR(0.0, solver.GetVelocity()->GetU(i, j, k), 1e-8);
    });

    solver.GetVelocity()->ForEachVIndex([&](size_t i, size_t j, size_t k) {
        if (j == 0 || j == 3)
        {
            EXPECT_NEAR(0.0, solver.GetVelocity()->GetV(i, j, k), 1e-8);
        }
        else
        {
            EXPECT_NEAR(-0.1, solver.GetVelocity()->GetV(i, j, k), 1e-8);
        }
    });

    solver.GetVelocity()->ForEachWIndex([&](size_t i, size_t j, size_t k) {
        EXPECT_NEAR(0.0, solver.GetVelocity()->GetW(i, j, k), 1e-8);
    });
}