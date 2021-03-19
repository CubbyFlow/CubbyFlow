#include "UnitTestsUtils.hpp"
#include "pch.hpp"

#include <Core/Geometry/Box.hpp>
#include <Core/Geometry/ImplicitSurfaceSet2.hpp>
#include <Core/Geometry/Plane.hpp>
#include <Core/Geometry/Sphere2.hpp>
#include <Core/Geometry/SurfaceToImplicit2.hpp>

using namespace CubbyFlow;

TEST(ImplicitSurfaceSet2, Constructor)
{
    ImplicitSurfaceSet2 sset;
    EXPECT_EQ(0u, sset.NumberOfSurfaces());

    sset.isNormalFlipped = true;
    auto box = std::make_shared<Box2>(BoundingBox2D({ 0, 0 }, { 1, 2 }));
    sset.AddExplicitSurface(box);

    ImplicitSurfaceSet2 sset2(sset);
    EXPECT_EQ(1u, sset2.NumberOfSurfaces());
    EXPECT_TRUE(sset2.isNormalFlipped);

    ImplicitSurfaceSet2 sset3({ box });
    EXPECT_EQ(1u, sset3.NumberOfSurfaces());
}

TEST(ImplicitSurfaceSet2, NumberOfSurfaces)
{
    ImplicitSurfaceSet2 sset;

    auto box = std::make_shared<Box2>(BoundingBox2D({ 0, 0 }, { 1, 2 }));
    sset.AddExplicitSurface(box);

    EXPECT_EQ(1u, sset.NumberOfSurfaces());
}

TEST(ImplicitSurfaceSet2, SurfaceAt)
{
    ImplicitSurfaceSet2 sset;

    auto box1 = std::make_shared<Box2>(BoundingBox2D({ 0, 0 }, { 1, 2 }));
    auto box2 = std::make_shared<Box2>(BoundingBox2D({ 3, 4 }, { 5, 6 }));
    sset.AddExplicitSurface(box1);
    sset.AddExplicitSurface(box2);

    auto implicitSurfaceAt0 =
        std::dynamic_pointer_cast<SurfaceToImplicit2>(sset.SurfaceAt(0));
    auto implicitSurfaceAt1 =
        std::dynamic_pointer_cast<SurfaceToImplicit2>(sset.SurfaceAt(1));

    EXPECT_EQ(std::dynamic_pointer_cast<Surface2>(box1),
              implicitSurfaceAt0->GetSurface());
    EXPECT_EQ(std::dynamic_pointer_cast<Surface2>(box2),
              implicitSurfaceAt1->GetSurface());
}

TEST(ImplicitSurfaceSet2, AddSurface)
{
    ImplicitSurfaceSet2 sset;

    auto box1 = std::make_shared<Box2>(BoundingBox2D({ 0, 0 }, { 1, 2 }));
    auto box2 = std::make_shared<Box2>(BoundingBox2D({ 3, 4 }, { 5, 6 }));
    auto implicitBox = std::make_shared<SurfaceToImplicit2>(box2);

    sset.AddExplicitSurface(box1);
    sset.AddSurface(implicitBox);

    EXPECT_EQ(2u, sset.NumberOfSurfaces());

    auto implicitSurfaceAt0 =
        std::dynamic_pointer_cast<SurfaceToImplicit2>(sset.SurfaceAt(0));
    auto implicitSurfaceAt1 =
        std::dynamic_pointer_cast<SurfaceToImplicit2>(sset.SurfaceAt(1));

    EXPECT_EQ(std::dynamic_pointer_cast<Surface2>(box1),
              implicitSurfaceAt0->GetSurface());
    EXPECT_EQ(implicitBox, implicitSurfaceAt1);
}

TEST(ImplicitSurfaceSet2, ClosestPoint)
{
    BoundingBox2D bbox(Vector2D(), Vector2D(1, 2));

    Box2Ptr box = std::make_shared<Box2>(bbox);
    box->isNormalFlipped = true;

    ImplicitSurfaceSet2Ptr sset = std::make_shared<ImplicitSurfaceSet2>();
    Vector2D emptyPoint = sset->ClosestPoint({ 1.0, 2.0 });
    EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(), emptyPoint.x);
    EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(), emptyPoint.y);

    sset->AddExplicitSurface(box);

    Vector2D pt(0.5, 2.5);
    Vector2D boxPoint = box->ClosestPoint(pt);
    Vector2D setPoint = sset->ClosestPoint(pt);
    EXPECT_DOUBLE_EQ(boxPoint.x, setPoint.x);
    EXPECT_DOUBLE_EQ(boxPoint.y, setPoint.y);
}

