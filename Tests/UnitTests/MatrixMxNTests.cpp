#include "pch.h"

#include <Core/Matrix/MatrixMxN.h>
#include <Core/Vector/VectorN.h>

using namespace CubbyFlow;

namespace
{
	template <typename T>
	inline std::ostream& operator<<(std::ostream& stream, const MatrixMxN<T>& m)
	{
		for (size_t i = 0; i < m.Rows(); ++i)
		{
			for (size_t j = 0; j < m.Cols(); ++j)
			{
				stream << m(i, j) << ' ';
			}

			stream << '\n';
		}

		return stream;
	}
}

TEST(MatrixMxN, Constructors)
{
	MatrixMxND mat;
	EXPECT_EQ(0u, mat.Rows());
	EXPECT_EQ(0u, mat.Cols());

	MatrixMxND mat3(4, 2, 5.0);
	EXPECT_EQ(4u, mat3.Rows());
	EXPECT_EQ(2u, mat3.Cols());
	for (size_t i = 0; i < 8; ++i)
	{
		EXPECT_EQ(5.0, mat3[i]);
	}

	MatrixMxND mat4 =
	{
		{ 1.0, 2.0, 3.0, 4.0 },
		{ 5.0, 6.0, 7.0, 8.0 },
		{ 9.0, 10.0, 11.0, 12.0 }
	};
	EXPECT_EQ(3u, mat4.Rows());
	EXPECT_EQ(4u, mat4.Cols());
	for (size_t i = 0; i < 12; ++i)
	{
		EXPECT_EQ(i + 1.0, mat4[i]);
	}

	MatrixMxND mat5(mat4);
	EXPECT_EQ(3u, mat5.Rows());
	EXPECT_EQ(4u, mat5.Cols());
	for (size_t i = 0; i < 12; ++i)
	{
		EXPECT_EQ(mat4[i], mat5[i]);
	}

	double ans6[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
	MatrixMxND mat6(3, 2, ans6);
	EXPECT_EQ(3u, mat6.Rows());
	EXPECT_EQ(2u, mat6.Cols());
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(ans6[i], mat6[i]);
	}
}

TEST(MatrixMxN, BasicSetters)
{
	MatrixMxND mat;
	mat.Resize(4, 2, 5.0);
	EXPECT_EQ(4u, mat.Rows());
	EXPECT_EQ(2u, mat.Cols());
	for (size_t i = 0; i < 8; ++i)
	{
		EXPECT_EQ(5.0, mat[i]);
	}

	mat.Set(7.0);
	for (size_t i = 0; i < 8; ++i)
	{
		EXPECT_EQ(7.0, mat[i]);
	}

	mat.Set({ { 1.0, 2.0, 3.0, 4.0 }, { 5.0, 6.0, 7.0, 8.0 } });
	EXPECT_EQ(2u, mat.Rows());
	EXPECT_EQ(4u, mat.Cols());
	for (size_t i = 0; i < 8; ++i)
	{
		EXPECT_EQ(i + 1.0, mat[i]);
	}

	MatrixMxND mat2;
	mat2.Set(mat);
	EXPECT_EQ(2u, mat2.Rows());
	EXPECT_EQ(4u, mat2.Cols());
	for (size_t i = 0; i < 8; ++i)
	{
		EXPECT_EQ(i + 1.0, mat2[i]);
	}

	double arr[] = { 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0 };
	mat.Set(4, 2, arr);
	EXPECT_EQ(4u, mat.Rows());
	EXPECT_EQ(2u, mat.Cols());
	for (size_t i = 0; i < 8; ++i)
	{
		EXPECT_EQ(arr[i], mat[i]);
	}

	mat.SetDiagonal(10.0);
	for (size_t i = 0; i < 8; ++i)
	{
		if (i == 0 || i == 3)
		{
			EXPECT_EQ(10.0, mat[i]);
		}
		else
		{
			EXPECT_EQ(arr[i], mat[i]);
		}
	}

	mat.SetOffDiagonal(-1.0);
	for (size_t i = 0; i < 8; ++i)
	{
		if (i == 0 || i == 3)
		{
			EXPECT_EQ(10.0, mat[i]);
		}
		else
		{
			EXPECT_EQ(-1.0, mat[i]);
		}
	}

	VectorND row = { 2.0, 3.0 };
	mat.SetRow(2, row);
	for (size_t i = 0; i < 8; ++i)
	{
		if (i == 0 || i == 3)
		{
			EXPECT_EQ(10.0, mat[i]);
		}
		else if (i == 4)
		{
			EXPECT_EQ(2.0, mat[i]);
		}
		else if (i == 5)
		{
			EXPECT_EQ(3.0, mat[i]);
		}
		else
		{
			EXPECT_EQ(-1.0, mat[i]);
		}
	}

	mat.Set({ { 1.0, 2.0, 3.0, 4.0 }, { 5.0, 6.0, 7.0, 8.0 } });
	mat2.Set({ { 1.0, 2.0, 3.0, 4.0 }, { 5.0, 6.0, 7.0, 8.0 } });
	EXPECT_TRUE(mat.IsEqual(mat2));

	mat2.Set({ { 1.01, 2.01, 3.01, 4.01 }, { 4.99, 5.99, 6.99, 7.99 } });
	EXPECT_TRUE(mat.IsSimilar(mat2, 0.02));
	EXPECT_FALSE(mat.IsSimilar(mat2, 0.005));

	EXPECT_FALSE(mat.IsSquare());
	mat.Set({ { 1.0, 2.0 }, { 3.0, 4.0 } });
	EXPECT_TRUE(mat.IsSquare());
}

