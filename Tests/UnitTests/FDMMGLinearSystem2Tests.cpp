#include "pch.h"

#include <FDM/FDMMGLinearSystem2.h>

using namespace CubbyFlow;

TEST(FDMMGUtils2, ResizeArrayWithFinest)
{
	std::vector<Array2<double>> levels;
	FDMMGUtils2::ResizeArrayWithFinest({ 100, 200 }, 4, &levels);

	EXPECT_EQ(3u, levels.size());
	EXPECT_EQ(Size2(100, 200), levels[0].size());
	EXPECT_EQ(Size2(50, 100), levels[1].size());
	EXPECT_EQ(Size2(25, 50), levels[2].size());

	FDMMGUtils2::ResizeArrayWithFinest({ 32, 16 }, 6, &levels);
	EXPECT_EQ(5u, levels.size());
	EXPECT_EQ(Size2(32, 16), levels[0].size());
	EXPECT_EQ(Size2(16, 8), levels[1].size());
	EXPECT_EQ(Size2(8, 4), levels[2].size());
	EXPECT_EQ(Size2(4, 2), levels[3].size());
	EXPECT_EQ(Size2(2, 1), levels[4].size());

	FDMMGUtils2::ResizeArrayWithFinest({ 16, 16 }, 6, &levels);
	EXPECT_EQ(5u, levels.size());
}