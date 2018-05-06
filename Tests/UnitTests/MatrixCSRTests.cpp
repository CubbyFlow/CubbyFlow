#include "pch.h"

#include <Core/Matrix/MatrixCSR.h>
#include <Core/Matrix/MatrixMxN.h>
#include <Core/Vector/VectorN.h>

using namespace CubbyFlow;

TEST(MatrixCSR, Constructors)
{
	const MatrixCSRD emptyMat;

	EXPECT_EQ(0u, emptyMat.Rows());
	EXPECT_EQ(0u, emptyMat.Cols());
	EXPECT_EQ(0u, emptyMat.NumberOfNonZeros());
	EXPECT_EQ(emptyMat.NonZeroBegin(), emptyMat.NonZeroEnd());
	EXPECT_EQ(1, emptyMat.RowPointersEnd() - emptyMat.RowPointersBegin());
	EXPECT_EQ(emptyMat.ColumnIndicesBegin(), emptyMat.ColumnIndicesEnd());

	const MatrixCSRD matInitLst =
	{
		{ 1.0, 0.0, 0.0, -3.0 },
		{ 0.0, 3.0, -5.0, 1.0 },
		{ -4.0, 0.0, 1.0, 5.0 }
	};
	EXPECT_EQ(3u, matInitLst.Rows());
	EXPECT_EQ(4u, matInitLst.Cols());
	EXPECT_EQ(8u, matInitLst.NumberOfNonZeros());

	auto iterInitLst = matInitLst.NonZeroBegin();
	EXPECT_EQ(1.0, iterInitLst[0]);
	EXPECT_EQ(-3.0, iterInitLst[1]);
	EXPECT_EQ(3.0, iterInitLst[2]);
	EXPECT_EQ(-5.0, iterInitLst[3]);
	EXPECT_EQ(1.0, iterInitLst[4]);
	EXPECT_EQ(-4.0, iterInitLst[5]);
	EXPECT_EQ(1.0, iterInitLst[6]);
	EXPECT_EQ(5.0, iterInitLst[7]);

	const MatrixMxND matDense =
	{
		{ 1.0, 0.01, 0.0, -3.0 },
		{ 0.01, 3.0, -5.0, 1.0 },
		{ -4.0, 0.01, 1.0, 5.0 }
	};
	const MatrixCSRD matSparse(matDense, 0.02);
	EXPECT_EQ(3u, matSparse.Rows());
	EXPECT_EQ(4u, matSparse.Cols());
	EXPECT_EQ(8u, matSparse.NumberOfNonZeros());

	auto iterSparse = matSparse.NonZeroBegin();
	EXPECT_EQ(1.0, iterSparse[0]);
	EXPECT_EQ(-3.0, iterSparse[1]);
	EXPECT_EQ(3.0, iterSparse[2]);
	EXPECT_EQ(-5.0, iterSparse[3]);
	EXPECT_EQ(1.0, iterSparse[4]);
	EXPECT_EQ(-4.0, iterSparse[5]);
	EXPECT_EQ(1.0, iterSparse[6]);
	EXPECT_EQ(5.0, iterSparse[7]);

	MatrixCSRD matCopied = matSparse;
	EXPECT_EQ(3u, matCopied.Rows());
	EXPECT_EQ(4u, matCopied.Cols());
	EXPECT_EQ(8u, matCopied.NumberOfNonZeros());

	auto iterCopied = matCopied.NonZeroBegin();
	EXPECT_EQ(1.0, iterCopied[0]);
	EXPECT_EQ(-3.0, iterCopied[1]);
	EXPECT_EQ(3.0, iterCopied[2]);
	EXPECT_EQ(-5.0, iterCopied[3]);
	EXPECT_EQ(1.0, iterCopied[4]);
	EXPECT_EQ(-4.0, iterCopied[5]);
	EXPECT_EQ(1.0, iterCopied[6]);
	EXPECT_EQ(5.0, iterCopied[7]);

	const MatrixCSRD matMoved = std::move(matCopied);
	EXPECT_EQ(3u, matMoved.Rows());
	EXPECT_EQ(4u, matMoved.Cols());
	EXPECT_EQ(8u, matMoved.NumberOfNonZeros());

	auto iterMovied = matMoved.NonZeroBegin();
	EXPECT_EQ(1.0, iterMovied[0]);
	EXPECT_EQ(-3.0, iterMovied[1]);
	EXPECT_EQ(3.0, iterMovied[2]);
	EXPECT_EQ(-5.0, iterMovied[3]);
	EXPECT_EQ(1.0, iterMovied[4]);
	EXPECT_EQ(-4.0, iterMovied[5]);
	EXPECT_EQ(1.0, iterMovied[6]);
	EXPECT_EQ(5.0, iterMovied[7]);

	EXPECT_EQ(0u, matCopied.Rows());
	EXPECT_EQ(0u, matCopied.Cols());
	EXPECT_EQ(0u, matCopied.NumberOfNonZeros());
	EXPECT_EQ(matCopied.NonZeroBegin(), matCopied.NonZeroEnd());
	EXPECT_EQ(matCopied.RowPointersBegin(), matCopied.RowPointersEnd());
	EXPECT_EQ(matCopied.ColumnIndicesBegin(), matCopied.ColumnIndicesEnd());
}

