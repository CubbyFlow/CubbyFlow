#include "gtest/gtest.h"

#include <Core/Geometry/Sphere.hpp>

using namespace CubbyFlow;

TEST(Sphere3, Constructors)
{
    Sphere3 sph1;
    EXPECT_DOUBLE_EQ(0.0, sph1.center.x);
    EXPECT_DOUBLE_EQ(0.0, sph1.center.y);
    EXPECT_DOUBLE_EQ(0.0, sph1.center.z);
    EXPECT_DOUBLE_EQ(1.0, sph1.radius);

    Sphere3 sph2({ 3.0, -1.0, 2.0 }, 5.0);
    EXPECT_DOUBLE_EQ(3.0, sph2.center.x);
    EXPECT_DOUBLE_EQ(-1.0, sph2.center.y);
    EXPECT_DOUBLE_EQ(2.0, sph2.center.z);
    EXPECT_DOUBLE_EQ(5.0, sph2.radius);

    sph2.isNormalFlipped = true;

    Sphere3 sph3(sph2);
    EXPECT_DOUBLE_EQ(3.0, sph3.center.x);
    EXPECT_DOUBLE_EQ(-1.0, sph3.center.y);
    EXPECT_DOUBLE_EQ(2.0, sph3.center.z);
    EXPECT_DOUBLE_EQ(5.0, sph3.radius);
    EXPECT_TRUE(sph3.isNormalFlipped);
}

TEST(Sphere3, ClosestPoint)
{
    Sphere3 sph({ 3.0, -1.0, 2.0 }, 5.0);

    auto result1 = sph.ClosestPoint({ 10.0, -1.0, 2.0 });
    EXPECT_DOUBLE_EQ(8.0, result1.x);
    EXPECT_DOUBLE_EQ(-1.0, result1.y);
    EXPECT_DOUBLE_EQ(2.0, result1.z);

    auto result2 = sph.ClosestPoint({ 3.0, -10.0, 2.0 });
    EXPECT_DOUBLE_EQ(3.0, result2.x);
    EXPECT_DOUBLE_EQ(-6.0, result2.y);
    EXPECT_DOUBLE_EQ(2.0, result2.z);

    auto result3 = sph.ClosestPoint({ 3.0, 3.0, 2.0 });
    EXPECT_DOUBLE_EQ(3.0, result3.x);
    EXPECT_DOUBLE_EQ(4.0, result3.y);
    EXPECT_DOUBLE_EQ(2.0, result3.z);
}

TEST(Sphere3, ClosestDistance)
{
    Sphere3 sph({ 3.0, -1.0, 2.0 }, 5.0);

    double result1 = sph.ClosestDistance({ 10.0, -1.0, 2.0 });
    EXPECT_DOUBLE_EQ(2.0, result1);

    double result2 = sph.ClosestDistance({ 3.0, -10.0, 2.0 });
    EXPECT_DOUBLE_EQ(4.0, result2);

    double result3 = sph.ClosestDistance({ 3.0, 3.0, 2.0 });
    EXPECT_DOUBLE_EQ(1.0, result3);
}

TEST(Sphere3, Intersects)
{
    Sphere3 sph({ 3.0, -1.0, 2.0 }, 5.0);
    sph.isNormalFlipped = true;

    bool result1 =
        sph.Intersects(Ray3D({ 10.0, -1.0, 2.0 }, { -1.0, 0.0, 0.0 }));
    EXPECT_TRUE(result1);

    bool result2 =
        sph.Intersects(Ray3D({ 3.0, -10.0, 2.0 }, { 0.0, -1.0, 0.0 }));
    EXPECT_FALSE(result2);

    bool result3 = sph.Intersects(Ray3D({ 3.0, 3.0, 2.0 }, { 1.0, 0.0, 0.0 }));
    EXPECT_TRUE(result3);
}

