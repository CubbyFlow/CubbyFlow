#include "pch.h"

#include <Point/Point.h>

using namespace CubbyFlow;

TEST(Point, Constructors)
{
	Point<double, 5> pt1;

	EXPECT_DOUBLE_EQ(0.0, pt1[0]);
	EXPECT_DOUBLE_EQ(0.0, pt1[1]);
	EXPECT_DOUBLE_EQ(0.0, pt1[2]);
	EXPECT_DOUBLE_EQ(0.0, pt1[3]);
	EXPECT_DOUBLE_EQ(0.0, pt1[4]);

	Point<double, 5> pt2({ 1.0, 2.0, 3.0, 4.0, 5.0 });

	EXPECT_DOUBLE_EQ(1.0, pt2[0]);
	EXPECT_DOUBLE_EQ(2.0, pt2[1]);
	EXPECT_DOUBLE_EQ(3.0, pt2[2]);
	EXPECT_DOUBLE_EQ(4.0, pt2[3]);
	EXPECT_DOUBLE_EQ(5.0, pt2[4]);

	Point<double, 5> pt3(pt2);

	EXPECT_DOUBLE_EQ(1.0, pt3[0]);
	EXPECT_DOUBLE_EQ(2.0, pt3[1]);
	EXPECT_DOUBLE_EQ(3.0, pt3[2]);
	EXPECT_DOUBLE_EQ(4.0, pt3[3]);
	EXPECT_DOUBLE_EQ(5.0, pt3[4]);
}

TEST(Point, SetMethods)
{
	Point<double, 5> pt1;

	pt1.Set({ 1.0, 2.0, 3.0, 4.0, 5.0 });

	EXPECT_DOUBLE_EQ(1.0, pt1[0]);
	EXPECT_DOUBLE_EQ(2.0, pt1[1]);
	EXPECT_DOUBLE_EQ(3.0, pt1[2]);
	EXPECT_DOUBLE_EQ(4.0, pt1[3]);
	EXPECT_DOUBLE_EQ(5.0, pt1[4]);

	Point<double, 5> pt2;

	pt2.Set(pt1);

	EXPECT_DOUBLE_EQ(1.0, pt2[0]);
	EXPECT_DOUBLE_EQ(2.0, pt2[1]);
	EXPECT_DOUBLE_EQ(3.0, pt2[2]);
	EXPECT_DOUBLE_EQ(4.0, pt2[3]);
	EXPECT_DOUBLE_EQ(5.0, pt2[4]);
}

TEST(Point, AssignmentOperators)
{
	Point<double, 5> pt1;

	pt1 = { 1.0, 2.0, 3.0, 4.0, 5.0 };

	EXPECT_DOUBLE_EQ(1.0, pt1[0]);
	EXPECT_DOUBLE_EQ(2.0, pt1[1]);
	EXPECT_DOUBLE_EQ(3.0, pt1[2]);
	EXPECT_DOUBLE_EQ(4.0, pt1[3]);
	EXPECT_DOUBLE_EQ(5.0, pt1[4]);

	Point<double, 5> pt2;

	pt2 = pt1;

	EXPECT_DOUBLE_EQ(1.0, pt2[0]);
	EXPECT_DOUBLE_EQ(2.0, pt2[1]);
	EXPECT_DOUBLE_EQ(3.0, pt2[2]);
	EXPECT_DOUBLE_EQ(4.0, pt2[3]);
	EXPECT_DOUBLE_EQ(5.0, pt2[4]);
}

TEST(Point, BracketOperators)
{
	Point<double, 5> pt1;

	pt1[0] = 1.0;
	pt1[1] = 2.0;
	pt1[2] = 3.0;
	pt1[3] = 4.0;
	pt1[4] = 5.0;

	EXPECT_DOUBLE_EQ(1.0, pt1[0]);
	EXPECT_DOUBLE_EQ(2.0, pt1[1]);
	EXPECT_DOUBLE_EQ(3.0, pt1[2]);
	EXPECT_DOUBLE_EQ(4.0, pt1[3]);
	EXPECT_DOUBLE_EQ(5.0, pt1[4]);
}