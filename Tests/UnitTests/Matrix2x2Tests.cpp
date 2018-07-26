#include "pch.h"

#include <Core/Matrix/Matrix2x2.h>

using namespace CubbyFlow;

TEST(Matrix2x2, Constructors)
{
	Matrix2x2D mat;
	EXPECT_TRUE(mat == Matrix2x2D(1.0, 0.0, 0.0, 1.0));

	Matrix2x2D mat2(3.1);
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_DOUBLE_EQ(3.1, mat2[i]);
	}

	Matrix2x2D mat3(1.0, 2.0, 3.0, 4.0);
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat3[i]);
	}

	Matrix2x2D mat4({ { 1.0, 2.0 }, { 3.0, 4.0 } });
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat4[i]);
	}

	Matrix2x2D mat5(mat4);
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat5[i]);
	}

	double arr[4] = { 1.0, 2.0, 3.0, 4.0 };
	Matrix2x2D mat6(arr);
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat6[i]);
	}
}

TEST(Matrix2x2, SetMethods)
{
	Matrix2x2D mat;

	mat.Set(3.1);
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_DOUBLE_EQ(3.1, mat[i]);
	}

	mat.Set(0.0);
	mat.Set(1.0, 2.0, 3.0, 4.0);
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
	}

	mat.Set(0.0);
	mat.Set({ { 1.0, 2.0 }, { 3.0, 4.0 } });
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
	}

	mat.Set(0.0);
	mat.Set(Matrix2x2D(1.0, 2.0, 3.0, 4.0));
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
	}

	mat.Set(0.0);
	double arr[4] = { 1.0, 2.0, 3.0, 4.0 };
	mat.Set(arr);
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
	}

	mat.Set(0.0);
	mat.SetDiagonal(3.1);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
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
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
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
	mat.SetRow(0, Vector2D(1.0, 2.0));
	mat.SetRow(1, Vector2D(3.0, 4.0));
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
	}

	mat.Set(0.0);
	mat.SetColumn(0, Vector2D(1.0, 3.0));
	mat.SetColumn(1, Vector2D(2.0, 4.0));
	for (int i = 0; i < 4; ++i)
	{
		EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
	}
}

TEST(Matrix2x2, BasicGetters)
{
	Matrix2x2D mat(1.0, 2.0, 3.0, 4.0), mat2(1.01, 2.01, 2.99, 4.0);

	EXPECT_TRUE(mat.IsSimilar(mat2, 0.02));
	EXPECT_FALSE(mat.IsSimilar(mat2, 0.001));

	EXPECT_TRUE(mat.IsSquare());

	EXPECT_EQ(2u, mat.Rows());
	EXPECT_EQ(2u, mat.Cols());
}

TEST(Matrix2x2, BinaryOperators)
{
	Matrix2x2D mat(-4.0, 3.0, -2.0, 1.0), mat2;
	Vector2D vec;

	mat2 = mat.Add(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-2.0, 5.0, 0.0, 3.0)));

	mat2 = mat.Add(Matrix2x2D(1.0, 2.0, 3.0, 4.0));
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-3.0, 5.0, 1.0, 5.0)));

	mat2 = mat.Sub(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-6.0, 1.0, -4.0, -1.0)));

	mat2 = mat.Sub(Matrix2x2D(1.0, 2.0, 3.0, 4.0));
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-5.0, 1.0, -5.0, -3.0)));

	mat2 = mat.Mul(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-8.0, 6.0, -4.0, 2.0)));

	vec = mat.Mul(Vector2D(1, 2));
	EXPECT_TRUE(vec.IsSimilar(Vector2D(2.0, 0.0)));

	mat2 = mat.Mul(Matrix2x2D(1.0, 2.0, 3.0, 4.0));
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(5.0, 4.0, 1.0, 0.0)));

	mat2 = mat.Div(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-2.0, 1.5, -1.0, 0.5)));

	mat2 = mat.RAdd(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-2.0, 5.0, 0.0, 3.0)));

	mat2 = mat.RAdd(Matrix2x2D(1.0, 2.0, 3.0, 4.0));
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-3.0, 5.0, 1.0, 5.0)));

	mat2 = mat.RSub(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(6.0, -1.0, 4.0, 1.0)));

	mat2 = mat.RSub(Matrix2x2D(1.0, 2.0, 3.0, 4.0));
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(5.0, -1.0, 5.0, 3.0)));

	mat2 = mat.RMul(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-8.0, 6.0, -4.0, 2.0)));

	mat2 = mat.RMul(Matrix2x2D(1.0, 2.0, 3.0, 4.0));
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-8.0, 5.0, -20.0, 13.0)));

	mat2 = mat.RDiv(2.0);
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-0.5, 2.0 / 3.0, -1.0, 2.0)));
}