TEST(ImplicitSurfaceSet2, ClosestDistance)
{
    BoundingBox2D bbox(Vector2D(), Vector2D(1, 2));

    Box2Ptr box = std::make_shared<Box2>(bbox);
    box->isNormalFlipped = true;

    ImplicitSurfaceSet2Ptr sset = std::make_shared<ImplicitSurfaceSet2>();
    sset->AddExplicitSurface(box);

    Vector2D pt(0.5, 2.5);
    double boxDist = box->ClosestDistance(pt);
    double setDist = sset->ClosestDistance(pt);
    EXPECT_DOUBLE_EQ(boxDist, setDist);
}

TEST(ImplicitSurfaceSet2, Intersects)
{
    ImplicitSurfaceSet2 sset;
    auto box = std::make_shared<Box2>(BoundingBox2D({ -1, 2 }, { 5, 3 }));
    sset.AddExplicitSurface(box);

    bool result0 =
        sset.Intersects(Ray2D(Vector2D(1, 4), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result0);

    bool result1 =
        sset.Intersects(Ray2D(Vector2D(1, 2.5), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result1);

    bool result2 =
        sset.Intersects(Ray2D(Vector2D(1, 1), Vector2D(-1, -1).Normalized()));
    EXPECT_FALSE(result2);
}

TEST(ImplicitSurfaceSet2, ClosestIntersection)
{
    ImplicitSurfaceSet2 sset;
    auto box = std::make_shared<Box2>(BoundingBox2D({ -1, 2 }, { 5, 3 }));
    sset.AddExplicitSurface(box);

    SurfaceRayIntersection2 result0 = sset.ClosestIntersection(
        Ray2D(Vector2D(1, 4), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result0.isIntersecting);
    EXPECT_DOUBLE_EQ(std::sqrt(2), result0.distance);
    EXPECT_EQ(Vector2D(0, 3), result0.point);

    SurfaceRayIntersection2 result1 = sset.ClosestIntersection(
        Ray2D(Vector2D(1, 2.5), Vector2D(-1, -1).Normalized()));
    EXPECT_TRUE(result1.isIntersecting);
    EXPECT_DOUBLE_EQ(std::sqrt(0.5), result1.distance);
    EXPECT_EQ(Vector2D(0.5, 2), result1.point);

    SurfaceRayIntersection2 result2 = sset.ClosestIntersection(
        Ray2D(Vector2D(1, 1), Vector2D(-1, -1).Normalized()));
    EXPECT_FALSE(result2.isIntersecting);
}

TEST(ImplicitSurfaceSet2, BoundingBox)
{
    ImplicitSurfaceSet2 sset;

    auto box1 = std::make_shared<Box2>(BoundingBox2D({ 0, 0 }, { 1, 2 }));
    auto box2 = std::make_shared<Box2>(BoundingBox2D({ 3, 4 }, { 5, 6 }));
    sset.AddExplicitSurface(box1);
    sset.AddExplicitSurface(box2);

    auto bbox = sset.GetBoundingBox();
    EXPECT_DOUBLE_EQ(0.0, bbox.lowerCorner.x);
    EXPECT_DOUBLE_EQ(0.0, bbox.lowerCorner.y);
    EXPECT_DOUBLE_EQ(5.0, bbox.upperCorner.x);
    EXPECT_DOUBLE_EQ(6.0, bbox.upperCorner.y);
}

TEST(ImplicitSurfaceSet2, SignedDistance)
{
    BoundingBox2D bbox(Vector2D(1, 4), Vector2D(5, 6));

    Box2Ptr box = std::make_shared<Box2>(bbox);
    auto implicitBox = std::make_shared<SurfaceToImplicit2>(box);

    ImplicitSurfaceSet2Ptr sset = std::make_shared<ImplicitSurfaceSet2>();
    sset->AddExplicitSurface(box);

    Vector2D pt(-1, 7);
    double boxDist = implicitBox->SignedDistance(pt);
    double setDist = sset->SignedDistance(pt);
    EXPECT_DOUBLE_EQ(boxDist, setDist);
}

TEST(ImplicitSurfaceSet2, ClosestNormal)
{
    BoundingBox2D bbox(Vector2D(), Vector2D(1, 2));

    Box2Ptr box = std::make_shared<Box2>(bbox);
    box->isNormalFlipped = true;

    ImplicitSurfaceSet2Ptr sset = std::make_shared<ImplicitSurfaceSet2>();
    Vector2D emptyNormal = sset->ClosestNormal({ 1.0, 2.0 });
    // No expected value -- just see if it doesn't crash
    (void)emptyNormal;

    sset->AddExplicitSurface(box);

    Vector2D pt(0.5, 2.5);
    Vector2D boxNormal = box->ClosestNormal(pt);
    Vector2D setNormal = sset->ClosestNormal(pt);
    EXPECT_DOUBLE_EQ(boxNormal.x, setNormal.x);
    EXPECT_DOUBLE_EQ(boxNormal.y, setNormal.y);
}

TEST(ImplicitSurfaceSet2, MixedBoundTypes)
{
    const BoundingBox2D domain(Vector2D{}, Vector2D{ 1, 2 });

    const auto plane = Plane2::Builder{}
                           .WithNormal({ 0, 1 })
                           .WithPoint({ 0.0, 0.25 * domain.Height() })
                           .MakeShared();

    const auto sphere = Sphere2::Builder{}
                            .WithCenter(domain.MidPoint())
                            .WithRadius(0.15 * domain.Width())
                            .MakeShared();

    const auto surfaceSet = ImplicitSurfaceSet2::Builder{}
                                .WithExplicitSurfaces({ plane, sphere })
                                .MakeShared();

    EXPECT_FALSE(surfaceSet->IsBounded());

    const auto cp = surfaceSet->ClosestPoint(Vector2D{ 0.5, 0.4 });
    const Vector2D answer{ 0.5, 0.5 };

    EXPECT_VECTOR2_NEAR(answer, cp, 1e-9);
}

TEST(ImplicitSurfaceSet2, IsValidGeometry)
{
    const auto surfaceSet{ ImplicitSurfaceSet2::Builder().MakeShared() };

    EXPECT_FALSE(surfaceSet->IsValidGeometry());

    const auto box =
        std::make_shared<Box2>(BoundingBox2D{ { 0, 0 }, { 1, 2 } });
    auto surfaceSet2{ ImplicitSurfaceSet2::Builder().MakeShared() };
    surfaceSet2->AddExplicitSurface(box);

    EXPECT_TRUE(surfaceSet2->IsValidGeometry());

    surfaceSet2->AddSurface(surfaceSet);

    EXPECT_FALSE(surfaceSet2->IsValidGeometry());
}

TEST(ImplicitSurfaceSet2, IsInside)
{
    const BoundingBox2D domain(Vector2D{}, Vector2D{ 1, 2 });
    const Vector2D offset{ 1, 2 };

    const auto plane = Plane2::Builder{}
                           .WithNormal({ 0, 1 })
                           .WithPoint({ 0, 0.25 * domain.Height() })
                           .MakeShared();

    const auto sphere = Sphere2::Builder{}
                            .WithCenter(domain.MidPoint())
                            .WithRadius(0.15 * domain.Width())
                            .MakeShared();

    const auto surfaceSet = ImplicitSurfaceSet2::Builder{}
                                .WithExplicitSurfaces({ plane, sphere })
                                .WithTransform(Transform2{ offset, 0.0 })
                                .MakeShared();

    EXPECT_TRUE(surfaceSet->IsInside(Vector2D{ 0.5, 0.25 } + offset));
    EXPECT_TRUE(surfaceSet->IsInside(Vector2D{ 0.5, 1.0 } + offset));
    EXPECT_FALSE(surfaceSet->IsInside(Vector2D{ 0.5, 1.5 } + offset));
}

TEST(ImplicitSurfaceSet2, UpdateQueryEngine)
{
    auto sphere = Sphere2::Builder{}
                      .WithCenter({ -1.0, 1.0 })
                      .WithRadius(0.5)
                      .MakeShared();

    auto surfaceSet = ImplicitSurfaceSet2::Builder{}
                          .WithExplicitSurfaces({ sphere })
                          .WithTransform(Transform2{ { 1.0, 2.0 }, 0.0 })
                          .MakeShared();

    const auto bbox1 = surfaceSet->GetBoundingBox();
    EXPECT_BOUNDING_BOX2_EQ(BoundingBox2D({ -0.5, 2.5 }, { 0.5, 3.5 }), bbox1);

    surfaceSet->transform = Transform2{ { 3.0, -4.0 }, 0.0 };
    surfaceSet->UpdateQueryEngine();
    const auto bbox2 = surfaceSet->GetBoundingBox();
    EXPECT_BOUNDING_BOX2_EQ(BoundingBox2D({ 1.5, -3.5 }, { 2.5, -2.5 }), bbox2);

    sphere->transform = Transform2{ { -6.0, 9.0 }, 0.0 };
    surfaceSet->UpdateQueryEngine();
    const auto bbox3 = surfaceSet->GetBoundingBox();
    EXPECT_BOUNDING_BOX2_EQ(BoundingBox2D({ -4.5, 5.5 }, { -3.5, 6.5 }), bbox3);

    // Plane is unbounded. Total bbox should ignore it.
    auto plane = Plane2::Builder{}.WithNormal({ 1.0, 0.0 }).MakeShared();
    surfaceSet->AddExplicitSurface(plane);
    surfaceSet->UpdateQueryEngine();
    auto bbox4 = surfaceSet->GetBoundingBox();
    EXPECT_BOUNDING_BOX2_EQ(BoundingBox2D({ -4.5, 5.5 }, { -3.5, 6.5 }), bbox4);
}