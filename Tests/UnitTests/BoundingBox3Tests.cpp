#include "pch.h"
#include "UnitTestsUtils.h"

#include <BoundingBox/BoundingBox3.h>

using namespace CubbyFlow;

TEST(BoundingBox3, Constructors)
{
	{
		BoundingBox3D box;

		EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(), box.lowerCorner.x);
		EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(), box.lowerCorner.y);
		EXPECT_DOUBLE_EQ(std::numeric_limits<double>::max(), box.lowerCorner.z);

		EXPECT_DOUBLE_EQ(-std::numeric_limits<double>::max(), box.upperCorner.x);
		EXPECT_DOUBLE_EQ(-std::numeric_limits<double>::max(), box.upperCorner.y);
		EXPECT_DOUBLE_EQ(-std::numeric_limits<double>::max(), box.upperCorner.z);
	}

	{
		BoundingBox3D box(Vector3D(-2.0, 3.0, 5.0), Vector3D(4.0, -2.0, 1.0));

		EXPECT_DOUBLE_EQ(-2.0, box.lowerCorner.x);
		EXPECT_DOUBLE_EQ(-2.0, box.lowerCorner.y);
		EXPECT_DOUBLE_EQ(1.0, box.lowerCorner.z);

		EXPECT_DOUBLE_EQ(4.0, box.upperCorner.x);
		EXPECT_DOUBLE_EQ(3.0, box.upperCorner.y);
		EXPECT_DOUBLE_EQ(5.0, box.upperCorner.z);
	}

	{
		BoundingBox3D box(Vector3D(-2.0, 3.0, 5.0), Vector3D(4.0, -2.0, 1.0));
		BoundingBox3D box2(box);

		EXPECT_DOUBLE_EQ(-2.0, box2.lowerCorner.x);
		EXPECT_DOUBLE_EQ(-2.0, box2.lowerCorner.y);
		EXPECT_DOUBLE_EQ(1.0, box2.lowerCorner.z);

		EXPECT_DOUBLE_EQ(4.0, box2.upperCorner.x);
		EXPECT_DOUBLE_EQ(3.0, box2.upperCorner.y);
		EXPECT_DOUBLE_EQ(5.0, box2.upperCorner.z);
	}
}

TEST(BoundingBox3, BasicGetters)
{
	BoundingBox3D box(Vector3D(-2.0, 3.0, 5.0), Vector3D(4.0, -2.0, 1.0));

	EXPECT_DOUBLE_EQ(6.0, box.GetWidth());
	EXPECT_DOUBLE_EQ(5.0, box.GetHeight());
	EXPECT_DOUBLE_EQ(4.0, box.GetDepth());
	EXPECT_DOUBLE_EQ(6.0, box.Length(0));
	EXPECT_DOUBLE_EQ(5.0, box.Length(1));
	EXPECT_DOUBLE_EQ(4.0, box.Length(2));
}

TEST(BoundingBox3, Overlaps)
{
	// x-axis is not overlapping
	{
		BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
		BoundingBox3D box2(Vector3D(5.0, 1.0, 3.0), Vector3D(8.0, 2.0, 4.0));

		EXPECT_FALSE(box1.Overlaps(box2));
	}

	// y-axis is not overlapping
	{
		BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
		BoundingBox3D box2(Vector3D(3.0, 4.0, 3.0), Vector3D(8.0, 6.0, 4.0));

		EXPECT_FALSE(box1.Overlaps(box2));
	}

	// z-axis is not overlapping
	{
		BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
		BoundingBox3D box2(Vector3D(3.0, 1.0, 6.0), Vector3D(8.0, 2.0, 9.0));

		EXPECT_FALSE(box1.Overlaps(box2));
	}

	// overlapping
	{
		BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
		BoundingBox3D box2(Vector3D(3.0, 1.0, 3.0), Vector3D(8.0, 2.0, 7.0));

		EXPECT_TRUE(box1.Overlaps(box2));
	}
}

