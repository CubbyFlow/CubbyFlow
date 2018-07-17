#include "pch.h"

#include <Geometry/Cylinder3.h>

using namespace CubbyFlow;

TEST(Cylinder3, Constructors)
{
	Cylinder3 cyl1;
	EXPECT_FALSE(cyl1.isNormalFlipped);
	EXPECT_DOUBLE_EQ(0.0, cyl1.center.x);
	EXPECT_DOUBLE_EQ(0.0, cyl1.center.y);
	EXPECT_DOUBLE_EQ(0.0, cyl1.center.z);
	EXPECT_DOUBLE_EQ(1.0, cyl1.radius);
	EXPECT_DOUBLE_EQ(1.0, cyl1.height);
	EXPECT_DOUBLE_EQ(-1.0, cyl1.BoundingBox().lowerCorner.x);
	EXPECT_DOUBLE_EQ(-0.5, cyl1.BoundingBox().lowerCorner.y);
	EXPECT_DOUBLE_EQ(-1.0, cyl1.BoundingBox().lowerCorner.z);
	EXPECT_DOUBLE_EQ(1.0, cyl1.BoundingBox().upperCorner.x);
	EXPECT_DOUBLE_EQ(0.5, cyl1.BoundingBox().upperCorner.y);
	EXPECT_DOUBLE_EQ(1.0, cyl1.BoundingBox().upperCorner.z);

	Cylinder3 cyl2(Vector3D(1, 2, 3), 4.0, 5.0);
	EXPECT_FALSE(cyl2.isNormalFlipped);
	EXPECT_DOUBLE_EQ(1.0, cyl2.center.x);
	EXPECT_DOUBLE_EQ(2.0, cyl2.center.y);
	EXPECT_DOUBLE_EQ(3.0, cyl2.center.z);
	EXPECT_DOUBLE_EQ(4.0, cyl2.radius);
	EXPECT_DOUBLE_EQ(5.0, cyl2.height);
	EXPECT_DOUBLE_EQ(-3.0, cyl2.BoundingBox().lowerCorner.x);
	EXPECT_DOUBLE_EQ(-0.5, cyl2.BoundingBox().lowerCorner.y);
	EXPECT_DOUBLE_EQ(-1.0, cyl2.BoundingBox().lowerCorner.z);
	EXPECT_DOUBLE_EQ(5.0, cyl2.BoundingBox().upperCorner.x);
	EXPECT_DOUBLE_EQ(4.5, cyl2.BoundingBox().upperCorner.y);
	EXPECT_DOUBLE_EQ(7.0, cyl2.BoundingBox().upperCorner.z);

	cyl2.isNormalFlipped = true;
	Cylinder3 cyl3(cyl2);
	EXPECT_TRUE(cyl3.isNormalFlipped);
	EXPECT_DOUBLE_EQ(1.0, cyl3.center.x);
	EXPECT_DOUBLE_EQ(2.0, cyl3.center.y);
	EXPECT_DOUBLE_EQ(3.0, cyl3.center.z);
	EXPECT_DOUBLE_EQ(4.0, cyl3.radius);
	EXPECT_DOUBLE_EQ(5.0, cyl3.height);
	EXPECT_DOUBLE_EQ(-3.0, cyl3.BoundingBox().lowerCorner.x);
	EXPECT_DOUBLE_EQ(-0.5, cyl3.BoundingBox().lowerCorner.y);
	EXPECT_DOUBLE_EQ(-1.0, cyl3.BoundingBox().lowerCorner.z);
	EXPECT_DOUBLE_EQ(5.0, cyl3.BoundingBox().upperCorner.x);
	EXPECT_DOUBLE_EQ(4.5, cyl3.BoundingBox().upperCorner.y);
	EXPECT_DOUBLE_EQ(7.0, cyl3.BoundingBox().upperCorner.z);
}

TEST(Cylinder3, ClosestPoint)
{
	Cylinder3 cyl(Vector3D(1, 2, 3), 4.0, 6.0);

	Vector3D result1 = cyl.ClosestPoint({ 7, 2, 3 });
	EXPECT_DOUBLE_EQ(5.0, result1.x);
	EXPECT_DOUBLE_EQ(2.0, result1.y);
	EXPECT_DOUBLE_EQ(3.0, result1.z);

	Vector3D result2 = cyl.ClosestPoint({ 1, 6, 2 });
	EXPECT_DOUBLE_EQ(1.0, result2.x);
	EXPECT_DOUBLE_EQ(5.0, result2.y);
	EXPECT_DOUBLE_EQ(2.0, result2.z);

	Vector3D result3 = cyl.ClosestPoint({ 6, -5, 3 });
	EXPECT_DOUBLE_EQ(5.0, result3.x);
	EXPECT_DOUBLE_EQ(-1.0, result3.y);
	EXPECT_DOUBLE_EQ(3.0, result3.z);
}

TEST(Cylinder3, ClosestDistance)
{
	Cylinder3 cyl(Vector3D(1, 2, 3), 4.0, 6.0);

	double result1 = cyl.ClosestDistance({ 7, 2, 3 });
	EXPECT_DOUBLE_EQ(Vector3D(5, 2, 3).DistanceTo({ 7, 2, 3 }), result1);

	double result2 = cyl.ClosestDistance({ 1, 6, 2 });
	EXPECT_DOUBLE_EQ(Vector3D(1, 5, 2).DistanceTo({ 1, 6, 2 }), result2);

	double result3 = cyl.ClosestDistance({ 6, -5, 3 });
	EXPECT_DOUBLE_EQ(Vector3D(5, -1, 3).DistanceTo({ 6, -5, 3 }), result3);
}

