#include "gtest/gtest.h"

#include <Core/Matrix/Matrix.hpp>

using namespace CubbyFlow;

TEST(Matrix2x2, Constructors)
{
    Matrix2x2D mat;
    // Deprecated behavior: default ctor will Make zero matrix, not an identity.
    // EXPECT_TRUE(mat == Matrix2x2D(1.0, 0.0, 0.0, 1.0));
    EXPECT_TRUE(mat == Matrix2x2D(0.0, 0.0, 0.0, 0.0));

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

    mat.Fill(3.1);
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_DOUBLE_EQ(3.1, mat[i]);
    }

    mat.Fill([](size_t i) -> double { return i + 1.0; });
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
    }

    mat.Fill([](size_t i, size_t j) -> double {
        return static_cast<double>(i + j);
    });
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            EXPECT_DOUBLE_EQ(static_cast<double>(i + j), mat(i, j));
        }
    }

    mat.Fill(0.0);
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

    mat.Fill(0.0);
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

    mat.Fill(0.0);
    mat.SetRow(0, Vector<double, 2>(1.0, 2.0));
    mat.SetRow(1, Vector<double, 2>(3.0, 4.0));
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
    }

    mat.Fill(0.0);
    mat.SetColumn(0, Vector<double, 2>(1.0, 3.0));
    mat.SetColumn(1, Vector<double, 2>(2.0, 4.0));
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

    EXPECT_EQ(2u, mat.GetRows());
    EXPECT_EQ(2u, mat.GetCols());
}

TEST(Matrix2x2, BinaryOperators)
{
    Matrix2x2D mat(-4.0, 3.0, -2.0, 1.0), mat2;
    Vector2D vec;

    mat2 = -mat;
    EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(4.0, -3.0, 2.0, -1.0)));

    mat2 = mat + 2.0;
    EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-2.0, 5.0, 0.0, 3.0)));

    mat2 = mat + Matrix2x2D(1.0, 2.0, 3.0, 4.0);
    EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-3.0, 5.0, 1.0, 5.0)));

    mat2 = mat - 2.0;
    EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-6.0, 1.0, -4.0, -1.0)));

    mat2 = mat - Matrix2x2D(1.0, 2.0, 3.0, 4.0);
    EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-5.0, 1.0, -5.0, -3.0)));

    mat2 = mat * 2.0;
    EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-8.0, 6.0, -4.0, 2.0)));

    vec = mat * Vector2D(1, 2);
    EXPECT_TRUE(vec.IsSimilar(Vector2D(2.0, 0.0)));

    mat2 = mat * Matrix2x2D(1.0, 2.0, 3.0, 4.0);
    EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(5.0, 4.0, 1.0, 0.0)));

    mat2 = mat / 2.0;
    EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-2.0, 1.5, -1.0, 0.5)));

    vec = ((mat * Matrix2x2D(1.0, 2.0, 3.0, 4.0)) + 1.0) * Vector2D(1, 2);
    EXPECT_TRUE(vec.IsSimilar(Vector2D(16.0, 4.0)));
}

TEST(Matrix2x2, Modifiers)
{
    Matrix2x2D mat(-4.0, 3.0, -2.0, 1.0);

    mat.Transpose();
    EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-4.0, -2.0, 3.0, 1.0)));

    mat = { -4.0, 3.0, -2.0, 1.0 };
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

    mat2 = mat.StrictLowerTri();
    EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(0.0, 0.0, -2.0, 0.0)));

    mat2 = mat.StrictUpperTri();
    EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(0.0, 3.0, 0.0, 0.0)));

    mat2 = mat.LowerTri();
    EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-4.0, 0.0, -2.0, 1.0)));

    mat2 = mat.UpperTri();
    EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-4.0, 3.0, 0.0, 1.0)));

    mat2 = mat.Transposed();
    EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-4.0, -2.0, 3.0, 1.0)));

    Matrix<float, 2, 2> mat3 = mat.CastTo<float>();
    EXPECT_TRUE(mat3.IsSimilar(Matrix<float, 2, 2>(-4.f, 3.f, -2.f, 1.f)));
}

TEST(Matrix2x2, SetterOperatorOverloadings)
{
    Matrix2x2D mat(-4.0, 3.0, -2.0, 1.0), mat2;

    mat2 = mat;
    EXPECT_TRUE(mat2.IsSimilar(Matrix2x2D(-4.0, 3.0, -2.0, 1.0)));

    mat += 2.0;
    EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-2.0, 5.0, 0.0, 3.0)));

    mat = { -4.0, 3.0, -2.0, 1.0 };
    mat += Matrix2x2D(1.0, 2.0, 3.0, 4.0);
    EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-3.0, 5.0, 1.0, 5.0)));

    mat = { -4.0, 3.0, -2.0, 1.0 };
    mat -= 2.0;
    EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-6.0, 1.0, -4.0, -1.0)));

    mat = { -4.0, 3.0, -2.0, 1.0 };
    mat -= Matrix2x2D(1.0, 2.0, 3.0, 4.0);
    EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-5.0, 1.0, -5.0, -3.0)));

    mat = { -4.0, 3.0, -2.0, 1.0 };
    mat *= 2.0;
    EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(-8.0, 6.0, -4.0, 2.0)));

    mat = { -4.0, 3.0, -2.0, 1.0 };
    mat *= Matrix2x2D(1.0, 2.0, 3.0, 4.0);
    EXPECT_TRUE(mat.IsSimilar(Matrix2x2D(5.0, 4.0, 1.0, 0.0)));

    mat = { -4.0, 3.0, -2.0, 1.0 };
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

    mat = { -4.0, 3.0, -2.0, 1.0 };
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

    mat = { -4.0, 3.0, -2.0, 1.0 };
    EXPECT_TRUE(mat == Matrix2x2D(-4.0, 3.0, -2.0, 1.0));

    mat = { 4.0, 3.0, 2.0, 1.0 };
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
    EXPECT_TRUE(mat.IsSimilar(
        Matrix2x2D(0.5, -std::sqrt(3.0) / 2.0, std::sqrt(3.0) / 2.0, 0.5)));
}