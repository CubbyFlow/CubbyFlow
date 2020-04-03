#include "pch.h"

#include <Core/Vector/Vector4.hpp>

using namespace CubbyFlow;

TEST(Vector4, Constructors) {
	Vector4F vec;
	EXPECT_FLOAT_EQ(0.f, vec.x);
	EXPECT_FLOAT_EQ(0.f, vec.y);
	EXPECT_FLOAT_EQ(0.f, vec.z);
	EXPECT_FLOAT_EQ(0.f, vec.w);

	Vector4F vec2(1.f, 2.f, 3.f, 4.f);
	EXPECT_FLOAT_EQ(1.f, vec2.x);
	EXPECT_FLOAT_EQ(2.f, vec2.y);
	EXPECT_FLOAT_EQ(3.f, vec2.z);
	EXPECT_FLOAT_EQ(4.f, vec2.w);

	Vector3F vec3(4.f, 7.f, 5.f);
	Vector4F vec4(vec3, 2.f);
	EXPECT_FLOAT_EQ(4.f, vec4.x);
	EXPECT_FLOAT_EQ(7.f, vec4.y);
	EXPECT_FLOAT_EQ(5.f, vec4.z);
	EXPECT_FLOAT_EQ(2.f, vec4.w);

	Vector4F vec5 = { 2.f, 4.f, 5.f, 1.f };
	EXPECT_FLOAT_EQ(2.f, vec5.x);
	EXPECT_FLOAT_EQ(4.f, vec5.y);
	EXPECT_FLOAT_EQ(5.f, vec5.z);
	EXPECT_FLOAT_EQ(1.f, vec5.w);

	Vector4F vec6(vec5);
	EXPECT_FLOAT_EQ(2.f, vec6.x);
	EXPECT_FLOAT_EQ(4.f, vec6.y);
	EXPECT_FLOAT_EQ(5.f, vec6.z);
	EXPECT_FLOAT_EQ(1.f, vec6.w);
}

TEST(Vector4, SetMethods)
{
	Vector4F vec;
	vec.Set(2.f, 4.f, 8.f, 5.f);
	EXPECT_FLOAT_EQ(2.f, vec.x);
	EXPECT_FLOAT_EQ(4.f, vec.y);
	EXPECT_FLOAT_EQ(8.f, vec.z);
	EXPECT_FLOAT_EQ(5.f, vec.w);

	vec.Set(Vector3F(1.f, 2.f, 3.f), 10.f);
	EXPECT_FLOAT_EQ(1.f, vec.x);
	EXPECT_FLOAT_EQ(2.f, vec.y);
	EXPECT_FLOAT_EQ(3.f, vec.z);
	EXPECT_FLOAT_EQ(10.f, vec.w);

	auto list = { 3.f, 1.f, 2.f, 4.f};
	vec.Set(list);
	EXPECT_FLOAT_EQ(3.f, vec.x);
	EXPECT_FLOAT_EQ(1.f, vec.y);
	EXPECT_FLOAT_EQ(2.f, vec.z);
	EXPECT_FLOAT_EQ(4.f, vec.w);

	vec.Set(Vector4F(1.f, 3.f, 4.f, 2.f));
	EXPECT_FLOAT_EQ(1.f, vec.x);
	EXPECT_FLOAT_EQ(3.f, vec.y);
	EXPECT_FLOAT_EQ(4.f, vec.z);
	EXPECT_FLOAT_EQ(2.f, vec.w);
}

TEST(Vector4, BasicSetterMethods)
{
	Vector4F vec(3.f, 4.f, 9.f, 2.f);
	vec.SetZero();
	EXPECT_FLOAT_EQ(0.f, vec.x);
	EXPECT_FLOAT_EQ(0.f, vec.y);
	EXPECT_FLOAT_EQ(0.f, vec.z);
	EXPECT_FLOAT_EQ(0.f, vec.w);

	vec.Set(4.f, 2.f, 8.f, 1.f);
	vec.Normalize();
	float len = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
	EXPECT_TRUE(fabsf(len - 1.f) < 1e-6);
}

