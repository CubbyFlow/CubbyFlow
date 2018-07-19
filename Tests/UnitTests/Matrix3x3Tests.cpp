#include "pch.h"

#include <Matrix/Matrix3x3.h>

using namespace CubbyFlow;

TEST(Matrix3x3, Constructors)
{
	Matrix3x3D mat;
	EXPECT_TRUE(mat == Matrix3x3D(1.0, 0.0, 0.0,
								  0.0, 1.0, 0.0,
								  0.0, 0.0, 1.0));

	Matrix3x3D mat2(3.1);
	for (int i = 0; i < 9; ++i)
	{
		EXPECT_DOUBLE_EQ(3.1, mat2[i]);
	}

	Matrix3x3D mat3(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
	for (int i = 0; i < 9; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat3[i]);
	}

	Matrix3x3D mat4({ { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } });
	for (int i = 0; i < 9; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat4[i]);
	}

	Matrix3x3D mat5(mat4);
	for (int i = 0; i < 9; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat5[i]);
	}

	double arr[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
	Matrix3x3D mat6(arr);
	for (int i = 0; i < 9; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat6[i]);
	}
}

TEST(Matrix3x3, SetMethods)
{
	Matrix3x3D mat;

	mat.Set(3.1);
	for (int i = 0; i < 9; ++i)
	{
		EXPECT_DOUBLE_EQ(3.1, mat[i]);
	}

	mat.Set(0.0);
	mat.Set(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
	for (int i = 0; i < 9; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
	}

	mat.Set(0.0);
	mat.Set({ { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 }, { 7.0, 8.0, 9.0 } });
	for (int i = 0; i < 9; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
	}

	mat.Set(0.0);
	mat.Set(Matrix3x3D(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0));
	for (int i = 0; i < 9; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
	}

	mat.Set(0.0);
	double arr[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
	mat.Set(arr);
	for (int i = 0; i < 9; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
	}

	mat.Set(0.0);
	mat.SetDiagonal(3.1);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (i == j)
			{
				EXPECT_DOUBLE_EQ(3.1, mat(i, j));
			}
			else
			{
				EXPECT_DOUBLE_EQ(0.0, mat(i, j));
			}
		}
	}

	mat.Set(0.0);
	mat.SetOffDiagonal(4.2);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (i != j)
			{
				EXPECT_DOUBLE_EQ(4.2, mat(i, j));
			}
			else
			{
				EXPECT_DOUBLE_EQ(0.0, mat(i, j));
			}
		}
	}

	mat.Set(0.0);
	mat.SetRow(0, Vector3D(1.0, 2.0, 3.0));
	mat.SetRow(1, Vector3D(4.0, 5.0, 6.0));
	mat.SetRow(2, Vector3D(7.0, 8.0, 9.0));
	for (int i = 0; i < 9; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
	}

	mat.Set(0.0);
	mat.SetColumn(0, Vector3D(1.0, 4.0, 7.0));
	mat.SetColumn(1, Vector3D(2.0, 5.0, 8.0));
	mat.SetColumn(2, Vector3D(3.0, 6.0, 9.0));
	for (int i = 0; i < 9; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
	}
}

TEST(Matrix3x3, BasicGetters)
{
	Matrix3x3D mat(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0),
		mat2(1.01, 2.01, 2.99, 4.0, 4.99, 6.001, 7.0003, 8.0, 8.99);

	EXPECT_TRUE(mat.IsSimilar(mat2, 0.02));
	EXPECT_FALSE(mat.IsSimilar(mat2, 0.001));

	EXPECT_TRUE(mat.IsSquare());

	EXPECT_EQ(3u, mat.Rows());
	EXPECT_EQ(3u, mat.Cols());
}

