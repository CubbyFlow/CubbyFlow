#include "gtest/gtest.h"

#include <Core/Matrix/Matrix.hpp>

using namespace CubbyFlow;

TEST(Matrix4x4, Constructors)
{
    Matrix4x4D mat;

    // Deprecated behavior: default ctor will Make zero matrix, not an identity.
    //    for (int i = 0; i < 4; ++i) {
    //        for (int j = 0; j < 4; ++j) {
    //            if (i == j) {
    //                EXPECT_DOUBLE_EQ(1.0, mat(i, j));
    //            } else {
    //                EXPECT_DOUBLE_EQ(0.0, mat(i, j));
    //            }
    //        }
    //    }
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            EXPECT_DOUBLE_EQ(0.0, mat(i, j));
        }
    }

    Matrix4x4D mat2(3.1);
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_DOUBLE_EQ(3.1, mat2[i]);
    }

    Matrix4x4D mat3(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat3[i]);
    }

    Matrix4x4D mat4({ { 1, 2, 3, 4 },
                      { 5, 6, 7, 8 },
                      { 9, 10, 11, 12 },
                      { 13, 14, 15, 16 } });
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat4[i]);
    }

    Matrix4x4D mat5(mat4);
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat5[i]);
    }

    double arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    Matrix4x4D mat6(arr);
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat6[i]);
    }
}

TEST(Matrix4x4, SetMethods)
{
    Matrix4x4D mat;

    mat.Fill(3.1);
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_DOUBLE_EQ(3.1, mat[i]);
    }

    mat.Fill(3.1);
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_DOUBLE_EQ(3.1, mat[i]);
    }

    mat.Fill([](size_t i) -> double { return i + 1.0; });
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
    }

    mat.Fill([](size_t i, size_t j) -> double {
        return static_cast<double>(i + j);
    });
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            EXPECT_DOUBLE_EQ(static_cast<double>(i + j), mat(i, j));
        }
    }

    mat.SetDiagonal(3.1);
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (i == j)
            {
                EXPECT_DOUBLE_EQ(3.1, mat(i, j));
            }
        }
    }

    mat.SetOffDiagonal(4.2);
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (i != j)
            {
                EXPECT_DOUBLE_EQ(4.2, mat(i, j));
            }
        }
    }

    mat.SetRow(0, Vector4D(1, 2, 3, 4));
    mat.SetRow(1, Vector4D(5, 6, 7, 8));
    mat.SetRow(2, Vector4D(9, 10, 11, 12));
    mat.SetRow(3, Vector4D(13, 14, 15, 16));
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
    }

    mat.SetColumn(0, Vector4D(1, 5, 9, 13));
    mat.SetColumn(1, Vector4D(2, 6, 10, 14));
    mat.SetColumn(2, Vector4D(3, 7, 11, 15));
    mat.SetColumn(3, Vector4D(4, 8, 12, 16));
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_DOUBLE_EQ(static_cast<double>(i + 1), mat[i]);
    }
}

TEST(Matrix4x4, BasicGetters)
{
    Matrix4x4D mat(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16),
        mat2(1.01, 2.01, 2.99, 4.0, 4.99, 6.001, 7.0003, 8.0, 8.99, 10.01, 11,
             11.99, 13.01, 14.001, 14.999, 16);

    EXPECT_TRUE(mat.IsSimilar(mat2, 0.02));
    EXPECT_FALSE(mat.IsSimilar(mat2, 0.001));

    EXPECT_TRUE(mat.IsSquare());

    EXPECT_EQ(4u, mat.GetRows());
    EXPECT_EQ(4u, mat.GetCols());
}

TEST(Matrix4x4, Modifiers)
{
    Matrix4x4D mat(-16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6, 5, -6, 3, -2,
                   1);

    mat.Transpose();
    EXPECT_TRUE(mat.IsSimilar(Matrix4x4D({ { -16, -12, -8, -6 },
                                           { 15, 11, 7, 3 },
                                           { -14, -10, -6, -2 },
                                           { 13, 9, 5, 1 } })));

    mat = { -16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6, 6, -6, 3, -2, 2 };
    mat.Invert();
    EXPECT_TRUE(mat.IsSimilar(
        Matrix4x4D({ { -1.0 / 2.0, 1.0 / 2.0, 1.0 / 2.0, -1.0 / 2.0 },
                     { -5.0 / 2.0, 5.0 / 2.0, 2.0, -1.0 },
                     { -5.0 / 4.0, 1.0 / 4.0, 5.0 / 2.0, -1.0 / 2.0 },
                     { 1.0, -2.0, 1.0, 0.0 } })));
}

