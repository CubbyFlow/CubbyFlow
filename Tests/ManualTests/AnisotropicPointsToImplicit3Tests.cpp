#include "pch.hpp"

#include <ManualTests.hpp>

#include <Core/Geometry/MarchingCubes.hpp>
#include <Core/Grid/VertexCenteredScalarGrid.hpp>
#include <Core/PointsToImplicit/AnisotropicPointsToImplicit3.hpp>

#include <random>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(AnisotropicPointsToImplicit3);

CUBBYFLOW_BEGIN_TEST_F(AnisotropicPointsToImplicit3, ConvertTwo)
{
    Array1<Vector3D> points;

    std::mt19937 rng{ 0 };
    std::uniform_real_distribution<> dist(0.2, 0.8);
    for (size_t i = 0; i < 2; ++i)
    {
        points.Append({ dist(rng), dist(rng), dist(rng) });
    }

    VertexCenteredScalarGrid3 grid({ 128, 128, 128 },
                                   { 1.0 / 128, 1.0 / 128, 1.0 / 128 });

    AnisotropicPointsToImplicit3 converter(0.3);
    converter.Convert(points.View(), &grid);

    TriangleMesh3 triMesh;
    MarchingCubes(grid.DataView(), grid.GridSpacing(), grid.DataOrigin(),
                  &triMesh, 0, DIRECTION_ALL);

    SaveTriangleMeshData(triMesh,
                         "anisotropic_points_to_implicit3_convert_two.obj");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(AnisotropicPointsToImplicit3, ConvertMany)
{
    Array1<Vector3D> points;

    std::mt19937 rng{ 0 };
    std::uniform_real_distribution<> dist(0.2, 0.8);
    for (size_t i = 0; i < 500; ++i)
    {
        points.Append({ dist(rng), dist(rng), dist(rng) });
    }

    VertexCenteredScalarGrid3 grid({ 128, 128, 128 },
                                   { 1.0 / 128, 1.0 / 128, 1.0 / 128 });

    AnisotropicPointsToImplicit3 converter(0.1);
    converter.Convert(points.View(), &grid);

    TriangleMesh3 triMesh;
    MarchingCubes(grid.DataView(), grid.GridSpacing(), grid.DataOrigin(),
                  &triMesh, 0, DIRECTION_ALL);

    SaveTriangleMeshData(triMesh,
                         "anisotropic_points_to_implicit3_convert_many.obj");
}
CUBBYFLOW_END_TEST_F