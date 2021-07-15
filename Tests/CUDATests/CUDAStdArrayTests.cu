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

#include <Core/CUDA/CUDAStdArray.hpp>

using namespace CubbyFlow;

TEST_CASE("[CUDAStdArray] - Constructors")
{
    {
        CUDAStdArray<int, 3> a;
        CHECK_EQ(a[0], 0);
        CHECK_EQ(a[1], 0);
        CHECK_EQ(a[2], 0);
    }

    {
        CUDAStdArray<int, 3> a(1, 2, 3);
        CHECK_EQ(a[0], 1);
        CHECK_EQ(a[1], 2);
        CHECK_EQ(a[2], 3);
    }

    {
        const std::array<int, 3> a = { 1, 2, 3 };
        CUDAStdArray<int, 3> b(a);
        CHECK_EQ(b[0], 1);
        CHECK_EQ(b[1], 2);
        CHECK_EQ(b[2], 3);
    }

    {
        Vector<int, 3> a(1, 2, 3);
        CUDAStdArray<int, 3> b(a);
        CHECK_EQ(b[0], 1);
        CHECK_EQ(b[1], 2);
        CHECK_EQ(b[2], 3);
    }
}

TEST_CASE("[CUDAStdArray] - Fill")
{
    CUDAStdArray<int, 3> a;
    a.Fill(5);
    CHECK_EQ(a[0], 5);
    CHECK_EQ(a[1], 5);
    CHECK_EQ(a[2], 5);
}
