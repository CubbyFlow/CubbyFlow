#include "UnitTestsUtils.hpp"
#include "gtest/gtest.h"

#include <Core/Geometry/Box.hpp>

using namespace CubbyFlow;

TEST(Box3, Constructors)
{
    {
        Box3 box;

        EXPECT_FALSE(box.isNormalFlipped);
        EXPECT_EQ(Vector3D(), box.bound.lowerCorner);
        EXPECT_EQ(Vector3D(1, 1, 1), box.bound.upperCorner);
    }

    {
        Box3 box(Vector3D(-1, 2, 1), Vector3D(5, 3, 4));

        EXPECT_FALSE(box.isNormalFlipped);
        EXPECT_EQ(Vector3D(-1, 2, 1), box.bound.lowerCorner);
        EXPECT_EQ(Vector3D(5, 3, 4), box.bound.upperCorner);
    }

    {
        Box3 box(BoundingBox3D(Vector3D(-1, 2, 1), Vector3D(5, 3, 4)));

        box.isNormalFlipped = true;
        EXPECT_TRUE(box.isNormalFlipped);
        EXPECT_EQ(Vector3D(-1, 2, 1), box.bound.lowerCorner);
        EXPECT_EQ(Vector3D(5, 3, 4), box.bound.upperCorner);
    }
}

TEST(Box3, ClosestPoint)
{
    Box3 box(Vector3D(-1, 2, 1), Vector3D(5, 3, 4));

    Vector3D result0 = box.ClosestPoint(Vector3D(-2, 4, 5));
    EXPECT_EQ(Vector3D(-1, 3, 4), result0);

    Vector3D result1 = box.ClosestPoint(Vector3D(1, 5, 0));
    EXPECT_EQ(Vector3D(1, 3, 1), result1);

    Vector3D result2 = box.ClosestPoint(Vector3D(9, 5, 7));
    EXPECT_EQ(Vector3D(5, 3, 4), result2);

    Vector3D result3 = box.ClosestPoint(Vector3D(-2, 2.4, 3));
    EXPECT_EQ(Vector3D(-1, 2.4, 3), result3);

    Vector3D result4 = box.ClosestPoint(Vector3D(1, 2.6, 1.1));
    EXPECT_EQ(Vector3D(1, 2.6, 1), result4);

    Vector3D result5 = box.ClosestPoint(Vector3D(9, 2.2, -1));
    EXPECT_EQ(Vector3D(5, 2.2, 1), result5);

    Vector3D result6 = box.ClosestPoint(Vector3D(-2, 1, 1.1));
    EXPECT_EQ(Vector3D(-1, 2, 1.1), result6);

    Vector3D result7 = box.ClosestPoint(Vector3D(1, 0, 3.5));
    EXPECT_EQ(Vector3D(1, 2, 3.5), result7);

    Vector3D result8 = box.ClosestPoint(Vector3D(9, -1, -3));
    EXPECT_EQ(Vector3D(5, 2, 1), result8);
}

TEST(Box3, ClosestDistance)
{
    Box3 box(Vector3D(-1, 2, 1), Vector3D(5, 3, 4));

    double result0 = box.ClosestDistance(Vector3D(-2, 4, 5));
    EXPECT_DOUBLE_EQ(Vector3D(-1, 3, 4).DistanceTo(Vector3D(-2, 4, 5)),
                     result0);

    double result1 = box.ClosestDistance(Vector3D(1, 5, 0));
    EXPECT_DOUBLE_EQ(Vector3D(1, 3, 1).DistanceTo(Vector3D(1, 5, 0)), result1);

    double result2 = box.ClosestDistance(Vector3D(9, 5, 7));
    EXPECT_DOUBLE_EQ(Vector3D(5, 3, 4).DistanceTo(Vector3D(9, 5, 7)), result2);

    double result3 = box.ClosestDistance(Vector3D(-2, 2.4, 3));
    EXPECT_DOUBLE_EQ(Vector3D(-1, 2.4, 3).DistanceTo(Vector3D(-2, 2.4, 3)),
                     result3);

    double result4 = box.ClosestDistance(Vector3D(1, 2.6, 1.1));
    EXPECT_DOUBLE_EQ(Vector3D(1, 2.6, 1).DistanceTo(Vector3D(1, 2.6, 1.1)),
                     result4);

    double result5 = box.ClosestDistance(Vector3D(9, 2.2, -1));
    EXPECT_DOUBLE_EQ(Vector3D(5, 2.2, 1).DistanceTo(Vector3D(9, 2.2, -1)),
                     result5);

    double result6 = box.ClosestDistance(Vector3D(-2, 1, 1.1));
    EXPECT_DOUBLE_EQ(Vector3D(-1, 2, 1.1).DistanceTo(Vector3D(-2, 1, 1.1)),
                     result6);

    double result7 = box.ClosestDistance(Vector3D(1, 0, 3.5));
    EXPECT_DOUBLE_EQ(Vector3D(1, 2, 3.5).DistanceTo(Vector3D(1, 0, 3.5)),
                     result7);

    double result8 = box.ClosestDistance(Vector3D(9, -1, -3));
    EXPECT_DOUBLE_EQ(Vector3D(5, 2, 1).DistanceTo(Vector3D(9, -1, -3)),
                     result8);
}

