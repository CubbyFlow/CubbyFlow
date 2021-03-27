#include "UnitTestsUtils.hpp"
#include "pch.hpp"

#include <Core/Geometry/Box.hpp>
#include <Core/Geometry/ImplicitSurfaceSet.hpp>
#include <Core/Geometry/Plane.hpp>
#include <Core/Geometry/Sphere.hpp>
#include <Core/Geometry/SurfaceToImplicit.hpp>

using namespace CubbyFlow;

TEST(ImplicitSurfaceSet3, Constructor)
{
    ImplicitSurfaceSet3 sset;
    EXPECT_EQ(0u, sset.NumberOfSurfaces());

    sset.isNormalFlipped = true;
    auto box = std::make_shared<Box3>(BoundingBox3D({ 0, 0, 0 }, { 1, 2, 3 }));
    sset.AddExplicitSurface(box);

    ImplicitSurfaceSet3 sset2(sset);
    EXPECT_EQ(1u, sset2.NumberOfSurfaces());
    EXPECT_TRUE(sset2.isNormalFlipped);

    ImplicitSurfaceSet3 sset3(Array1<Surface3Ptr>{ box });
    EXPECT_EQ(1u, sset3.NumberOfSurfaces());
}

TEST(ImplicitSurfaceSet3, NumberOfSurfaces)
{
    ImplicitSurfaceSet3 sset;

    auto box = std::make_shared<Box3>(BoundingBox3D({ 0, 0, 0 }, { 1, 2, 3 }));
    sset.AddExplicitSurface(box);

    EXPECT_EQ(1u, sset.NumberOfSurfaces());
}

TEST(ImplicitSurfaceSet3, SurfaceAt)
{
    ImplicitSurfaceSet3 sset;

    auto box1 = std::make_shared<Box3>(BoundingBox3D({ 0, 0, 0 }, { 1, 2, 3 }));
    auto box2 = std::make_shared<Box3>(BoundingBox3D({ 3, 4, 5 }, { 5, 6, 7 }));
    sset.AddExplicitSurface(box1);
    sset.AddExplicitSurface(box2);

    auto implicitSurfaceAt0 =
        std::dynamic_pointer_cast<SurfaceToImplicit3>(sset.SurfaceAt(0));
    auto implicitSurfaceAt1 =
        std::dynamic_pointer_cast<SurfaceToImplicit3>(sset.SurfaceAt(1));

    EXPECT_EQ(std::dynamic_pointer_cast<Surface3>(box1),
              implicitSurfaceAt0->GetSurface());
    EXPECT_EQ(std::dynamic_pointer_cast<Surface3>(box2),
              implicitSurfaceAt1->GetSurface());
}

TEST(ImplicitSurfaceSet3, AddSurface)
{
    ImplicitSurfaceSet3 sset;

    auto box1 = std::make_shared<Box3>(BoundingBox3D({ 0, 0, 0 }, { 1, 2, 3 }));
    auto box2 = std::make_shared<Box3>(BoundingBox3D({ 3, 4, 5 }, { 5, 6, 7 }));
    auto implicitBox = std::make_shared<SurfaceToImplicit3>(box2);

    sset.AddExplicitSurface(box1);
    sset.AddSurface(implicitBox);

    EXPECT_EQ(2u, sset.NumberOfSurfaces());

    auto implicitSurfaceAt0 =
        std::dynamic_pointer_cast<SurfaceToImplicit3>(sset.SurfaceAt(0));
    auto implicitSurfaceAt1 =
        std::dynamic_pointer_cast<SurfaceToImplicit3>(sset.SurfaceAt(1));

    EXPECT_EQ(std::dynamic_pointer_cast<Surface3>(box1),
              implicitSurfaceAt0->GetSurface());
    EXPECT_EQ(implicitBox, implicitSurfaceAt1);
}

