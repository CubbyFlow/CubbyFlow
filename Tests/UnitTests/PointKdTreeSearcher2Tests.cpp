#include "pch.h"

#include <Core/Searcher/PointKdTreeSearcher2.hpp>

using namespace CubbyFlow;

TEST(PointKdTreeSearcher2, ForEachNearbyPoint)
{
	Array1<Vector2D> points = { Vector2D(1, 3), Vector2D(2, 5), Vector2D(-1, 3) };

	PointKdTreeSearcher2 searcher;
	searcher.Build(points.Accessor());

	searcher.ForEachNearbyPoint(Vector2D(0, 0), std::sqrt(10.0),
		[&points](size_t i, const Vector2D& pt)
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

TEST(PointKdTreeSearcher2, ForEachNearbyPointEmpty)
{
	Array1<Vector2D> points;

	PointKdTreeSearcher2 searcher;
	searcher.Build(points.Accessor());

	searcher.ForEachNearbyPoint(Vector2D(0, 0), std::sqrt(10.0),
		[](size_t, const Vector2D&)
	{
		// Do nothing
	});
}

TEST(PointKdTreeSearcher2, Serialization)
{
	const Array1<Vector2D> points = { Vector2D(1, 3), Vector2D(2, 5), Vector2D(-1, 3) };

	PointKdTreeSearcher2 searcher;
	searcher.Build(points);

	std::vector<uint8_t> buffer;
	searcher.Serialize(&buffer);

	EXPECT_GT(buffer.size(), 0U);

	PointKdTreeSearcher2 searcher2;
	searcher2.Deserialize(buffer);

	std::vector<uint8_t> buffer2;
	searcher2.Serialize(&buffer2);

	ASSERT_EQ(buffer.size(), buffer2.size());

	for (size_t i = 0; i < buffer.size(); ++i)
	{
		EXPECT_EQ(buffer[i], buffer2[i]);
	}
}