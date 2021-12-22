// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include "doctest_proxy.hpp"

#include <Core/Math/BLAS.hpp>
#include <Core/Matrix/Matrix.hpp>

using namespace CubbyFlow;

TEST_CASE("[BLAS] - Set")
{
    Vector3D vec;

    BLAS<double, Vector3D, Matrix3x3D>::Set(3.14, &vec);

    CHECK_EQ(doctest::Approx(3.14), vec.x);
    CHECK_EQ(doctest::Approx(3.14), vec.y);
    CHECK_EQ(doctest::Approx(3.14), vec.z);

    Vector3D vec2(5.1, 3.7, 8.2);
    BLAS<double, Vector3D, Matrix3x3D>::Set(vec2, &vec);

    CHECK_EQ(doctest::Approx(5.1), vec.x);
    CHECK_EQ(doctest::Approx(3.7), vec.y);
    CHECK_EQ(doctest::Approx(8.2), vec.z);

    Matrix3x3D mat;

    BLAS<double, Vector3D, Matrix3x3D>::Set(0.414, &mat);

    for (int i = 0; i < 9; ++i)
    {
        double elem = mat[i];
        CHECK_EQ(doctest::Approx(0.414), elem);
    }

    Matrix3x3D mat2(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
    BLAS<double, Vector3D, Matrix3x3D>::Set(mat2, &mat);
    for (int i = 0; i < 9; ++i)
    {
        CHECK_EQ(doctest::Approx(static_cast<double>(i + 1)), mat[i]);
    }
}

TEST_CASE("[BLAS] - Dot")
{
    constexpr Vector3D vec(1.0, 2.0, 3.0), vec2(4.0, 5.0, 6.0);
    const double result = BLAS<double, Vector3D, Matrix3x3D>::Dot(vec, vec2);
    CHECK_EQ(doctest::Approx(32.0), result);
}

TEST_CASE("[BLAS] - AXPlusY")
{
    Vector3D result;
    BLAS<double, Vector3D, Matrix3x3D>::AXPlusY(2.5, Vector3D(1, 2, 3),
                                                Vector3D(4, 5, 6), &result);

    CHECK_EQ(doctest::Approx(6.5), result.x);
    CHECK_EQ(doctest::Approx(10.0), result.y);
    CHECK_EQ(doctest::Approx(13.5), result.z);
}

TEST_CASE("[BLAS] - MVM")
{
    constexpr Matrix3x3D mat(1, 2, 3, 4, 5, 6, 7, 8, 9);

    Vector3D result;
    BLAS<double, Vector3D, Matrix3x3D>::MVM(mat, Vector3D(1, 2, 3), &result);

    CHECK_EQ(doctest::Approx(14.0), result.x);
    CHECK_EQ(doctest::Approx(32.0), result.y);
    CHECK_EQ(doctest::Approx(50.0), result.z);
}

TEST_CASE("[BLAS] - Residual")
{
    constexpr Matrix3x3D mat(1, 2, 3, 4, 5, 6, 7, 8, 9);

    Vector3D result;
    BLAS<double, Vector3D, Matrix3x3D>::Residual(mat, Vector3D(1, 2, 3),
                                                 Vector3D(4, 5, 6), &result);

    CHECK_EQ(doctest::Approx(-10.0), result.x);
    CHECK_EQ(doctest::Approx(-27.0), result.y);
    CHECK_EQ(doctest::Approx(-44.0), result.z);
}

TEST_CASE("[BLAS] - L2Norm")
{
    constexpr Vector3D vec(-1.0, 2.0, -3.0);
    const double result = BLAS<double, Vector3D, Matrix3x3D>::L2Norm(vec);
    CHECK_EQ(doctest::Approx(std::sqrt(14.0)), result);
}

TEST_CASE("[BLAS] - LInfNorm")
{
    constexpr Vector3D vec(-1.0, 2.0, -3.0);
    const double result = BLAS<double, Vector3D, Matrix3x3D>::LInfNorm(vec);
    CHECK_EQ(doctest::Approx(3.0), result);
}