TEST(ImplicitSurfaceSet3, ClosestPoint)
{
    BoundingBox3D bbox(Vector3D(), Vector3D(1, 2, 3));

    Box3Ptr box = std::make_shared<Box3>(bbox);
    box->isNormalFlipped = true;

    ImplicitSurfaceSet3Ptr sset = std::make_shared<ImplicitSurfaceSet3>();
    Vector3D emptyPoint = sset->ClosestPoint({ 1.0, 2.0, 3.0 });
    EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(), emptyPoint.x);
    EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(), emptyPoint.y);
    EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(), emptyPoint.z);

    sset->AddExplicitSurface(box);

    Vector3D pt(0.5, 2.5, -1.0);
    Vector3D boxPoint = box->ClosestPoint(pt);
    Vector3D setPoint = sset->ClosestPoint(pt);
    EXPECT_DOUBLE_EQ(boxPoint.x, setPoint.x);
    EXPECT_DOUBLE_EQ(boxPoint.y, setPoint.y);
    EXPECT_DOUBLE_EQ(boxPoint.z, setPoint.z);
}

TEST(ImplicitSurfaceSet3, ClosestDistance)
{
    BoundingBox3D bbox(Vector3D(), Vector3D(1, 2, 3));

    Box3Ptr box = std::make_shared<Box3>(bbox);
    box->isNormalFlipped = true;

    ImplicitSurfaceSet3Ptr sset = std::make_shared<ImplicitSurfaceSet3>();
    sset->AddExplicitSurface(box);

    Vector3D pt(0.5, 2.5, -1.0);
    double boxDist = box->ClosestDistance(pt);
    double setDist = sset->ClosestDistance(pt);
    EXPECT_DOUBLE_EQ(boxDist, setDist);
}

TEST(ImplicitSurfaceSet3, Intersects)
{
    ImplicitSurfaceSet3 sset;
    auto box = std::make_shared<Box3>(BoundingBox3D({ -1, 2, 3 }, { 5, 3, 7 }));
    sset.AddExplicitSurface(box);

    bool result0 = sset.Intersects(
        Ray3D(Vector3D(1, 4, 5), Vector3D(-1, -1, -1).Normalized()));
    EXPECT_TRUE(result0);

    bool result1 = sset.Intersects(
        Ray3D(Vector3D(1, 2.5, 6), Vector3D(-1, -1, 1).Normalized()));
    EXPECT_TRUE(result1);

    bool result2 = sset.Intersects(
        Ray3D(Vector3D(1, 1, 2), Vector3D(-1, -1, -1).Normalized()));
    EXPECT_FALSE(result2);
}

TEST(ImplicitSurfaceSet3, ClosestIntersection)
{
    ImplicitSurfaceSet3 sset;
    auto box = std::make_shared<Box3>(BoundingBox3D({ -1, 2, 3 }, { 5, 3, 7 }));
    sset.AddExplicitSurface(box);

    SurfaceRayIntersection3 result0 = sset.ClosestIntersection(
        Ray3D(Vector3D(1, 4, 5), Vector3D(-1, -1, -1).Normalized()));
    EXPECT_TRUE(result0.isIntersecting);
    EXPECT_DOUBLE_EQ(std::sqrt(3), result0.distance);
    EXPECT_EQ(Vector3D(0, 3, 4), result0.point);

    SurfaceRayIntersection3 result1 = sset.ClosestIntersection(
        Ray3D(Vector3D(1, 2.5, 6), Vector3D(-1, -1, 1).Normalized()));
    EXPECT_TRUE(result1.isIntersecting);
    EXPECT_DOUBLE_EQ(std::sqrt(0.75), result1.distance);
    EXPECT_EQ(Vector3D(0.5, 2, 6.5), result1.point);

    SurfaceRayIntersection3 result2 = sset.ClosestIntersection(
        Ray3D(Vector3D(1, 1, 2), Vector3D(-1, -1, -1).Normalized()));
    EXPECT_FALSE(result2.isIntersecting);
}

