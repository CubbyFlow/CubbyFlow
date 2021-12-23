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

TEST_CASE("[BoundingBox3] - Constructors")
{
    {
        BoundingBox3D box;

        CHECK_EQ(doctest::Approx(std::numeric_limits<double>::max()),
                 box.lowerCorner.x);
        CHECK_EQ(doctest::Approx(std::numeric_limits<double>::max()),
                 box.lowerCorner.y);
        CHECK_EQ(doctest::Approx(std::numeric_limits<double>::max()),
                 box.lowerCorner.z);

        CHECK_EQ(doctest::Approx(-std::numeric_limits<double>::max()),
                 box.upperCorner.x);
        CHECK_EQ(doctest::Approx(-std::numeric_limits<double>::max()),
                 box.upperCorner.y);
        CHECK_EQ(doctest::Approx(-std::numeric_limits<double>::max()),
                 box.upperCorner.z);
    }

    {
        BoundingBox3D box(Vector3D(-2.0, 3.0, 5.0), Vector3D(4.0, -2.0, 1.0));

        CHECK_EQ(doctest::Approx(-2.0), box.lowerCorner.x);
        CHECK_EQ(doctest::Approx(-2.0), box.lowerCorner.y);
        CHECK_EQ(doctest::Approx(1.0), box.lowerCorner.z);

        CHECK_EQ(doctest::Approx(4.0), box.upperCorner.x);
        CHECK_EQ(doctest::Approx(3.0), box.upperCorner.y);
        CHECK_EQ(doctest::Approx(5.0), box.upperCorner.z);
    }

    {
        BoundingBox3D box(Vector3D(-2.0, 3.0, 5.0), Vector3D(4.0, -2.0, 1.0));
        BoundingBox3D box2(box);

        CHECK_EQ(doctest::Approx(-2.0), box2.lowerCorner.x);
        CHECK_EQ(doctest::Approx(-2.0), box2.lowerCorner.y);
        CHECK_EQ(doctest::Approx(1.0), box2.lowerCorner.z);

        CHECK_EQ(doctest::Approx(4.0), box2.upperCorner.x);
        CHECK_EQ(doctest::Approx(3.0), box2.upperCorner.y);
        CHECK_EQ(doctest::Approx(5.0), box2.upperCorner.z);
    }
}

TEST_CASE("[BoundingBox3] - BasicGetters")
{
    BoundingBox3D box(Vector3D(-2.0, 3.0, 5.0), Vector3D(4.0, -2.0, 1.0));

    CHECK_EQ(doctest::Approx(6.0), box.Width());
    CHECK_EQ(doctest::Approx(5.0), box.Height());
    CHECK_EQ(doctest::Approx(4.0), box.Depth());
    CHECK_EQ(doctest::Approx(6.0), box.Length(0));
    CHECK_EQ(doctest::Approx(5.0), box.Length(1));
    CHECK_EQ(doctest::Approx(4.0), box.Length(2));
}

TEST_CASE("[BoundingBox3] - Overlaps")
{
    // x-axis is not overlapping
    {
        BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        BoundingBox3D box2(Vector3D(5.0, 1.0, 3.0), Vector3D(8.0, 2.0, 4.0));

        CHECK_FALSE(box1.Overlaps(box2));
    }

    // y-axis is not overlapping
    {
        BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        BoundingBox3D box2(Vector3D(3.0, 4.0, 3.0), Vector3D(8.0, 6.0, 4.0));

        CHECK_FALSE(box1.Overlaps(box2));
    }

    // z-axis is not overlapping
    {
        BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        BoundingBox3D box2(Vector3D(3.0, 1.0, 6.0), Vector3D(8.0, 2.0, 9.0));

        CHECK_FALSE(box1.Overlaps(box2));
    }

    // overlapping
    {
        BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        BoundingBox3D box2(Vector3D(3.0, 1.0, 3.0), Vector3D(8.0, 2.0, 7.0));

        CHECK(box1.Overlaps(box2));
    }
}