TEST(Vector4, BinaryOperatorMethods)
{
	Vector4F vec(2.f, 5.f, 1.f, 6.f);
	vec = vec.Add(4.f);
	EXPECT_FLOAT_EQ(6.f, vec.x);
	EXPECT_FLOAT_EQ(9.f, vec.y);
	EXPECT_FLOAT_EQ(5.f, vec.z);
	EXPECT_FLOAT_EQ(10.f, vec.w);

	vec = vec.Add(Vector4F(-2.f, -4.f, -2.f, -1.f));
	EXPECT_FLOAT_EQ(4.f, vec.x);
	EXPECT_FLOAT_EQ(5.f, vec.y);
	EXPECT_FLOAT_EQ(3.f, vec.z);
	EXPECT_FLOAT_EQ(9.f, vec.w);

	vec = vec.Sub(3.f);
	EXPECT_FLOAT_EQ(1.f, vec.x);
	EXPECT_FLOAT_EQ(2.f, vec.y);
	EXPECT_FLOAT_EQ(0.f, vec.z);
	EXPECT_FLOAT_EQ(6.f, vec.w);

	vec = vec.Sub(Vector4F(-1.f, -2.f, -4.f, -3.f));
	EXPECT_FLOAT_EQ(2.f, vec.x);
	EXPECT_FLOAT_EQ(4.f, vec.y);
	EXPECT_FLOAT_EQ(4.f, vec.z);
	EXPECT_FLOAT_EQ(9.f, vec.w);

	vec = vec.Mul(2.f);
	EXPECT_FLOAT_EQ(4.f, vec.x);
	EXPECT_FLOAT_EQ(8.f, vec.y);
	EXPECT_FLOAT_EQ(8.f, vec.z);
	EXPECT_FLOAT_EQ(18.f, vec.w);

	vec = vec.Mul(Vector4F(3.f, 2.f, 1.f, -3.f));
	EXPECT_FLOAT_EQ(12.f, vec.x);
	EXPECT_FLOAT_EQ(16.f, vec.y);
	EXPECT_FLOAT_EQ(8.f, vec.z);
	EXPECT_FLOAT_EQ(-54.f, vec.w);

	vec = vec.Div(4.f);
	EXPECT_FLOAT_EQ(3.f, vec.x);
	EXPECT_FLOAT_EQ(4.f, vec.y);
	EXPECT_FLOAT_EQ(2.f, vec.z);
	EXPECT_FLOAT_EQ(-13.5f, vec.w);

	vec = vec.Div(Vector4F(2.f, 4.f, 4.f, 5.f));
	EXPECT_FLOAT_EQ(1.5f, vec.x);
	EXPECT_FLOAT_EQ(1.f, vec.y);
	EXPECT_FLOAT_EQ(0.5f, vec.z);
	EXPECT_FLOAT_EQ(-2.7f, vec.w);

	float d = vec.Dot(Vector4F(1.f, 2.f, -3.f, -4.f));
	EXPECT_FLOAT_EQ(12.8f, d);
}

TEST(Vector4, BinaryInverseOperatorMethods)
{
	Vector4F vec(5.f, 14.f, 13.f, 12.f);
	vec = vec.RSub(15.f);
	EXPECT_FLOAT_EQ(10.f, vec.x);
	EXPECT_FLOAT_EQ(1.f, vec.y);
	EXPECT_FLOAT_EQ(2.f, vec.z);
	EXPECT_FLOAT_EQ(3.f, vec.w);
	
	vec = vec.RSub(Vector4F(2.f, 4.f, 5.f, 1.f));
	EXPECT_FLOAT_EQ(-8.f, vec.x);
	EXPECT_FLOAT_EQ(3.f, vec.y);
	EXPECT_FLOAT_EQ(3.f, vec.z);
	EXPECT_FLOAT_EQ(-2.f, vec.w);

	vec = vec.RDiv(24.f);
	EXPECT_FLOAT_EQ(-3.f, vec.x);
	EXPECT_FLOAT_EQ(8.f, vec.y);
	EXPECT_FLOAT_EQ(8.f, vec.z);
	EXPECT_FLOAT_EQ(-12.f, vec.w);
}