TEST(BoundingBox3, Contains)
{
	// Not containing (x-axis is out)
	{
		BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
		Vector3D point(-3.0, 0.0, 4.0);

		EXPECT_FALSE(box.Contains(point));
	}

	// Not containing (y-axis is out)
	{
		BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
		Vector3D point(2.0, 3.5, 4.0);

		EXPECT_FALSE(box.Contains(point));
	}

	// Not containing (z-axis is out)
	{
		BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
		Vector3D point(2.0, 0.0, 0.0);

		EXPECT_FALSE(box.Contains(point));
	}

	// Containing
	{
		BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
		Vector3D point(2.0, 0.0, 4.0);

		EXPECT_TRUE(box.Contains(point));
	}
}

TEST(BoundingBox3, Intersects)
{
	BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));

	Ray3D ray1(Vector3D(-3, 0, 2), Vector3D(2, 1, 1).Normalized());
	EXPECT_TRUE(box.Intersects(ray1));

	Ray3D ray2(Vector3D(3, -1, 3), Vector3D(-1, 2, -3).Normalized());
	EXPECT_TRUE(box.Intersects(ray2));

	Ray3D ray3(Vector3D(1, -5, 1), Vector3D(2, 1, 2).Normalized());
	EXPECT_FALSE(box.Intersects(ray3));
}

TEST(BoundingBox3, ClosestIntersection)
{
	BoundingBox3D box(Vector3D(-2.0, -2.0, -1.0), Vector3D(1.0, 0.0, 1.0));

	Ray3D ray1(Vector3D(-4, -3, 0), Vector3D(1, 1, 0).Normalized());
	BoundingBoxRayIntersection3D intersection1 = box.ClosestIntersection(ray1);
	EXPECT_TRUE(intersection1.isIntersecting);
	EXPECT_DOUBLE_EQ(Vector3D(2, 2, 0).Length(), intersection1.near);
	EXPECT_DOUBLE_EQ(Vector3D(3, 3, 0).Length(), intersection1.far);

	Ray3D ray2(Vector3D(0, -1, 0), Vector3D(-2, 1, 1).Normalized());
	BoundingBoxRayIntersection3D intersection2 = box.ClosestIntersection(ray2);
	EXPECT_TRUE(intersection2.isIntersecting);
	EXPECT_DOUBLE_EQ(Vector3D(2, 1, 1).Length(), intersection2.near);
}

TEST(BoundingBox3, MidPoint)
{
	BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
	Vector3D midPoint = box.MidPoint();

	EXPECT_DOUBLE_EQ(1.0, midPoint.x);
	EXPECT_DOUBLE_EQ(0.5, midPoint.y);
	EXPECT_DOUBLE_EQ(3.0, midPoint.z);
}

TEST(BoundingBox3, DiagonalLength)
{
	BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
	double diagLen = box.DiagonalLength();

	EXPECT_DOUBLE_EQ(std::sqrt(6.0*6.0 + 5.0*5.0 + 4.0*4.0), diagLen);
}

TEST(BoundingBox3, DiagonalLengthSquared)
{
	BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
	double diagLenSqr = box.DiagonalLengthSquared();

	EXPECT_DOUBLE_EQ(6.0*6.0 + 5.0*5.0 + 4.0*4.0, diagLenSqr);
}

TEST(BoundingBox3, Reset)
{
	BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
	box.Reset();

	static const double maxDouble = std::numeric_limits<double>::max();

	EXPECT_DOUBLE_EQ(maxDouble, box.lowerCorner.x);
	EXPECT_DOUBLE_EQ(maxDouble, box.lowerCorner.y);
	EXPECT_DOUBLE_EQ(maxDouble, box.lowerCorner.z);

	EXPECT_DOUBLE_EQ(-maxDouble, box.upperCorner.x);
	EXPECT_DOUBLE_EQ(-maxDouble, box.upperCorner.y);
	EXPECT_DOUBLE_EQ(-maxDouble, box.upperCorner.z);
}