TEST(Matrix3x3, BinaryOperators)
{
	Matrix3x3D mat(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0), mat2;
	Vector3D vec;

	mat2 = mat.Add(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(11.0, -6.0, 9.0, -4.0, 7.0, -2.0, 5.0, 0.0, 3.0)));

	mat2 = mat.Add(Matrix3x3D(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0));
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(10.0, -6.0, 10.0, -2.0, 10.0, 2.0, 10.0, 6.0, 10.0)));

	mat2 = mat.Sub(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(7.0, -10.0, 5.0, -8.0, 3.0, -6.0, 1.0, -4.0, -1.0)));

	mat2 = mat.Sub(Matrix3x3D(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0));
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(8.0, -10.0, 4.0, -10.0, 0.0, -10.0, -4.0, -10.0, -8.0)));

	mat2 = mat.Mul(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(18.0, -16.0, 14.0, -12.0, 10.0, -8.0, 6.0, -4.0, 2.0)));

	vec = mat.Mul(Vector3D(1, 2, 3));
	EXPECT_TRUE(vec.IsSimilar(Vector3D(14.0, -8.0, 2.0)));

	mat2 = mat.Mul(Matrix3x3D(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0));
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(26.0, 34.0, 42.0, -14.0, -19.0, -24.0, 2.0, 4.0, 6.0)))
		<< mat2(0, 0) << ' ' << mat2(0, 1) << ' ' << mat2(0, 2) << "\n"
		<< mat2(1, 0) << ' ' << mat2(1, 1) << ' ' << mat2(1, 2) << "\n"
		<< mat2(2, 0) << ' ' << mat2(2, 1) << ' ' << mat2(2, 2) << "\n";

	mat2 = mat.Div(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(4.5, -4.0, 3.5, -3.0, 2.5, -2.0, 1.5, -1.0, 0.5)));

	mat2 = mat.RAdd(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(11.0, -6.0, 9.0, -4.0, 7.0, -2.0, 5.0, 0.0, 3.0)));

	mat2 = mat.RAdd(Matrix3x3D(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0));
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(10.0, -6.0, 10.0, -2.0, 10.0, 2.0, 10.0, 6.0, 10.0)));

	mat2 = mat.RSub(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(-7.0, 10.0, -5.0, 8.0, -3.0, 6.0, -1.0, 4.0, 1.0)));

	mat2 = mat.RSub(Matrix3x3D(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0));
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(-8.0, 10.0, -4.0, 10.0, 0.0, 10.0, 4.0, 10.0, 8.0)));

	mat2 = mat.RMul(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(18.0, -16.0, 14.0, -12.0, 10.0, -8.0, 6.0, -4.0, 2.0)));

	mat2 = mat.RMul(Matrix3x3D(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0));
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(6.0, -4.0, 2.0, 24.0, -19.0, 14.0, 42.0, -34.0, 26.0)))
		<< mat2(0, 0) << ' ' << mat2(0, 1) << ' ' << mat2(0, 2) << "\n"
		<< mat2(1, 0) << ' ' << mat2(1, 1) << ' ' << mat2(1, 2) << "\n"
		<< mat2(2, 0) << ' ' << mat2(2, 1) << ' ' << mat2(2, 2) << "\n";

	mat2 = mat.RDiv(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(2.0 / 9.0, -0.25, 2.0 / 7.0, -1.0 / 3.0, 0.4, -0.5, 2.0 / 3.0, -1.0, 2.0)));
}

TEST(Matrix3x3, AugmentedOperators) {
	Matrix3x3D mat(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);

	mat.IAdd(2.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(11.0, -6.0, 9.0, -4.0, 7.0, -2.0, 5.0, 0.0, 3.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	mat.IAdd(Matrix3x3D(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0));
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(10.0, -6.0, 10.0, -2.0, 10.0, 2.0, 10.0, 6.0, 10.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	mat.ISub(2.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(7.0, -10.0, 5.0, -8.0, 3.0, -6.0, 1.0, -4.0, -1.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	mat.ISub(Matrix3x3D(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0));
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(8.0, -10.0, 4.0, -10.0, 0.0, -10.0, -4.0, -10.0, -8.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	mat.IMul(2.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(18.0, -16.0, 14.0, -12.0, 10.0, -8.0, 6.0, -4.0, 2.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	mat.IMul(Matrix3x3D(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0));
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(26.0, 34.0, 42.0, -14.0, -19.0, -24.0, 2.0, 4.0, 6.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	mat.IDiv(2.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(4.5, -4.0, 3.5, -3.0, 2.5, -2.0, 1.5, -1.0, 0.5)));
}

TEST(Matrix3x3, Modifiers)
{
	Matrix3x3D mat(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);

	mat.Transpose();
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(9.0, -6.0, 3.0, -8.0, 5.0, -2.0, 7.0, -4.0, 1.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 2.0);
	mat.Invert();
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(-2.0 / 3.0, -2.0 / 3.0, 1.0, 0.0, 1.0, 2.0, 1.0, 2.0, 1.0)));
}

TEST(Matrix3x3, ComplexGetters) {
	Matrix3x3D mat(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0), mat2;

	EXPECT_DOUBLE_EQ(5.0, mat.Sum());

	EXPECT_DOUBLE_EQ(5.0 / 9.0, mat.Avg());

	EXPECT_DOUBLE_EQ(-8.0, mat.Min());

	EXPECT_DOUBLE_EQ(9.0, mat.Max());

	EXPECT_DOUBLE_EQ(1.0, mat.AbsMin());

	EXPECT_DOUBLE_EQ(9.0, mat.AbsMax());

	EXPECT_DOUBLE_EQ(15.0, mat.Trace());

	EXPECT_DOUBLE_EQ(0.0, mat.Determinant());

	mat2 = mat.Diagonal();
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(9.0, 0.0, 0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 1.0)));

	mat2 = mat.OffDiagonal();
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(0.0, -8.0, 7.0, -6.0, 0.0, -4.0, 3.0, -2.0, 0.0)));

	mat2 = mat.StrictLowerTriangle();
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(0.0, 0.0, 0.0,
										 -6.0, 0.0, 0.0,
										  3.0, -2.0, 0.0)));

	mat2 = mat.StrictUpperTriangle();
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(0.0, -8.0, 7.0,
										  0.0, 0.0, -4.0,
										  0.0, 0.0, 0.0)));

	mat2 = mat.LowerTriangle();
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(9.0, 0.0, 0.0,
										  -6.0, 5.0, 0.0,
										  3.0, -2.0, 1.0)));

	mat2 = mat.UpperTriangle();
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(9.0, -8.0, 7.0,
										  0.0, 5.0, -4.0,
										  0.0, 0.0, 1.0)));

	mat2 = mat.Transposed();
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(9.0, -6.0, 3.0, -8.0, 5.0, -2.0, 7.0, -4.0, 1.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 2.0);
	mat2 = mat.Inverse();
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(-2.0 / 3.0, -2.0 / 3.0, 1.0, 0.0, 1.0, 2.0, 1.0, 2.0, 1.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	Matrix3x3F mat3 = mat.CastTo<float>();
	EXPECT_TRUE(mat3.IsSimilar(Matrix3x3F(9.f, -8.f, 7.f, -6.f, 5.f, -4.f, 3.f, -2.f, 1.f)));
}