TEST(Vector4, AugmentedOperatorMethods)
{
	Vector4F vec(3.f, 1.f, 9.f, 4.f);
	vec.IAdd(4.f);
	EXPECT_FLOAT_EQ(7.f, vec.x);
	EXPECT_FLOAT_EQ(5.f, vec.y);
	EXPECT_FLOAT_EQ(13.f, vec.z);
	EXPECT_FLOAT_EQ(8.f, vec.w);

	vec.ISub(8.f);
	EXPECT_FLOAT_EQ(-1.f, vec.x);
	EXPECT_FLOAT_EQ(-3.f, vec.y);
	EXPECT_FLOAT_EQ(5.f, vec.z);
	EXPECT_FLOAT_EQ(0.f, vec.w);

	vec.IAdd(Vector4F(-1.f, 2.f, 3.f, 4.f));
	EXPECT_FLOAT_EQ(-2.f, vec.x);
	EXPECT_FLOAT_EQ(-1.f, vec.y);
	EXPECT_FLOAT_EQ(8.f, vec.z);
	EXPECT_FLOAT_EQ(4.f, vec.w);

	vec.ISub(Vector4F(-3.f, -2.f, -1.f, 2.f));
	EXPECT_FLOAT_EQ(1.f, vec.x);
	EXPECT_FLOAT_EQ(1.f, vec.y);
	EXPECT_FLOAT_EQ(9.f, vec.z);
	EXPECT_FLOAT_EQ(2.f, vec.w);

	vec.IMul(2.f);
	EXPECT_FLOAT_EQ(2.f, vec.x);
	EXPECT_FLOAT_EQ(2.f, vec.y);
	EXPECT_FLOAT_EQ(18.f, vec.z);
	EXPECT_FLOAT_EQ(4.f, vec.w);

	vec.IMul(Vector4F(-2.f, -1.f, 2.f, 1.f));
	EXPECT_FLOAT_EQ(-4.f, vec.x);
	EXPECT_FLOAT_EQ(-2.f, vec.y);
	EXPECT_FLOAT_EQ(36.f, vec.z);
	EXPECT_FLOAT_EQ(4.f, vec.w);

	vec.IDiv(4.f);
	EXPECT_FLOAT_EQ(-1.f, vec.x);
	EXPECT_FLOAT_EQ(-0.5f, vec.y);
	EXPECT_FLOAT_EQ(9.f, vec.z);
	EXPECT_FLOAT_EQ(1.f, vec.w);

	vec.IDiv(Vector4F(2.f, 4.f, 3.f, 4.f));
	EXPECT_FLOAT_EQ(-0.5f, vec.x);
	EXPECT_FLOAT_EQ(-0.125f, vec.y);
	EXPECT_FLOAT_EQ(3.f, vec.z);
	EXPECT_FLOAT_EQ(0.25f, vec.w);
}

TEST(Vector4, AtMethods)
{
	Vector4F vec(8.f, 7.f, 9.f, 1.f);
	EXPECT_FLOAT_EQ(8.f, vec.At(0));
	EXPECT_FLOAT_EQ(7.f, vec.At(1));
	EXPECT_FLOAT_EQ(9.f, vec.At(2));
	EXPECT_FLOAT_EQ(1.f, vec.At(3));

	vec.At(0) = 1.f;
	vec.At(1) = 2.f;
	vec.At(2) = 3.f;
	vec.At(3) = 5.f;

	EXPECT_FLOAT_EQ(1.f, vec.At(0));
	EXPECT_FLOAT_EQ(2.f, vec.At(1));
	EXPECT_FLOAT_EQ(3.f, vec.At(2));
	EXPECT_FLOAT_EQ(5.f, vec.At(3));
}

TEST(Vector4, BasicGetterMethods)
{
	Vector4F vec(2.f, 4.f, 1.f, 5.f), vec2(-13.f, -2.f, 1.f, 4.f);

	float sum = vec.Sum();
	EXPECT_FLOAT_EQ(12.f, sum);

	float avg = vec.Avg();
	EXPECT_FLOAT_EQ(3.f, avg);

	float min = vec.Min();
	EXPECT_FLOAT_EQ(1.f, min);

	float max = vec.Max();
	EXPECT_FLOAT_EQ(5.f, max);

	float absMin = vec2.AbsMin();
	EXPECT_FLOAT_EQ(1.f, absMin);

	float absMax = vec2.AbsMax();
	EXPECT_FLOAT_EQ(-13.f, absMax);

	size_t dAxis = vec.DominantAxis();
	EXPECT_EQ((size_t)3, dAxis);

	size_t sAxis = vec.SubdominantAxis();
	EXPECT_EQ((size_t)2, sAxis);

	float eps = 1e-6f;
	vec2 = vec.Normalized();
	float lenSqr = vec2.x * vec2.x + vec2.y * vec2.y + vec2.z*vec2.z + vec2.w * vec2.w;
	EXPECT_TRUE(lenSqr - 1.f < eps);

	vec2.IMul(2.f);
	float len = vec2.Length();
	EXPECT_TRUE(len - 2.f < eps);

	lenSqr = vec2.LengthSquared();
	EXPECT_TRUE(lenSqr - 4.f < eps);
}