TEST(MatrixCSR, BasicSetters)
{
	// Compress initializer list
	const std::initializer_list<std::initializer_list<double>> initLst =
	{
		{ 1.0, 0.01, 0.0, -3.0 },
		{ 0.01, 3.0, -5.0, 1.0 },
		{ -4.0, 0.01, 1.0, 5.0 }
	};
	MatrixCSRD matInitLst;
	matInitLst.Compress(initLst, 0.02);
	EXPECT_EQ(3u, matInitLst.Rows());
	EXPECT_EQ(4u, matInitLst.Cols());
	EXPECT_EQ(8u, matInitLst.NumberOfNonZeros());

	auto iterInitLst = matInitLst.NonZeroBegin();
	EXPECT_EQ(1.0, iterInitLst[0]);
	EXPECT_EQ(-3.0, iterInitLst[1]);
	EXPECT_EQ(3.0, iterInitLst[2]);
	EXPECT_EQ(-5.0, iterInitLst[3]);
	EXPECT_EQ(1.0, iterInitLst[4]);
	EXPECT_EQ(-4.0, iterInitLst[5]);
	EXPECT_EQ(1.0, iterInitLst[6]);
	EXPECT_EQ(5.0, iterInitLst[7]);

	// Set scalar
	matInitLst.Set(42.0);
	for (size_t i = 0; i < 8; ++i)
	{
		EXPECT_EQ(42.0, iterInitLst[i]);
	}

	// Compress dense matrix
	const MatrixMxND matDense =
	{
		{ 1.0, 0.01, 0.0, -3.0 },
		{ 0.01, 3.0, -5.0, 1.0 },
		{ -4.0, 0.01, 1.0, 5.0 }
	};
	MatrixCSRD matSparse;
	matSparse.Compress(matDense, 0.02);
	EXPECT_EQ(3u, matSparse.Rows());
	EXPECT_EQ(4u, matSparse.Cols());
	EXPECT_EQ(8u, matSparse.NumberOfNonZeros());

	auto iterSparse = matSparse.NonZeroBegin();
	EXPECT_EQ(1.0, iterSparse[0]);
	EXPECT_EQ(-3.0, iterSparse[1]);
	EXPECT_EQ(3.0, iterSparse[2]);
	EXPECT_EQ(-5.0, iterSparse[3]);
	EXPECT_EQ(1.0, iterSparse[4]);
	EXPECT_EQ(-4.0, iterSparse[5]);
	EXPECT_EQ(1.0, iterSparse[6]);
	EXPECT_EQ(5.0, iterSparse[7]);

	// Set other CSR mat
	matInitLst.Set(matSparse);
	iterInitLst = matInitLst.NonZeroBegin();

	for (size_t i = 0; i < 8; ++i)
	{
		EXPECT_EQ(iterSparse[i], iterInitLst[i]);
	}

	// Add/set element
	MatrixCSRD matAddElem;
	matAddElem.AddElement(0, 0, 1.0);
	matAddElem.SetElement(0, 3, -3.0);
	matAddElem.AddElement(1, 1, 3.0);
	matAddElem.SetElement(1, 2, -5.0);
	matAddElem.AddElement(1, 3, 1.0);
	matAddElem.SetElement(2, 0, -4.0);
	matAddElem.AddElement(2, 2, 1.0);
	matAddElem.SetElement(2, 3, 5.0);

	EXPECT_EQ(3u, matAddElem.Rows());
	EXPECT_EQ(4u, matAddElem.Cols());
	EXPECT_EQ(8u, matAddElem.NumberOfNonZeros());

	auto iterAddElem = matAddElem.NonZeroBegin();
	EXPECT_EQ(1.0, iterAddElem[0]);
	EXPECT_EQ(-3.0, iterAddElem[1]);
	EXPECT_EQ(3.0, iterAddElem[2]);
	EXPECT_EQ(-5.0, iterAddElem[3]);
	EXPECT_EQ(1.0, iterAddElem[4]);
	EXPECT_EQ(-4.0, iterAddElem[5]);
	EXPECT_EQ(1.0, iterAddElem[6]);
	EXPECT_EQ(5.0, iterAddElem[7]);

	matAddElem.SetElement(1, 3, 7.0);
	EXPECT_EQ(7.0, iterAddElem[4]);

	// Add element in random order
	MatrixCSRD matAddElemRandom;
	matAddElemRandom.AddElement(2, 2, 1.0);
	matAddElemRandom.AddElement(0, 3, -3.0);
	matAddElemRandom.AddElement(2, 0, -4.0);
	matAddElemRandom.AddElement(1, 1, 3.0);
	matAddElemRandom.AddElement(2, 3, 5.0);
	matAddElemRandom.AddElement(1, 3, 1.0);
	matAddElemRandom.AddElement(1, 2, -5.0);
	matAddElemRandom.AddElement(0, 0, 1.0);

	EXPECT_EQ(3u, matAddElemRandom.Rows());
	EXPECT_EQ(4u, matAddElemRandom.Cols());
	EXPECT_EQ(8u, matAddElemRandom.NumberOfNonZeros());

	auto iterAddElemRandom = matAddElemRandom.NonZeroBegin();
	EXPECT_EQ(1.0, iterAddElemRandom[0]);
	EXPECT_EQ(-3.0, iterAddElemRandom[1]);
	EXPECT_EQ(3.0, iterAddElemRandom[2]);
	EXPECT_EQ(-5.0, iterAddElemRandom[3]);
	EXPECT_EQ(1.0, iterAddElemRandom[4]);
	EXPECT_EQ(-4.0, iterAddElemRandom[5]);
	EXPECT_EQ(1.0, iterAddElemRandom[6]);
	EXPECT_EQ(5.0, iterAddElemRandom[7]);

	// Add row
	MatrixCSRD matAddRow;
	matAddRow.AddRow({ 1.0, -3.0 }, { 0, 3 });
	matAddRow.AddRow({ 3.0, -5.0, 1.0 }, { 1, 2, 3 });
	matAddRow.AddRow({ -4.0, 1.0, 5.0 }, { 0, 2, 3 });

	EXPECT_EQ(3u, matAddRow.Rows());
	EXPECT_EQ(4u, matAddRow.Cols());
	EXPECT_EQ(8u, matAddRow.NumberOfNonZeros());

	auto iterAddRow = matAddRow.NonZeroBegin();
	EXPECT_EQ(1.0, iterAddRow[0]);
	EXPECT_EQ(-3.0, iterAddRow[1]);
	EXPECT_EQ(3.0, iterAddRow[2]);
	EXPECT_EQ(-5.0, iterAddRow[3]);
	EXPECT_EQ(1.0, iterAddRow[4]);
	EXPECT_EQ(-4.0, iterAddRow[5]);
	EXPECT_EQ(1.0, iterAddRow[6]);
	EXPECT_EQ(5.0, iterAddRow[7]);

	// Clear
	matAddRow.Clear();
	EXPECT_EQ(0u, matAddRow.Rows());
	EXPECT_EQ(0u, matAddRow.Cols());
	EXPECT_EQ(0u, matAddRow.NumberOfNonZeros());
	EXPECT_EQ(1u, matAddRow.RowPointersEnd() - matAddRow.RowPointersBegin());
	EXPECT_EQ(0u, matAddRow.RowPointersBegin()[0]);
}