TEST_CASE("[BoundingBox3] - Contains")
{
    // Not containing (x-axis is out)
    {
        BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        Vector3D point(-3.0, 0.0, 4.0);

        CHECK_FALSE(box.Contains(point));
    }

    // Not containing (y-axis is out)
    {
        BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        Vector3D point(2.0, 3.5, 4.0);

        CHECK_FALSE(box.Contains(point));
    }

    // Not containing (z-axis is out)
    {
        BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        Vector3D point(2.0, 0.0, 0.0);

        CHECK_FALSE(box.Contains(point));
    }

    // Containing
    {
        BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        Vector3D point(2.0, 0.0, 4.0);

        CHECK(box.Contains(point));
    }
}

TEST_CASE("[BoundingBox3] - Intersects")
{
    BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));

    Ray3D ray1(Vector3D(-3, 0, 2), Vector3D(2, 1, 1).Normalized());
    CHECK(box.Intersects(ray1));

    Ray3D ray2(Vector3D(3, -1, 3), Vector3D(-1, 2, -3).Normalized());
    CHECK(box.Intersects(ray2));

    Ray3D ray3(Vector3D(1, -5, 1), Vector3D(2, 1, 2).Normalized());
    CHECK_FALSE(box.Intersects(ray3));
}

TEST_CASE("[BoundingBox3] - ClosestIntersection")
{
    BoundingBox3D box(Vector3D(-2.0, -2.0, -1.0), Vector3D(1.0, 0.0, 1.0));

    Ray3D ray1(Vector3D(-4, -3, 0), Vector3D(1, 1, 0).Normalized());
    BoundingBoxRayIntersectionD intersection1 = box.ClosestIntersection(ray1);
    CHECK(intersection1.isIntersecting);
    CHECK_EQ(doctest::Approx(Vector3D(2, 2, 0).Length()), intersection1.near);
    CHECK_EQ(doctest::Approx(Vector3D(3, 3, 0).Length()), intersection1.far);

    Ray3D ray2(Vector3D(0, -1, 0), Vector3D(-2, 1, 1).Normalized());
    BoundingBoxRayIntersectionD intersection2 = box.ClosestIntersection(ray2);
    CHECK(intersection2.isIntersecting);
    CHECK_EQ(doctest::Approx(Vector3D(2, 1, 1).Length()), intersection2.near);
}

TEST_CASE("[BoundingBox3] - MidPoint")
{
    BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
    Vector3D midPoint = box.MidPoint();

    CHECK_EQ(doctest::Approx(1.0), midPoint.x);
    CHECK_EQ(doctest::Approx(0.5), midPoint.y);
    CHECK_EQ(doctest::Approx(3.0), midPoint.z);
}

TEST_CASE("[BoundingBox3] - DiagonalLength")
{
    BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
    double diagLen = box.DiagonalLength();

    CHECK_EQ(doctest::Approx(std::sqrt(6.0 * 6.0 + 5.0 * 5.0 + 4.0 * 4.0)),
             diagLen);
}

TEST_CASE("[BoundingBox3] - DiagonalLengthSquared")
{
    BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
    double diagLenSqr = box.DiagonalLengthSquared();

    CHECK_EQ(doctest::Approx(6.0 * 6.0 + 5.0 * 5.0 + 4.0 * 4.0), diagLenSqr);
}

TEST_CASE("[BoundingBox3] - Reset")
{
    BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
    box.Reset();

    static const double maxDouble = std::numeric_limits<double>::max();

    CHECK_EQ(doctest::Approx(maxDouble), box.lowerCorner.x);
    CHECK_EQ(doctest::Approx(maxDouble), box.lowerCorner.y);
    CHECK_EQ(doctest::Approx(maxDouble), box.lowerCorner.z);

    CHECK_EQ(doctest::Approx(-maxDouble), box.upperCorner.x);
    CHECK_EQ(doctest::Approx(-maxDouble), box.upperCorner.y);
    CHECK_EQ(doctest::Approx(-maxDouble), box.upperCorner.z);
}