TEST(Vector4, BracketOperators)
{
	Vector4F vec(8.f, 9.f, 1.f, 2.f);
	EXPECT_FLOAT_EQ(8.f, vec[0]);
	EXPECT_FLOAT_EQ(9.f, vec[1]);
	EXPECT_FLOAT_EQ(1.f, vec[2]);
	EXPECT_FLOAT_EQ(2.f, vec[3]);

	vec[0] = 1.f;
	vec[1] = 2.f;
	vec[2] = 3.f;
	vec[3] = 4.f;

	EXPECT_FLOAT_EQ(1.f, vec[0]);
	EXPECT_FLOAT_EQ(2.f, vec[1]);
	EXPECT_FLOAT_EQ(3.f, vec[2]);
	EXPECT_FLOAT_EQ(4.f, vec[3]);
}

TEST(Vector4, AssignmentOperators)
{
	Vector4F vec(5.f, 1.f, 0.f, 2.f);
	Vector4F vec2;

	vec2 = vec;

EXPECT_FLOAT_EQ(5.f, vec2.x);
EXPECT_FLOAT_EQ(1.f, vec2.y);
EXPECT_FLOAT_EQ(0.f, vec2.z);
EXPECT_FLOAT_EQ(2.f, vec2.w);
}

TEST(Vector4, AugmentedOperators)
{
	Vector4F vec(3.f, 9.f, -2.f, 2.f);
	vec += 4.f;
	EXPECT_FLOAT_EQ(7.f, vec.x);
	EXPECT_FLOAT_EQ(13.f, vec.y);
	EXPECT_FLOAT_EQ(2.f, vec.z);
	EXPECT_FLOAT_EQ(6.f, vec.w);

	vec += Vector4F(-1.f, -2.f, 3.f, 1.f);
	EXPECT_FLOAT_EQ(6.f, vec.x);
	EXPECT_FLOAT_EQ(11.f, vec.y);
	EXPECT_FLOAT_EQ(5.f, vec.z);
	EXPECT_FLOAT_EQ(7.f, vec.w);

	vec -= 4.f;
	EXPECT_FLOAT_EQ(2.f, vec.x);
	EXPECT_FLOAT_EQ(7.f, vec.y);
	EXPECT_FLOAT_EQ(1.f, vec.z);
	EXPECT_FLOAT_EQ(3.f, vec.w);

	vec -= Vector4F(-1.f, -4.f, -3.f, -11.f);
	EXPECT_FLOAT_EQ(3.f, vec.x);
	EXPECT_FLOAT_EQ(11.f, vec.y);
	EXPECT_FLOAT_EQ(4.f, vec.z);
	EXPECT_FLOAT_EQ(14.f, vec.w);

	vec *= 2.f;
	EXPECT_FLOAT_EQ(6.f, vec.x);
	EXPECT_FLOAT_EQ(22.f, vec.y);
	EXPECT_FLOAT_EQ(8.f, vec.z);
	EXPECT_FLOAT_EQ(28.f, vec.w);

	vec *= Vector4F(4.f, 2.f, 3.f, 1.f);
	EXPECT_FLOAT_EQ(24.f, vec.x);
	EXPECT_FLOAT_EQ(44.f, vec.y);
	EXPECT_FLOAT_EQ(24.f, vec.z);
	EXPECT_FLOAT_EQ(28.f, vec.w);

	vec /= 4.f;
	EXPECT_FLOAT_EQ(6.f, vec.x);
	EXPECT_FLOAT_EQ(11.f, vec.y);
	EXPECT_FLOAT_EQ(6.f, vec.z);
	EXPECT_FLOAT_EQ(7.f, vec.w);

	vec /= Vector4F(3.f, 1.f, 2.f, 7.f);
	EXPECT_FLOAT_EQ(2.f, vec.x);
	EXPECT_FLOAT_EQ(11.f, vec.y);
	EXPECT_FLOAT_EQ(3.f, vec.z);
	EXPECT_FLOAT_EQ(1.f, vec.w);
}

TEST(Vector4, EqualOperatators)
{
	Vector4F vec, vec2(3.f, 7.f, 4.f, 1.f), vec3(3.f, 5.f, 4.f, 1.f), vec4(5.f, 1.f, 2.f, 3.f);
	vec = vec2;
	EXPECT_TRUE(vec == vec2);
	EXPECT_FALSE(vec == vec3);
	EXPECT_FALSE(vec != vec2);
	EXPECT_TRUE(vec != vec3);
	EXPECT_TRUE(vec != vec4);
}

