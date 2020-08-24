#include "pch.hpp"

#include <Core/Solver/Grid/GridBlockedBoundaryConditionSolver3.hpp>

using namespace CubbyFlow;

TEST(GridBlockedBoundaryConditionSolver3, ClosedDomain)
{
    GridBlockedBoundaryConditionSolver3 bndSolver;
    Size3 gridSize(10, 10, 10);
    Vector3D gridSpacing(1.0, 1.0, 1.0);
    Vector3D gridOrigin(-5.0, -5.0, -5.0);

    bndSolver.UpdateCollider(nullptr, gridSize, gridSpacing, gridOrigin);

    FaceCenteredGrid3 velocity(gridSize, gridSpacing, gridOrigin);
    velocity.Fill(Vector3D(1.0, 1.0, 1.0));

    bndSolver.ConstrainVelocity(&velocity);

    velocity.ForEachUIndex([&](size_t i, size_t j, size_t k) {
        if (i == 0 || i == gridSize.x)
        {
            EXPECT_DOUBLE_EQ(0.0, velocity.GetU(i, j, k));
        }
        else
        {
            EXPECT_DOUBLE_EQ(1.0, velocity.GetU(i, j, k));
        }
    });

    velocity.ForEachVIndex([&](size_t i, size_t j, size_t k) {
        if (j == 0 || j == gridSize.y)
        {
            EXPECT_DOUBLE_EQ(0.0, velocity.GetV(i, j, k));
        }
        else
        {
            EXPECT_DOUBLE_EQ(1.0, velocity.GetV(i, j, k));
        }
    });

    velocity.ForEachWIndex([&](size_t i, size_t j, size_t k) {
        if (k == 0 || k == gridSize.z)
        {
            EXPECT_DOUBLE_EQ(0.0, velocity.GetW(i, j, k));
        }
        else
        {
            EXPECT_DOUBLE_EQ(1.0, velocity.GetW(i, j, k));
        }
    });
}

TEST(GridBlockedBoundaryConditionSolver3, OpenDomain)
{
    GridBlockedBoundaryConditionSolver3 bndSolver;
    Size3 gridSize(10, 10, 10);
    Vector3D gridSpacing(1.0, 1.0, 1.0);
    Vector3D gridOrigin(-5.0, -5.0, -5.0);

    // Partially open domain
    bndSolver.SetClosedDomainBoundaryFlag(DIRECTION_LEFT | DIRECTION_UP |
                                          DIRECTION_FRONT);
    bndSolver.UpdateCollider(nullptr, gridSize, gridSpacing, gridOrigin);

    FaceCenteredGrid3 velocity(gridSize, gridSpacing, gridOrigin);
    velocity.Fill(Vector3D(1.0, 1.0, 1.0));

    bndSolver.ConstrainVelocity(&velocity);

    velocity.ForEachUIndex([&](size_t i, size_t j, size_t k) {
        if (i == 0)
        {
            EXPECT_DOUBLE_EQ(0.0, velocity.GetU(i, j, k));
        }
        else
        {
            EXPECT_DOUBLE_EQ(1.0, velocity.GetU(i, j, k));
        }
    });

    velocity.ForEachVIndex([&](size_t i, size_t j, size_t k) {
        if (j == gridSize.y)
        {
            EXPECT_DOUBLE_EQ(0.0, velocity.GetV(i, j, k));
        }
        else
        {
            EXPECT_DOUBLE_EQ(1.0, velocity.GetV(i, j, k));
        }
    });

    velocity.ForEachWIndex([&](size_t i, size_t j, size_t k) {
        if (k == gridSize.z)
        {
            EXPECT_DOUBLE_EQ(0.0, velocity.GetW(i, j, k));
        }
        else
        {
            EXPECT_DOUBLE_EQ(1.0, velocity.GetW(i, j, k));
        }
    });
}