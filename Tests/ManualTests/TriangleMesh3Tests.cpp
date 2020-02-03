#include "pch.h"

#include <ManualTests.h>

#include <Core/Geometry/TriangleMesh3.hpp>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(TriangleMesh3);

CUBBYFLOW_BEGIN_TEST_F(TriangleMesh3, PointsOnlyGeometries)
{
    TriangleMesh3 triMesh;

    triMesh.AddPoint({0, 0, 0});
    triMesh.AddPoint({0, 0, 1});
    triMesh.AddPoint({0, 1, 0});
    triMesh.AddPoint({0, 1, 1});
    triMesh.AddPoint({1, 0, 0});
    triMesh.AddPoint({1, 0, 1});
    triMesh.AddPoint({1, 1, 0});
    triMesh.AddPoint({1, 1, 1});

    // -x
    triMesh.AddPointTriangle({0, 1, 3});
    triMesh.AddPointTriangle({0, 3, 2});

    // +x
    triMesh.AddPointTriangle({4, 6, 7});
    triMesh.AddPointTriangle({4, 7, 5});

    // -y
    triMesh.AddPointTriangle({0, 4, 5});
    triMesh.AddPointTriangle({0, 5, 1});

    // +y
    triMesh.AddPointTriangle({2, 3, 7});
    triMesh.AddPointTriangle({2, 7, 6});

    // -z
    triMesh.AddPointTriangle({0, 2, 6});
    triMesh.AddPointTriangle({0, 6, 4});

    // +z
    triMesh.AddPointTriangle({1, 5, 7});
    triMesh.AddPointTriangle({1, 7, 3});

    SaveTriangleMeshData(triMesh, "cube.obj");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(TriangleMesh3, PointsAndNormalGeometries)
{
    TriangleMesh3 triMesh;

    triMesh.AddPoint({0, 0, 0});
    triMesh.AddPoint({0, 0, 1});
    triMesh.AddPoint({0, 1, 0});
    triMesh.AddPoint({0, 1, 1});
    triMesh.AddPoint({1, 0, 0});
    triMesh.AddPoint({1, 0, 1});
    triMesh.AddPoint({1, 1, 0});
    triMesh.AddPoint({1, 1, 1});

    triMesh.AddNormal({-1, 0, 0});
    triMesh.AddNormal({1, 0, 0});
    triMesh.AddNormal({0, -1, 0});
    triMesh.AddNormal({0, 1, 0});
    triMesh.AddNormal({0, 0, -1});
    triMesh.AddNormal({0, 0, 1});

    // -x
    triMesh.AddPointTriangle({0, 1, 3});
    triMesh.AddNormalTriangle({0, 0, 0});
    triMesh.AddPointTriangle({0, 3, 2});
    triMesh.AddNormalTriangle({0, 0, 0});

    // +x
    triMesh.AddPointTriangle({4, 6, 7});
    triMesh.AddNormalTriangle({1, 1, 1});
    triMesh.AddPointTriangle({4, 7, 5});
    triMesh.AddNormalTriangle({1, 1, 1});

    // -y
    triMesh.AddPointTriangle({0, 4, 5});
    triMesh.AddNormalTriangle({2, 2, 2});
    triMesh.AddPointTriangle({0, 5, 1});
    triMesh.AddNormalTriangle({2, 2, 2});

    // +y
    triMesh.AddPointTriangle({2, 3, 7});
    triMesh.AddNormalTriangle({3, 3, 3});
    triMesh.AddPointTriangle({2, 7, 6});
    triMesh.AddNormalTriangle({3, 3, 3});

    // -z
    triMesh.AddPointTriangle({0, 2, 6});
    triMesh.AddNormalTriangle({4, 4, 4});
    triMesh.AddPointTriangle({0, 6, 4});
    triMesh.AddNormalTriangle({4, 4, 4});

    // +z
    triMesh.AddPointTriangle({1, 5, 7});
    triMesh.AddNormalTriangle({5, 5, 5});
    triMesh.AddPointTriangle({1, 7, 3});
    triMesh.AddNormalTriangle({5, 5, 5});

    SaveTriangleMeshData(triMesh, "cube_with_normal.obj");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(TriangleMesh3, BasicIO)
{
    TriangleMesh3 triMesh;

    std::ifstream file("resources/bunny.obj");
    if (file)
    {
        triMesh.ReadObj(&file);
        file.close();
    }
}
CUBBYFLOW_END_TEST_F