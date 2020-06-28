#include "pch.hpp"

#include <ManualTests.hpp>

#include <Core/Array/Array2.hpp>
#include <Core/Geometry/MarchingCubes.hpp>
#include <Core/Geometry/TriangleMeshToSDF.hpp>
#include <Core/Grid/VertexCenteredScalarGrid3.hpp>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(TriangleMeshToSDF);

CUBBYFLOW_BEGIN_TEST_F(TriangleMeshToSDF, Cube)
{
    TriangleMesh3 triMesh;

    triMesh.AddPoint({ 0, 0, 0 });
    triMesh.AddPoint({ 0, 0, 1 });
    triMesh.AddPoint({ 0, 1, 0 });
    triMesh.AddPoint({ 0, 1, 1 });
    triMesh.AddPoint({ 1, 0, 0 });
    triMesh.AddPoint({ 1, 0, 1 });
    triMesh.AddPoint({ 1, 1, 0 });
    triMesh.AddPoint({ 1, 1, 1 });

    // -x
    triMesh.AddPointTriangle({ 0, 1, 3 });
    triMesh.AddPointTriangle({ 0, 3, 2 });

    // +x
    triMesh.AddPointTriangle({ 4, 6, 7 });
    triMesh.AddPointTriangle({ 4, 7, 5 });

    // -y
    triMesh.AddPointTriangle({ 0, 4, 5 });
    triMesh.AddPointTriangle({ 0, 5, 1 });

    // +y
    triMesh.AddPointTriangle({ 2, 3, 7 });
    triMesh.AddPointTriangle({ 2, 7, 6 });

    // -z
    triMesh.AddPointTriangle({ 0, 2, 6 });
    triMesh.AddPointTriangle({ 0, 6, 4 });

    // +z
    triMesh.AddPointTriangle({ 1, 5, 7 });
    triMesh.AddPointTriangle({ 1, 7, 3 });

    VertexCenteredScalarGrid3 grid(64, 64, 64, 3.0 / 64, 3.0 / 64, 3.0 / 64,
                                   -1.0, -1.0, -1.0);

    TriangleMeshToSDF(triMesh, &grid);

    Array2<double> temp(64, 64);
    for (size_t j = 0; j < 64; ++j)
    {
        for (size_t i = 0; i < 64; ++i)
        {
            temp(i, j) = grid(i, j, 32);
        }
    }

    SaveData(temp.ConstAccessor(), "sdf_#grid2.npy");

    TriangleMesh3 triMesh2;
    MarchingCubes(grid.GetConstDataAccessor(), grid.GridSpacing(),
                  grid.Origin(), &triMesh2, 0, DIRECTION_ALL);

    SaveTriangleMeshData(triMesh2, "cube.obj");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(TriangleMeshToSDF, Bunny)
{
    TriangleMesh3 triMesh;

    std::ifstream file(RESOURCES_DIR "/bunny.obj");
    if (file)
    {
        triMesh.ReadObj(&file);
        file.close();
    }

    BoundingBox3D box = triMesh.BoundingBox();
    Vector3D scale(box.GetWidth(), box.GetHeight(), box.GetDepth());
    box.lowerCorner -= 0.2 * scale;
    box.upperCorner += 0.2 * scale;

    VertexCenteredScalarGrid3 grid(100, 100, 100, box.GetWidth() / 100,
                                   box.GetHeight() / 100, box.GetDepth() / 100,
                                   box.lowerCorner.x, box.lowerCorner.y,
                                   box.lowerCorner.z);

    TriangleMeshToSDF(triMesh, &grid);

    TriangleMesh3 triMesh2;
    MarchingCubes(grid.GetConstDataAccessor(), grid.GridSpacing(),
                  grid.Origin(), &triMesh2, 0, DIRECTION_ALL);

    SaveTriangleMeshData(triMesh2, "bunny.obj");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(TriangleMeshToSDF, Dragon)
{
    TriangleMesh3 triMesh;

    std::ifstream file(RESOURCES_DIR "/dragon.obj");
    if (file)
    {
        triMesh.ReadObj(&file);
        file.close();
    }

    BoundingBox3D box = triMesh.BoundingBox();
    Vector3D scale(box.GetWidth(), box.GetHeight(), box.GetDepth());
    box.lowerCorner -= 0.2 * scale;
    box.upperCorner += 0.2 * scale;

    VertexCenteredScalarGrid3 grid(100, 100, 100, box.GetWidth() / 100,
                                   box.GetHeight() / 100, box.GetDepth() / 100,
                                   box.lowerCorner.x, box.lowerCorner.y,
                                   box.lowerCorner.z);

    TriangleMeshToSDF(triMesh, &grid);

    TriangleMesh3 triMesh2;
    MarchingCubes(grid.GetConstDataAccessor(), grid.GridSpacing(),
                  grid.Origin(), &triMesh2, 0, DIRECTION_ALL);

    SaveTriangleMeshData(triMesh2, "dragon.obj");
}
CUBBYFLOW_END_TEST_F