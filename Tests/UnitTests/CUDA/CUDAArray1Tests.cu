// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include "gtest/gtest.h"

#include <Core/Array/ArrayView.hpp>
#include <Core/CUDA/CUDAArray.hpp>
#include <Core/CUDA/CUDAArrayView.hpp>

using namespace CubbyFlow;

TEST(CUDAArray1, Constructors)
{
    CUDAArray1<float> arr0;
    EXPECT_EQ(0u, arr0.Length());

    CUDAArray1<float> arr1(9, 1.5f);
    EXPECT_EQ(9u, arr1.Length());
    for (size_t i = 0; i < arr1.Length(); ++i)
    {
        EXPECT_FLOAT_EQ(1.5f, arr1[i]);
    }

    Array1<float> arr9({ 1.0f, 2.0f, 3.0f });
    ArrayView1<float> view9(arr9);
    CUDAArray1<float> arr10(view9);
    EXPECT_EQ(3u, arr10.Length());
    for (size_t i = 0; i < arr10.Length(); ++i)
    {
        EXPECT_FLOAT_EQ(1.0f + i, arr10[i]);
    }
    CUDAArray1<float> arr11(arr9.View());
    EXPECT_EQ(3u, arr11.Length());
    for (size_t i = 0; i < arr11.Length(); ++i)
    {
        EXPECT_FLOAT_EQ(1.0f + i, arr11[i]);
    }

    CUDAArray1<float> arr2(arr1.View());
    EXPECT_EQ(9u, arr2.Length());
    for (size_t i = 0; i < arr2.Length(); ++i)
    {
        EXPECT_FLOAT_EQ(1.5f, arr2[i]);
    }

    CUDAArray1<float> arr3({ 1.0f, 2.0f, 3.0f });
    EXPECT_EQ(3u, arr3.Length());
    for (size_t i = 0; i < arr3.Length(); ++i)
    {
        float a = arr3[i];
        EXPECT_FLOAT_EQ(1.0f + i, arr3[i]);
    }

    CUDAArray1<float> arr8(std::vector<float>{ 1.0f, 2.0f, 3.0f });
    EXPECT_EQ(3u, arr8.Length());
    for (size_t i = 0; i < arr8.Length(); ++i)
    {
        EXPECT_FLOAT_EQ(1.0f + i, arr8[i]);
    }

    CUDAArray1<float> arr6(arr8);
    EXPECT_EQ(3u, arr6.Length());
    for (size_t i = 0; i < arr8.Length(); ++i)
    {
        EXPECT_FLOAT_EQ(arr8[i], arr6[i]);
    }

    CUDAArray1<float> arr7 = std::move(arr6);
    EXPECT_EQ(3u, arr7.Length());
    EXPECT_EQ(0u, arr6.Length());
    for (size_t i = 0; i < arr6.Length(); ++i)
    {
        EXPECT_FLOAT_EQ(arr6[i], arr7[i]);
    }
}