TEST(MatrixMxN, BinaryOperatorMethod)
{
	const MatrixMxND matA = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };

	MatrixMxND matB = matA.Add(3.5);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i + 4.5, matB[i]);
	}

	MatrixMxND matC = { { 3.0, -1.0, 2.0 }, { 9.0, 2.0, 8.0 } };
	matB = matA.Add(matC);
	MatrixMxND ans = { { 4.0, 1.0, 5.0 }, { 13.0, 7.0, 14.0 } };
	EXPECT_TRUE(ans.IsEqual(matB));

	matB = matA.Sub(1.5);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i - 0.5, matB[i]);
	}

	matB = matA.Sub(matC);
	ans = { { -2.0, 3.0, 1.0 }, { -5.0, 3.0, -2.0 } };
	EXPECT_TRUE(ans.IsEqual(matB));

	matB = matA.Mul(2.0);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(2.0 * (i + 1.0), matB[i]);
	}

	matC = { { 3.0, -1.0 }, { 2.0, 9.0 }, { 2.0, 8.0 } };
	matB = matA.Mul(matC);
	ans = { { 13.0, 41.0 }, { 34.0, 89.0 } };
	EXPECT_TRUE(ans.IsEqual(matB));

	matB = matA.Div(2.0);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ((i + 1.0) / 2.0, matB[i]);
	}

	matB = matA.RAdd(3.5);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i + 4.5, matB[i]);
	}

	matB = matA.RSub(1.5);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(0.5 - i, matB[i]);
	}

	matC = { { 3.0, -1.0, 2.0 }, { 9.0, 2.0, 8.0 } };
	matB = matA.RSub(matC);
	ans = { { 2.0, -3.0, -1.0 }, { 5.0, -3.0, 2.0 } };
	EXPECT_EQ(ans, matB);

	matB = matA.RMul(2.0);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(2.0 * (i + 1.0), matB[i]);
	}

	matC = { { 3.0, -1.0 }, { 2.0, 9.0 }, { 2.0, 8.0 } };
	matB = matC.RMul(matA);
	ans = { { 13.0, 41.0 }, { 34.0, 89.0 } };
	EXPECT_EQ(ans, matB);

	matB = matA.RDiv(2.0);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(2.0 / (i + 1.0), matB[i]);
	}
}

TEST(MatrixMxN, AugmentedOperatorMethod)
{
	const MatrixMxND matA = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };
	const MatrixMxND matB = { { 3.0, -1.0, 2.0 }, { 9.0, 2.0, 8.0 } };

	MatrixMxND mat = matA;
	mat.IAdd(3.5);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i + 4.5, mat[i]);
	}

	mat = matA;
	mat.IAdd(matB);
	MatrixMxND ans = { { 4.0, 1.0, 5.0 }, { 13.0, 7.0, 14.0 } };
	EXPECT_EQ(ans, mat);

	mat = matA;
	mat.ISub(1.5);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i - 0.5, mat[i]) << i;
	}

	mat = matA;
	mat.ISub(matB);
	ans = { { -2.0, 3.0, 1.0 }, { -5.0, 3.0, -2.0 } };
	EXPECT_EQ(ans, mat);

	mat = matA;
	mat.IMul(2.0);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(2.0 * (i + 1.0), mat[i]);
	}

	mat = matA;
	const MatrixMxND matC = { { 3.0, -1.0 }, { 2.0, 9.0 }, { 2.0, 8.0 } };
	mat.IMul(matC);
	EXPECT_EQ(2u, mat.Rows());
	EXPECT_EQ(2u, mat.Cols());
	ans = { { 13.0, 41.0 }, { 34.0, 89.0 } };
	EXPECT_EQ(ans, mat);

	mat = matA;
	mat.IDiv(2.0);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ((i + 1.0) / 2.0, mat[i]);
	}
}