TEST(BoundingBox3, Merge)
{
	// Merge with point
	{
		BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
		Vector3D point(5.0, 1.0, -1.0);

		box.Merge(point);

		EXPECT_DOUBLE_EQ(-2.0, box.lowerCorner.x);
		EXPECT_DOUBLE_EQ(-2.0, box.lowerCorner.y);
		EXPECT_DOUBLE_EQ(-1.0, box.lowerCorner.z);

		EXPECT_DOUBLE_EQ(5.0, box.upperCorner.x);
		EXPECT_DOUBLE_EQ(3.0, box.upperCorner.y);
		EXPECT_DOUBLE_EQ(5.0, box.upperCorner.z);
	}

	// Merge with other box
	{
		BoundingBox3D box1(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
		BoundingBox3D box2(Vector3D(3.0, 1.0, 3.0), Vector3D(8.0, 2.0, 7.0));

		box1.Merge(box2);

		EXPECT_DOUBLE_EQ(-2.0, box1.lowerCorner.x);
		EXPECT_DOUBLE_EQ(-2.0, box1.lowerCorner.y);
		EXPECT_DOUBLE_EQ(1.0, box1.lowerCorner.z);

		EXPECT_DOUBLE_EQ(8.0, box1.upperCorner.x);
		EXPECT_DOUBLE_EQ(3.0, box1.upperCorner.y);
		EXPECT_DOUBLE_EQ(7.0, box1.upperCorner.z);
	}
}

TEST(BoundingBox3, Expand)
{
	BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
	box.Expand(3.0);

	EXPECT_DOUBLE_EQ(-5.0, box.lowerCorner.x);
	EXPECT_DOUBLE_EQ(-5.0, box.lowerCorner.y);
	EXPECT_DOUBLE_EQ(-2.0, box.lowerCorner.z);

	EXPECT_DOUBLE_EQ(7.0, box.upperCorner.x);
	EXPECT_DOUBLE_EQ(6.0, box.upperCorner.y);
	EXPECT_DOUBLE_EQ(8.0, box.upperCorner.z);
}

TEST(BoundingBox3, Corner)
{
	BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
	
	EXPECT_VECTOR3_EQ(Vector3D(-2.0, -2.0, 1.0), box.Corner(0));
	EXPECT_VECTOR3_EQ(Vector3D(4.0, -2.0, 1.0), box.Corner(1));
	EXPECT_VECTOR3_EQ(Vector3D(-2.0, 3.0, 1.0), box.Corner(2));
	EXPECT_VECTOR3_EQ(Vector3D(4.0, 3.0, 1.0), box.Corner(3));
	EXPECT_VECTOR3_EQ(Vector3D(-2.0, -2.0, 5.0), box.Corner(4));
	EXPECT_VECTOR3_EQ(Vector3D(4.0, -2.0, 5.0), box.Corner(5));
	EXPECT_VECTOR3_EQ(Vector3D(-2.0, 3.0, 5.0), box.Corner(6));
	EXPECT_VECTOR3_EQ(Vector3D(4.0, 3.0, 5.0), box.Corner(7));
}

TEST(BoundingBox3, IsEmpty)
{
	BoundingBox3D box(Vector3D(-2.0, -2.0, 1.0), Vector3D(4.0, 3.0, 5.0));
	EXPECT_FALSE(box.IsEmpty());

	box.lowerCorner = Vector3D(5.0, 1.0, 3.0);
	EXPECT_TRUE(box.IsEmpty());

	box.lowerCorner = Vector3D(2.0, 4.0, 3.0);
	EXPECT_TRUE(box.IsEmpty());

	box.lowerCorner = Vector3D(2.0, 1.0, 6.0);
	EXPECT_TRUE(box.IsEmpty());

	box.lowerCorner = Vector3D(4.0, 1.0, 3.0);
	EXPECT_TRUE(box.IsEmpty());
}