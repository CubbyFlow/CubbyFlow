#include "pch.hpp"

#include <ManualTests.hpp>

#include <Core/Array/Array.hpp>
#include <Core/Geometry/BoundingBox.hpp>
#include <Core/PointGenerator/BccLatticePointGenerator.hpp>
#include <Core/PointGenerator/TrianglePointGenerator.hpp>
#include <Core/Searcher/PointHashGridSearcher.hpp>
#include <Core/Searcher/PointParallelHashGridSearcher2.hpp>
#include <Core/Searcher/PointParallelHashGridSearcher3.hpp>

using namespace CubbyFlow;

CUBBYFLOW_TESTS(PointHashGridSearcher2);

CUBBYFLOW_BEGIN_TEST_F(PointHashGridSearcher2, Build)
{
    Array1<Vector2D> points;
    TrianglePointGenerator pointsGenerator;
    BoundingBox2D bbox(Vector2D(0, 0), Vector2D(1, 1));
    double spacing = 0.1;

    pointsGenerator.Generate(bbox, spacing, &points);

    PointHashGridSearcher2 pointSearcher(Vector2UZ{ 4, 4 }, 0.18);
    pointSearcher.Build(ArrayView1<Vector2D>(points.data(), points.Size()));

    Array2<double> grid(4, 4, 0.0);

    for (size_t j = 0; j < grid.Size().y; ++j)
    {
        for (size_t i = 0; i < grid.Size().x; ++i)
        {
            size_t key = pointSearcher.GetHashKeyFromBucketIndex(
                Vector2Z(static_cast<ssize_t>(i), static_cast<ssize_t>(j)));
            size_t value = pointSearcher.Buckets()[key].Length();
            grid(i, j) += static_cast<double>(value);
        }
    }

    SaveData(grid.View(), "Data_#grid2.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_TESTS(PointHashGridSearcher3);

CUBBYFLOW_BEGIN_TEST_F(PointHashGridSearcher3, Build)
{
    Array1<Vector3D> points;
    BccLatticePointGenerator pointsGenerator;
    BoundingBox3D bbox(Vector3D(0, 0, 0), Vector3D(1, 1, 1));
    double spacing = 0.1;

    pointsGenerator.Generate(bbox, spacing, &points);

    PointHashGridSearcher3 pointSearcher(Vector3UZ{ 4, 4, 4 }, 0.18);
    pointSearcher.Build(ArrayView1<Vector3D>(points.data(), points.Size()));

    Array2<double> grid(4, 4, 0.0);

    for (size_t j = 0; j < grid.Size().y; ++j)
    {
        for (size_t i = 0; i < grid.Size().x; ++i)
        {
            size_t key = pointSearcher.GetHashKeyFromBucketIndex(
                Vector3Z(static_cast<ssize_t>(i), static_cast<ssize_t>(j), 0));
            size_t value = pointSearcher.Buckets()[key].Length();
            grid(i, j) += static_cast<double>(value);
        }
    }

    SaveData(grid.View(), "Data_#grid2.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_TESTS(PointParallelHashGridSearcher2);

CUBBYFLOW_BEGIN_TEST_F(PointParallelHashGridSearcher2, Build)
{
    Array1<Vector2D> points;
    TrianglePointGenerator pointsGenerator;
    BoundingBox2D bbox(Vector2D(0, 0), Vector2D(1, 1));
    double spacing = 0.1;

    pointsGenerator.Generate(bbox, spacing, &points);

    PointParallelHashGridSearcher2 pointSearcher(4, 4, 0.18);
    pointSearcher.Build(ArrayView1<Vector2D>(points.data(), points.Size()));

    Array2<double> grid(4, 4, 0.0);

    for (size_t j = 0; j < grid.Size().y; ++j)
    {
        for (size_t i = 0; i < grid.Size().x; ++i)
        {
            size_t key = pointSearcher.GetHashKeyFromBucketIndex(
                Vector2Z(static_cast<ssize_t>(i), static_cast<ssize_t>(j)));
            size_t start = pointSearcher.StartIndexTable()[key];
            size_t end = pointSearcher.EndIndexTable()[key];
            size_t value = end - start;
            grid(i, j) += static_cast<double>(value);
        }
    }

    SaveData(grid.View(), "Data_#grid2.npy");
}
CUBBYFLOW_END_TEST_F

CUBBYFLOW_TESTS(PointParallelHashGridSearcher3);

CUBBYFLOW_BEGIN_TEST_F(PointParallelHashGridSearcher3, Build)
{
    Array1<Vector3D> points;
    BccLatticePointGenerator pointsGenerator;
    BoundingBox3D bbox(Vector3D(0, 0, 0), Vector3D(1, 1, 1));
    double spacing = 0.1;

    pointsGenerator.Generate(bbox, spacing, &points);

    PointParallelHashGridSearcher3 pointSearcher(4, 4, 4, 0.18);
    pointSearcher.Build(ArrayView1<Vector3D>(points.data(), points.Size()));

    Array2<double> grid(4, 4, 0.0);

    for (size_t j = 0; j < grid.Size().y; ++j)
    {
        for (size_t i = 0; i < grid.Size().x; ++i)
        {
            size_t key = pointSearcher.GetHashKeyFromBucketIndex(
                Vector3Z(static_cast<ssize_t>(i), static_cast<ssize_t>(j), 0));
            size_t start = pointSearcher.StartIndexTable()[key];
            size_t end = pointSearcher.EndIndexTable()[key];
            size_t value = end - start;
            grid(i, j) += static_cast<double>(value);
        }
    }

    SaveData(grid.View(), "Data_#grid2.npy");
}
CUBBYFLOW_END_TEST_F