TEST(MatrixMxN, ComplexGetters)
{
	const MatrixMxND matA = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };

	EXPECT_EQ(21.0, matA.Sum());
	EXPECT_DOUBLE_EQ(21.0 / 6.0, matA.Avg());

	const MatrixMxND matB = { { 3.0, -1.0, 2.0 }, { -9.0, 2.0, 8.0 } };
	EXPECT_EQ(-9.0, matB.Min());
	EXPECT_EQ(8.0, matB.Max());
	EXPECT_EQ(-1.0, matB.AbsMin());
	EXPECT_EQ(-9.0, matB.AbsMax());

	const MatrixMxND matC =
	{
		{ 3.0, -1.0, 2.0 },
		{ -9.0, 2.0, 8.0 },
		{ 4.0, 3.0, 6.0 }
	};
	EXPECT_EQ(11.0, matC.Trace());

	EXPECT_DOUBLE_EQ(-192.0, matC.Determinant());

	MatrixMxND mat = matA.Diagonal();
	MatrixMxND ans = { { 1.0, 0.0, 0.0 }, { 0.0, 5.0, 0.0 } };
	EXPECT_EQ(ans, mat);

	mat = matA.OffDiagonal();
	ans = { { 0.0, 2.0, 3.0 }, { 4.0, 0.0, 6.0 } };
	EXPECT_EQ(ans, mat);

	mat = matC.StrictLowerTri();
	ans = { { 0.0, 0.0, 0.0 }, { -9.0, 0.0, 0.0 }, { 4.0, 3.0, 0.0 } };
	EXPECT_EQ(ans, mat);

	mat = matC.StrictUpperTri();
	ans = { { 0.0, -1.0, 2.0 }, { 0.0, 0.0, 8.0 }, { 0.0, 0.0, 0.0 } };
	EXPECT_EQ(ans, mat);

	mat = matC.LowerTri();
	ans = { { 3.0, 0.0, 0.0 }, { -9.0, 2.0, 0.0 }, { 4.0, 3.0, 6.0 } };
	EXPECT_EQ(ans, mat);

	mat = matC.UpperTri();
	ans = { { 3.0, -1.0, 2.0 }, { 0.0, 2.0, 8.0 }, { 0.0, 0.0, 6.0 } };
	EXPECT_EQ(ans, mat);

	MatrixMxNF matF = matC.CastTo<float>();
	MatrixMxNF ansF = { { 3.f, -1.f, 2.f }, { -9.f, 2.f, 8.f }, { 4.f, 3.f, 6.f } };
	EXPECT_EQ(ansF, matF);

	mat = matA.Transposed();
	ans = { { 1.0, 4.0 }, { 2.0, 5.0 }, { 3.0, 6.0 } };
	EXPECT_EQ(ans, mat);

	mat = { { 1.0, 2.0, 3.0 }, { 2.0, 5.0, 3.0 }, { 1.0, 0.0, 8.0 } };
	MatrixMxND mat2 = mat.Inverse();
	ans = { { -40.0, 16.0, 9.0 }, { 13.0, -5.0, -3.0 }, { 5.0, -2.0, -1.0 } };
	EXPECT_TRUE(mat2.IsSimilar(ans, 1e-9));

	mat = { { 1.0, 2.0, 3.0 }, { 0.0, 1.0, 4.0 }, { 5.0, 6.0, 0.0 } };
	mat2 = mat.Inverse();
	ans = { { -24.0, 18.0, 5.0 }, { 20.0, -15.0, -4.0 }, { -5.0, 4.0, 1.0 } };
	EXPECT_TRUE(mat2.IsSimilar(ans, 1e-9));

	mat = { { 0.0, 1.0, 4.0 }, { 1.0, 2.0, 3.0 }, { 5.0, 6.0, 0.0 } };
	mat2 = mat.Inverse();
	ans = { { 18.0, -24.0, 5.0 }, { -15.0, 20.0, -4.0 }, { 4.0, -5.0, 1.0 } };
	EXPECT_TRUE(mat2.IsSimilar(ans, 1e-9));
}

