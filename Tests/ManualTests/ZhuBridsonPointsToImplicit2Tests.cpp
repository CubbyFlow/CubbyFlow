#include "pch.hpp"

#include <ManualTests.hpp>

#include <Core/Grid/CellCenteredScalarGrid2.hpp>
#include <Core/PointsToImplicit/ZhuBridsonPointsToImplicit2.hpp>

#include <random>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(ZhuBridsonPointsToImplicit2);

CUBBYFLOW_BEGIN_TEST_F(ZhuBridsonPointsToImplicit2, ConvertTwo)
{
    Array1<Vector2D> points;

    std::mt19937 rng{ 0 };
    std::uniform_real_distribution<> dist(0.2, 0.8);
    for (size_t i = 0; i < 2; ++i)
    {
        points.Append({ dist(rng), dist(rng) });
    }

    CellCenteredScalarGrid2 grid(512, 512, 1.0 / 512, 1.0 / 512);

    ZhuBridsonPointsToImplicit2 converter(0.1);
    converter.Convert(points.ConstAccessor(), &grid);

    SaveData(grid.GetConstDataAccessor(), "data_#grid2.npy");
    SaveData(grid.GetConstDataAccessor(), "data_#grid2,iso.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(ZhuBridsonPointsToImplicit2, ConvertMany)
{
    Array1<Vector2D> points;

    std::mt19937 rng{ 0 };
    std::uniform_real_distribution<> dist(0.2, 0.8);
    for (size_t i = 0; i < 200; ++i)
    {
        points.Append({ dist(rng), dist(rng) });
    }

    CellCenteredScalarGrid2 grid(512, 512, 1.0 / 512, 1.0 / 512);

    ZhuBridsonPointsToImplicit2 converter(0.1);
    converter.Convert(points.ConstAccessor(), &grid);

    SaveData(grid.GetConstDataAccessor(), "data_#grid2.npy");
    SaveData(grid.GetConstDataAccessor(), "data_#grid2,iso.npy");
}
CUBBYFLOW_END_TEST_F