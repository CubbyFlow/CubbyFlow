#include "pch.h"

#include <Core/Math/PDE.hpp>

using namespace CubbyFlow;

TEST(PDE, Upwind1)
{
	double values[3] = { 0.0, 1.0, -1.0 };
	auto result = Upwind1(values, 0.5);

	EXPECT_DOUBLE_EQ(2.0, result[0]);
	EXPECT_DOUBLE_EQ(-4.0, result[1]);

	double d0 = Upwind1(values, 2.0, true);
	double d1 = Upwind1(values, 2.0, false);

	EXPECT_DOUBLE_EQ(0.5, d0);
	EXPECT_DOUBLE_EQ(-1.0, d1);
}

TEST(PDE, CD2)
{
	double values[3] = { 0.0, 1.0, -1.0 };
	double result = CD2(values, 0.5);

	EXPECT_DOUBLE_EQ(-1.0, result);
}

TEST(PDE, ENO3)
{
	double values0[7] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
	auto result0 = ENO3(values0, 0.5);

	// Sanity check for linear case
	EXPECT_DOUBLE_EQ(2.0, result0[0]);
	EXPECT_DOUBLE_EQ(2.0, result0[1]);

	double d0 = ENO3(values0, 2.0, true);
	double d1 = ENO3(values0, 2.0, false);

	EXPECT_DOUBLE_EQ(0.5, d0);
	EXPECT_DOUBLE_EQ(0.5, d1);

	// Unit-step function
	double values1[7] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };
	auto result1 = ENO3(values1, 0.5);

	// Check monotonicity
	EXPECT_LE(0.0, result1[0]);
	EXPECT_DOUBLE_EQ(0.0, result1[1]);
}

TEST(PDE, WENO5)
{
	double values0[7] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
	auto result0 = WENO5(values0, 0.5);

	// Sanity check for linear case
	EXPECT_DOUBLE_EQ(2.0, result0[0]);
	EXPECT_DOUBLE_EQ(2.0, result0[1]);

	double d0 = WENO5(values0, 2.0, true);
	double d1 = WENO5(values0, 2.0, false);

	EXPECT_DOUBLE_EQ(0.5, d0);
	EXPECT_DOUBLE_EQ(0.5, d1);

	// Unit-step function
	double values1[7] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };
	auto result1 = WENO5(values1, 0.5);

	// Check monotonicity
	EXPECT_LE(0.0, result1[0]);
	EXPECT_LE(std::fabs(result1[1]), 1e-10);
}