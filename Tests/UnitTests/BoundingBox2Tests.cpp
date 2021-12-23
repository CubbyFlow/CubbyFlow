// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include "UnitTestsUtils.hpp"
#include "doctest_proxy.hpp"

#include <Core/Geometry/BoundingBox.hpp>

using namespace CubbyFlow;

TEST_CASE("[BoundingBox2] - Constructors")
{
    {
        BoundingBox2D box;

        CHECK_EQ(doctest::Approx(std::numeric_limits<double>::max()),
                 box.lowerCorner.x);
        CHECK_EQ(doctest::Approx(std::numeric_limits<double>::max()),
                 box.lowerCorner.y);

        CHECK_EQ(doctest::Approx(-std::numeric_limits<double>::max()),
                 box.upperCorner.x);
        CHECK_EQ(doctest::Approx(-std::numeric_limits<double>::max()),
                 box.upperCorner.y);
    }

    {
        BoundingBox2D box(Vector2D(-2.0, 3.0), Vector2D(4.0, -2.0));

        CHECK_EQ(doctest::Approx(-2.0), box.lowerCorner.x);
        CHECK_EQ(doctest::Approx(-2.0), box.lowerCorner.y);

        CHECK_EQ(doctest::Approx(4.0), box.upperCorner.x);
        CHECK_EQ(doctest::Approx(3.0), box.upperCorner.y);
    }

    {
        BoundingBox2D box(Vector2D(-2.0, 3.0), Vector2D(4.0, -2.0));
        BoundingBox2D box2(box);

        CHECK_EQ(doctest::Approx(-2.0), box2.lowerCorner.x);
        CHECK_EQ(doctest::Approx(-2.0), box2.lowerCorner.y);

        CHECK_EQ(doctest::Approx(4.0), box2.upperCorner.x);
        CHECK_EQ(doctest::Approx(3.0), box2.upperCorner.y);
    }
}

TEST_CASE("[BoundingBox2] - BasicGetters")
{
    BoundingBox2D box(Vector2D(-2.0, 3.0), Vector2D(4.0, -2.0));

    CHECK_EQ(doctest::Approx(6.0), box.Width());
    CHECK_EQ(doctest::Approx(5.0), box.Height());
    CHECK_EQ(doctest::Approx(6.0), box.Length(0));
    CHECK_EQ(doctest::Approx(5.0), box.Length(1));
}

TEST_CASE("[BoundingBox2] - Overlaps")
{
    // x-axis is not overlapping
    {
        BoundingBox2D box1(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        BoundingBox2D box2(Vector2D(5.0, 1.0), Vector2D(8.0, 2.0));

        CHECK_FALSE(box1.Overlaps(box2));
    }

    // y-axis is not overlapping
    {
        BoundingBox2D box1(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        BoundingBox2D box2(Vector2D(3.0, 4.0), Vector2D(8.0, 6.0));

        CHECK_FALSE(box1.Overlaps(box2));
    }

    // overlapping
    {
        BoundingBox2D box1(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        BoundingBox2D box2(Vector2D(3.0, 1.0), Vector2D(8.0, 2.0));

        CHECK(box1.Overlaps(box2));
    }
}

TEST_CASE("[BoundingBox2] - Contains")
{
    // Not containing (x-axis is out)
    {
        BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        Vector2D point(-3.0, 0.0);

        CHECK_FALSE(box.Contains(point));
    }

    // Not containing (y-axis is out)
    {
        BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        Vector2D point(2.0, 3.5);

        CHECK_FALSE(box.Contains(point));
    }

    // Containing
    {
        BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        Vector2D point(2.0, 0.0);

        CHECK(box.Contains(point));
    }
}

TEST_CASE("[BoundingBox2] - Intersects")
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));

    Ray2D ray1(Vector2D(-3, 0), Vector2D(2, 1).Normalized());
    CHECK(box.Intersects(ray1));

    Ray2D ray2(Vector2D(3, -1), Vector2D(-1, 2).Normalized());
    CHECK(box.Intersects(ray2));

    Ray2D ray3(Vector2D(1, -5), Vector2D(2, 1).Normalized());
    CHECK_FALSE(box.Intersects(ray3));
}

