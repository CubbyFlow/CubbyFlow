#include "pch.hpp"

#include <Core/Geometry/Sphere2.hpp>
#include <Core/Geometry/Sphere3.hpp>
#include <Core/Size/Size2.hpp>
#include <Core/Size/Size3.hpp>
#include <Core/Solver/LevelSet/LevelSetLiquidSolver2.hpp>
#include <Core/Solver/LevelSet/LevelSetLiquidSolver3.hpp>
#include <Core/Surface/ImplicitSurface2.hpp>
#include <Core/Surface/ImplicitSurface3.hpp>
#include <Core/Surface/ImplicitSurfaceSet2.hpp>
#include <Core/Surface/ImplicitSurfaceSet3.hpp>

using namespace CubbyFlow;

TEST(LevelSetLiquidSolver2, ComputeVolume)
{
    LevelSetLiquidSolver2 solver;
    solver.SetIsGlobalCompensationEnabled(true);

    auto data = solver.GetGridSystemData();
    double dx = 1.0 / 32.0;
    data->Resize(Size2(32, 64), Vector2D(dx, dx), Vector2D());

    // Source setting
    const double radius = 0.15;
    BoundingBox2D domain = data->GetBoundingBox();
    ImplicitSurfaceSet2 surfaceSet;
    surfaceSet.AddExplicitSurface(
        std::make_shared<Sphere2>(domain.MidPoint(), radius));

    auto sdf = solver.GetSignedDistanceField();
    sdf->Fill([&](const Vector2D& x) { return surfaceSet.SignedDistance(x); });

    // Measure the volume
    double volume = solver.ComputeVolume();
    const double ans = Square(radius) * PI_DOUBLE;

    EXPECT_NEAR(ans, volume, 0.001);
}

TEST(LevelSetLiquidSolver3, ComputeVolume)
{
    LevelSetLiquidSolver3 solver;
    solver.SetIsGlobalCompensationEnabled(true);

    auto data = solver.GetGridSystemData();
    double dx = 1.0 / 32.0;
    data->Resize(Size3(32, 64, 32), Vector3D(dx, dx, dx), Vector3D());

    // Source setting
    const double radius = 0.15;
    BoundingBox3D domain = data->GetBoundingBox();
    ImplicitSurfaceSet3 surfaceSet;
    surfaceSet.AddExplicitSurface(
        std::make_shared<Sphere3>(domain.MidPoint(), radius));

    auto sdf = solver.GetSignedDistanceField();
    sdf->Fill([&](const Vector3D& x) { return surfaceSet.SignedDistance(x); });

    // Measure the volume
    double volume = solver.ComputeVolume();
    const double ans = 4.0 / 3.0 * Cubic(radius) * PI_DOUBLE;

    EXPECT_NEAR(ans, volume, 0.001);
}