TEST(Matrix3x3, SetterOperatorOverloadings)
{
	Matrix3x3D mat(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0), mat2;

	mat2 = mat;
	EXPECT_TRUE(mat2.IsSimilar(Matrix3x3D(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0)));

	mat += 2.0;
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(11.0, -6.0, 9.0, -4.0, 7.0, -2.0, 5.0, 0.0, 3.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	mat += Matrix3x3D(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(10.0, -6.0, 10.0, -2.0, 10.0, 2.0, 10.0, 6.0, 10.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	mat -= 2.0;
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(7.0, -10.0, 5.0, -8.0, 3.0, -6.0, 1.0, -4.0, -1.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	mat -= Matrix3x3D(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(8.0, -10.0, 4.0, -10.0, 0.0, -10.0, -4.0, -10.0, -8.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	mat *= 2.0;
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(18.0, -16.0, 14.0, -12.0, 10.0, -8.0, 6.0, -4.0, 2.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	mat *= Matrix3x3D(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(26.0, 34.0, 42.0, -14.0, -19.0, -24.0, 2.0, 4.0, 6.0)));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	mat /= 2.0;
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(4.5, -4.0, 3.5, -3.0, 2.5, -2.0, 1.5, -1.0, 0.5)));
}

TEST(Matrix3x3, GetterOperatorOverloadings)
{
	Matrix3x3D mat(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);

	EXPECT_DOUBLE_EQ(9.0, mat[0]);
	EXPECT_DOUBLE_EQ(-8.0, mat[1]);
	EXPECT_DOUBLE_EQ(7.0, mat[2]);
	EXPECT_DOUBLE_EQ(-6.0, mat[3]);
	EXPECT_DOUBLE_EQ(5.0, mat[4]);
	EXPECT_DOUBLE_EQ(-4.0, mat[5]);
	EXPECT_DOUBLE_EQ(3.0, mat[6]);
	EXPECT_DOUBLE_EQ(-2.0, mat[7]);
	EXPECT_DOUBLE_EQ(1.0, mat[8]);

	mat[0] = -9.0;
	mat[1] = 8.0;
	mat[2] = -7.0;
	mat[3] = 6.0;
	mat[4] = -5.0;
	mat[5] = 4.0;
	mat[6] = -3.0;
	mat[7] = 2.0;
	mat[8] = -1.0;
	EXPECT_DOUBLE_EQ(-9.0, mat[0]);
	EXPECT_DOUBLE_EQ(8.0, mat[1]);
	EXPECT_DOUBLE_EQ(-7.0, mat[2]);
	EXPECT_DOUBLE_EQ(6.0, mat[3]);
	EXPECT_DOUBLE_EQ(-5.0, mat[4]);
	EXPECT_DOUBLE_EQ(4.0, mat[5]);
	EXPECT_DOUBLE_EQ(-3.0, mat[6]);
	EXPECT_DOUBLE_EQ(2.0, mat[7]);
	EXPECT_DOUBLE_EQ(-1.0, mat[8]);

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	EXPECT_DOUBLE_EQ(9.0, mat[0]);
	EXPECT_DOUBLE_EQ(-8.0, mat[1]);
	EXPECT_DOUBLE_EQ(7.0, mat[2]);
	EXPECT_DOUBLE_EQ(-6.0, mat[3]);
	EXPECT_DOUBLE_EQ(5.0, mat[4]);
	EXPECT_DOUBLE_EQ(-4.0, mat[5]);
	EXPECT_DOUBLE_EQ(3.0, mat[6]);
	EXPECT_DOUBLE_EQ(-2.0, mat[7]);
	EXPECT_DOUBLE_EQ(1.0, mat[8]);

	mat(0, 0) = -9.0;
	mat(0, 1) = 8.0;
	mat(0, 2) = -7.0;
	mat(1, 0) = 6.0;
	mat(1, 1) = -5.0;
	mat(1, 2) = 4.0;
	mat(2, 0) = -3.0;
	mat(2, 1) = 2.0;
	mat(2, 2) = -1.0;
	EXPECT_DOUBLE_EQ(-9.0, mat[0]);
	EXPECT_DOUBLE_EQ(8.0, mat[1]);
	EXPECT_DOUBLE_EQ(-7.0, mat[2]);
	EXPECT_DOUBLE_EQ(6.0, mat[3]);
	EXPECT_DOUBLE_EQ(-5.0, mat[4]);
	EXPECT_DOUBLE_EQ(4.0, mat[5]);
	EXPECT_DOUBLE_EQ(-3.0, mat[6]);
	EXPECT_DOUBLE_EQ(2.0, mat[7]);
	EXPECT_DOUBLE_EQ(-1.0, mat[8]);
	EXPECT_DOUBLE_EQ(-9.0, mat(0, 0));
	EXPECT_DOUBLE_EQ(8.0, mat(0, 1));
	EXPECT_DOUBLE_EQ(-7.0, mat(0, 2));
	EXPECT_DOUBLE_EQ(6.0, mat(1, 0));
	EXPECT_DOUBLE_EQ(-5.0, mat(1, 1));
	EXPECT_DOUBLE_EQ(4.0, mat(1, 2));
	EXPECT_DOUBLE_EQ(-3.0, mat(2, 0));
	EXPECT_DOUBLE_EQ(2.0, mat(2, 1));
	EXPECT_DOUBLE_EQ(-1.0, mat(2, 2));

	mat.Set(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0);
	EXPECT_TRUE(mat == Matrix3x3D(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0));

	mat.Set(9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0);
	EXPECT_TRUE(mat != Matrix3x3D(9.0, -8.0, 7.0, -6.0, 5.0, -4.0, 3.0, -2.0, 1.0));
}

TEST(Matrix3x3, Helpers)
{
	Matrix3x3D mat = Matrix3x3D::MakeZero();
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)));

	mat = Matrix3x3D::MakeIdentity();
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(1.0, 0.0, 0.0,
										 0.0, 1.0, 0.0,
										 0.0, 0.0, 1.0)));

	mat = Matrix3x3D::MakeScaleMatrix(3.0, -4.0, 2.4);
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(3.0, 0.0, 0.0,
										 0.0, -4.0, 0.0,
										 0.0, 0.0, 2.4)));

	mat = Matrix3x3D::MakeScaleMatrix(Vector3D(-2.0, 5.0, 3.5));
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(-2.0, 0.0, 0.0,
										 0.0, 5.0, 0.0,
										 0.0, 0.0, 3.5)));

	mat = Matrix3x3D::MakeRotationMatrix(Vector3D(-1.0 / 3.0, 2.0 / 3.0, 2.0 / 3.0), -74.0 / 180.0 * PI_DOUBLE);
	EXPECT_TRUE(mat.IsSimilar(Matrix3x3D(0.36, 0.48, -0.8, -0.8, 0.60, 0.0, 0.48, 0.64, 0.6), 0.05))
		<< mat(0, 0) << ' ' << mat(0, 1) << ' ' << mat(0, 2) << "\n"
		<< mat(1, 0) << ' ' << mat(1, 1) << ' ' << mat(1, 2) << "\n"
		<< mat(2, 0) << ' ' << mat(2, 1) << ' ' << mat(2, 2) << "\n";
}