TEST(Box3, Intersects)
{
    Box3 box(Vector3D(-1, 2, 3), Vector3D(5, 3, 7));

    bool result0 = box.Intersects(
        Ray3D(Vector3D(1, 4, 5), Vector3D(-1, -1, -1).Normalized()));
    EXPECT_TRUE(result0);

    bool result1 = box.Intersects(
        Ray3D(Vector3D(1, 2.5, 6), Vector3D(-1, -1, 1).Normalized()));
    EXPECT_TRUE(result1);

    bool result2 = box.Intersects(
        Ray3D(Vector3D(1, 1, 2), Vector3D(-1, -1, -1).Normalized()));
    EXPECT_FALSE(result2);
}

TEST(Box3, ClosestIntersection)
{
    const Box3 box{ Vector3D{ -1, 2, 3 }, Vector3D{ 5, 3, 7 },
                    Transform3{ Vector3D{ 1, -3, 2 }, QuaternionD{} } };

    const SurfaceRayIntersection3 result0 = box.ClosestIntersection(
        Ray3D{ Vector3D{ 2, 1, 7 }, Vector3D{ -1, -1, -1 }.Normalized() });
    EXPECT_TRUE(result0.isIntersecting);
    EXPECT_DOUBLE_EQ(std::sqrt(3), result0.distance);
    EXPECT_VECTOR3_EQ(Vector3D(1, 0, 6), result0.point);
    EXPECT_VECTOR3_EQ(Vector3D(0, 1, 0), result0.normal);

    const SurfaceRayIntersection3 result1 = box.ClosestIntersection(
        Ray3D{ Vector3D{ 2, -0.5, 8 }, Vector3D{ -1, -1, 1 }.Normalized() });
    EXPECT_TRUE(result1.isIntersecting);
    EXPECT_DOUBLE_EQ(std::sqrt(0.75), result1.distance);
    EXPECT_VECTOR3_EQ(Vector3D(1.5, -1, 8.5), result1.point);
    EXPECT_VECTOR3_EQ(Vector3D(0, -1, 0), result1.normal);

    const SurfaceRayIntersection3 result2 = box.ClosestIntersection(
        Ray3D{ Vector3D{ 2, -2, 4 }, Vector3D{ -1, -1, -1 }.Normalized() });
    EXPECT_FALSE(result2.isIntersecting);
}

TEST(Box3, GetBoundingBox)
{
    Box3 box(Vector3D(-1, 2, 3), Vector3D(5, 3, 7));
    BoundingBox3D boundingBox = box.GetBoundingBox();

    EXPECT_EQ(Vector3D(-1, 2, 3), boundingBox.lowerCorner);
    EXPECT_EQ(Vector3D(5, 3, 7), boundingBox.upperCorner);
}

TEST(Box3, ClosestNormal)
{
    Box3 box(Vector3D(-1, 2, 1), Vector3D(5, 3, 4));
    box.isNormalFlipped = true;

    Vector3D result0 = box.ClosestNormal(Vector3D(-2, 2, 3));
    EXPECT_EQ(Vector3D(1, 0, 0), result0);

    Vector3D result1 = box.ClosestNormal(Vector3D(3, 5, 2));
    EXPECT_EQ(Vector3D(0, -1, 0), result1);

    Vector3D result2 = box.ClosestNormal(Vector3D(9, 3, 4));
    EXPECT_EQ(Vector3D(-1, 0, 0), result2);

    Vector3D result3 = box.ClosestNormal(Vector3D(4, 1, 1));
    EXPECT_EQ(Vector3D(0, 1, 0), result3);

    Vector3D result4 = box.ClosestNormal(Vector3D(4, 2.5, -1));
    EXPECT_EQ(Vector3D(0, 0, 1), result4);

    Vector3D result5 = box.ClosestNormal(Vector3D(4, 2, 9));
    EXPECT_EQ(Vector3D(0, 0, -1), result5);
}