TEST(Sphere3, ClosestIntersection)
{
    Sphere3 sph({ 3.0, -1.0, 2.0 }, 5.0);
    sph.isNormalFlipped = true;

    auto result1 =
        sph.ClosestIntersection(Ray3D({ 10.0, -1.0, 2.0 }, { -1.0, 0.0, 0.0 }));
    EXPECT_TRUE(result1.isIntersecting);
    EXPECT_DOUBLE_EQ(2.0, result1.distance);
    EXPECT_DOUBLE_EQ(8.0, result1.point.x);
    EXPECT_DOUBLE_EQ(-1.0, result1.point.y);
    EXPECT_DOUBLE_EQ(2.0, result1.point.z);
    EXPECT_DOUBLE_EQ(-1.0, result1.normal.x);
    EXPECT_DOUBLE_EQ(0.0, result1.normal.y);
    EXPECT_DOUBLE_EQ(0.0, result1.normal.z);

    auto result2 =
        sph.ClosestIntersection(Ray3D({ 3.0, -10.0, 2.0 }, { 0.0, -1.0, 0.0 }));
    EXPECT_FALSE(result2.isIntersecting);

    auto result3 =
        sph.ClosestIntersection(Ray3D({ 3.0, 3.0, 2.0 }, { 0.0, 1.0, 0.0 }));
    EXPECT_TRUE(result3.isIntersecting);
    EXPECT_DOUBLE_EQ(1.0, result3.distance);
    EXPECT_DOUBLE_EQ(3.0, result3.point.x);
    EXPECT_DOUBLE_EQ(4.0, result3.point.y);
    EXPECT_DOUBLE_EQ(2.0, result3.point.z);
    EXPECT_DOUBLE_EQ(0.0, result3.normal.x);
    EXPECT_DOUBLE_EQ(-1.0, result3.normal.y);
    EXPECT_DOUBLE_EQ(0.0, result3.normal.z);
}

TEST(Sphere3, GetBoundingBox)
{
    Sphere3 sph({ 3.0, -1.0, 2.0 }, 5.0);
    BoundingBox3D bbox = sph.GetBoundingBox();

    EXPECT_DOUBLE_EQ(-2.0, bbox.lowerCorner.x);
    EXPECT_DOUBLE_EQ(-6.0, bbox.lowerCorner.y);
    EXPECT_DOUBLE_EQ(-3.0, bbox.lowerCorner.z);
    EXPECT_DOUBLE_EQ(8.0, bbox.upperCorner.x);
    EXPECT_DOUBLE_EQ(4.0, bbox.upperCorner.y);
    EXPECT_DOUBLE_EQ(7.0, bbox.upperCorner.z);
}

TEST(Sphere3, ClosestNormal)
{
    Sphere3 sph({ 3.0, -1.0, 2.0 }, 5.0);
    sph.isNormalFlipped = true;

    auto result1 = sph.ClosestNormal({ 10.0, -1.0, 2.0 });
    EXPECT_DOUBLE_EQ(-1.0, result1.x);
    EXPECT_DOUBLE_EQ(0.0, result1.y);
    EXPECT_DOUBLE_EQ(0.0, result1.z);

    auto result2 = sph.ClosestNormal({ 3.0, -10.0, 2.0 });
    EXPECT_DOUBLE_EQ(0.0, result2.x);
    EXPECT_DOUBLE_EQ(1.0, result2.y);
    EXPECT_DOUBLE_EQ(0.0, result2.z);

    auto result3 = sph.ClosestNormal({ 3.0, 3.0, 2.0 });
    EXPECT_DOUBLE_EQ(0.0, result3.x);
    EXPECT_DOUBLE_EQ(-1.0, result3.y);
    EXPECT_DOUBLE_EQ(0.0, result3.z);
}

TEST(Sphere3, Builder)
{
    Sphere3 sph = Sphere3::GetBuilder()
                      .WithCenter({ 3.0, -1.0, 2.0 })
                      .WithRadius(5.0)
                      .WithIsNormalFlipped(true)
                      .Build();
    EXPECT_DOUBLE_EQ(3.0, sph.center.x);
    EXPECT_DOUBLE_EQ(-1.0, sph.center.y);
    EXPECT_DOUBLE_EQ(2.0, sph.center.z);
    EXPECT_DOUBLE_EQ(5.0, sph.radius);
    EXPECT_TRUE(sph.isNormalFlipped);
}