TEST(MatrixCSR, BinaryOperatorMethods)
{
	const MatrixCSRD matA = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };
	const MatrixCSRD addResult1 = matA.Add(3.5);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i + 4.5, addResult1.NonZero(i));
	}

	const MatrixCSRD matC = { { 3.0, -1.0, 2.0 }, { 9.0, 2.0, 8.0 } };
	const MatrixCSRD addResult2 = matA.Add(matC);
	const MatrixCSRD addAns1 = { { 4.0, 1.0, 5.0 }, { 13.0, 7.0, 14.0 } };
	EXPECT_TRUE(addAns1.IsEqual(addResult2));

	const MatrixCSRD matD = { { 3.0, 0.0, 2.0 }, { 0.0, 2.0, 0.0 } };
	const MatrixCSRD addResult3 = matA.Add(matD);
	const MatrixCSRD addAns2 = { { 4.0, 2.0, 5.0 }, { 4.0, 7.0, 6.0 } };
	EXPECT_TRUE(addAns2.IsEqual(addResult3));

	const MatrixCSRD matE = { { 3.0, 0.0, 2.0 }, { 0.0, 0.0, 0.0 } };
	const MatrixCSRD addResult4 = matA.Add(matE);
	const MatrixCSRD addAns3 = { { 4.0, 2.0, 5.0 }, { 4.0, 5.0, 6.0 } };
	EXPECT_TRUE(addAns3.IsEqual(addResult4));

	const MatrixCSRD subResult1 = matA.Sub(1.5);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i - 0.5, subResult1.NonZero(i));
	}

	const MatrixCSRD subResult2 = matA.Sub(matC);
	const MatrixCSRD ans2 = { { -2.0, 3.0, 1.0 }, { -5.0, 3.0, -2.0 } };
	EXPECT_TRUE(ans2.IsSimilar(subResult2));

	const MatrixCSRD matB = matA.Mul(2.0);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(2.0 * (i + 1.0), matB.NonZero(i));
	}

	const VectorND vecA = { -1.0, 9.0, 8.0 };
	const VectorND vecB = matA.Mul(vecA);
	const VectorND ansV = { 41.0, 89.0 };
	EXPECT_TRUE(ansV.IsEqual(vecB));

	const MatrixCSRD matF = { { 3.0, -1.0 }, { 2.0, 9.0 }, { 2.0, 8.0 } };
	const MatrixCSRD matG = matA.Mul(matF);
	const MatrixCSRD ans3 = { { 13.0, 41.0 }, { 34.0, 89.0 } };
	EXPECT_TRUE(ans3.IsEqual(matG));

	const MatrixCSRD matH = matA.Div(2.0);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ((i + 1.0) / 2.0, matH.NonZero(i));
	}

	const MatrixCSRD matI = matA.RAdd(3.5);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i + 4.5, matI.NonZero(i));
	}

	const MatrixCSRD matJ = matA.RSub(1.5);
	for (size_t i = 0; i < 6; ++i) 
	{
		EXPECT_EQ(0.5 - i, matJ.NonZero(i));
	}

	const MatrixCSRD matK = { { 3.0, -1.0, 2.0 }, { 9.0, 2.0, 8.0 } };
	const MatrixCSRD matL = matA.RSub(matK);
	const MatrixCSRD ans4 = { { 2.0, -3.0, -1.0 }, { 5.0, -3.0, 2.0 } };
	EXPECT_EQ(ans4, matL);

	const MatrixCSRD matM = matA.RMul(2.0);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(2.0 * (i + 1.0), matM.NonZero(i));
	}

	const MatrixCSRD matP = matA.RDiv(2.0);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(2.0 / (i + 1.0), matP.NonZero(i));
	}
}

