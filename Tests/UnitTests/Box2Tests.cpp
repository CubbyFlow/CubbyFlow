#include "pch.h"

#include <Geometry/Box2.h>

using namespace CubbyFlow;

TEST(Box2, Constructors)
{
	{
		Box2 box;

		EXPECT_EQ(Vector2D(), box.bound.lowerCorner);
		EXPECT_EQ(Vector2D(1, 1), box.bound.upperCorner);
	}

	{
		Box2 box(Vector2D(-1, 2), Vector2D(5, 3));

		EXPECT_EQ(Vector2D(-1, 2), box.bound.lowerCorner);
		EXPECT_EQ(Vector2D(5, 3), box.bound.upperCorner);
	}

	{
		Box2 box(BoundingBox2D(Vector2D(-1, 2), Vector2D(5, 3)));

		box.isNormalFlipped = true;
		EXPECT_TRUE(box.isNormalFlipped);
		EXPECT_EQ(Vector2D(-1, 2), box.bound.lowerCorner);
		EXPECT_EQ(Vector2D(5, 3), box.bound.upperCorner);
	}
}

TEST(Box2, ClosestPoint)
{
	Box2 box(Vector2D(-1, 2), Vector2D(5, 3));

	Vector2D result0 = box.ClosestPoint(Vector2D(-2, 4));
	EXPECT_EQ(Vector2D(-1, 3), result0);

	Vector2D result1 = box.ClosestPoint(Vector2D(1, 5));
	EXPECT_EQ(Vector2D(1, 3), result1);

	Vector2D result2 = box.ClosestPoint(Vector2D(9, 5));
	EXPECT_EQ(Vector2D(5, 3), result2);

	Vector2D result3 = box.ClosestPoint(Vector2D(-2, 2.4));
	EXPECT_EQ(Vector2D(-1, 2.4), result3);

	Vector2D result4 = box.ClosestPoint(Vector2D(1, 2.6));
	EXPECT_EQ(Vector2D(1, 3), result4);

	Vector2D result5 = box.ClosestPoint(Vector2D(9, 2.2));
	EXPECT_EQ(Vector2D(5, 2.2), result5);

	Vector2D result6 = box.ClosestPoint(Vector2D(-2, 1));
	EXPECT_EQ(Vector2D(-1, 2), result6);

	Vector2D result7 = box.ClosestPoint(Vector2D(1, 0));
	EXPECT_EQ(Vector2D(1, 2), result7);

	Vector2D result8 = box.ClosestPoint(Vector2D(9, -1));
	EXPECT_EQ(Vector2D(5, 2), result8);
}

TEST(Box2, ClosestDistance)
{
	Box2 box(Vector2D(-1, 2), Vector2D(5, 3));

	double result0 = box.ClosestDistance(Vector2D(-2, 4));
	EXPECT_DOUBLE_EQ(Vector2D(-2, 4).DistanceTo(Vector2D(-1, 3)), result0);

	double result1 = box.ClosestDistance(Vector2D(1, 5));
	EXPECT_DOUBLE_EQ(Vector2D(1, 5).DistanceTo(Vector2D(1, 3)), result1);

	double result2 = box.ClosestDistance(Vector2D(9, 5));
	EXPECT_DOUBLE_EQ(Vector2D(9, 5).DistanceTo(Vector2D(5, 3)), result2);

	double result3 = box.ClosestDistance(Vector2D(-2, 2.4));
	EXPECT_DOUBLE_EQ(Vector2D(-2, 2.4).DistanceTo(Vector2D(-1, 2.4)), result3);

	double result4 = box.ClosestDistance(Vector2D(1, 2.6));
	EXPECT_DOUBLE_EQ(Vector2D(1, 2.6).DistanceTo(Vector2D(1, 3)), result4);

	double result5 = box.ClosestDistance(Vector2D(9, 2.2));
	EXPECT_DOUBLE_EQ(Vector2D(9, 2.2).DistanceTo(Vector2D(5, 2.2)), result5);

	double result6 = box.ClosestDistance(Vector2D(-2, 1));
	EXPECT_DOUBLE_EQ(Vector2D(-2, 1).DistanceTo(Vector2D(-1, 2)), result6);

	double result7 = box.ClosestDistance(Vector2D(1, 0));
	EXPECT_DOUBLE_EQ(Vector2D(1, 0).DistanceTo(Vector2D(1, 2)), result7);

	double result8 = box.ClosestDistance(Vector2D(9, -1));
	EXPECT_DOUBLE_EQ(Vector2D(9, -1).DistanceTo(Vector2D(5, 2)), result8);
}

