#include "pch.h"

#include <ManualTests.h>

#include <Core/Array/Array3.hpp>
#include <Core/Grid/VertexCenteredScalarGrid3.hpp>
#include <Core/MarchingCubes/MarchingCubes.hpp>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(MarchingCubes);

CUBBYFLOW_BEGIN_TEST_F(MarchingCubes, SingleCube)
{
	TriangleMesh3 triMesh;

	Array3<double> grid(2, 2, 2);
	grid(0, 0, 0) = -0.5;
	grid(0, 0, 1) = -0.5;
	grid(0, 1, 0) = 0.5;
	grid(0, 1, 1) = 0.5;
	grid(1, 0, 0) = -0.5;
	grid(1, 0, 1) = -0.5;
	grid(1, 1, 0) = 0.5;
	grid(1, 1, 1) = 0.5;

	MarchingCubes(
		grid,
		Vector3D(1, 1, 1),
		Vector3D(),
		&triMesh,
		0,
		DIRECTION_ALL);

	SaveTriangleMeshData(triMesh, "single_cube.obj");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(MarchingCubes, FourCubes)
{
	TriangleMesh3 triMesh;

	VertexCenteredScalarGrid3 grid(2, 1, 2);
	grid.Fill([](const Vector3D& x)
	{
		return x.y - 0.5;
	});

	MarchingCubes(
		grid.GetConstDataAccessor(),
		grid.GridSpacing(),
		grid.Origin(),
		&triMesh,
		0,
		DIRECTION_ALL);

	SaveTriangleMeshData(triMesh, "four_cubes.obj");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(MarchingCubes, Sphere)
{
	TriangleMesh3 triMesh;

	VertexCenteredScalarGrid3 grid(16, 16, 16);
	grid.Fill([](const Vector3D& x)
	{
		return x.DistanceTo({ 8.0, 8.0, 8.0 }) - 3.0;
	});

	MarchingCubes(
		grid.GetConstDataAccessor(),
		grid.GridSpacing(),
		grid.Origin(),
		&triMesh,
		0,
		DIRECTION_ALL);

	SaveTriangleMeshData(triMesh, "sphere.obj");

	grid.Fill([](const Vector3D& x)
	{
		return x.DistanceTo({ 0.0, 4.0, 3.0 }) - 6.0;
	});

	triMesh.Clear();

	MarchingCubes(
		grid.GetConstDataAccessor(),
		grid.GridSpacing(),
		grid.Origin(),
		&triMesh,
		0,
		DIRECTION_ALL);

	SaveTriangleMeshData(triMesh, "clamped_sphere.obj");

	grid.Fill([](const Vector3D& x)
	{
		return x.DistanceTo({ 11.0, 14.0, 12.0 }) - 6.0;
	});

	triMesh.Clear();

	MarchingCubes(
		grid.GetConstDataAccessor(),
		grid.GridSpacing(),
		grid.Origin(),
		&triMesh,
		0,
		DIRECTION_ALL);

	SaveTriangleMeshData(triMesh, "clamped_sphere2.obj");
}
CUBBYFLOW_END_TEST_F