TEST(MatrixCSR, AugmentedMethods)
{
	const MatrixCSRD matA = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };
	const MatrixCSRD matB = { { 3.0, -1.0, 2.0 }, { 9.0, 2.0, 8.0 } };

	MatrixCSRD mat = matA;
	mat.IAdd(3.5);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i + 4.5, mat.NonZero(i));
	}

	mat = matA;
	mat.IAdd(matB);
	MatrixCSRD ans = { { 4.0, 1.0, 5.0 }, { 13.0, 7.0, 14.0 } };
	EXPECT_EQ(ans, mat);

	mat = matA;
	mat.ISub(1.5);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i - 0.5, mat.NonZero(i));
	}

	mat = matA;
	mat.ISub(matB);
	ans = { { -2.0, 3.0, 1.0 }, { -5.0, 3.0, -2.0 } };
	EXPECT_EQ(ans, mat);

	mat = matA;
	mat.IMul(2.0);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(2.0 * (i + 1.0), mat.NonZero(i));
	}

	MatrixCSRD matA2;
	MatrixCSRD matC2;
	int cnt = 0;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			matA2.SetElement(i, j, cnt + 1.0);
			matC2.SetElement(i, j, 25.0 - cnt);
			++cnt;
		}
	}
	matA2.IMul(matC2);

	MatrixCSRD ans2 =
	{
		{ 175.0, 160.0, 145.0, 130.0, 115.0 },
		{ 550.0, 510.0, 470.0, 430.0, 390.0 },
		{ 925.0, 860.0, 795.0, 730.0, 665.0 },
		{ 1300.0, 1210.0, 1120.0, 1030.0, 940.0 },
		{ 1675.0, 1560.0, 1445.0, 1330.0, 1215.0 }
	};
	EXPECT_EQ(ans2, matA2);

	mat = matA;
	mat.IDiv(2.0);
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ((i + 1.0) / 2.0, mat.NonZero(i));
	}
}

