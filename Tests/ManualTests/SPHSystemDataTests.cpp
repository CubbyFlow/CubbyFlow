#include "pch.h"

#include <ManualTests.h>

#include <Core/BoundingBox/BoundingBox3.h>
#include <Core/Grid/CellCenteredScalarGrid2.h>
#include <Core/PointGenerator/BccLatticePointGenerator.h>
#include <Core/PointGenerator/TrianglePointGenerator.h>
#include <Core/SPH/SPHSystemData2.h>
#include <Core/SPH/SPHSystemData3.h>
#include <Core/Utils/Parallel.h>

#include <random>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(SPHSystemData2);

CUBBYFLOW_BEGIN_TEST_F(SPHSystemData2, Interpolate)
{
	Array1<Vector2D> points;
	TrianglePointGenerator pointsGenerator;
	BoundingBox2D bbox(Vector2D(0, 0), Vector2D(1, 1));
	double spacing = 0.1;

	pointsGenerator.Generate(bbox, spacing, &points);

	SPHSystemData2 sphSystem;
	sphSystem.AddParticles(ConstArrayAccessor1<Vector2D>(points.size(), points.data()));
	sphSystem.SetTargetSpacing(spacing);
	sphSystem.BuildNeighborSearcher();
	sphSystem.BuildNeighborLists();
	sphSystem.UpdateDensities();

	Array1<double> data(points.size(), 1.0);

	CellCenteredScalarGrid2 grid(512, 512, 1.0 / 512, 1.0 / 512);

	auto gridPos = grid.GetDataPosition();
	ParallelFor(ZERO_SIZE, grid.GetDataSize().x, ZERO_SIZE, grid.GetDataSize().y,
		[&](size_t i, size_t j)
	{
		Vector2D xy = gridPos(i, j);
		Vector2D p(xy.x, xy.y);
		grid(i, j) = sphSystem.Interpolate(p, data);
	});

	SaveData(grid.GetConstDataAccessor(), "data_#grid2.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(SPHSystemData2, Gradient)
{
	Array1<Vector2D> points;
	TrianglePointGenerator pointsGenerator;
	BoundingBox2D bbox(Vector2D(0, 0), Vector2D(1, 1));
	double spacing = 0.1;

	pointsGenerator.Generate(bbox, spacing, &points);

	SPHSystemData2 sphSystem;
	sphSystem.AddParticles(ConstArrayAccessor1<Vector2D>(points.size(), points.data()));
	sphSystem.SetTargetSpacing(spacing);
	sphSystem.BuildNeighborSearcher();
	sphSystem.BuildNeighborLists();
	sphSystem.UpdateDensities();

	Array1<double> data(points.size()), gradX(points.size()), gradY(points.size());
	std::mt19937 rng(0);
	std::uniform_real_distribution<> d(0.0, 1.0);

	for (size_t i = 0; i < data.size(); ++i)
	{
		data[i] = d(rng);
	}

	for (size_t i = 0; i < data.size(); ++i)
	{
		Vector2D g = sphSystem.GradientAt(i, data);
		gradX[i] = g.x;
		gradY[i] = g.y;
	}

	CellCenteredScalarGrid2 grid(64, 64, 1.0 / 64, 1.0 / 64);
	CellCenteredScalarGrid2 grid2(64, 64, 1.0 / 64, 1.0 / 64);

	auto gridPos = grid.GetDataPosition();
	ParallelFor(ZERO_SIZE, grid.GetDataSize().x, ZERO_SIZE, grid.GetDataSize().y,
		[&](size_t i, size_t j)
	{
		Vector2D xy = gridPos(i, j);
		Vector2D p(xy.x, xy.y);
		grid(i, j) = sphSystem.Interpolate(p, data);
	});

	SaveData(grid.GetConstDataAccessor(), "data_#grid2.npy");

	ParallelFor(ZERO_SIZE, grid.GetDataSize().x, ZERO_SIZE, grid.GetDataSize().y,
		[&](size_t i, size_t j)
	{
		Vector2D xy = gridPos(i, j);
		Vector2D p(xy.x, xy.y);
		grid(i, j) = sphSystem.Interpolate(p, gradX);
		grid2(i, j) = sphSystem.Interpolate(p, gradY);
	});

	SaveData(grid.GetConstDataAccessor(), "gradient_#grid2,x.npy");
	SaveData(grid2.GetConstDataAccessor(), "gradient_#grid2,y.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(SPHSystemData2, Laplacian)
{
	Array1<Vector2D> points;
	TrianglePointGenerator pointsGenerator;
	BoundingBox2D bbox(Vector2D(0, 0), Vector2D(1, 1));
	double spacing = 0.1;

	pointsGenerator.Generate(bbox, spacing, &points);

	SPHSystemData2 sphSystem;
	sphSystem.AddParticles(ConstArrayAccessor1<Vector2D>(points.size(), points.data()));
	sphSystem.SetTargetSpacing(spacing);
	sphSystem.BuildNeighborSearcher();
	sphSystem.BuildNeighborLists();
	sphSystem.UpdateDensities();

	Array1<double> data(points.size()), laplacian(points.size());
	std::mt19937 rng(0);
	std::uniform_real_distribution<> d(0.0, 1.0);

	for (size_t i = 0; i < data.size(); ++i)
	{
		data[i] = d(rng);
	}

	for (size_t i = 0; i < data.size(); ++i)
	{
		laplacian[i] = sphSystem.LaplacianAt(i, data);
	}

	CellCenteredScalarGrid2 grid(512, 512, 1.0 / 512, 1.0 / 512);

	auto gridPos = grid.GetDataPosition();
	ParallelFor(ZERO_SIZE, grid.GetDataSize().x, ZERO_SIZE, grid.GetDataSize().y,
		[&](size_t i, size_t j)
	{
		Vector2D xy = gridPos(i, j);
		Vector2D p(xy.x, xy.y);
		grid(i, j) = sphSystem.Interpolate(p, data);
	});

	SaveData(grid.GetConstDataAccessor(), "data_#grid2.npy");

	ParallelFor(ZERO_SIZE, grid.GetDataSize().x, ZERO_SIZE, grid.GetDataSize().y,
		[&](size_t i, size_t j)
	{
		Vector2D xy = gridPos(i, j);
		Vector2D p(xy.x, xy.y);
		grid(i, j) = sphSystem.Interpolate(p, laplacian);
	});

	SaveData(grid.GetConstDataAccessor(), "laplacian_#grid2.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_TESTS(SPHSystemData3);

CUBBYFLOW_BEGIN_TEST_F(SPHSystemData3, Interpolate)
{
	Array1<Vector3D> points;
	BccLatticePointGenerator pointsGenerator;
	BoundingBox3D bbox(Vector3D(0, 0, 0), Vector3D(1, 1, 1));
	double spacing = 0.1;

	pointsGenerator.Generate(bbox, spacing, &points);

	SPHSystemData3 sphSystem;
	sphSystem.AddParticles(ConstArrayAccessor1<Vector3D>(points.size(), points.data()));
	sphSystem.SetTargetSpacing(spacing);
	sphSystem.BuildNeighborSearcher();
	sphSystem.BuildNeighborLists();
	sphSystem.UpdateDensities();

	Array1<double> data(points.size(), 1.0);

	CellCenteredScalarGrid2 grid(512, 512, 1.0 / 512, 1.0 / 512);

	auto gridPos = grid.GetDataPosition();
	ParallelFor(ZERO_SIZE, grid.GetDataSize().x, ZERO_SIZE, grid.GetDataSize().y,
		[&](size_t i, size_t j)
	{
		Vector2D xy = gridPos(i, j);
		Vector3D p(xy.x, xy.y, 0.5);
		grid(i, j) = sphSystem.Interpolate(p, data);
	});

	SaveData(grid.GetConstDataAccessor(), "data_#grid2.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(SPHSystemData3, Gradient)
{
	Array1<Vector3D> points;
	BccLatticePointGenerator pointsGenerator;
	BoundingBox3D bbox(Vector3D(0, 0, 0), Vector3D(1, 1, 1));
	double spacing = 0.1;

	pointsGenerator.Generate(bbox, spacing, &points);

	SPHSystemData3 sphSystem;
	sphSystem.AddParticles(ConstArrayAccessor1<Vector3D>(points.size(), points.data()));
	sphSystem.SetTargetSpacing(spacing);
	sphSystem.BuildNeighborSearcher();
	sphSystem.BuildNeighborLists();
	sphSystem.UpdateDensities();

	Array1<double> data(points.size());
	Array1<double> gradX(points.size()), gradY(points.size());
	std::mt19937 rng(0);
	std::uniform_real_distribution<> d(0.0, 1.0);

	for (size_t i = 0; i < data.size(); ++i)
	{
		data[i] = d(rng);
	}

	for (size_t i = 0; i < data.size(); ++i)
	{
		Vector3D g = sphSystem.GradientAt(i, data);
		gradX[i] = g.x;
		gradY[i] = g.y;
	}

	CellCenteredScalarGrid2 grid(64, 64, 1.0 / 64, 1.0 / 64);
	CellCenteredScalarGrid2 grid2(64, 64, 1.0 / 64, 1.0 / 64);

	auto gridPos = grid.GetDataPosition();
	ParallelFor(ZERO_SIZE, grid.GetDataSize().x, ZERO_SIZE, grid.GetDataSize().y,
		[&](size_t i, size_t j)
	{
		Vector2D xy = gridPos(i, j);
		Vector3D p(xy.x, xy.y, 0.5);
		grid(i, j) = sphSystem.Interpolate(p, data);
	});

	SaveData(grid.GetConstDataAccessor(), "data_#grid2.npy");

	ParallelFor(ZERO_SIZE, grid.GetDataSize().x, ZERO_SIZE, grid.GetDataSize().y,
		[&](size_t i, size_t j)
	{
		Vector2D xy = gridPos(i, j);
		Vector3D p(xy.x, xy.y, 0.5);
		grid(i, j) = sphSystem.Interpolate(p, gradX);
		grid2(i, j) = sphSystem.Interpolate(p, gradY);
	});

	SaveData(grid.GetConstDataAccessor(), "gradient_#grid2,x.npy");
	SaveData(grid2.GetConstDataAccessor(), "gradient_#grid2,y.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_BEGIN_TEST_F(SPHSystemData3, Laplacian)
{
	Array1<Vector3D> points;
	BccLatticePointGenerator pointsGenerator;
	BoundingBox3D bbox(Vector3D(0, 0, 0), Vector3D(1, 1, 1));
	double spacing = 0.1;

	pointsGenerator.Generate(bbox, spacing, &points);

	SPHSystemData3 sphSystem;
	sphSystem.AddParticles(ConstArrayAccessor1<Vector3D>(points.size(), points.data()));
	sphSystem.SetTargetSpacing(spacing);
	sphSystem.BuildNeighborSearcher();
	sphSystem.BuildNeighborLists();
	sphSystem.UpdateDensities();

	Array1<double> data(points.size()), laplacian(points.size());
	std::mt19937 rng(0);
	std::uniform_real_distribution<> d(0.0, 1.0);

	for (size_t i = 0; i < data.size(); ++i)
	{
		data[i] = d(rng);
	}

	for (size_t i = 0; i < data.size(); ++i)
	{
		laplacian[i] = sphSystem.LaplacianAt(i, data);
	}

	CellCenteredScalarGrid2 grid(512, 512, 1.0 / 512, 1.0 / 512);

	auto gridPos = grid.GetDataPosition();
	ParallelFor(ZERO_SIZE, grid.GetDataSize().x, ZERO_SIZE, grid.GetDataSize().y,
		[&](size_t i, size_t j)
	{
		Vector2D xy = gridPos(i, j);
		Vector3D p(xy.x, xy.y, 0.5);
		grid(i, j) = sphSystem.Interpolate(p, data);
	});

	SaveData(grid.GetConstDataAccessor(), "data_#grid2.npy");

	ParallelFor(ZERO_SIZE, grid.GetDataSize().x, ZERO_SIZE, grid.GetDataSize().y,
		[&](size_t i, size_t j)
	{
		Vector2D xy = gridPos(i, j);
		Vector3D p(xy.x, xy.y, 0.5);
		grid(i, j) = sphSystem.Interpolate(p, laplacian);
	});

	SaveData(grid.GetConstDataAccessor(), "laplacian_#grid2.npy");
}
CUBBYFLOW_END_TEST_F