TEST(Matrix2x2, AugmentedOperators)
{
	Matrix2x2D mat(-4.0, 3.0, -2.0, 1.0);

	mat.IAdd(2.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-2.0, 5.0, 0.0, 3.0)));

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	mat.IAdd(Matrix2x2D(1.0, 2.0, 3.0, 4.0));
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-3.0, 5.0, 1.0, 5.0)));

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	mat.ISub(2.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-6.0, 1.0, -4.0, -1.0)));

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	mat.ISub(Matrix2x2D(1.0, 2.0, 3.0, 4.0));
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-5.0, 1.0, -5.0, -3.0)));

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	mat.IMul(2.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-8.0, 6.0, -4.0, 2.0)));

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	mat.IMul(Matrix2x2D(1.0, 2.0, 3.0, 4.0));
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(5.0, 4.0, 1.0, 0.0)));

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	mat.IDiv(2.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-2.0, 1.5, -1.0, 0.5)));
}

TEST(Matrix2x2, Modifiers)
{
	Matrix2x2D mat(-4.0, 3.0, -2.0, 1.0);

	mat.Transpose();
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-4.0, -2.0, 3.0, 1.0)));

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	mat.Invert();
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(0.5, -1.5, 1.0, -2.0)));
}

TEST(Matrix2x2, ComplexGetters)
{
	Matrix2x2D mat(-4.0, 3.0, -2.0, 1.0), mat2;

	EXPECT_DOUBLE_EQ(-2.0, mat.Sum());

	EXPECT_DOUBLE_EQ(-0.5, mat.Avg());

	EXPECT_DOUBLE_EQ(-4.0, mat.Min());

	EXPECT_DOUBLE_EQ(3.0, mat.Max());

	EXPECT_DOUBLE_EQ(1.0, mat.AbsMin());

	EXPECT_DOUBLE_EQ(-4.0, mat.AbsMax());

	EXPECT_DOUBLE_EQ(-3.0, mat.Trace());

	EXPECT_DOUBLE_EQ(2.0, mat.Determinant());

	mat2 = mat.Diagonal();
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-4.0, 0.0, 0.0, 1.0)));

	mat2 = mat.OffDiagonal();
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(0.0, 3.0, -2.0, 0.0)));

	mat2 = mat.StrictLowerTriangle();
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(0.0, 0.0, -2.0, 0.0)));

	mat2 = mat.StrictUpperTriangle();
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(0.0, 3.0, 0.0, 0.0)));

	mat2 = mat.LowerTriangle();
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-4.0, 0.0, -2.0, 1.0)));

	mat2 = mat.UpperTriangle();
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-4.0, 3.0, 0.0, 1.0)));

	mat2 = mat.Transposed();
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-4.0, -2.0, 3.0, 1.0)));

	mat2 = mat.Inverse();
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(0.5, -1.5, 1.0, -2.0)));

	Matrix2x2F mat3 = mat.CastTo<float>();
	EXPECT_TRUE(mat3.IsSimilar(Matrix2x2F(-4.f, 3.f, -2.f, 1.f)));
}

