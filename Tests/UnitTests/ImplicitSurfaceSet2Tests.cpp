#include "pch.h"

#include <Core/Geometry/Box2.hpp>
#include <Core/Surface/ImplicitSurfaceSet2.h>
#include <Core/Surface/SurfaceToImplicit2.h>

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

	auto implicitSurfaceAt0 = std::dynamic_pointer_cast<SurfaceToImplicit2>(sset.SurfaceAt(0));
	auto implicitSurfaceAt1 = std::dynamic_pointer_cast<SurfaceToImplicit2>(sset.SurfaceAt(1));

	EXPECT_EQ(std::dynamic_pointer_cast<Surface2>(box1), implicitSurfaceAt0->GetSurface());
	EXPECT_EQ(std::dynamic_pointer_cast<Surface2>(box2), implicitSurfaceAt1->GetSurface());
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

	auto implicitSurfaceAt0 = std::dynamic_pointer_cast<SurfaceToImplicit2>(sset.SurfaceAt(0));
	auto implicitSurfaceAt1 = std::dynamic_pointer_cast<SurfaceToImplicit2>(sset.SurfaceAt(1));

	EXPECT_EQ(std::dynamic_pointer_cast<Surface2>(box1), implicitSurfaceAt0->GetSurface());
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

	bool result0 = sset.Intersects(Ray2D(Vector2D(1, 4), Vector2D(-1, -1).Normalized()));
	EXPECT_TRUE(result0);

	bool result1 = sset.Intersects(Ray2D(Vector2D(1, 2.5), Vector2D(-1, -1).Normalized()));
	EXPECT_TRUE(result1);

	bool result2 = sset.Intersects(Ray2D(Vector2D(1, 1), Vector2D(-1, -1).Normalized()));
	EXPECT_FALSE(result2);
}

TEST(ImplicitSurfaceSet2, ClosestIntersection)
{
	ImplicitSurfaceSet2 sset;
	auto box = std::make_shared<Box2>(BoundingBox2D({ -1, 2 }, { 5, 3 }));
	sset.AddExplicitSurface(box);

	SurfaceRayIntersection2 result0 = sset.ClosestIntersection(Ray2D(Vector2D(1, 4), Vector2D(-1, -1).Normalized()));
	EXPECT_TRUE(result0.isIntersecting);
	EXPECT_DOUBLE_EQ(std::sqrt(2), result0.distance);
	EXPECT_EQ(Vector2D(0, 3), result0.point);

	SurfaceRayIntersection2 result1 = sset.ClosestIntersection(Ray2D(Vector2D(1, 2.5), Vector2D(-1, -1).Normalized()));
	EXPECT_TRUE(result1.isIntersecting);
	EXPECT_DOUBLE_EQ(std::sqrt(0.5), result1.distance);
	EXPECT_EQ(Vector2D(0.5, 2), result1.point);

	SurfaceRayIntersection2 result2 = sset.ClosestIntersection(Ray2D(Vector2D(1, 1), Vector2D(-1, -1).Normalized()));
	EXPECT_FALSE(result2.isIntersecting);
}

TEST(ImplicitSurfaceSet2, BoundingBox)
{
	ImplicitSurfaceSet2 sset;

	auto box1 = std::make_shared<Box2>(BoundingBox2D({ 0, 0 }, { 1, 2 }));
	auto box2 = std::make_shared<Box2>(BoundingBox2D({ 3, 4 }, { 5, 6 }));
	sset.AddExplicitSurface(box1);
	sset.AddExplicitSurface(box2);

	auto bbox = sset.BoundingBox();
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