TEST(Matrix4x4, ComplexGetters)
{
    Matrix4x4D mat(-16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6, 5, -4, 3, -2,
                   1),
        mat2;

    EXPECT_DOUBLE_EQ(-8.0, mat.Sum());

    EXPECT_DOUBLE_EQ(-0.5, mat.Avg());

    EXPECT_DOUBLE_EQ(-16.0, mat.Min());

    EXPECT_DOUBLE_EQ(15.0, mat.Max());

    EXPECT_DOUBLE_EQ(1.0, mat.AbsMin());

    EXPECT_DOUBLE_EQ(-16.0, mat.AbsMax());

    EXPECT_DOUBLE_EQ(-10.0, mat.Trace());

    EXPECT_DOUBLE_EQ(0.0, mat.Determinant());

    mat2 = mat.Diagonal();
    EXPECT_TRUE(mat2.IsSimilar(
        Matrix4x4D(-16, 0, 0, 0, 0, 11, 0, 0, 0, 0, -6, 0, 0, 0, 0, 1)));

    mat2 = mat.OffDiagonal();
    EXPECT_TRUE(mat2.IsSimilar(
        Matrix4x4D(0, 15, -14, 13, -12, 0, -10, 9, -8, 7, 0, 5, -4, 3, -2, 0)));

    mat2 = mat.StrictLowerTri();
    EXPECT_TRUE(mat2.IsSimilar(
        Matrix4x4D(0, 0, 0, 0, -12, 0, 0, 0, -8, 7, 0, 0, -4, 3, -2, 0)));

    mat2 = mat.StrictUpperTri();
    EXPECT_TRUE(mat2.IsSimilar(
        Matrix4x4D(0, 15, -14, 13, 0, 0, -10, 9, 0, 0, 0, 5, 0, 0, 0, 0)));

    mat2 = mat.LowerTri();
    EXPECT_TRUE(mat2.IsSimilar(
        Matrix4x4D(-16, 0, 0, 0, -12, 11, 0, 0, -8, 7, -6, 0, -4, 3, -2, 1)));

    mat2 = mat.UpperTri();
    EXPECT_TRUE(mat2.IsSimilar(
        Matrix4x4D(-16, 15, -14, 13, 0, 11, -10, 9, 0, 0, -6, 5, 0, 0, 0, 1)));

    mat2 = mat.Transposed();
    EXPECT_TRUE(mat2.IsSimilar(Matrix4x4D({ { -16, -12, -8, -4 },
                                            { 15, 11, 7, 3 },
                                            { -14, -10, -6, -2 },
                                            { 13, 9, 5, 1 } })));

    mat = { -16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6, 6, -6, 3, -2, 2 };
    mat2 = mat.Inverse();
    EXPECT_TRUE(mat2.IsSimilar(
        Matrix4x4D({ { -1.0 / 2.0, 1.0 / 2.0, 1.0 / 2.0, -1.0 / 2.0 },
                     { -5.0 / 2.0, 5.0 / 2.0, 2.0, -1.0 },
                     { -5.0 / 4.0, 1.0 / 4.0, 5.0 / 2.0, -1.0 / 2.0 },
                     { 1.0, -2.0, 1.0, 0.0 } })));

    mat = { -16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6, 5, -6, 3, -2, 1 };
    Matrix4x4F mat3 = mat.CastTo<float>();
    EXPECT_TRUE(mat3.IsSimilar(Matrix4x4F(-16, 15, -14, 13, -12, 11, -10, 9, -8,
                                          7, -6, 5, -6, 3, -2, 1)));
}

