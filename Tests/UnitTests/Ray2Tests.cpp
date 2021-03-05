#include "pch.hpp"

#include <Core/Geometry/Ray.hpp>

using namespace CubbyFlow;

TEST(Ray2, Constructors)
{
    Ray2D ray;
    EXPECT_EQ(Vector2D(), ray.origin);
    EXPECT_EQ(Vector2D(1, 0), ray.direction);

    Ray2D ray2({ 1, 2 }, { 3, 4 });
    EXPECT_EQ(Vector2D(1, 2), ray2.origin);
    EXPECT_EQ(Vector2D(3, 4).Normalized(), ray2.direction);

    Ray2D ray3(ray2);
    EXPECT_EQ(Vector2D(1, 2), ray3.origin);
    EXPECT_EQ(Vector2D(3, 4).Normalized(), ray3.direction);
}

TEST(Ray2, PointAt)
{
    Ray2D ray;
    EXPECT_EQ(Vector2D(4.5, 0.0), ray.PointAt(4.5));
}