TEST(Matrix2x2, SetterOperatorOverloadings)
{
	Matrix2x2D mat(-4.0, 3.0, -2.0, 1.0), mat2;

	mat2 = mat;
	EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-4.0, 3.0, -2.0, 1.0)));

	mat += 2.0;
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-2.0, 5.0, 0.0, 3.0)));

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	mat += Matrix2x2D(1.0, 2.0, 3.0, 4.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-3.0, 5.0, 1.0, 5.0)));

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	mat -= 2.0;
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-6.0, 1.0, -4.0, -1.0)));

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	mat -= Matrix2x2D(1.0, 2.0, 3.0, 4.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-5.0, 1.0, -5.0, -3.0)));

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	mat *= 2.0;
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-8.0, 6.0, -4.0, 2.0)));

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	mat *= Matrix2x2D(1.0, 2.0, 3.0, 4.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(5.0, 4.0, 1.0, 0.0)));

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	mat /= 2.0;
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-2.0, 1.5, -1.0, 0.5)));
}

TEST(Matrix2x2, GetterOperatorOverloadings)
{
	Matrix2x2D mat(-4.0, 3.0, -2.0, 1.0);

	EXPECT_DOUBLE_EQ(-4.0, mat[0]);
	EXPECT_DOUBLE_EQ(3.0, mat[1]);
	EXPECT_DOUBLE_EQ(-2.0, mat[2]);
	EXPECT_DOUBLE_EQ(1.0, mat[3]);

	mat[0] = 4.0;
	mat[1] = -3.0;
	mat[2] = 2.0;
	mat[3] = -1.0;
	EXPECT_DOUBLE_EQ(4.0, mat[0]);
	EXPECT_DOUBLE_EQ(-3.0, mat[1]);
	EXPECT_DOUBLE_EQ(2.0, mat[2]);
	EXPECT_DOUBLE_EQ(-1.0, mat[3]);

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	EXPECT_DOUBLE_EQ(-4.0, mat(0, 0));
	EXPECT_DOUBLE_EQ(3.0, mat(0, 1));
	EXPECT_DOUBLE_EQ(-2.0, mat(1, 0));
	EXPECT_DOUBLE_EQ(1.0, mat(1, 1));

	mat(0, 0) = 4.0;
	mat(0, 1) = -3.0;
	mat(1, 0) = 2.0;
	mat(1, 1) = -1.0;
	EXPECT_DOUBLE_EQ(4.0, mat[0]);
	EXPECT_DOUBLE_EQ(-3.0, mat[1]);
	EXPECT_DOUBLE_EQ(2.0, mat[2]);
	EXPECT_DOUBLE_EQ(-1.0, mat[3]);

	mat.Set(-4.0, 3.0, -2.0, 1.0);
	EXPECT_TRUE(mat == Matrix2x2D(-4.0, 3.0, -2.0, 1.0));

	mat.Set(4.0, 3.0, 2.0, 1.0);
	EXPECT_TRUE(mat != Matrix2x2D(-4.0, 3.0, -2.0, 1.0));
}

TEST(Matrix2x2, Helpers)
{
	Matrix2x2D mat = Matrix2x2D::MakeZero();
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(0.0, 0.0, 0.0, 0.0)));

	mat = Matrix2x2D::MakeIdentity();
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(1.0, 0.0, 0.0, 1.0)));

	mat = Matrix2x2D::MakeScaleMatrix(3.0, -4.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(3.0, 0.0, 0.0, -4.0)));

	mat = Matrix2x2D::MakeScaleMatrix(Vector2D(-2.0, 5.0));
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-2.0, 0.0, 0.0, 5.0)));

	mat = Matrix2x2D::MakeRotationMatrix(PI_DOUBLE / 3.0);
	EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(0.5, -std::sqrt(3.0) / 2.0, std::sqrt(3.0) / 2.0, 0.5)));
}