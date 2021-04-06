#include "pch.hpp"

#include <Core/Searcher/PointHashGridSearcher.hpp>

using namespace CubbyFlow;

TEST(PointHashGridSearcher2, ForEachNearByPoint)
{
    Array1<Vector2D> points = { Vector2D(1, 1), Vector2D(3, 4),
                                Vector2D(-1, 2) };

    PointHashGridSearcher2 searcher(Vector2UZ{ 4, 4 }, std::sqrt(10));
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

TEST(PointHashGridSearcher2, HasEachNearByPoint)
{
    Array1<Vector2D> points = { Vector2D(1, 1), Vector2D(3, 4),
                                Vector2D(4, 5) };

    PointHashGridSearcher2 searcher(Vector2UZ{ 4, 4 }, std::sqrt(10));
    searcher.Build(points);

    bool result;
    result = searcher.HasNearbyPoint(Vector2D(), std::sqrt(15.0));

    EXPECT_TRUE(result);
}

TEST(PointHashGridSearcher2, Build)
{
    Array1<Vector2D> points = { Vector2D(3, 4), Vector2D(1, 5),
                                Vector2D(-3, 0) };

    PointHashGridSearcher2 searcher(Vector2UZ{ 4, 4 }, std::sqrt(10));
    searcher.Build(points);

    EXPECT_EQ(Vector2Z(0, 1), searcher.GetBucketIndex(Vector2D(3, 4)));
    EXPECT_EQ(Vector2Z(0, 1), searcher.GetBucketIndex(Vector2D(1, 5)));
    EXPECT_EQ(Vector2Z(-1, 0), searcher.GetBucketIndex(Vector2D(-3, 0)));

    EXPECT_EQ(4, searcher.GetHashKeyFromBucketIndex(Vector2Z(0, 1)));
    EXPECT_EQ(8, searcher.GetHashKeyFromBucketIndex(Vector2Z(0, 2)));
    EXPECT_EQ(3, searcher.GetHashKeyFromBucketIndex(Vector2Z(-1, 0)));
}