TEST(MatrixCSR, ComplexGetters)
{
	const MatrixCSRD matA = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };

	EXPECT_EQ(21.0, matA.Sum());
	EXPECT_DOUBLE_EQ(21.0 / 6.0, matA.Avg());

	const MatrixCSRD matB = { { 3.0, -1.0, 2.0 }, { -9.0, 2.0, 8.0 } };
	EXPECT_EQ(-9.0, matB.Min());
	EXPECT_EQ(8.0, matB.Max());
	EXPECT_EQ(-1.0, matB.AbsMin());
	EXPECT_EQ(-9.0, matB.AbsMax());

	const MatrixCSRD matC =
	{
		{ 3.0, -1.0, 2.0, 4.0, 5.0 },
		{ -9.0, 2.0, 8.0, -1.0, 2.0 },
		{ 4.0, 3.0, 6.0, 7.0, -5.0 },
		{ -2.0, 6.0, 7.0, 1.0, 0.0 },
		{ 4.0, 2.0, 3.0, 3.0, -9.0 }
	};
	EXPECT_EQ(3.0, matC.Trace());

	const MatrixCSRF matF = matC.CastTo<float>();
	const MatrixCSRF ansF =
	{
		{ 3.f, -1.f, 2.f, 4.f, 5.f },
		{ -9.f, 2.f, 8.f, -1.f, 2.f },
		{ 4.f, 3.f, 6.f, 7.f, -5.f },
		{ -2.f, 6.f, 7.f, 1.f, 0.f },
		{ 4.f, 2.f, 3.f, 3.f, -9.f }
	};
	EXPECT_EQ(ansF, matF);
}

TEST(MatrixCSR, SetterOperators)
{
	const MatrixCSRD matA = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };
	const MatrixCSRD matB = { { 3.0, -1.0, 2.0 }, { 9.0, 2.0, 8.0 } };

	MatrixCSRD mat = matA;
	mat += 3.5;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i + 4.5, mat.NonZero(i));
	}

	mat = matA;
	mat += matB;
	MatrixCSRD ans = { { 4.0, 1.0, 5.0 }, { 13.0, 7.0, 14.0 } };
	EXPECT_EQ(ans, mat);

	mat = matA;
	mat -= 1.5;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i - 0.5, mat.NonZero(i)) << i;
	}

	mat = matA;
	mat -= matB;
	ans = { { -2.0, 3.0, 1.0 }, { -5.0, 3.0, -2.0 } };
	EXPECT_EQ(ans, mat);

	mat = matA;
	mat *= 2.0;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(2.0 * (i + 1.0), mat.NonZero(i));
	}

	MatrixCSRD matA2;
	MatrixCSRD matC2;
	int cnt = 0;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			matA2.SetElement(i, j, cnt + 1.0);
			matC2.SetElement(i, j, 25.0 - cnt);
			++cnt;
		}
	}
	matA2 *= matC2;

	const MatrixCSRD ans2 =
	{
		{ 175.0, 160.0, 145.0, 130.0, 115.0 },
		{ 550.0, 510.0, 470.0, 430.0, 390.0 },
		{ 925.0, 860.0, 795.0, 730.0, 665.0 },
		{ 1300.0, 1210.0, 1120.0, 1030.0, 940.0 },
		{ 1675.0, 1560.0, 1445.0, 1330.0, 1215.0 }
	};
	EXPECT_EQ(ans2, matA2);

	mat = matA;
	mat /= 2.0;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ((i + 1.0) / 2.0, mat.NonZero(i));
	}
}

