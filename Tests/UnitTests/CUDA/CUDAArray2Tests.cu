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

TEST(CUDAArray2, Constructors)
{
    {
        CUDAArray2<float> arr;
        EXPECT_EQ(0u, arr.Width());
        EXPECT_EQ(0u, arr.Height());
    }
    {
        CUDAArray2<float> arr(CUDAStdArray<size_t, 2>(3, 7));
        EXPECT_EQ(3u, arr.Width());
        EXPECT_EQ(7u, arr.Height());
        for (size_t i = 0; i < 21; ++i)
        {
            EXPECT_FLOAT_EQ(0.f, arr[i]);
        }
    }
    {
        CUDAArray2<float> arr(CUDAStdArray<size_t, 2>(1, 9), 1.5f);
        EXPECT_EQ(1u, arr.Width());
        EXPECT_EQ(9u, arr.Height());
        for (size_t i = 0; i < 9; ++i)
        {
            EXPECT_FLOAT_EQ(1.5f, arr[i]);
        }
    }
    {
        CUDAArray2<float> arr(5, 2);
        EXPECT_EQ(5u, arr.Width());
        EXPECT_EQ(2u, arr.Height());
        for (size_t i = 0; i < 10; ++i)
        {
            EXPECT_FLOAT_EQ(0.f, arr[i]);
        }
    }
    {
        CUDAArray2<float> arr(3, 4, 7.f);
        EXPECT_EQ(3u, arr.Width());
        EXPECT_EQ(4u, arr.Height());
        for (size_t i = 0; i < 12; ++i)
        {
            EXPECT_FLOAT_EQ(7.f, arr[i]);
        }
    }
    {
        CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                                { 5.f, 6.f, 7.f, 8.f },
                                { 9.f, 10.f, 11.f, 12.f } });
        EXPECT_EQ(4u, arr.Width());
        EXPECT_EQ(3u, arr.Height());
        for (size_t i = 0; i < 12; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr[i]);
        }
    }
    {
        Array2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                            { 5.f, 6.f, 7.f, 8.f },
                            { 9.f, 10.f, 11.f, 12.f } });
        CUDAArray2<float> arr2(arr);
        EXPECT_EQ(4u, arr2.Width());
        EXPECT_EQ(3u, arr2.Height());
        for (size_t i = 0; i < 12; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
        }
    }
    {
        CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                                { 5.f, 6.f, 7.f, 8.f },
                                { 9.f, 10.f, 11.f, 12.f } });
        CUDAArray2<float> arr2(arr);
        EXPECT_EQ(4u, arr2.Width());
        EXPECT_EQ(3u, arr2.Height());
        for (size_t i = 0; i < 12; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
        }
    }
    {
        CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                                { 5.f, 6.f, 7.f, 8.f },
                                { 9.f, 10.f, 11.f, 12.f } });
        CUDAArrayView2<float> arrVew(arr.data(), arr.Size());
        EXPECT_EQ(4u, arrVew.Width());
        EXPECT_EQ(3u, arrVew.Height());
        for (size_t i = 0; i < 12; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arrVew[i]);
        }
    }
}

TEST(CUDAArray2, At)
{
    {
        float values[12] = { 0.f, 1.f, 2.f, 3.f, 4.f,  5.f,
                             6.f, 7.f, 8.f, 9.f, 10.f, 11.f };
        CUDAArray2<float> arr(4, 3);
        for (size_t i = 0; i < 12; ++i)
        {
            arr[i] = values[i];
        }

        // Test row-major
        EXPECT_FLOAT_EQ(0.f, arr(0, 0));
        EXPECT_FLOAT_EQ(1.f, arr(1, 0));
        EXPECT_FLOAT_EQ(2.f, arr(2, 0));
        EXPECT_FLOAT_EQ(3.f, arr(3, 0));
        EXPECT_FLOAT_EQ(4.f, arr(0, 1));
        EXPECT_FLOAT_EQ(5.f, arr(1, 1));
        EXPECT_FLOAT_EQ(6.f, arr(2, 1));
        EXPECT_FLOAT_EQ(7.f, arr(3, 1));
        EXPECT_FLOAT_EQ(8.f, arr(0, 2));
        EXPECT_FLOAT_EQ(9.f, arr(1, 2));
        EXPECT_FLOAT_EQ(10.f, arr(2, 2));
        EXPECT_FLOAT_EQ(11.f, arr(3, 2));
    }
}