TEST_CASE("[BoundingBox3] - Merge")
{
    // Merge with point
    {
        BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        Vector3D point(5.0, 1.0, -1.0);

        box.Merge(point);

        CHECK_EQ(doctest::Approx(-2.0), box.lowerCorner.x);
        CHECK_EQ(doctest::Approx(-2.0), box.lowerCorner.y);
        CHECK_EQ(doctest::Approx(-1.0), box.lowerCorner.z);

        CHECK_EQ(doctest::Approx(5.0), box.upperCorner.x);
        CHECK_EQ(doctest::Approx(3.0), box.upperCorner.y);
        CHECK_EQ(doctest::Approx(5.0), box.upperCorner.z);
    }

    // Merge with other box
    {
        BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
        BoundingBox3D box2(Vector3D(3.0, 1.0, 3.0), Vector3D(8.0, 2.0, 7.0));

        box1.Merge(box2);

        CHECK_EQ(doctest::Approx(-2.0), box1.lowerCorner.x);
        CHECK_EQ(doctest::Approx(-2.0), box1.lowerCorner.y);
        CHECK_EQ(doctest::Approx(1.0), box1.lowerCorner.z);

        CHECK_EQ(doctest::Approx(8.0), box1.upperCorner.x);
        CHECK_EQ(doctest::Approx(3.0), box1.upperCorner.y);
        CHECK_EQ(doctest::Approx(7.0), box1.upperCorner.z);
    }
}

TEST_CASE("[BoundingBox3] - Expand")
{
    BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
    box.Expand(3.0);

    CHECK_EQ(doctest::Approx(-5.0), box.lowerCorner.x);
    CHECK_EQ(doctest::Approx(-5.0), box.lowerCorner.y);
    CHECK_EQ(doctest::Approx(-2.0), box.lowerCorner.z);

    CHECK_EQ(doctest::Approx(7.0), box.upperCorner.x);
    CHECK_EQ(doctest::Approx(6.0), box.upperCorner.y);
    CHECK_EQ(doctest::Approx(8.0), box.upperCorner.z);
}

TEST_CASE("[BoundingBox3] - Corner")
{
    BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));

    CHECK_VECTOR3_EQ(Vector3D(-2.0, -2.0, 1.0), box.Corner(0))
    CHECK_VECTOR3_EQ(Vector3D(4.0, -2.0, 1.0), box.Corner(1))
    CHECK_VECTOR3_EQ(Vector3D(-2.0, 3.0, 1.0), box.Corner(2))
    CHECK_VECTOR3_EQ(Vector3D(4.0, 3.0, 1.0), box.Corner(3))
    CHECK_VECTOR3_EQ(Vector3D(-2.0, -2.0, 5.0), box.Corner(4))
    CHECK_VECTOR3_EQ(Vector3D(4.0, -2.0, 5.0), box.Corner(5))
    CHECK_VECTOR3_EQ(Vector3D(-2.0, 3.0, 5.0), box.Corner(6))
    CHECK_VECTOR3_EQ(Vector3D(4.0, 3.0, 5.0), box.Corner(7))
}

TEST_CASE("[BoundingBox3] - IsEmpty")
{
    BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
    CHECK_FALSE(box.IsEmpty());

    box.lowerCorner = Vector3D(5.0, 1.0, 3.0);
    CHECK(box.IsEmpty());

    box.lowerCorner = Vector3D(2.0, 4.0, 3.0);
    CHECK(box.IsEmpty());

    box.lowerCorner = Vector3D(2.0, 1.0, 6.0);
    CHECK(box.IsEmpty());

    box.lowerCorner = Vector3D(4.0, 1.0, 3.0);
    CHECK(box.IsEmpty());
}