TEST(MatrixCSR, GetterOperators)
{
	const MatrixMxND matDense =
	{
		{ 1.0, 0.0, 0.0, -3.0 },
		{ 0.0, 3.0, -5.0, 1.0 },
		{ -4.0, 0.0, 1.0, 5.0 }
	};
	const MatrixCSRD matSparse(matDense, 0.02);
	
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 4; ++j)
		{
			EXPECT_EQ(matDense(i, j), matSparse(i, j));
		}
	}
}

TEST(MatrixCSR, Builders)
{
	const MatrixCSRD matIden = MatrixCSRD::MakeIdentity(5);
	
	for (size_t i = 0; i < 5; ++i)
	{
		for (size_t j = 0; j < 5; ++j)
		{
			if (i == j)
			{
				EXPECT_EQ(1.0, matIden(i, j));
			}
			else
			{
				EXPECT_EQ(0.0, matIden(i, j));
			}
		}
	}
}

TEST(MatrixCSR, OperatorOverloadings)
{
	const MatrixCSRD matA = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };
	const MatrixCSRD addResult1 = matA + 3.5;
	
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i + 4.5, addResult1.NonZero(i));
	}

	const MatrixCSRD matC = { { 3.0, -1.0, 2.0 }, { 9.0, 2.0, 8.0 } };
	const MatrixCSRD addResult2 = matA + matC;
	const MatrixCSRD addAns1 = { { 4.0, 1.0, 5.0 }, { 13.0, 7.0, 14.0 } };
	EXPECT_TRUE(addAns1.IsEqual(addResult2));

	const MatrixCSRD matD = { { 3.0, 0.0, 2.0 }, { 0.0, 2.0, 0.0 } };
	const MatrixCSRD addResult3 = matA + matD;
	const MatrixCSRD addAns2 = { { 4.0, 2.0, 5.0 }, { 4.0, 7.0, 6.0 } };
	EXPECT_TRUE(addAns2.IsEqual(addResult3));

	const MatrixCSRD matE = { { 3.0, 0.0, 2.0 }, { 0.0, 0.0, 0.0 } };
	const MatrixCSRD addResult4 = matA + matE;
	const MatrixCSRD addAns3 = { { 4.0, 2.0, 5.0 }, { 4.0, 5.0, 6.0 } };
	EXPECT_TRUE(addAns3.IsEqual(addResult4));

	const MatrixCSRD subResult1 = matA - 1.5;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i - 0.5, subResult1.NonZero(i));
	}

	const MatrixCSRD subResult2 = matA - matC;
	const MatrixCSRD ans2 = { { -2.0, 3.0, 1.0 }, { -5.0, 3.0, -2.0 } };
	EXPECT_TRUE(ans2.IsSimilar(subResult2));

	const MatrixCSRD matB = matA * 2.0;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(2.0 * (i + 1.0), matB.NonZero(i));
	}

	const VectorND vecA = { -1.0, 9.0, 8.0 };
	const VectorND vecB = matA * vecA;
	const VectorND ansV = { 41.0, 89.0 };
	EXPECT_TRUE(ansV.IsEqual(vecB));

	const MatrixCSRD matF = { { 3.0, -1.0 }, { 2.0, 9.0 }, { 2.0, 8.0 } };
	const MatrixCSRD matG = matA * matF;
	const MatrixCSRD ans3 = { { 13.0, 41.0 }, { 34.0, 89.0 } };
	EXPECT_TRUE(ans3.IsEqual(matG));

	const MatrixCSRD matH = matA / 2.0;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ((i + 1.0) / 2.0, matH.NonZero(i));
	}

	const MatrixCSRD matI = 3.5 + matA;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(i + 4.5, matI.NonZero(i));
	}

	const MatrixCSRD matJ = 1.5 - matA;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(0.5 - i, matJ.NonZero(i));
	}

	const MatrixCSRD matM = 2.0 * matA;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(2.0 * (i + 1.0), matM.NonZero(i));
	}

	const MatrixCSRD matP = 2.0 / matA;
	for (size_t i = 0; i < 6; ++i)
	{
		EXPECT_EQ(2.0 / (i + 1.0), matP.NonZero(i));
	}
}