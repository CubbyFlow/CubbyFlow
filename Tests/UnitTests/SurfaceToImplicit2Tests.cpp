#include "pch.hpp"

#include <Core/Geometry/Box2.hpp>
#include <Core/Geometry/Plane2.hpp>
#include <Core/Surface/SurfaceSet2.hpp>
#include <Core/Surface/SurfaceToImplicit2.hpp>

using namespace CubbyFlow;

namespace
{
class MockSurface2 final : public Surface2
{
 public:
    MockSurface2() = default;

    ~MockSurface2() = default;

    MOCK_METHOD0(UpdateQueryEngine, void());

 protected:
    MOCK_CONST_METHOD1(ClosestPointLocal, Vector2D(const Vector2D&));
    MOCK_CONST_METHOD0(BoundingBoxLocal, BoundingBox2D());
    MOCK_CONST_METHOD1(ClosestIntersectionLocal,
                       SurfaceRayIntersection2(const Ray2D&));
    MOCK_CONST_METHOD1(ClosestNormalLocal, Vector2D(const Vector2D&));
};
}  // namespace

TEST(SurfaceToImplicit2, Constructor)
{
    auto box = std::make_shared<Box2>(BoundingBox2D({ 0, 0 }, { 1, 2 }));

    SurfaceToImplicit2 s2i(box);
    EXPECT_EQ(box, s2i.GetSurface());

    s2i.isNormalFlipped = true;
    SurfaceToImplicit2 s2i2(s2i);
    EXPECT_EQ(box, s2i2.GetSurface());
    EXPECT_TRUE(s2i2.isNormalFlipped);
}

TEST(SurfaceToImplicit2, UpdateQueryEngine)
{
    const auto mockSurface2 = std::make_shared<MockSurface2>();
    SurfaceToImplicit2 s2i(mockSurface2);

    EXPECT_CALL(*mockSurface2, UpdateQueryEngine()).Times(1);

    s2i.UpdateQueryEngine();
}

TEST(SurfaceToImplicit2, ClosestPoint)
{
    BoundingBox2D bbox(Vector2D(), Vector2D(1, 2));

    Box2Ptr box = std::make_shared<Box2>(bbox);

    SurfaceToImplicit2 s2i(box);

    Vector2D pt(0.5, 2.5);
    Vector2D boxPoint = box->ClosestPoint(pt);
    Vector2D s2iPoint = s2i.ClosestPoint(pt);
    EXPECT_DOUBLE_EQ(boxPoint.x, s2iPoint.x);
    EXPECT_DOUBLE_EQ(boxPoint.y, s2iPoint.y);
}

TEST(SurfaceToImplicit2, ClosestDistance)
{
    BoundingBox2D bbox(Vector2D(), Vector2D(1, 2));

    Box2Ptr box = std::make_shared<Box2>(bbox);

    SurfaceToImplicit2 s2i(box);

    Vector2D pt(0.5, 2.5);
    double boxDist = box->ClosestDistance(pt);
    double s2iDist = s2i.ClosestDistance(pt);
    EXPECT_DOUBLE_EQ(boxDist, s2iDist);
}