TEST(ImplicitSurfaceSet3, BoundingBox)
{
    ImplicitSurfaceSet3 sset;

    auto box1 =
        std::make_shared<Box3>(BoundingBox3D({ 0, -3, -1 }, { 1, 2, 4 }));
    auto box2 = std::make_shared<Box3>(BoundingBox3D({ 3, 4, 2 }, { 5, 6, 9 }));
    sset.AddExplicitSurface(box1);
    sset.AddExplicitSurface(box2);

    auto bbox = sset.GetBoundingBox();
    EXPECT_DOUBLE_EQ(0.0, bbox.lowerCorner.x);
    EXPECT_DOUBLE_EQ(-3.0, bbox.lowerCorner.y);
    EXPECT_DOUBLE_EQ(-1.0, bbox.lowerCorner.z);
    EXPECT_DOUBLE_EQ(5.0, bbox.upperCorner.x);
    EXPECT_DOUBLE_EQ(6.0, bbox.upperCorner.y);
    EXPECT_DOUBLE_EQ(9.0, bbox.upperCorner.z);
}

TEST(ImplicitSurfaceSet3, SignedDistance)
{
    BoundingBox3D bbox(Vector3D(1, 4, 3), Vector3D(5, 6, 9));

    Box3Ptr box = std::make_shared<Box3>(bbox);
    auto implicitBox = std::make_shared<SurfaceToImplicit3>(box);

    ImplicitSurfaceSet3Ptr sset = std::make_shared<ImplicitSurfaceSet3>();
    sset->AddExplicitSurface(box);

    Vector3D pt(-1, 7, 8);
    double boxDist = implicitBox->SignedDistance(pt);
    double setDist = sset->SignedDistance(pt);
    EXPECT_DOUBLE_EQ(boxDist, setDist);
}

TEST(ImplicitSurfaceSet3, ClosestNormal)
{
    BoundingBox3D bbox(Vector3D(), Vector3D(1, 2, 3));

    Box3Ptr box = std::make_shared<Box3>(bbox);
    box->isNormalFlipped = true;

    ImplicitSurfaceSet3Ptr sset = std::make_shared<ImplicitSurfaceSet3>();
    Vector3D emptyNormal = sset->ClosestNormal({ 1.0, 2.0, 3.0 });
    // No excepted value -- just see if it doesn't crash
    (void)emptyNormal;

    sset->AddExplicitSurface(box);

    Vector3D pt(0.5, 2.5, -1.0);
    Vector3D boxNormal = box->ClosestNormal(pt);
    Vector3D setNormal = sset->ClosestNormal(pt);
    EXPECT_DOUBLE_EQ(boxNormal.x, setNormal.x);
    EXPECT_DOUBLE_EQ(boxNormal.y, setNormal.y);
    EXPECT_DOUBLE_EQ(boxNormal.z, setNormal.z);
}

TEST(ImplicitSurfaceSet3, MixedBoundTypes)
{
    const BoundingBox3D domain(Vector3D{}, Vector3D{ 1, 2, 1 });

    const auto plane = Plane3::Builder{}
                           .WithNormal({ 0, 1, 0 })
                           .WithPoint({ 0, 0.25 * domain.Height(), 0 })
                           .MakeShared();

    const auto sphere = Sphere3::Builder{}
                            .WithCenter(domain.MidPoint())
                            .WithRadius(0.15 * domain.Width())
                            .MakeShared();

    const auto surfaceSet =
        ImplicitSurfaceSet3::Builder{}
            .WithExplicitSurfaces(Array1<Surface3Ptr>{ plane, sphere })
            .MakeShared();

    EXPECT_FALSE(surfaceSet->IsBounded());

    const auto cp = surfaceSet->ClosestPoint(Vector3D{ 0.5, 0.4, 0.5 });
    const Vector3D answer{ 0.5, 0.5, 0.5 };

    EXPECT_VECTOR3_NEAR(answer, cp, 1e-9);
}

