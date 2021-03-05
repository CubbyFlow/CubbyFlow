#include "pch.hpp"

#include <Core/Searcher/PointParallelHashGridSearcher2.hpp>

using namespace CubbyFlow;

TEST(PointParallelHashGridSearcher2, ForEachNearByPoint)
{
    Array1<Vector2D> points = { Vector2D(1, 1), Vector2D(3, 4),
                                Vector2D(-1, 2) };

    PointParallelHashGridSearcher2 searcher(4, 4, std::sqrt(10));
    searcher.Build(points);

    searcher.ForEachNearbyPoint(Vector2D(0, 0), std::sqrt(15.0),
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
    Array1<Vector2D> points;

    PointParallelHashGridSearcher2 searcher(4, 4, std::sqrt(10));
    searcher.Build(points);

    searcher.ForEachNearbyPoint(Vector2D(0, 0), std::sqrt(10.0),
                                [](size_t, const Vector2D&) {
                                    // Do nothing
                                });
}