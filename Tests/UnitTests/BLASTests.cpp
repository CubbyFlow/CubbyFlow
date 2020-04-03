#include "pch.h"

#include <Core/Math/BLAS.hpp>
#include <Core/Matrix/Matrix3x3.hpp>
#include <Core/Vector/Vector3.hpp>

using namespace CubbyFlow;

TEST(BLAS, Set)
{
	Vector3D vec;

	BLAS<double, Vector3D, Matrix3x3D>::Set(3.14, &vec);

	EXPECT_DOUBLE_EQ(3.14, vec.x);
	EXPECT_DOUBLE_EQ(3.14, vec.y);
	EXPECT_DOUBLE_EQ(3.14, vec.z);

	Vector3D vec2(5.1, 3.7, 8.2);
	BLAS<double, Vector3D, Matrix3x3D>::Set(vec2, &vec);

	EXPECT_DOUBLE_EQ(5.1, vec.x);
	EXPECT_DOUBLE_EQ(3.7, vec.y);
	EXPECT_DOUBLE_EQ(8.2, vec.z);

	Matrix3x3D mat;

	BLAS<double, Vector3D, Matrix3x3D>::Set(0.414, &mat);

	for (int i = 0; i < 9; ++i)
	{
		double elem = mat[i];
		EXPECT_DOUBLE_EQ(0.414, elem);
	}

	Matrix3x3D mat2(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
	BLAS<double, Vector3D, Matrix3x3D>::Set(mat2, &mat);
	for (int i = 0; i < 9; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
	}
}

TEST(BLAS, Dot)
{
	Vector3D vec(1.0, 2.0, 3.0), vec2(4.0, 5.0, 6.0);
	double result = BLAS<double, Vector3D, Matrix3x3D>::Dot(vec, vec2);
	EXPECT_DOUBLE_EQ(32.0, result);
}

TEST(BLAS, AXPlusY)
{
	Vector3D result;
	BLAS<double, Vector3D, Matrix3x3D>::AXPlusY(2.5, Vector3D(1, 2, 3), Vector3D(4, 5, 6), &result);

	EXPECT_DOUBLE_EQ(6.5, result.x);
	EXPECT_DOUBLE_EQ(10.0, result.y);
	EXPECT_DOUBLE_EQ(13.5, result.z);
}

TEST(BLAS, MVM)
{
	Matrix3x3D mat(1, 2, 3, 4, 5, 6, 7, 8, 9);

	Vector3D result;
	BLAS<double, Vector3D, Matrix3x3D>::MVM(mat, Vector3D(1, 2, 3), &result);

	EXPECT_DOUBLE_EQ(14.0, result.x);
	EXPECT_DOUBLE_EQ(32.0, result.y);
	EXPECT_DOUBLE_EQ(50.0, result.z);
}

TEST(BLAS, Residual)
{
	Matrix3x3D mat(1, 2, 3, 4, 5, 6, 7, 8, 9);

	Vector3D result;
	BLAS<double, Vector3D, Matrix3x3D>::Residual(mat, Vector3D(1, 2, 3), Vector3D(4, 5, 6), &result);

	EXPECT_DOUBLE_EQ(-10.0, result.x);
	EXPECT_DOUBLE_EQ(-27.0, result.y);
	EXPECT_DOUBLE_EQ(-44.0, result.z);
}

TEST(BLAS, L2Norm)
{
	Vector3D vec(-1.0, 2.0, -3.0);
	double result = BLAS<double, Vector3D, Matrix3x3D>::L2Norm(vec);
	EXPECT_DOUBLE_EQ(std::sqrt(14.0), result);
}

TEST(BLAS, LInfNorm)
{
	Vector3D vec(-1.0, 2.0, -3.0);
	double result = BLAS<double, Vector3D, Matrix3x3D>::LInfNorm(vec);
	EXPECT_DOUBLE_EQ(3.0, result);
}