TEST_CASE("[BoundingBox2] - ClosestIntersection")
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(1.0, 0.0));

    Ray2D ray1(Vector2D(-4, -3), Vector2D(1, 1).Normalized());
    BoundingBoxRayIntersectionD intersection1 = box.ClosestIntersection(ray1);
    CHECK(intersection1.isIntersecting);
    CHECK_EQ(doctest::Approx(Vector2D(2, 2).Length()), intersection1.near);
    CHECK_EQ(doctest::Approx(Vector2D(3, 3).Length()), intersection1.far);

    Ray2D ray2(Vector2D(0, -1), Vector2D(-2, 1).Normalized());
    BoundingBoxRayIntersectionD intersection2 = box.ClosestIntersection(ray2);
    CHECK(intersection2.isIntersecting);
    CHECK_EQ(doctest::Approx(Vector2D(2, 1).Length()), intersection2.near);
}

TEST_CASE("[BoundingBox2] - MidPoint")
{
    const BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    const Vector2D midPoint = box.MidPoint();

    CHECK_EQ(doctest::Approx(1.0), midPoint.x);
    CHECK_EQ(doctest::Approx(0.5), midPoint.y);
}

TEST_CASE("[BoundingBox2] - DiagonalLength")
{
    const BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    const double diagLen = box.DiagonalLength();

    CHECK_EQ(doctest::Approx(std::sqrt(6.0 * 6.0 + 5.0 * 5.0)), diagLen);
}

TEST_CASE("[BoundingBox2] - DiagonalLengthSquared")
{
    const BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    const double diagLenSqr = box.DiagonalLengthSquared();

    CHECK_EQ(doctest::Approx(6.0 * 6.0 + 5.0 * 5.0), diagLenSqr);
}

TEST_CASE("[BoundingBox2] - Reset")
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    box.Reset();

    static constexpr double maxDouble = std::numeric_limits<double>::max();

    CHECK_EQ(doctest::Approx(maxDouble), box.lowerCorner.x);
    CHECK_EQ(doctest::Approx(maxDouble), box.lowerCorner.y);

    CHECK_EQ(doctest::Approx(-maxDouble), box.upperCorner.x);
    CHECK_EQ(doctest::Approx(-maxDouble), box.upperCorner.y);
}

TEST_CASE("[BoundingBox2] - Merge")
{
    // Merge with point
    {
        BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        Vector2D point(5.0, 1.0);

        box.Merge(point);

        CHECK_EQ(doctest::Approx(-2.0), box.lowerCorner.x);
        CHECK_EQ(doctest::Approx(-2.0), box.lowerCorner.y);

        CHECK_EQ(doctest::Approx(5.0), box.upperCorner.x);
        CHECK_EQ(doctest::Approx(3.0), box.upperCorner.y);
    }

    // Merge with other box
    {
        BoundingBox2D box1(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
        BoundingBox2D box2(Vector2D(3.0, 1.0), Vector2D(8.0, 2.0));

        box1.Merge(box2);

        CHECK_EQ(doctest::Approx(-2.0), box1.lowerCorner.x);
        CHECK_EQ(doctest::Approx(-2.0), box1.lowerCorner.y);

        CHECK_EQ(doctest::Approx(8.0), box1.upperCorner.x);
        CHECK_EQ(doctest::Approx(3.0), box1.upperCorner.y);
    }
}

TEST_CASE("[BoundingBox2] - Expand")
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    box.Expand(3.0);

    CHECK_EQ(doctest::Approx(-5.0), box.lowerCorner.x);
    CHECK_EQ(doctest::Approx(-5.0), box.lowerCorner.y);

    CHECK_EQ(doctest::Approx(7.0), box.upperCorner.x);
    CHECK_EQ(doctest::Approx(6.0), box.upperCorner.y);
}

TEST_CASE("[BoundingBox2] - Corner")
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    CHECK_VECTOR2_EQ(Vector2D(-2.0, -2.0), box.Corner(0))
    CHECK_VECTOR2_EQ(Vector2D(4.0, -2.0), box.Corner(1))
    CHECK_VECTOR2_EQ(Vector2D(-2.0, 3.0), box.Corner(2))
    CHECK_VECTOR2_EQ(Vector2D(4.0, 3.0), box.Corner(3))
}

TEST_CASE("[BoundingBox2] - IsEmpty")
{
    BoundingBox2D box(Vector2D(-2.0, -2.0), Vector2D(4.0, 3.0));
    CHECK_FALSE(box.IsEmpty());

    box.lowerCorner = Vector2D(5.0, 1.0);
    CHECK(box.IsEmpty());

    box.lowerCorner = Vector2D(2.0, 4.0);
    CHECK(box.IsEmpty());

    box.lowerCorner = Vector2D(4.0, 1.0);
    CHECK(box.IsEmpty());
}