TEST(CUDAArray2, CopyFrom)
{
    // From Array
    {
        Array2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                            { 5.f, 6.f, 7.f, 8.f },
                            { 9.f, 10.f, 11.f, 12.f } });
        CUDAArray2<float> arr2(2, 5);

        arr2.CopyFrom(arr);
        EXPECT_EQ(4u, arr2.Width());
        EXPECT_EQ(3u, arr2.Height());
        for (size_t i = 0; i < 12; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
        }
    }

    // From ArrayView
    {
        Array2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                            { 5.f, 6.f, 7.f, 8.f },
                            { 9.f, 10.f, 11.f, 12.f } });
        CUDAArray2<float> arr2(2, 5);

        arr2.CopyFrom(arr.View());
        EXPECT_EQ(4u, arr2.Width());
        EXPECT_EQ(3u, arr2.Height());
        for (size_t i = 0; i < 12; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
        }
    }

    // From CUDAArray
    {
        CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                                { 5.f, 6.f, 7.f, 8.f },
                                { 9.f, 10.f, 11.f, 12.f } });
        CUDAArray2<float> arr2(2, 5);

        arr2.CopyFrom(arr);
        EXPECT_EQ(4u, arr2.Width());
        EXPECT_EQ(3u, arr2.Height());
        for (size_t i = 0; i < 12; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
        }
    }

    // From CUDAArrayView
    {
        CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                                { 5.f, 6.f, 7.f, 8.f },
                                { 9.f, 10.f, 11.f, 12.f } });
        CUDAArray2<float> arr2(2, 5);

        arr2.CopyFrom(arr.View());
        EXPECT_EQ(4u, arr2.Width());
        EXPECT_EQ(3u, arr2.Height());
        for (size_t i = 0; i < 12; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
        }
    }
}

TEST(CUDAArray2, CopyTo)
{
    // To Array
    {
        CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                                { 5.f, 6.f, 7.f, 8.f },
                                { 9.f, 10.f, 11.f, 12.f } });
        Array2<float> arr2(2, 5);

        arr.CopyTo(arr2);
        EXPECT_EQ(4u, arr2.Width());
        EXPECT_EQ(3u, arr2.Height());
        for (size_t i = 0; i < 12; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
        }
    }

    // To ArrayView
    {
        CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                                { 5.f, 6.f, 7.f, 8.f },
                                { 9.f, 10.f, 11.f, 12.f } });
        Array2<float> arr2(4, 3);
        ArrayView2<float> arrView2 = arr2.View();

        arr.CopyTo(arrView2);
        EXPECT_EQ(4u, arr2.Width());
        EXPECT_EQ(3u, arr2.Height());
        for (size_t i = 0; i < 12; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
        }
    }

    // From CUDAArray
    {
        CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                                { 5.f, 6.f, 7.f, 8.f },
                                { 9.f, 10.f, 11.f, 12.f } });
        CUDAArray2<float> arr2(2, 5);

        arr.CopyTo(arr2);
        EXPECT_EQ(4u, arr2.Width());
        EXPECT_EQ(3u, arr2.Height());
        for (size_t i = 0; i < 12; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
        }
    }

    // From CUDAArrayView
    {
        CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                                { 5.f, 6.f, 7.f, 8.f },
                                { 9.f, 10.f, 11.f, 12.f } });
        CUDAArray2<float> arr2(4, 3);
        CUDAArrayView2<float> arrView2 = arr2.View();

        arr.CopyTo(arrView2);
        EXPECT_EQ(4u, arr2.Width());
        EXPECT_EQ(3u, arr2.Height());
        for (size_t i = 0; i < 12; ++i)
        {
            EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
        }
    }
}

TEST(CUDAArray2, Fill)
{
    CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                            { 5.f, 6.f, 7.f, 8.f },
                            { 9.f, 10.f, 11.f, 12.f } });

    arr.Fill(42.0f);
    EXPECT_EQ(4u, arr.Width());
    EXPECT_EQ(3u, arr.Height());
    for (size_t i = 0; i < 12; ++i)
    {
        EXPECT_FLOAT_EQ(42.0f, arr[i]);
    }
}

TEST(CUDAArray2, Resize)
{
    {
        CUDAArray2<float> arr;
        arr.Resize(CUDAStdArray<size_t, 2>(2, 9));
        EXPECT_EQ(2u, arr.Width());
        EXPECT_EQ(9u, arr.Height());
        for (size_t i = 0; i < 18; ++i)
        {
            EXPECT_FLOAT_EQ(0.f, arr[i]);
        }

        arr.Resize(CUDAStdArray<size_t, 2>(8, 13), 4.f);
        cudaDeviceSynchronize();
        EXPECT_EQ(8u, arr.Width());
        EXPECT_EQ(13u, arr.Height());
        for (size_t i = 0; i < 8; ++i)
        {
            for (size_t j = 0; j < 13; ++j)
            {
                if (i < 2 && j < 9)
                {
                    EXPECT_FLOAT_EQ(0.f, arr(i, j));
                }
                else
                {
                    EXPECT_FLOAT_EQ(4.f, arr(i, j));
                }
            }
        }
    }
    {
        CUDAArray2<float> arr;
        arr.Resize(7, 6);
        EXPECT_EQ(7u, arr.Width());
        EXPECT_EQ(6u, arr.Height());
        for (size_t i = 0; i < 42; ++i)
        {
            EXPECT_FLOAT_EQ(0.f, arr[i]);
        }

        arr.Resize(1, 9, 3.f);
        EXPECT_EQ(1u, arr.Width());
        EXPECT_EQ(9u, arr.Height());
        for (size_t i = 0; i < 1; ++i)
        {
            for (size_t j = 0; j < 9; ++j)
            {
                if (j < 6)
                {
                    EXPECT_FLOAT_EQ(0.f, arr(i, j));
                }
                else
                {
                    EXPECT_FLOAT_EQ(3.f, arr(i, j));
                }
            }
        }
    }
}