TEST(MatrixMxN, Modifiers)
{
	MatrixMxND mat = { { 9.0, -8.0, 7.0 }, { -6.0, 5.0, -4.0 }, { 3.0, -2.0, 1.0 } };
	mat.Transpose();

	MatrixMxND ans = { { 9.0, -6.0, 3.0 }, { -8.0, 5.0, -2.0 }, { 7.0, -4.0, 1.0 } };
	EXPECT_EQ(ans, mat);

	mat = { { 1.0, 2.0, 3.0 }, { 2.0, 5.0, 3.0 }, { 1.0, 0.0, 8.0 } };
	mat.Invert();
	ans = { { -40.0, 16.0, 9.0 }, { 13.0, -5.0, -3.0 }, { 5.0, -2.0, -1.0 } };
	EXPECT_TRUE(mat.IsSimilar(ans, 1e-9));
}

TEST(MatrixMxN, SetterOperators)
{
	const MatrixMxND matA = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };
	const MatrixMxND matB = { { 3.0, -1.0, 2.0 }, { 9.0, 2.0, 8.0 } };

	MatrixMxND mat = matA;
	mat += 3.5;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i + 4.5, mat[i]);
	}

	mat = matA;
	mat += matB;
	MatrixMxND ans = { { 4.0, 1.0, 5.0 }, { 13.0, 7.0, 14.0 } };
	EXPECT_EQ(ans, mat);

	mat = matA;
	mat -= 1.5;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i - 0.5, mat[i]) << i;
	}

	mat = matA;
	mat -= matB;
	ans = { { -2.0, 3.0, 1.0 }, { -5.0, 3.0, -2.0 } };
	EXPECT_EQ(ans, mat);

	mat = matA;
	mat *= 2.0;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(2.0 * (i + 1.0), mat[i]);
	}

	mat = matA;
	const MatrixMxND matC = { { 3.0, -1.0 }, { 2.0, 9.0 }, { 2.0, 8.0 } };
	mat *= matC;
	EXPECT_EQ(2u, mat.Rows());
	EXPECT_EQ(2u, mat.Cols());
	ans = { { 13.0, 41.0 }, { 34.0, 89.0 } };
	EXPECT_EQ(ans, mat);

	mat = matA;
	mat /= 2.0;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ((i + 1.0) / 2.0, mat[i]);
	}
}

TEST(MatrixMxN, GetterOperator)
{
	MatrixMxND mat, mat2;
	mat.Set({ { 1.0, 2.0, 3.0, 4.0 }, { 5.0, 6.0, 7.0, 8.0 } });
	double cnt = 1.0;
	for (size_t i = 0; i < 2; ++i)
	{
		for (size_t j = 0; j < 4; ++j)
		{
			EXPECT_EQ(cnt, mat(i, j));
			cnt += 1.0;
		}
	}

	for (size_t i = 0; i < 8; ++i)
	{
		EXPECT_EQ(i + 1.0, mat[i]);
	}

	mat.Set({ { 1.0, 2.0, 3.0, 4.0 }, { 5.0, 6.0, 7.0, 8.0 } });
	mat2.Set({ { 1.0, 2.0, 3.0, 4.0 }, { 5.0, 6.0, 7.0, 8.0 } });
	EXPECT_EQ(mat, mat2);
}

TEST(MatrixMxN, Builders)
{
	MatrixMxND mat = MatrixMxND::MakeZero(3, 4);
	EXPECT_EQ(3u, mat.Rows());
	EXPECT_EQ(4u, mat.Cols());
	for (size_t i = 0; i < 12; ++i)
	{
		EXPECT_EQ(0.0, mat[i]);
	}

	mat = MatrixMxND::MakeIdentity(5);
	EXPECT_EQ(5u, mat.Rows());
	EXPECT_EQ(5u, mat.Cols());
	for (size_t i = 0; i < 25; ++i)
	{
		if (i % 6 == 0)
		{
			EXPECT_EQ(1.0, mat[i]);
		}
		else
		{
			EXPECT_EQ(0.0, mat[i]);
		}
	}
}