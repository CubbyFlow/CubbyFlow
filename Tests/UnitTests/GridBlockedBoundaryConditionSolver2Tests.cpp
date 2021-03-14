#include "pch.hpp"

#include <Core/Solver/Grid/GridBlockedBoundaryConditionSolver2.hpp>

using namespace CubbyFlow;

TEST(GridBlockedBoundaryConditionSolver2, ClosedDomain)
{
    GridBlockedBoundaryConditionSolver2 bndSolver;
    Vector2UZ gridSize(10, 10);
    Vector2D gridSpacing(1.0, 1.0);
    Vector2D gridOrigin(-5.0, -5.0);

    bndSolver.UpdateCollider(nullptr, gridSize, gridSpacing, gridOrigin);

    FaceCenteredGrid2 velocity(gridSize, gridSpacing, gridOrigin);
    velocity.Fill(Vector2D(1.0, 1.0));

    bndSolver.ConstrainVelocity(&velocity);

    velocity.ForEachUIndex([&](size_t i, size_t j) {
        if (i == 0 || i == gridSize.x)
        {
            EXPECT_DOUBLE_EQ(0.0, velocity.GetU(i, j));
        }
        else
        {
            EXPECT_DOUBLE_EQ(1.0, velocity.GetU(i, j));
        }
    });

    velocity.ForEachVIndex([&](size_t i, size_t j) {
        if (j == 0 || j == gridSize.y)
        {
            EXPECT_DOUBLE_EQ(0.0, velocity.GetV(i, j));
        }
        else
        {
            EXPECT_DOUBLE_EQ(1.0, velocity.GetV(i, j));
        }
    });
}

TEST(GridBlockedBoundaryConditionSolver2, OpenDomain)
{
    GridBlockedBoundaryConditionSolver2 bndSolver;
    Vector2UZ gridSize(10, 10);
    Vector2D gridSpacing(1.0, 1.0);
    Vector2D gridOrigin(-5.0, -5.0);

    // Partially open domain
    bndSolver.SetClosedDomainBoundaryFlag(DIRECTION_LEFT | DIRECTION_UP);
    bndSolver.UpdateCollider(nullptr, gridSize, gridSpacing, gridOrigin);

    FaceCenteredGrid2 velocity(gridSize, gridSpacing, gridOrigin);
    velocity.Fill(Vector2D(1.0, 1.0));

    bndSolver.ConstrainVelocity(&velocity);

    velocity.ForEachUIndex([&](size_t i, size_t j) {
        if (i == 0)
        {
            EXPECT_DOUBLE_EQ(0.0, velocity.GetU(i, j));
        }
        else
        {
            EXPECT_DOUBLE_EQ(1.0, velocity.GetU(i, j));
        }
    });

    velocity.ForEachVIndex([&](size_t i, size_t j) {
        if (j == gridSize.y)
        {
            EXPECT_DOUBLE_EQ(0.0, velocity.GetV(i, j));
        }
        else
        {
            EXPECT_DOUBLE_EQ(1.0, velocity.GetV(i, j));
        }
    });
}