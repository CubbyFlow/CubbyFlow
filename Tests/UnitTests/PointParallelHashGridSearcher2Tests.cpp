#include "pch.hpp"

#include <Core/PointGenerator/TrianglePointGenerator.hpp>
#include <Core/Searcher/PointHashGridSearcher.hpp>
#include <Core/Searcher/PointParallelHashGridSearcher.hpp>
#include <Core/Utils/PointHashGridUtils.hpp>

using namespace CubbyFlow;

TEST(PointParallelHashGridSearcher2, ForEachNearByPoint)
{
    Array1<Vector2D> points = { Vector2D{ 1, 1 }, Vector2D{ 3, 4 },
                                Vector2D{ -1, 2 } };

    PointParallelHashGridSearcher2 searcher{ Vector2UZ{ 4, 4 }, std::sqrt(10) };
    searcher.Build(points);

    searcher.ForEachNearbyPoint(Vector2D{ 0, 0 }, std::sqrt(15.0),
                                [&points](size_t i, const Vector2D& pt) {
                                    EXPECT_TRUE(i == 0 || i == 2);
                                    if (i == 0)
                                    {
                                        EXPECT_EQ(points[0], pt);
                                    }
                                    else if (i == 2)
                                    {
                                        EXPECT_EQ(points[2], pt);
                                    }
                                });
}

TEST(PointParallelHashGridSearcher2, ForEachNearbyPointEmpty)
{
    const Array1<Vector2D> points;

    PointParallelHashGridSearcher2 searcher{ Vector2UZ{ 4, 4 }, std::sqrt(10) };
    searcher.Build(points);

    searcher.ForEachNearbyPoint(Vector2D{ 0, 0 }, std::sqrt(10.0),
                                [](size_t, const Vector2D&) {
                                    // Do nothing
                                });
}

TEST(PointParallelHashGridSearcher2, Build)
{
    Array1<Vector2D> points;
    TrianglePointGenerator pointsGenerator;
    BoundingBox2D bbox{ Vector2D{ 0, 0 }, Vector2D{ 1, 1 } };
    double spacing = 0.1;

    pointsGenerator.Generate(bbox, spacing, &points);

    PointHashGridSearcher2 pointSearcher{ Vector2UZ{ 4, 4 }, 0.18 };
    pointSearcher.Build(points);

    Array2<size_t> grid(4, 4);

    for (size_t j = 0; j < grid.Size().y; ++j)
    {
        for (size_t i = 0; i < grid.Size().x; ++i)
        {
            size_t key = PointHashGridUtils2::GetHashKeyFromBucketIndex(
                Vector2Z{ static_cast<ssize_t>(i), static_cast<ssize_t>(j) },
                Vector2Z{ 4, 4 });
            size_t value = pointSearcher.Buckets()[key].Length();
            grid(i, j) = value;
        }
    }

    PointParallelHashGridSearcher2 parallelSearcher{ Vector2UZ{ 4, 4 }, 0.18 };
    parallelSearcher.Build(points);

    for (size_t j = 0; j < grid.Size().y; ++j)
    {
        for (size_t i = 0; i < grid.Size().x; ++i)
        {
            size_t key = PointHashGridUtils2::GetHashKeyFromBucketIndex(
                Vector2Z{ static_cast<ssize_t>(i), static_cast<ssize_t>(j) },
                Vector2Z{ 4, 4 });
            size_t start = parallelSearcher.StartIndexTable()[key];
            size_t end = parallelSearcher.EndIndexTable()[key];
            size_t value = end - start;
            EXPECT_EQ(grid(i, j), value);
        }
    }
}