TEST(Matrix4x4, SetterOperatorOverloadings)
{
    Matrix4x4D mat(-16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6, 5, -6, 3, -2,
                   1),
        mat2;

    mat2 = mat;
    EXPECT_TRUE(mat2.IsSimilar(Matrix4x4D(-16, 15, -14, 13, -12, 11, -10, 9, -8,
                                          7, -6, 5, -6, 3, -2, 1)));

    mat += 2.0;
    EXPECT_TRUE(mat.IsSimilar(Matrix4x4D({ { -14, 17, -12, 15 },
                                           { -10, 13, -8, 11 },
                                           { -6, 9, -4, 7 },
                                           { -4, 5, 0, 3 } })));

    mat = { -16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6, 5, -6, 3, -2, 1 };
    mat += Matrix4x4D(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    EXPECT_TRUE(mat.IsSimilar(Matrix4x4D({ { -15, 17, -11, 17 },
                                           { -7, 17, -3, 17 },
                                           { 1, 17, 5, 17 },
                                           { 7, 17, 13, 17 } })));

    mat = { -16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6, 5, -6, 3, -2, 1 };
    mat -= 2.0;
    EXPECT_TRUE(mat.IsSimilar(Matrix4x4D({ { -18, 13, -16, 11 },
                                           { -14, 9, -12, 7 },
                                           { -10, 5, -8, 3 },
                                           { -8, 1, -4, -1 } })));

    mat = { -16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6, 5, -6, 3, -2, 1 };
    mat -= Matrix4x4D(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    EXPECT_TRUE(mat.IsSimilar(Matrix4x4D({ { -17, 13, -17, 9 },
                                           { -17, 5, -17, 1 },
                                           { -17, -3, -17, -7 },
                                           { -19, -11, -17, -15 } })));

    mat = { -16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6, 5, -6, 3, -2, 1 };
    mat *= 2.0;
    EXPECT_TRUE(mat.IsSimilar(Matrix4x4D({ { -32, 30, -28, 26 },
                                           { -24, 22, -20, 18 },
                                           { -16, 14, -12, 10 },
                                           { -12, 6, -4, 2 } })));

    mat = { -16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6, 5, -6, 3, -2, 1 };
    mat *= Matrix4x4D(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    EXPECT_TRUE(mat.IsSimilar(Matrix4x4D({ { 102, 100, 98, 96 },
                                           { 70, 68, 66, 64 },
                                           { 38, 36, 34, 32 },
                                           { 4, 0, -4, -8 } })));

    mat = { -16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6, 5, -6, 3, -2, 1 };
    mat /= 2.0;
    EXPECT_TRUE(
        mat.IsSimilar(Matrix4x4D({ { -8.0, 15.0 / 2.0, -7.0, 13.0 / 2.0 },
                                   { -6.0, 11.0 / 2.0, -5.0, 9.0 / 2.0 },
                                   { -4.0, 7.0 / 2.0, -3.0, 5.0 / 2.0 },
                                   { -3.0, 3.0 / 2.0, -1.0, 1.0 / 2.0 } })));
}

TEST(Matrix4x4, GetterOperatorOverloadings)
{
    Matrix4x4D mat(-16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6, 5, -4, 3, -2,
                   1);

    double sign = -1.0;
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_DOUBLE_EQ(sign * (16 - i), mat[i]);
        sign *= -1.0;

        mat[i] *= -1.0;
    }

    sign = 1.0;
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_DOUBLE_EQ(sign * (16 - i), mat[i]);
        sign *= -1.0;
    }

    sign = 1.0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            double ans = sign * (16 - (4 * i + j));
            EXPECT_DOUBLE_EQ(ans, mat(i, j));
            sign *= -1.0;
        }
    }

    mat = { -16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6, 5, -4, 3, -2, 1 };
    EXPECT_TRUE(mat == Matrix4x4D(-16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6,
                                  5, -4, 3, -2, 1));

    mat = { 16, -15, 14, -13, 12, -11, 10, -9, -8, 7, -6, 5, -4, 3, -2, 1 };
    EXPECT_TRUE(mat != Matrix4x4D(-16, 15, -14, 13, -12, 11, -10, 9, -8, 7, -6,
                                  5, -4, 3, -2, 1));
}

TEST(Matrix4x4, Helpers)
{
    Matrix4x4D mat = Matrix4x4D::MakeZero();
    for (int i = 0; i < 16; ++i)
    {
        EXPECT_EQ(0.0, mat[i]);
    }

    mat = Matrix4x4D::MakeIdentity();
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (i == j)
            {
                EXPECT_DOUBLE_EQ(1.0, mat(i, j));
            }
            else
            {
                EXPECT_DOUBLE_EQ(0.0, mat(i, j));
            }
        }
    }

    mat = Matrix4x4D::MakeScaleMatrix(3.0, -4.0, 2.4, 1.0);
    EXPECT_TRUE(
        mat.IsSimilar(Matrix4x4D(3.0, 0.0, 0.0, 0.0, 0.0, -4.0, 0.0, 0.0, 0.0,
                                 0.0, 2.4, 0.0, 0.0, 0.0, 0.0, 1.0)));

    mat = Matrix4x4D::MakeScaleMatrix(Vector4D(-2.0, 5.0, 3.5, 1.0));
    EXPECT_TRUE(
        mat.IsSimilar(Matrix4x4D(-2.0, 0.0, 0.0, 0.0, 0.0, 5.0, 0.0, 0.0, 0.0,
                                 0.0, 3.5, 0.0, 0.0, 0.0, 0.0, 1.0)));

    mat = Matrix4x4D::MakeRotationMatrix(
        Vector3D(-1.0 / 3.0, 2.0 / 3.0, 2.0 / 3.0), -74.0 / 180.0 * PI_DOUBLE);
    EXPECT_TRUE(mat.IsSimilar(Matrix4x4D(0.36, 0.48, -0.8, 0, -0.8, 0.60, 0.0,
                                         0, 0.48, 0.64, 0.6, 0, 0, 0, 0, 1),
                              0.05));

    mat = Matrix4x4D::MakeTranslationMatrix(Vector3D(-2.0, 5.0, 3.5));
    EXPECT_TRUE(
        mat.IsSimilar(Matrix4x4D(1.0, 0.0, 0.0, -2.0, 0.0, 1.0, 0.0, 5.0, 0.0,
                                 0.0, 1.0, 3.5, 0.0, 0.0, 0.0, 1.0)));
}