TEST(Box2, Intersects)
{
	Box2 box(Vector2D(-1, 2), Vector2D(5, 3));

	bool result0 = box.Intersects(Ray2D(Vector2D(1, 4), Vector2D(-1, -1).Normalized()));
	EXPECT_TRUE(result0);

	bool result1 = box.Intersects(Ray2D(Vector2D(1, 2.5), Vector2D(-1, -1).Normalized()));
	EXPECT_TRUE(result1);

	bool result2 = box.Intersects(Ray2D(Vector2D(1, 1), Vector2D(-1, -1).Normalized()));
	EXPECT_FALSE(result2);
}

TEST(Box2, ClosestIntersection)
{
	Box2 box(Vector2D(-1, 2), Vector2D(5, 3));

	SurfaceRayIntersection2 result0 = box.ClosestIntersection(Ray2D(Vector2D(1, 4), Vector2D(-1, -1).Normalized()));
	EXPECT_TRUE(result0.isIntersecting);
	EXPECT_DOUBLE_EQ(std::sqrt(2), result0.distance);
	EXPECT_EQ(Vector2D(0, 3), result0.point);

	SurfaceRayIntersection2 result1 = box.ClosestIntersection(Ray2D(Vector2D(1, 2.5), Vector2D(-1, -1).Normalized()));
	EXPECT_TRUE(result1.isIntersecting);
	EXPECT_DOUBLE_EQ(std::sqrt(0.5), result1.distance);
	EXPECT_EQ(Vector2D(0.5, 2), result1.point);

	SurfaceRayIntersection2 result2 = box.ClosestIntersection(Ray2D(Vector2D(1, 1), Vector2D(-1, -1).Normalized()));
	EXPECT_FALSE(result2.isIntersecting);
}

TEST(Box2, BoundingBox)
{
	Box2 box(Vector2D(-1, 2), Vector2D(5, 3));
	BoundingBox2D boundingBox = box.BoundingBox();

	EXPECT_EQ(Vector2D(-1, 2), boundingBox.lowerCorner);
	EXPECT_EQ(Vector2D(5, 3), boundingBox.upperCorner);
}

TEST(Box2, ClosestNormal)
{
	Box2 box(Vector2D(-1, 2), Vector2D(5, 3));
	box.isNormalFlipped = true;

	Vector2D result0 = box.ClosestNormal(Vector2D(-2, 2));
	EXPECT_EQ(Vector2D(1, -0), result0);

	Vector2D result1 = box.ClosestNormal(Vector2D(3, 5));
	EXPECT_EQ(Vector2D(0, -1), result1);

	Vector2D result2 = box.ClosestNormal(Vector2D(9, 3));
	EXPECT_EQ(Vector2D(-1, 0), result2);

	Vector2D result3 = box.ClosestNormal(Vector2D(4, 1));
	EXPECT_EQ(Vector2D(0, 1), result3);
}

TEST(Box2, Builder)
{
	Box2 box = Box2::GetBuilder()
		.WithLowerCorner({ -3.0, -2.0 })
		.WithUpperCorner({ 5.0, 4.0 })
		.Build();

	EXPECT_EQ(Vector2D(-3.0, -2.0), box.bound.lowerCorner);
	EXPECT_EQ(Vector2D(5.0, 4.0), box.bound.upperCorner);
}