#include "pch.hpp"

#include <ManualTests.hpp>

#include <Core/Array/Array3.hpp>
#include <Core/Field/ConstantVectorField2.hpp>
#include <Core/Field/CustomScalarField2.hpp>
#include <Core/Field/CustomVectorField2.hpp>
#include <Core/Geometry/Box2.hpp>
#include <Core/Grid/CellCenteredScalarGrid2.hpp>
#include <Core/Grid/CellCenteredVectorGrid2.hpp>
#include <Core/SemiLagrangian/CubicSemiLagrangian2.hpp>
#include <Core/SemiLagrangian/SemiLagrangian2.hpp>
#include <Core/Utils/Constants.hpp>

#include <algorithm>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(SemiLagrangian2);

CUBBYFLOW_BEGIN_TEST_F(SemiLagrangian2, Boundary)
{
    CellCenteredVectorGrid2 src(200, 200, 1.0 / 200.0, 1.0 / 200.0);
    CellCenteredVectorGrid2 dst(200, 200, 1.0 / 200.0, 1.0 / 200.0);
    src.Fill([&](const Vector2D& pt) -> Vector2D {
        return { 0.5 * (std::sin(15 * pt.x) + 1.0),
                 0.5 * (std::sin(15 * pt.y) + 1.0) };
    });

    ConstantVectorField2 flow(Vector2D(1.0, 1.0));
    CustomScalarField2 boundarySdf([](const Vector2D& pt) {
        return Vector2D(0.5, 0.5).DistanceTo(pt) - 0.25;
    });

    Array3<double> data(3, src.Resolution().x, src.Resolution().y);
    data.ForEachIndex([&](size_t i, size_t j, size_t k) {
        if (i < 2)
        {
            data(i, j, k) = src(j, k)[i];
        }
    });
    SaveData(data.ConstAccessor(), "src_#grid2.npy");

    SemiLagrangian2 solver;
    solver.Advect(src, flow, 0.1, &dst, boundarySdf);

    data.ForEachIndex([&](size_t i, size_t j, size_t k) {
        if (i < 2)
        {
            data(i, j, k) = dst(j, k)[i];
        }
    });
    SaveData(data.ConstAccessor(), "dst_#grid2.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(SemiLagrangian2, Zalesak)
{
    Box2 box(Vector2D(0.5 - 0.025, 0.6), Vector2D(0.5 + 0.025, 0.85));
    CellCenteredScalarGrid2 sdf(200, 200, 1.0 / 200.0, 1.0 / 200.0);
    CellCenteredScalarGrid2 sdf2(200, 200, 1.0 / 200.0, 1.0 / 200.0);
    sdf.Fill([box](const Vector2D& pt) {
        double disk = pt.DistanceTo(Vector2D(0.5, 0.75)) - 0.15;
        double slot = box.ClosestDistance(pt);
        if (!box.BoundingBox().Contains(pt))
        {
            slot *= -1.0;
        }
        return std::max(disk, slot);
    });

    CustomVectorField2 flow([](const Vector2D& pt) {
        return Vector2D(PI_DOUBLE / 3.14 * (0.5 - pt.y),
                        PI_DOUBLE / 3.14 * (pt.x - 0.5));
    });

    SaveData(sdf.GetConstDataAccessor(), "orig_#grid2,iso.npy");

    SemiLagrangian2 solver;

    for (int i = 0; i < 628; ++i)
    {
        solver.Advect(sdf, flow, 0.02, &sdf2);
        sdf.Swap(&sdf2);
    }

    SaveData(sdf.GetConstDataAccessor(), "rev0628_#grid2,iso.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_TESTS(CubicSemiLagrangian2);

CUBBYFLOW_BEGIN_TEST_F(CubicSemiLagrangian2, Zalesak)
{
    Box2 box(Vector2D(0.5 - 0.025, 0.6), Vector2D(0.5 + 0.025, 0.85));
    CellCenteredScalarGrid2 sdf(200, 200, 1.0 / 200.0, 1.0 / 200.0);
    CellCenteredScalarGrid2 sdf2(200, 200, 1.0 / 200.0, 1.0 / 200.0);
    sdf.Fill([box](const Vector2D& pt) {
        double disk = pt.DistanceTo(Vector2D(0.5, 0.75)) - 0.15;
        double slot = box.ClosestDistance(pt);
        if (!box.BoundingBox().Contains(pt))
        {
            slot *= -1.0;
        }
        return std::max(disk, slot);
    });

    CustomVectorField2 flow([](const Vector2D& pt) {
        return Vector2D(PI_DOUBLE / 3.14 * (0.5 - pt.y),
                        PI_DOUBLE / 3.14 * (pt.x - 0.5));
    });

    SaveData(sdf.GetConstDataAccessor(), "orig_#grid2,iso.npy");

    CubicSemiLagrangian2 solver;

    for (int i = 0; i < 628; ++i)
    {
        solver.Advect(sdf, flow, 0.02, &sdf2);
        sdf.Swap(&sdf2);
    }

    SaveData(sdf.GetConstDataAccessor(), "rev0628_#grid2,iso.npy");
}
CUBBYFLOW_END_TEST_F