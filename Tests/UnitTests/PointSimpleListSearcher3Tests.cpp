#include "pch.h"

#include <Core/Searcher/PointSimpleListSearcher3.hpp>

using namespace CubbyFlow;

TEST(PointSimpleListSearcher3, ForEachNearByPoint)
{
	Array1<Vector3D> points =
	{
		Vector3D(1, 1, 3),
		Vector3D(3, 4, 1),
		Vector3D(-1, 2, 1)
	};

	PointSimpleListSearcher3 searcher;
	searcher.Build(points.ConstAccessor());

	searcher.ForEachNearbyPoint(
		Vector3D(0, 0, 1), std::sqrt(15.0),
		[&](size_t i, const Vector3D& pt)
	{
		EXPECT_TRUE(i == 0 || i == 2);
		if (i == 0)
		{
			EXPECT_EQ(points[0], pt);
		}
		else if (i == 2)
		{
			EXPECT_EQ(points[2], pt);
		}
	});
}