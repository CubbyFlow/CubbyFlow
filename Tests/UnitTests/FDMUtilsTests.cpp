#include "pch.hpp"

#include <Core/FDM/FDMUtils.hpp>
#include <Core/Grid/CellCenteredScalarGrid2.hpp>
#include <Core/Grid/CellCenteredScalarGrid3.hpp>
#include <Core/Grid/CellCenteredVectorGrid2.hpp>
#include <Core/Grid/CellCenteredVectorGrid3.hpp>

using namespace CubbyFlow;

TEST(FDMUtils, ScalarToGradient2)
{
    CellCenteredScalarGrid2 grid(10, 10, 2.0, 3.0, -1.0, 4.0);
    grid.Fill([&](const Vector2D& x) { return -5.0 * x.x + 4.0 * x.y; });

    Vector2D grad =
        Gradient2(grid.GetConstDataAccessor(), grid.GridSpacing(), 5, 3);
    EXPECT_DOUBLE_EQ(-5.0, grad.x);
    EXPECT_DOUBLE_EQ(4.0, grad.y);
}

TEST(FDMUtils, VectorToGradient2)
{
    CellCenteredVectorGrid2 grid(10, 10, 2.0, 3.0, -1.0, 4.0);
    grid.Fill([&](const Vector2D& x) {
        return Vector2D(-5.0 * x.x + 4.0 * x.y, 2.0 * x.x - 7.0 * x.y);
    });

    auto grad =
        Gradient2(grid.GetConstDataAccessor(), grid.GridSpacing(), 5, 3);
    EXPECT_DOUBLE_EQ(-5.0, grad[0].x);
    EXPECT_DOUBLE_EQ(4.0, grad[0].y);
    EXPECT_DOUBLE_EQ(2.0, grad[1].x);
    EXPECT_DOUBLE_EQ(-7.0, grad[1].y);
}

TEST(FDMUtils, ScalarToGradient3)
{
    CellCenteredScalarGrid3 grid(10, 10, 10, 2.0, 3.0, 0.5, -1.0, 4.0, 2.0);
    grid.Fill(
        [&](const Vector3D& x) { return -5.0 * x.x + 4.0 * x.y + 2.0 * x.z; });

    Vector3D grad =
        Gradient3(grid.GetConstDataAccessor(), grid.GridSpacing(), 5, 3, 4);
    EXPECT_DOUBLE_EQ(-5.0, grad.x);
    EXPECT_DOUBLE_EQ(4.0, grad.y);
    EXPECT_DOUBLE_EQ(2.0, grad.z);
}

TEST(FDMUtils, VectorToGradient3)
{
    CellCenteredVectorGrid3 grid(10, 10, 10, 2.0, 3.0, 0.5, -1.0, 4.0, 2.0);
    grid.Fill([&](const Vector3D& x) {
        return Vector3D(-5.0 * x.x + 4.0 * x.y + 2.0 * x.z,
                        2.0 * x.x - 7.0 * x.y, x.y + 3.0 * x.z);
    });

    auto grad =
        Gradient3(grid.GetConstDataAccessor(), grid.GridSpacing(), 5, 3, 4);
    EXPECT_DOUBLE_EQ(-5.0, grad[0].x);
    EXPECT_DOUBLE_EQ(4.0, grad[0].y);
    EXPECT_DOUBLE_EQ(2.0, grad[0].z);
    EXPECT_DOUBLE_EQ(2.0, grad[1].x);
    EXPECT_DOUBLE_EQ(-7.0, grad[1].y);
    EXPECT_DOUBLE_EQ(0.0, grad[1].z);
    EXPECT_DOUBLE_EQ(0.0, grad[2].x);
    EXPECT_DOUBLE_EQ(1.0, grad[2].y);
    EXPECT_DOUBLE_EQ(3.0, grad[2].z);
}

TEST(FDMUtils, ScalarToLaplacian2)
{
    CellCenteredScalarGrid2 grid(10, 10, 2.0, 3.0, -1.0, 4.0);
    grid.Fill(
        [&](const Vector2D& x) { return -5.0 * x.x * x.x + 4.0 * x.y * x.y; });

    double lapl =
        Laplacian2(grid.GetConstDataAccessor(), grid.GridSpacing(), 5, 3);
    EXPECT_DOUBLE_EQ(-2.0, lapl);
}

TEST(FDMUtils, VectorToLaplacian2)
{
    CellCenteredVectorGrid2 grid(10, 10, 2.0, 3.0, -1.0, 4.0);
    grid.Fill([&](const Vector2D& x) {
        return Vector2D(-5.0 * x.x * x.x + 4.0 * x.y * x.y,
                        2.0 * x.x * x.x - 7.0 * x.y * x.y);
    });

    auto lapl =
        Laplacian2(grid.GetConstDataAccessor(), grid.GridSpacing(), 5, 3);
    EXPECT_DOUBLE_EQ(-2.0, lapl.x);
    EXPECT_DOUBLE_EQ(-10.0, lapl.y);
}

TEST(FDMUtils, ScalarToLaplacian3)
{
    CellCenteredScalarGrid3 grid(10, 10, 10, 2.0, 3.0, 0.5, -1.0, 4.0, 2.0);
    grid.Fill([&](const Vector3D& x) {
        return -5.0 * x.x * x.x + 4.0 * x.y * x.y - 3.0 * x.z * x.z;
    });

    double lapl =
        Laplacian3(grid.GetConstDataAccessor(), grid.GridSpacing(), 5, 3, 4);
    EXPECT_DOUBLE_EQ(-8.0, lapl);
}

TEST(FDMUtils, VectorToLaplacian3)
{
    CellCenteredVectorGrid3 grid(10, 10, 10, 2.0, 3.0, 0.5, -1.0, 4.0, 2.0);
    grid.Fill([&](const Vector3D& x) {
        return Vector3D(-5.0 * x.x * x.x + 4.0 * x.y * x.y + 2.0 * x.z * x.z,
                        2.0 * x.x * x.x - 7.0 * x.y * x.y,
                        x.y * x.y + 3.0 * x.z * x.z);
    });

    auto lapl =
        Laplacian3(grid.GetConstDataAccessor(), grid.GridSpacing(), 5, 3, 4);
    EXPECT_DOUBLE_EQ(2.0, lapl.x);
    EXPECT_DOUBLE_EQ(-10.0, lapl.y);
    EXPECT_DOUBLE_EQ(8.0, lapl.z);
}