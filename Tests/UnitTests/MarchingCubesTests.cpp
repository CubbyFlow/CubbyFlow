#include "UnitTestsUtils.hpp"
#include "gtest/gtest.h"

#include <Core/Array/Array.hpp>
#include <Core/Geometry/MarchingCubes.hpp>

using namespace CubbyFlow;

TEST(MarchingCubes, Connectivity)
{
    TriangleMesh3 triMesh;

    Array3<double> grid{ 2, 2, 2 };
    grid(0, 0, 0) = -0.5;
    grid(0, 0, 1) = -0.5;
    grid(0, 1, 0) = 0.5;
    grid(0, 1, 1) = 0.5;
    grid(1, 0, 0) = -0.5;
    grid(1, 0, 1) = -0.5;
    grid(1, 1, 0) = 0.5;
    grid(1, 1, 1) = 0.5;

    MarchingCubes(grid, Vector3D(1, 1, 1), Vector3D(), &triMesh, 0,
                  DIRECTION_ALL, DIRECTION_NONE);
    EXPECT_EQ(24u, triMesh.NumberOfPoints());

    triMesh.Clear();
    MarchingCubes(grid, Vector3D(1, 1, 1), Vector3D(), &triMesh, 0,
                  DIRECTION_ALL, DIRECTION_BACK);
    EXPECT_EQ(22u, triMesh.NumberOfPoints());

    triMesh.Clear();
    MarchingCubes(grid, Vector3D(1, 1, 1), Vector3D(), &triMesh, 0,
                  DIRECTION_ALL, DIRECTION_FRONT);
    EXPECT_EQ(22u, triMesh.NumberOfPoints());

    triMesh.Clear();
    MarchingCubes(grid, Vector3D(1, 1, 1), Vector3D(), &triMesh, 0,
                  DIRECTION_ALL, DIRECTION_LEFT);
    EXPECT_EQ(22u, triMesh.NumberOfPoints());

    triMesh.Clear();
    MarchingCubes(grid, Vector3D(1, 1, 1), Vector3D(), &triMesh, 0,
                  DIRECTION_ALL, DIRECTION_RIGHT);
    EXPECT_EQ(22u, triMesh.NumberOfPoints());

    triMesh.Clear();
    MarchingCubes(grid, Vector3D(1, 1, 1), Vector3D(), &triMesh, 0,
                  DIRECTION_ALL, DIRECTION_DOWN);
    EXPECT_EQ(24u, triMesh.NumberOfPoints());

    triMesh.Clear();
    MarchingCubes(grid, Vector3D(1, 1, 1), Vector3D(), &triMesh, 0,
                  DIRECTION_ALL, DIRECTION_UP);
    EXPECT_EQ(24u, triMesh.NumberOfPoints());

    triMesh.Clear();
    MarchingCubes(grid, Vector3D(1, 1, 1), Vector3D(), &triMesh, 0,
                  DIRECTION_ALL, DIRECTION_ALL);
    EXPECT_EQ(8u, triMesh.NumberOfPoints());
}