TEST(CUDAArray1, CopyFrom)
{
    // Copy from std::vector
    CUDAArray1<float> arr1;
    std::vector<float> vec({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    arr1.CopyFrom(vec);

    EXPECT_EQ(vec.size(), arr1.Length());
    for (size_t i = 0; i < arr1.Length(); ++i)
    {
        EXPECT_FLOAT_EQ(vec[i], arr1[i]);
    }

    // Copy from CPU Array
    CUDAArray1<float> arr2;
    Array1<float> cpuArr({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    arr2.CopyFrom(cpuArr);

    EXPECT_EQ(cpuArr.Length(), arr2.Length());
    for (size_t i = 0; i < arr2.Length(); ++i)
    {
        EXPECT_FLOAT_EQ(cpuArr[i], arr2[i]);
    }

    // Copy from CPU ArrayView
    CUDAArray1<float> arr3;
    ArrayView1<float> cpuArrView = cpuArr.View();
    arr3.CopyFrom(cpuArrView);

    EXPECT_EQ(cpuArrView.Length(), arr3.Length());
    for (size_t i = 0; i < arr3.Length(); ++i)
    {
        EXPECT_FLOAT_EQ(cpuArrView[i], arr3[i]);
    }

    // Copy from CPU ConstArrayView
    CUDAArray1<float> arr4;
    ConstArrayView1<float> constCPUArrView = cpuArr.View();
    arr4.CopyFrom(constCPUArrView);

    EXPECT_EQ(constCPUArrView.Length(), arr4.Length());
    for (size_t i = 0; i < arr4.Length(); ++i)
    {
        EXPECT_FLOAT_EQ(constCPUArrView[i], arr4[i]);
    }

    // Copy from CUDAArray
    CUDAArray1<float> arr5;
    CUDAArray1<float> cudaArr({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    arr5.CopyFrom(cudaArr);

    EXPECT_EQ(cudaArr.Length(), arr5.Length());
    for (size_t i = 0; i < arr5.Length(); ++i)
    {
        EXPECT_FLOAT_EQ(cudaArr[i], arr5[i]);
    }

    // Copy from CUDAArrayView
    CUDAArray1<float> arr6;
    CUDAArrayView1<float> cudaArrView = arr6.View();
    arr6.CopyFrom(cudaArrView);

    EXPECT_EQ(cudaArrView.Length(), arr6.Length());
    for (size_t i = 0; i < arr6.Length(); ++i)
    {
        EXPECT_FLOAT_EQ(cudaArrView[i], arr6[i]);
    }

    // Copy from ConstCUDAArrayView
    CUDAArray1<float> arr7;
    ConstCUDAArrayView1<float> constCUDAArrView = arr7.View();
    arr7.CopyFrom(constCUDAArrView);

    EXPECT_EQ(constCUDAArrView.Length(), arr7.Length());
    for (size_t i = 0; i < arr7.Length(); ++i)
    {
        EXPECT_FLOAT_EQ(constCUDAArrView[i], arr7[i]);
    }
}

TEST(CUDAArray1, Append)
{
    // CUDA + Scalar
    {
        CUDAArray1<float> arr1({ 1.0f, 2.0f, 3.0f });
        arr1.Append(4.0f);
        arr1.Append(5.0f);
        EXPECT_EQ(5u, arr1.Length());
        for (size_t i = 0; i < arr1.Length(); ++i)
        {
            float a = arr1[i];
            EXPECT_FLOAT_EQ(1.0f + i, arr1[i]);
        }
    }

    // CUDA + CUDA
    {
        CUDAArray1<float> arr1({ 1.0f, 2.0f, 3.0f });
        CUDAArray1<float> arr2({ 4.0f, 5.0f });
        arr1.Append(arr2);
        EXPECT_EQ(5u, arr1.Length());
        for (size_t i = 0; i < arr1.Length(); ++i)
        {
            float a = arr1[i];
            EXPECT_FLOAT_EQ(1.0f + i, arr1[i]);
        }
    }

    // CUDA + CPU
    {
        CUDAArray1<float> arr1({ 1.0f, 2.0f, 3.0f });
        Array1<float> arr2({ 4.0f, 5.0f });
        arr1.Append(arr2);
        EXPECT_EQ(5u, arr1.Length());
        for (size_t i = 0; i < arr1.Length(); ++i)
        {
            float a = arr1[i];
            EXPECT_FLOAT_EQ(1.0f + i, arr1[i]);
        }
    }
}

TEST(CUDAArray1, View)
{
    CUDAArray1<float> arr(15, 3.14f);
    CUDAArrayView1<float> view = arr.View();
    EXPECT_EQ(15u, view.Length());
    EXPECT_EQ(arr.data(), view.data());
    for (size_t i = 0; i < 15; ++i)
    {
        float val = arr[i];
        EXPECT_FLOAT_EQ(3.14f, val);
    }
}