#include "gtest/gtest.h"

#include <Core/Geometry/Plane.hpp>

using namespace CubbyFlow;

TEST(Plane2, Builder)
{
    Plane2 plane =
        Plane2::GetBuilder().WithNormal({ 1, 0 }).WithPoint({ 2, 3 }).Build();

    EXPECT_EQ(Vector2D(1, 0), plane.normal);
    EXPECT_EQ(Vector2D(2, 3), plane.point);
}