#include "pch.h"

#include <Searcher/PointHashGridSearcher3.h>

using namespace CubbyFlow;

TEST(PointHashGridSearcher3, ForEachNearByPoint)
{
	Array1<Vector3D> points =
	{
		Vector3D(1, 1, 4),
		Vector3D(3, 4, 5),
		Vector3D(-1, 2, 4)
	};

	PointHashGridSearcher3 searcher(Size3(4, 4, 4), std::sqrt(10));
	searcher.Build(points.Accessor());

	searcher.ForEachNearbyPoint(
		Vector3D(0, 0, 0),
		std::sqrt(25.0),
		[&](size_t i, const Vector3D& pt)
	{
		EXPECT_TRUE(i == 0 || i == 2);
		if (i == 0)
		{
			EXPECT_EQ(points[0].x, pt.x);
			EXPECT_EQ(points[0].y, pt.y);
			EXPECT_EQ(points[0].z, pt.z);
		}
		else if (i == 2)
		{
			EXPECT_EQ(points[2].x, pt.x);
			EXPECT_EQ(points[2].y, pt.y);
			EXPECT_EQ(points[2].z, pt.z);
		}
	});
}

TEST(PointHashGridSearcher3, HasEachNearByPoint)
{
	Array1<Vector3D> points =
	{
		Vector3D(1, 1, 1),
		Vector3D(3, 444, 1),
		Vector3D(4, 15, 111)
	};

	PointHashGridSearcher3 searcher(Size3(4, 4, 4), std::sqrt(10));
	searcher.Build(points.Accessor());

	bool result;
	result = searcher.HasNearbyPoint(Vector3D(), std::sqrt(15.0));

	EXPECT_TRUE(result);
}

TEST(PointHashGridSearcher3, Build)
{
	Array1<Vector3D> points =
	{
		Vector3D(3, 4, 111),
		Vector3D(111, 5, 1),
		Vector3D(-311, 1123, 0)
	};

	PointHashGridSearcher3 searcher(Size3(4, 4, 4), std::sqrt(9));
	searcher.Build(points.Accessor());

	EXPECT_EQ(Point3I(1, 1, 37), searcher.GetBucketIndex(points[0]));
	EXPECT_EQ(Point3I(37, 1, 0), searcher.GetBucketIndex(points[1]));
	EXPECT_EQ(Point3I(-104, 374, 0), searcher.GetBucketIndex(points[2]));

	EXPECT_EQ(21, searcher.GetHashKeyFromBucketIndex(Point3I(1, 1, 37)));
	EXPECT_EQ(5, searcher.GetHashKeyFromBucketIndex(Point3I(37, 1, 0)));
	EXPECT_EQ(8, searcher.GetHashKeyFromBucketIndex(Point3I(-104, 374, 0)));
}