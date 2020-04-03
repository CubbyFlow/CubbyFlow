#include "pch.hpp"

#include <Core/Geometry/Box3.hpp>
#include <Core/Surface/SurfaceToImplicit3.hpp>

using namespace CubbyFlow;

TEST(SurfaceToImplicit3, Constructor)
{
    auto box = std::make_shared<Box3>(BoundingBox3D({ 0, 0, 0 }, { 1, 2, 3 }));

    SurfaceToImplicit3 s2i(box);
    EXPECT_EQ(box, s2i.GetSurface());

    s2i.isNormalFlipped = true;
    SurfaceToImplicit3 s2i2(s2i);
    EXPECT_EQ(box, s2i2.GetSurface());
    EXPECT_TRUE(s2i2.isNormalFlipped);
}

TEST(SurfaceToImplicit3, ClosestPoint)
{
    BoundingBox3D bbox(Vector3D(), Vector3D(1, 2, 3));

    Box3Ptr box = std::make_shared<Box3>(bbox);

    SurfaceToImplicit3 s2i(box);

    Vector3D pt(0.5, 2.5, -1.0);
    Vector3D boxPoint = box->ClosestPoint(pt);
    Vector3D s2iPoint = s2i.ClosestPoint(pt);
    EXPECT_DOUBLE_EQ(boxPoint.x, s2iPoint.x);
    EXPECT_DOUBLE_EQ(boxPoint.y, s2iPoint.y);
    EXPECT_DOUBLE_EQ(boxPoint.z, s2iPoint.z);
}

TEST(SurfaceToImplicit3, ClosestDistance)
{
    BoundingBox3D bbox(Vector3D(), Vector3D(1, 2, 3));

    Box3Ptr box = std::make_shared<Box3>(bbox);

    SurfaceToImplicit3 s2i(box);

    Vector3D pt(0.5, 2.5, -1.0);
    double boxDist = box->ClosestDistance(pt);
    double s2iDist = s2i.ClosestDistance(pt);
    EXPECT_DOUBLE_EQ(boxDist, s2iDist);
}

TEST(SurfaceToImplicit3, Intersects)
{
    auto box = std::make_shared<Box3>(BoundingBox3D({ -1, 2, 3 }, { 5, 3, 7 }));
    SurfaceToImplicit3 s2i(box);

    bool result0 = s2i.Intersects(
        Ray3D(Vector3D(1, 4, 5), Vector3D(-1, -1, -1).Normalized()));
    EXPECT_TRUE(result0);

    bool result1 = s2i.Intersects(
        Ray3D(Vector3D(1, 2.5, 6), Vector3D(-1, -1, 1).Normalized()));
    EXPECT_TRUE(result1);

    bool result2 = s2i.Intersects(
        Ray3D(Vector3D(1, 1, 2), Vector3D(-1, -1, -1).Normalized()));
    EXPECT_FALSE(result2);
}

TEST(SurfaceToImplicit3, ClosestIntersection)
{
    auto box = std::make_shared<Box3>(BoundingBox3D({ -1, 2, 3 }, { 5, 3, 7 }));
    SurfaceToImplicit3 s2i(box);

    SurfaceRayIntersection3 result0 = s2i.ClosestIntersection(
        Ray3D(Vector3D(1, 4, 5), Vector3D(-1, -1, -1).Normalized()));
    EXPECT_TRUE(result0.isIntersecting);
    EXPECT_DOUBLE_EQ(std::sqrt(3), result0.distance);
    EXPECT_EQ(Vector3D(0, 3, 4), result0.point);

    SurfaceRayIntersection3 result1 = s2i.ClosestIntersection(
        Ray3D(Vector3D(1, 2.5, 6), Vector3D(-1, -1, 1).Normalized()));
    EXPECT_TRUE(result1.isIntersecting);
    EXPECT_DOUBLE_EQ(std::sqrt(0.75), result1.distance);
    EXPECT_EQ(Vector3D(0.5, 2, 6.5), result1.point);

    SurfaceRayIntersection3 result2 = s2i.ClosestIntersection(
        Ray3D(Vector3D(1, 1, 2), Vector3D(-1, -1, -1).Normalized()));
    EXPECT_FALSE(result2.isIntersecting);
}

TEST(SurfaceToImplicit3, BoundingBox)
{
    auto box =
        std::make_shared<Box3>(BoundingBox3D({ 0, -3, -1 }, { 1, 2, 4 }));
    SurfaceToImplicit3 s2i(box);

    auto bbox = s2i.BoundingBox();
    EXPECT_DOUBLE_EQ(0.0, bbox.lowerCorner.x);
    EXPECT_DOUBLE_EQ(-3.0, bbox.lowerCorner.y);
    EXPECT_DOUBLE_EQ(-1.0, bbox.lowerCorner.z);
    EXPECT_DOUBLE_EQ(1.0, bbox.upperCorner.x);
    EXPECT_DOUBLE_EQ(2.0, bbox.upperCorner.y);
    EXPECT_DOUBLE_EQ(4.0, bbox.upperCorner.z);
}

TEST(SurfaceToImplicit3, SignedDistance)
{
    BoundingBox3D bbox(Vector3D(1, 4, 3), Vector3D(5, 6, 9));

    Box3Ptr box = std::make_shared<Box3>(bbox);
    SurfaceToImplicit3 s2i(box);

    Vector3D pt(-1, 7, 8);
    double boxDist = box->ClosestDistance(pt);
    double s2iDist = s2i.SignedDistance(pt);
    EXPECT_DOUBLE_EQ(boxDist, s2iDist);

    s2i.isNormalFlipped = true;
    s2iDist = s2i.SignedDistance(pt);
    EXPECT_DOUBLE_EQ(-boxDist, s2iDist);
}

TEST(SurfaceToImplicit3, ClosestNormal)
{
    BoundingBox3D bbox(Vector3D(), Vector3D(1, 2, 3));

    Box3Ptr box = std::make_shared<Box3>(bbox);
    box->isNormalFlipped = true;

    SurfaceToImplicit3 s2i(box);

    Vector3D pt(0.5, 2.5, -1.0);
    Vector3D boxNormal = box->ClosestNormal(pt);
    Vector3D s2iNormal = s2i.ClosestNormal(pt);
    EXPECT_DOUBLE_EQ(boxNormal.x, s2iNormal.x);
    EXPECT_DOUBLE_EQ(boxNormal.y, s2iNormal.y);
    EXPECT_DOUBLE_EQ(boxNormal.z, s2iNormal.z);
}