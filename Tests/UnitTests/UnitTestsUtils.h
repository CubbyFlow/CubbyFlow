#ifndef UNIT_TESTS_UTILS_H
#define UNIT_TESTS_UTILS_H

#include <Core/Vector/Vector2.hpp>
#include <Core/Vector/Vector3.hpp>

#define EXPECT_VECTOR2_EQ(expected, actual) \
	EXPECT_DOUBLE_EQ(expected.x, actual.x); \
	EXPECT_DOUBLE_EQ(expected.y, actual.y); \

#define EXPECT_VECTOR2_NEAR(expected, actual, eps) \
	EXPECT_NEAR(expected.x, actual.x, eps); \
	EXPECT_NEAR(expected.y, actual.y, eps); \

#define EXPECT_VECTOR3_EQ(expected, actual) \
	EXPECT_DOUBLE_EQ(expected.x, actual.x); \
	EXPECT_DOUBLE_EQ(expected.y, actual.y); \
	EXPECT_DOUBLE_EQ(expected.z, actual.z); \

#define EXPECT_VECTOR3_NEAR(expected, actual, eps) \
	EXPECT_NEAR(expected.x, actual.x, eps); \
	EXPECT_NEAR(expected.y, actual.y, eps); \
	EXPECT_NEAR(expected.z, actual.z, eps); \

#define EXPECT_VECTOR4_EQ(expected, actual) \
	EXPECT_DOUBLE_EQ(expected.x, actual.x); \
	EXPECT_DOUBLE_EQ(expected.y, actual.y); \
	EXPECT_DOUBLE_EQ(expected.z, actual.z); \
	EXPECT_DOUBLE_EQ(expected.w, actual.w); \

#define EXPECT_VECTOR4_NEAR(expected, actual, eps) \
	EXPECT_NEAR(expected.x, actual.x, eps); \
	EXPECT_NEAR(expected.y, actual.y, eps); \
	EXPECT_NEAR(expected.z, actual.z, eps); \
	EXPECT_NEAR(expected.w, actual.w, eps); \

#define EXPECT_BOUNDING_BOX2_EQ(expected, actual) \
	EXPECT_VECTOR2_EQ(expected.lowerCorner, actual.lowerCorner); \
	EXPECT_VECTOR2_EQ(expected.upperCorner, actual.upperCorner); \

#define EXPECT_BOUNDING_BOX2_NEAR(expected, actual, eps) \
	EXPECT_VECTOR2_NEAR(expected.lowerCorner, actual.lowerCorner, eps); \
	EXPECT_VECTOR2_NEAR(expected.upperCorner, actual.upperCorner, eps); \

#define EXPECT_BOUNDING_BOX3_EQ(expected, actual) \
	EXPECT_VECTOR3_EQ(expected.lowerCorner, actual.lowerCorner); \
	EXPECT_VECTOR3_EQ(expected.upperCorner, actual.upperCorner); \

#define EXPECT_BOUNDING_BOX3_NEAR(expected, actual, eps) \
	EXPECT_VECTOR3_NEAR(expected.lowerCorner, actual.lowerCorner, eps); \
	EXPECT_VECTOR3_NEAR(expected.upperCorner, actual.upperCorner, eps); \

namespace CubbyFlow
{
	const Vector2D* GetSamplePoints2();

	size_t GetNumberOfSamplePoints2();

	const Vector3D* GetSamplePoints3();

	size_t GetNumberOfSamplePoints3();

	const Vector2D* GetSampleDirs2();

	size_t GetNumberOfSampleDirs2();

	const Vector3D* GetSampleDirs3();

	size_t GetNumberOfSampleDirs3();

	const char* GetCubeTriMesh3x3x3Obj();

	const char* GetSphereTriMesh5x5Obj();
}

#endif