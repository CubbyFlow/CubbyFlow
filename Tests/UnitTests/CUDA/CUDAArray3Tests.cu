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

TEST(CUDAArray3, Constructors)
{
    {
        CUDAArray3<float> arr;
        EXPECT_EQ(0u, arr.Width());
        EXPECT_EQ(0u, arr.Height());
        EXPECT_EQ(0u, arr.Depth());
    }
    {
        CUDAArray3<float> arr(CUDAStdArray<size_t, 3>(3, 7, 4));
        EXPECT_EQ(3u, arr.Width());
        EXPECT_EQ(7u, arr.Height());
        EXPECT_EQ(4u, arr.Depth());
        for (size_t i = 0; i < 84; ++i)
        {
            EXPECT_FLOAT_EQ(0.f, arr[i]);
        }
    }
    {
        CUDAArray3<float> arr(CUDAStdArray<size_t, 3>(1, 9, 5), 1.5f);
        EXPECT_EQ(1u, arr.Width());
        EXPECT_EQ(9u, arr.Height());
        EXPECT_EQ(5u, arr.Depth());
        for (size_t i = 0; i < 45; ++i)
        {
            EXPECT_FLOAT_EQ(1.5f, arr[i]);
        }
    }
    {
        CUDAArray3<float> arr(5, 2, 8);
        EXPECT_EQ(5u, arr.Width());
        EXPECT_EQ(2u, arr.Height());
        EXPECT_EQ(8u, arr.Depth());
        for (size_t i = 0; i < 80; ++i)
        {
            EXPECT_FLOAT_EQ(0.f, arr[i]);
        }
    }
    {
        CUDAArray3<float> arr(3, 4, 2, 7.f);
        EXPECT_EQ(3u, arr.Width());
        EXPECT_EQ(4u, arr.Height());
        EXPECT_EQ(2u, arr.Depth());
        for (size_t i = 0; i < 24; ++i)
        {
            EXPECT_FLOAT_EQ(7.f, arr[i]);
        }
    }
    {
        CUDAArray3<float> arr({ { { 1.f, 2.f, 3.f, 4.f },
                                  { 5.f, 6.f, 7.f, 8.f },
                                  { 9.f, 10.f, 11.f, 12.f } },
                                { { 13.f, 14.f, 15.f, 16.f },
                                  { 17.f, 18.f, 19.f, 20.f },
                                  { 21.f, 22.f, 23.f, 24.f } } });

        EXPECT_EQ(4u, arr.Width());
        EXPECT_EQ(3u, arr.Height());
        EXPECT_EQ(2u, arr.Depth());
        for (size_t i = 0; i < 24; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr[i]);
        }
    }
    {
        CUDAArray3<float> arr({ { { 1.f, 2.f, 3.f, 4.f },
                                  { 5.f, 6.f, 7.f, 8.f },
                                  { 9.f, 10.f, 11.f, 12.f } },
                                { { 13.f, 14.f, 15.f, 16.f },
                                  { 17.f, 18.f, 19.f, 20.f },
                                  { 21.f, 22.f, 23.f, 24.f } } });
        CUDAArray3<float> arr2(arr);

        EXPECT_EQ(4u, arr2.Width());
        EXPECT_EQ(3u, arr2.Height());
        EXPECT_EQ(2u, arr2.Depth());
        for (size_t i = 0; i < 24; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
        }
    }
    {
        CUDAArray3<float> arr({ { { 1.f, 2.f, 3.f, 4.f },
                                  { 5.f, 6.f, 7.f, 8.f },
                                  { 9.f, 10.f, 11.f, 12.f } },
                                { { 13.f, 14.f, 15.f, 16.f },
                                  { 17.f, 18.f, 19.f, 20.f },
                                  { 21.f, 22.f, 23.f, 24.f } } });
        CUDAArrayView3<float> arrVew(arr.data(), arr.Size());
        EXPECT_EQ(4u, arrVew.Width());
        EXPECT_EQ(3u, arrVew.Height());
        EXPECT_EQ(2u, arrVew.Depth());
        for (size_t i = 0; i < 24; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arrVew[i]);
        }
    }
}