TEST(Cylinder3, Intersects)
{
	Cylinder3 cyl(Vector3D(1, 2, 3), 4.0, 6.0);

	EXPECT_TRUE(cyl.Intersects(Ray3D({ 7, 2, 3 }, { -1, 0, 0 })));

	EXPECT_TRUE(cyl.Intersects(Ray3D({ 1, 6, 2 }, { 0, -1, 0 })));

	EXPECT_TRUE(cyl.Intersects(Ray3D({ 1, 2, 3 }, { 0, 1, 0 })));

	EXPECT_FALSE(cyl.Intersects(Ray3D({ 6, -5, 3 }, { 0, 0, 1 })));
}

TEST(Cylinder3, ClosestIntersection)
{
	Cylinder3 cyl(Vector3D(1, 2, 3), 4.0, 6.0);

	auto result1 = cyl.ClosestIntersection(Ray3D({ 7, 2, 3 }, { -1, 0, 0 }));
	EXPECT_TRUE(result1.isIntersecting);
	EXPECT_DOUBLE_EQ(2.0, result1.distance);

	auto result2 = cyl.ClosestIntersection(Ray3D({ 1, 6, 2 }, { 0, -1, 0 }));
	EXPECT_TRUE(result2.isIntersecting);
	EXPECT_DOUBLE_EQ(1.0, result2.distance);

	auto result3 = cyl.ClosestIntersection(Ray3D({ 1, 2, 3 }, { 0, 1, 0 }));
	EXPECT_TRUE(result3.isIntersecting);
	EXPECT_DOUBLE_EQ(3.0, result3.distance);

	auto result4 = cyl.ClosestIntersection(Ray3D({ 6, -5, 3 }, { 0, 0, 1 }));
	EXPECT_FALSE(result4.isIntersecting);
}

TEST(Cylinder3, BoundingBox)
{
	Cylinder3 cyl(Vector3D(1, 2, 3), 4.0, 6.0);
	BoundingBox3D bbox = cyl.BoundingBox();
	EXPECT_DOUBLE_EQ(-3.0, bbox.lowerCorner.x);
	EXPECT_DOUBLE_EQ(-1.0, bbox.lowerCorner.y);
	EXPECT_DOUBLE_EQ(-1.0, bbox.lowerCorner.z);
	EXPECT_DOUBLE_EQ(5.0, bbox.upperCorner.x);
	EXPECT_DOUBLE_EQ(5.0, bbox.upperCorner.y);
	EXPECT_DOUBLE_EQ(7.0, bbox.upperCorner.z);
}

TEST(Cylinder3, ClosestNormal)
{
	Cylinder3 cyl(Vector3D(1, 2, 3), 4.0, 6.0);
	cyl.isNormalFlipped = true;

	Vector3D result1 = cyl.ClosestNormal({ 7, 2, 3 });
	EXPECT_DOUBLE_EQ(-1.0, result1.x);
	EXPECT_DOUBLE_EQ(0.0, result1.y);
	EXPECT_DOUBLE_EQ(0.0, result1.z);

	Vector3D result2 = cyl.ClosestNormal({ 1, 6, 2 });
	EXPECT_DOUBLE_EQ(0.0, result2.x);
	EXPECT_DOUBLE_EQ(-1.0, result2.y);
	EXPECT_DOUBLE_EQ(0.0, result2.z);

	Vector3D result3 = cyl.ClosestNormal({ 6, -1.5, 3 });
	EXPECT_DOUBLE_EQ(-1.0, result3.x);
	EXPECT_DOUBLE_EQ(0.0, result3.y);
	EXPECT_DOUBLE_EQ(0.0, result3.z);

	Vector3D result4 = cyl.ClosestNormal({ 3, 0, 3 });
	EXPECT_DOUBLE_EQ(0.0, result4.x);
	EXPECT_DOUBLE_EQ(1.0, result4.y);
	EXPECT_DOUBLE_EQ(0.0, result4.z);
}

TEST(Cylinder3, Builder)
{
	Cylinder3 cyl2 = Cylinder3::GetBuilder()
		.WithCenter({ 1, 2, 3 })
		.WithRadius(4.0)
		.WithHeight(5.0)
		.Build();

	EXPECT_FALSE(cyl2.isNormalFlipped);
	EXPECT_DOUBLE_EQ(1.0, cyl2.center.x);
	EXPECT_DOUBLE_EQ(2.0, cyl2.center.y);
	EXPECT_DOUBLE_EQ(3.0, cyl2.center.z);
	EXPECT_DOUBLE_EQ(4.0, cyl2.radius);
	EXPECT_DOUBLE_EQ(5.0, cyl2.height);
	EXPECT_DOUBLE_EQ(-3.0, cyl2.BoundingBox().lowerCorner.x);
	EXPECT_DOUBLE_EQ(-0.5, cyl2.BoundingBox().lowerCorner.y);
	EXPECT_DOUBLE_EQ(-1.0, cyl2.BoundingBox().lowerCorner.z);
	EXPECT_DOUBLE_EQ(5.0, cyl2.BoundingBox().upperCorner.x);
	EXPECT_DOUBLE_EQ(4.5, cyl2.BoundingBox().upperCorner.y);
	EXPECT_DOUBLE_EQ(7.0, cyl2.BoundingBox().upperCorner.z);
}