TEST(SurfaceToImplicit2, Intersects)
{
    auto box = std::make_shared<Box2>(BoundingBox2D({ -1, 2 }, { 5, 3 }));
    SurfaceToImplicit2 s2i(box);

    bool result0 =
        s2i.Intersects(Ray2D(Vector2D(1, 4), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result0);

    bool result1 =
        s2i.Intersects(Ray2D(Vector2D(1, 2.5), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result1);

    bool result2 =
        s2i.Intersects(Ray2D(Vector2D(1, 1), Vector2D(-1, -1).Normalized()));
    EXPECT_FALSE(result2);
}

TEST(SurfaceToImplicit2, ClosestIntersection)
{
    auto box = std::make_shared<Box2>(BoundingBox2D({ -1, 2 }, { 5, 3 }));
    SurfaceToImplicit2 s2i(box);

    SurfaceRayIntersection2 result0 = s2i.ClosestIntersection(
        Ray2D(Vector2D(1, 4), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result0.isIntersecting);
    EXPECT_DOUBLE_EQ(std::sqrt(2), result0.distance);
    EXPECT_EQ(Vector2D(0, 3), result0.point);

    SurfaceRayIntersection2 result1 = s2i.ClosestIntersection(
        Ray2D(Vector2D(1, 2.5), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result1.isIntersecting);
    EXPECT_DOUBLE_EQ(std::sqrt(0.5), result1.distance);
    EXPECT_EQ(Vector2D(0.5, 2), result1.point);

    SurfaceRayIntersection2 result2 = s2i.ClosestIntersection(
        Ray2D(Vector2D(1, 1), Vector2D(-1, -1).Normalized()));
    EXPECT_FALSE(result2.isIntersecting);
}

TEST(SurfaceToImplicit2, BoundingBox)
{
    auto box = std::make_shared<Box2>(BoundingBox2D({ -1, 2 }, { 5, 3 }));
    SurfaceToImplicit2 s2i(box);

    auto bbox = s2i.BoundingBox();
    EXPECT_DOUBLE_EQ(-1.0, bbox.lowerCorner.x);
    EXPECT_DOUBLE_EQ(2.0, bbox.lowerCorner.y);
    EXPECT_DOUBLE_EQ(5.0, bbox.upperCorner.x);
    EXPECT_DOUBLE_EQ(3.0, bbox.upperCorner.y);
}

TEST(SurfaceToImplicit2, SignedDistance)
{
    BoundingBox2D bbox(Vector2D(1, 4), Vector2D(5, 6));

    Box2Ptr box = std::make_shared<Box2>(bbox);
    SurfaceToImplicit2 s2i(box);

    Vector2D pt(-1, 7);
    double boxDist = box->ClosestDistance(pt);
    double s2iDist = s2i.SignedDistance(pt);
    EXPECT_DOUBLE_EQ(boxDist, s2iDist);

    s2i.isNormalFlipped = true;
    s2iDist = s2i.SignedDistance(pt);
    EXPECT_DOUBLE_EQ(-boxDist, s2iDist);
}

TEST(SurfaceToImplicit2, ClosestNormal)
{
    BoundingBox2D bbox(Vector2D(), Vector2D(1, 2));

    Box2Ptr box = std::make_shared<Box2>(bbox);
    box->isNormalFlipped = true;

    SurfaceToImplicit2 s2i(box);

    Vector2D pt(0.5, 2.5);
    Vector2D boxNormal = box->ClosestNormal(pt);
    Vector2D s2iNormal = s2i.ClosestNormal(pt);
    EXPECT_DOUBLE_EQ(boxNormal.x, s2iNormal.x);
    EXPECT_DOUBLE_EQ(boxNormal.y, s2iNormal.y);
}

TEST(SurfaceToImplicit2, IsBounded)
{
    const Plane2Ptr plane =
        Plane2::Builder{}.WithPoint({ 0, 0 }).WithNormal({ 0, 1 }).MakeShared();
    const SurfaceToImplicit2Ptr s2i =
        SurfaceToImplicit2::Builder{}.WithSurface(plane).MakeShared();
    EXPECT_FALSE(s2i->IsBounded());
}

TEST(SurfaceToImplicit2, IsValidGeometry)
{
    const SurfaceSet2Ptr sset = SurfaceSet2::Builder{}.MakeShared();
    const SurfaceToImplicit2Ptr s2i =
        SurfaceToImplicit2::Builder{}.WithSurface(sset).MakeShared();
    EXPECT_FALSE(s2i->IsValidGeometry());
}

TEST(SurfaceToImplicit2, IsInside)
{
    const Plane2Ptr plane = Plane2::Builder{}
                                .WithPoint({ 0, 0 })
                                .WithNormal({ 0, 1 })
                                .WithTranslation({ 0, -1 })
                                .MakeShared();
    const SurfaceToImplicit2Ptr s2i =
        SurfaceToImplicit2::Builder{}.WithSurface(plane).MakeShared();
    EXPECT_FALSE(s2i->IsInside({ 0, -0.5 }));
    EXPECT_TRUE(s2i->IsInside({ 0, -1.5 }));
}