TEST(CUDAArray3, Clear)
{
    CUDAArray3<float> arr({ { { 1.f, 2.f, 3.f, 4.f },
                              { 5.f, 6.f, 7.f, 8.f },
                              { 9.f, 10.f, 11.f, 12.f } },
                            { { 13.f, 14.f, 15.f, 16.f },
                              { 17.f, 18.f, 19.f, 20.f },
                              { 21.f, 22.f, 23.f, 24.f } } });

    arr.Clear();
    EXPECT_EQ(0u, arr.Width());
    EXPECT_EQ(0u, arr.Height());
    EXPECT_EQ(0u, arr.Depth());
}

TEST(CUDAArray3, Resize)
{
    {
        CUDAArray3<float> arr;
        arr.Resize(CUDAStdArray<size_t, 3>(2, 9, 5));
        EXPECT_EQ(2u, arr.Width());
        EXPECT_EQ(9u, arr.Height());
        EXPECT_EQ(5u, arr.Depth());
        for (size_t i = 0; i < 90; ++i)
        {
            EXPECT_FLOAT_EQ(0.f, arr[i]);
        }

        arr.Resize(CUDAStdArray<size_t, 3>(8, 13, 7), 4.f);
        EXPECT_EQ(8u, arr.Width());
        EXPECT_EQ(13u, arr.Height());
        EXPECT_EQ(7u, arr.Depth());
        for (size_t k = 0; k < 7; ++k)
        {
            for (size_t j = 0; j < 13; ++j)
            {
                for (size_t i = 0; i < 8; ++i)
                {
                    if (i < 2 && j < 9 && k < 5)
                    {
                        EXPECT_FLOAT_EQ(0.f, arr(i, j, k));
                    }
                    else
                    {
                        EXPECT_FLOAT_EQ(4.f, arr(i, j, k));
                    }
                }
            }
        }
    }
    {
        CUDAArray3<float> arr;
        arr.Resize(7, 6, 3);
        EXPECT_EQ(7u, arr.Width());
        EXPECT_EQ(6u, arr.Height());
        EXPECT_EQ(3u, arr.Depth());
        for (size_t i = 0; i < 126; ++i)
        {
            EXPECT_FLOAT_EQ(0.f, arr[i]);
        }

        arr.Resize(1, 9, 4, 3.f);
        EXPECT_EQ(1u, arr.Width());
        EXPECT_EQ(9u, arr.Height());
        EXPECT_EQ(4u, arr.Depth());
        for (size_t k = 0; k < 4; ++k)
        {
            for (size_t j = 0; j < 9; ++j)
            {
                for (size_t i = 0; i < 1; ++i)
                {
                    if (j < 6 && k < 3)
                    {
                        EXPECT_FLOAT_EQ(0.f, arr(i, j, k));
                    }
                    else
                    {
                        EXPECT_FLOAT_EQ(3.f, arr(i, j, k));
                    }
                }
            }
        }
    }
}

TEST(CUDAArray3, View)
{
    CUDAArray3<float> arr({ { { 1.f, 2.f, 3.f, 4.f },
                              { 5.f, 6.f, 7.f, 8.f },
                              { 9.f, 10.f, 11.f, 12.f } },
                            { { 13.f, 14.f, 15.f, 16.f },
                              { 17.f, 18.f, 19.f, 20.f },
                              { 21.f, 22.f, 23.f, 24.f } } });
    CUDAArrayView3<float> arr2 = arr.View();
    EXPECT_EQ(4u, arr2.Width());
    EXPECT_EQ(3u, arr2.Height());
    for (size_t i = 0; i < 12; ++i)
    {
        EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
    }
}