TEST(Vector4, MinMaxFunctions)
{
	Vector4F vec(1.f, 4.f, -2.f, 3.f);
	Vector4F vec2(2.f, 5.f, 1.f, 2.f);
	Vector4F minVector = Min(vec, vec2);
	Vector4F maxVector = Max(vec, vec2);
	EXPECT_TRUE(minVector == Vector4F(1.f, 4.f, -2.f, 2.f));
	EXPECT_TRUE(maxVector == Vector4F(2.f, 5.f, 1.f, 3.f));
}

TEST(Vector4, ClampFunction)
{
	Vector4F vec(2.f, 3.f, 1.f, -1.f), low(3.f, 1.f, 2.f, -3.f), high(3.f, 2.f, 2.f, -1.f);
	Vector4F clampedVec = Clamp(vec, low, high);
	EXPECT_TRUE(clampedVec == Vector4F(3.f, 2.f, 2.f, -1.f));
}

TEST(Vector4, CeilFloorFunctions)
{
	Vector4F vec(2.2f, 4.7f, -0.3f, 1.1f);
	Vector4F ceilVec = Ceil(vec);
	EXPECT_TRUE(ceilVec == Vector4F(3.f, 5.f, 0.f, 2.f));

	Vector4F floorVec = Floor(vec);
	EXPECT_TRUE(floorVec == Vector4F(2.f, 4.f, -1.f, 1.f));
}

TEST(Vector4, BinaryOperators)
{
	Vector4F vec(2.f, 1.f, 4.f, 5.f);

	vec = vec + 4.f;
	EXPECT_FLOAT_EQ(6.f, vec.x);
	EXPECT_FLOAT_EQ(5.f, vec.y);
	EXPECT_FLOAT_EQ(8.f, vec.z);
	EXPECT_FLOAT_EQ(9.f, vec.w);

	vec = vec + Vector4F(-3.f, -2.f, -1.f, -5.f);
	EXPECT_FLOAT_EQ(3.f, vec.x);
	EXPECT_FLOAT_EQ(3.f, vec.y);
	EXPECT_FLOAT_EQ(7.f, vec.z);
	EXPECT_FLOAT_EQ(4.f, vec.w);

	vec = vec - 8.f;
	EXPECT_FLOAT_EQ(-5.f, vec.x);
	EXPECT_FLOAT_EQ(-5.f, vec.y);
	EXPECT_FLOAT_EQ(-1.f, vec.z);
	EXPECT_FLOAT_EQ(-4.f, vec.w);

	vec = vec - Vector4F(1.f, 2.f, 3.f, 4.f);
	EXPECT_FLOAT_EQ(-6.f, vec.x);
	EXPECT_FLOAT_EQ(-7.f, vec.y);
	EXPECT_FLOAT_EQ(-4.f, vec.z);
	EXPECT_FLOAT_EQ(-8.f, vec.w);

	vec = vec * 2.f;
	EXPECT_FLOAT_EQ(-12.f, vec.x);
	EXPECT_FLOAT_EQ(-14.f, vec.y);
	EXPECT_FLOAT_EQ(-8.f, vec.z);
	EXPECT_FLOAT_EQ(-16.f, vec.w);

	vec = vec * Vector4F(2.f, -1.f, 3.f, 4.f);
	EXPECT_FLOAT_EQ(-24.f, vec.x);
	EXPECT_FLOAT_EQ(14.f, vec.y);
	EXPECT_FLOAT_EQ(-24.f, vec.z);
	EXPECT_FLOAT_EQ(-64.f, vec.w);

	vec = vec / 2.f;
	EXPECT_FLOAT_EQ(-12.f, vec.x);
	EXPECT_FLOAT_EQ(7.f, vec.y);
	EXPECT_FLOAT_EQ(-12.f, vec.z);
	EXPECT_FLOAT_EQ(-32.f, vec.w);

	vec = vec / Vector4F(2.f, 7.f, 6.f, 4.f);
	EXPECT_FLOAT_EQ(-6.f, vec.x);
	EXPECT_FLOAT_EQ(1.f, vec.y);
	EXPECT_FLOAT_EQ(-2.f, vec.z);
	EXPECT_FLOAT_EQ(-8.f, vec.w);
}