TEST(ImplicitSurfaceSet3, IsValidGeometry)
{
    const auto surfaceSet{ ImplicitSurfaceSet3::Builder().MakeShared() };

    EXPECT_FALSE(surfaceSet->IsValidGeometry());

    const auto box =
        std::make_shared<Box3>(BoundingBox3D{ { 0, 0, 0 }, { 1, 2, 3 } });
    auto surfaceSet2{ ImplicitSurfaceSet3::Builder().MakeShared() };
    surfaceSet2->AddExplicitSurface(box);

    EXPECT_TRUE(surfaceSet2->IsValidGeometry());

    surfaceSet2->AddSurface(surfaceSet);

    EXPECT_FALSE(surfaceSet2->IsValidGeometry());
}

TEST(ImplicitSurfaceSet3, IsInside)
{
    const BoundingBox3D domain(Vector3D{}, Vector3D{ 1, 2, 1 });
    const Vector3D offset{ 1, 2, 3 };

    const auto plane = Plane3::Builder{}
                           .WithNormal({ 0, 1, 0 })
                           .WithPoint({ 0, 0.25 * domain.Height(), 0 })
                           .MakeShared();

    const auto sphere = Sphere3::Builder{}
                            .WithCenter(domain.MidPoint())
                            .WithRadius(0.15 * domain.Width())
                            .MakeShared();

    const auto surfaceSet =
        ImplicitSurfaceSet3::Builder{}
            .WithExplicitSurfaces(Array1<Surface3Ptr>{ plane, sphere })
            .WithTransform(Transform3{ offset, QuaternionD{} })
            .MakeShared();

    EXPECT_TRUE(surfaceSet->IsInside(Vector3D{ 0.5, 0.25, 0.5 } + offset));
    EXPECT_TRUE(surfaceSet->IsInside(Vector3D{ 0.5, 1.0, 0.5 } + offset));
    EXPECT_FALSE(surfaceSet->IsInside(Vector3D{ 0.5, 1.5, 0.5 } + offset));
}

TEST(ImplicitSurfaceSet3, UpdateQueryEngine)
{
    auto sphere = Sphere3::Builder{}
                      .WithCenter({ -1.0, 1.0, 2.0 })
                      .WithRadius(0.5)
                      .MakeShared();

    auto surfaceSet =
        ImplicitSurfaceSet3::Builder{}
            .WithExplicitSurfaces(Array1<Surface3Ptr>{ sphere })
            .WithTransform(Transform3{ { 1.0, 2.0, -1.0 }, QuaternionD{} })
            .MakeShared();

    const auto bbox1 = surfaceSet->GetBoundingBox();
    EXPECT_BOUNDING_BOX2_EQ(
        BoundingBox3D({ -0.5, 2.5, 0.5 }, { 0.5, 3.5, 1.5 }), bbox1);

    surfaceSet->transform = Transform3({ 3.0, -4.0, 7.0 }, QuaternionD{});
    surfaceSet->UpdateQueryEngine();
    const auto bbox2 = surfaceSet->GetBoundingBox();
    EXPECT_BOUNDING_BOX2_EQ(
        BoundingBox3D({ 1.5, -3.5, 4.5 }, { 2.5, -2.5, 5.5 }), bbox2);

    sphere->transform = Transform3{ { -6.0, 9.0, 2.0 }, QuaternionD{} };
    surfaceSet->UpdateQueryEngine();
    const auto bbox3 = surfaceSet->GetBoundingBox();
    EXPECT_BOUNDING_BOX2_EQ(
        BoundingBox3D({ -4.5, 5.5, 10.5 }, { -3.5, 6.5, 11.5 }), bbox3);

    // Plane is unbounded. Total bbox should ignore it.
    auto plane = Plane3::Builder{}.WithNormal({ 1.0, 0.0, 0.0 }).MakeShared();
    surfaceSet->AddExplicitSurface(plane);
    surfaceSet->UpdateQueryEngine();
    auto bbox4 = surfaceSet->GetBoundingBox();
    EXPECT_BOUNDING_BOX2_EQ(
        BoundingBox3D({ -4.5, 5.5, 10.5 }, { -3.5, 6.5, 11.5 }), bbox4);
}