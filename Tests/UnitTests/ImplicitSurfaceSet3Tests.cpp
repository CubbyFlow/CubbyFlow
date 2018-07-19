#include "pch.h"

#include <Geometry/Box3.h>
#include <Surface/ImplicitSurfaceSet3.h>
#include <Surface/SurfaceToImplicit3.h>

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

	auto implicitSurfaceAt0	= std::dynamic_pointer_cast<SurfaceToImplicit3>(sset.SurfaceAt(0));
	auto implicitSurfaceAt1 = std::dynamic_pointer_cast<SurfaceToImplicit3>(sset.SurfaceAt(1));

	EXPECT_EQ(std::dynamic_pointer_cast<Surface3>(box1), implicitSurfaceAt0->GetSurface());
	EXPECT_EQ(std::dynamic_pointer_cast<Surface3>(box2), implicitSurfaceAt1->GetSurface());
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

	auto implicitSurfaceAt0 = std::dynamic_pointer_cast<SurfaceToImplicit3>(sset.SurfaceAt(0));
	auto implicitSurfaceAt1 = std::dynamic_pointer_cast<SurfaceToImplicit3>(sset.SurfaceAt(1));

	EXPECT_EQ(std::dynamic_pointer_cast<Surface3>(box1), implicitSurfaceAt0->GetSurface());
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

	bool result0 = sset.Intersects(Ray3D(Vector3D(1, 4, 5), Vector3D(-1, -1, -1).Normalized()));
	EXPECT_TRUE(result0);

	bool result1 = sset.Intersects(Ray3D(Vector3D(1, 2.5, 6), Vector3D(-1, -1, 1).Normalized()));
	EXPECT_TRUE(result1);

	bool result2 = sset.Intersects(Ray3D(Vector3D(1, 1, 2), Vector3D(-1, -1, -1).Normalized()));
	EXPECT_FALSE(result2);
}

TEST(ImplicitSurfaceSet3, ClosestIntersection)
{
	ImplicitSurfaceSet3 sset;
	auto box = std::make_shared<Box3>(BoundingBox3D({ -1, 2, 3 }, { 5, 3, 7 }));
	sset.AddExplicitSurface(box);

	SurfaceRayIntersection3 result0 = sset.ClosestIntersection(Ray3D(Vector3D(1, 4, 5), Vector3D(-1, -1, -1).Normalized()));
	EXPECT_TRUE(result0.isIntersecting);
	EXPECT_DOUBLE_EQ(std::sqrt(3), result0.distance);
	EXPECT_EQ(Vector3D(0, 3, 4), result0.point);

	SurfaceRayIntersection3 result1 = sset.ClosestIntersection(Ray3D(Vector3D(1, 2.5, 6), Vector3D(-1, -1, 1).Normalized()));
	EXPECT_TRUE(result1.isIntersecting);
	EXPECT_DOUBLE_EQ(std::sqrt(0.75), result1.distance);
	EXPECT_EQ(Vector3D(0.5, 2, 6.5), result1.point);

	SurfaceRayIntersection3 result2 = sset.ClosestIntersection(Ray3D(Vector3D(1, 1, 2), Vector3D(-1, -1, -1).Normalized()));
	EXPECT_FALSE(result2.isIntersecting);
}

TEST(ImplicitSurfaceSet3, BoundingBox)
{
	ImplicitSurfaceSet3 sset;

	auto box1 = std::make_shared<Box3>(BoundingBox3D({ 0, -3, -1 }, { 1, 2, 4 }));
	auto box2 = std::make_shared<Box3>(BoundingBox3D({ 3, 4, 2 }, { 5, 6, 9 }));
	sset.AddExplicitSurface(box1);
	sset.AddExplicitSurface(box2);

	auto bbox = sset.BoundingBox();
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