TEST(CUDAArray2, Clear)
{
    CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                            { 5.f, 6.f, 7.f, 8.f },
                            { 9.f, 10.f, 11.f, 12.f } });

    arr.Clear();
    EXPECT_EQ(0u, arr.Width());
    EXPECT_EQ(0u, arr.Height());
}

TEST(CUDAArray2, Swap)
{
    CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                            { 5.f, 6.f, 7.f, 8.f },
                            { 9.f, 10.f, 11.f, 12.f } });
    CUDAArray2<float> arr2(2, 5, 42.f);

    arr.Swap(arr2);

    EXPECT_EQ(2u, arr.Width());
    EXPECT_EQ(5u, arr.Height());
    for (size_t i = 0; i < 10; ++i)
    {
        EXPECT_FLOAT_EQ(42.0f, arr[i]);
    }

    EXPECT_EQ(4u, arr2.Width());
    EXPECT_EQ(3u, arr2.Height());
    for (size_t i = 0; i < 12; ++i)
    {
        EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
    }
}

TEST(CUDAArray2, View)
{
    CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                            { 5.f, 6.f, 7.f, 8.f },
                            { 9.f, 10.f, 11.f, 12.f } });

    auto view = arr.View();

    EXPECT_EQ(4u, view.Width());
    EXPECT_EQ(3u, view.Height());
    for (size_t i = 0; i < 12; ++i)
    {
        EXPECT_FLOAT_EQ((float)i + 1.f, view[i]);
    }
    for (size_t j = 0; j < 3; ++j)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            EXPECT_FLOAT_EQ(arr(i, j), view(i, j));
        }
    }

    const auto& arrRef = arr;
    auto constView = arrRef.View();

    for (size_t i = 0; i < 12; ++i)
    {
        EXPECT_FLOAT_EQ((float)i + 1.f, constView[i]);
    }
    for (size_t j = 0; j < 3; ++j)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            EXPECT_FLOAT_EQ(arr(i, j), constView(i, j));
        }
    }

    for (size_t j = 0; j < 3; ++j)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            view(i, j) = float(i + 4 * j);
        }
    }

    for (size_t j = 0; j < 3; ++j)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            EXPECT_FLOAT_EQ(float(i + 4 * j), arr(i, j));
            EXPECT_FLOAT_EQ(float(i + 4 * j), constView(i, j));
        }
    }
}

TEST(CUDAArray2, AssignmentOperator)
{
    CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                            { 5.f, 6.f, 7.f, 8.f },
                            { 9.f, 10.f, 11.f, 12.f } });
    CUDAArray2<float> arr2(2, 5, 42.f);

    arr2 = arr;

    EXPECT_EQ(4u, arr.Width());
    EXPECT_EQ(3u, arr.Height());
    for (size_t i = 0; i < 12; ++i)
    {
        EXPECT_FLOAT_EQ((float)i + 1.f, arr[i]);
    }

    EXPECT_EQ(4u, arr2.Width());
    EXPECT_EQ(3u, arr2.Height());
    for (size_t i = 0; i < 12; ++i)
    {
        EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
    }
}

TEST(CUDAArray2, MoveOperator)
{
    CUDAArray2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                            { 5.f, 6.f, 7.f, 8.f },
                            { 9.f, 10.f, 11.f, 12.f } });
    CUDAArray2<float> arr2(2, 5, 42.f);

    arr2 = std::move(arr);

    EXPECT_EQ(0u, arr.Width());
    EXPECT_EQ(0u, arr.Height());
    EXPECT_EQ(nullptr, arr.data());
    EXPECT_EQ(4u, arr2.Width());
    EXPECT_EQ(3u, arr2.Height());
    for (size_t i = 0; i < 12; ++i)
    {
        EXPECT_FLOAT_EQ((float)i + 1.f, arr2[i]);
    }
}