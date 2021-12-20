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

#include <Core/Array/Array.hpp>
#include <Core/Utils/Serialization.hpp>

using namespace CubbyFlow;

TEST_CASE("[Array2] - Constructors")
{
    Array2<float> arr1;
    CHECK_EQ(0u, arr1.Width());
    CHECK_EQ(0u, arr1.Height());

    Array2<float> arr2(Vector2UZ(3, 7));
    CHECK_EQ(3u, arr2.Width());
    CHECK_EQ(7u, arr2.Height());
    for (size_t i = 0; i < 21; ++i)
    {
        CHECK_EQ(doctest::Approx(0.0), arr2[i]);
    }

    Array2<float> arr3(Vector2UZ(1, 9), 1.5f);
    CHECK_EQ(1u, arr3.Width());
    CHECK_EQ(9u, arr3.Height());
    for (size_t i = 0; i < 9; ++i)
    {
        CHECK_EQ(doctest::Approx(1.5), arr3[i]);
    }

    Array2<float> arr4(5, 2);
    CHECK_EQ(5u, arr4.Width());
    CHECK_EQ(2u, arr4.Height());
    for (size_t i = 0; i < 10; ++i)
    {
        CHECK_EQ(doctest::Approx(0.0), arr4[i]);
    }

    Array2<float> arr5(3, 4, 7.f);
    CHECK_EQ(3u, arr5.Width());
    CHECK_EQ(4u, arr5.Height());
    for (size_t i = 0; i < 12; ++i)
    {
        CHECK_EQ(doctest::Approx(7.0), arr5[i]);
    }

    Array2<float> arr6({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });
    CHECK_EQ(4u, arr6.Width());
    CHECK_EQ(3u, arr6.Height());
    for (size_t i = 0; i < 12; ++i)
    {
        CHECK_EQ(doctest::Approx(static_cast<double>(i) + 1.0), arr6[i]);
    }

    Array2<float> arr7({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });
    Array2<float> arr8(arr7);
    CHECK_EQ(4u, arr8.Width());
    CHECK_EQ(3u, arr8.Height());
    for (size_t i = 0; i < 12; ++i)
    {
        CHECK_EQ(doctest::Approx(static_cast<double>(i) + 1.0), arr8[i]);
    }
}

TEST_CASE("[Array2] - Clear")
{
    Array2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                        { 5.f, 6.f, 7.f, 8.f },
                        { 9.f, 10.f, 11.f, 12.f } });

    arr.Clear();
    CHECK_EQ(0u, arr.Width());
    CHECK_EQ(0u, arr.Height());
}

TEST_CASE("[Array2] - ResizeMethod")
{
    Array2<float> arr1;
    arr1.Resize(Vector2UZ(2, 9));
    CHECK_EQ(2u, arr1.Width());
    CHECK_EQ(9u, arr1.Height());
    for (size_t i = 0; i < 18; ++i)
    {
        CHECK_EQ(doctest::Approx(0.0), arr1[i]);
    }

    arr1.Resize(Vector2UZ(8, 13), 4.f);
    CHECK_EQ(8u, arr1.Width());
    CHECK_EQ(13u, arr1.Height());
    for (size_t i = 0; i < 8; ++i)
    {
        for (size_t j = 0; j < 13; ++j)
        {
            if (i < 2 && j < 9)
            {
                CHECK_EQ(doctest::Approx(0.0), arr1(i, j));
            }
            else
            {
                CHECK_EQ(doctest::Approx(4.0), arr1(i, j));
            }
        }
    }

    Array2<float> arr2;
    arr2.Resize(7, 6);
    CHECK_EQ(7u, arr2.Width());
    CHECK_EQ(6u, arr2.Height());
    for (size_t i = 0; i < 42; ++i)
    {
        CHECK_EQ(doctest::Approx(0.0), arr2[i]);
    }

    arr2.Resize(1, 9, 3.f);
    CHECK_EQ(1u, arr2.Width());
    CHECK_EQ(9u, arr2.Height());
    for (size_t i = 0; i < 1; ++i)
    {
        for (size_t j = 0; j < 9; ++j)
        {
            if (j < 6)
            {
                CHECK_EQ(doctest::Approx(0.0), arr2(i, j));
            }
            else
            {
                CHECK_EQ(doctest::Approx(3.0), arr2(i, j));
            }
        }
    }
}

TEST_CASE("[Array2] - Iterators")
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });

    double cnt = 1.0;
    for (float& elem : arr1)
    {
        CHECK_EQ(doctest::Approx(cnt), elem);
        cnt += 1.0;
    }

    cnt = 1.0;
    for (const float& elem : arr1)
    {
        CHECK_EQ(doctest::Approx(cnt), elem);
        cnt += 1.0;
    }
}

TEST_CASE("[Array2] - ForEach")
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });

    size_t i = 0;
    std::for_each(arr1.begin(), arr1.end(), [&](float val) {
        CHECK_EQ(doctest::Approx(static_cast<double>(arr1[i])), val);
        ++i;
    });
}

TEST_CASE("[Array2] - ForEachIndex")
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });

    ForEachIndex(arr1.Size(), [&](size_t i, size_t j) {
        const size_t idx = i + (4 * j) + 1;
        CHECK_EQ(doctest::Approx(static_cast<double>(idx)), arr1(i, j));
    });
}

TEST_CASE("[Array2] - ParallelForEachIndex")
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });

    ParallelForEachIndex(arr1.Size(), [&](size_t i, size_t j) {
        const size_t idx = i + (4 * j) + 1;
        CHECK_EQ(doctest::Approx(static_cast<double>(idx)), arr1(i, j));
    });
}

TEST_CASE("[Array2] - View")
{
    Array2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                        { 5.f, 6.f, 7.f, 8.f },
                        { 9.f, 10.f, 11.f, 12.f } });

    ArrayView2<float> arr2 = arr.View();
    CHECK_EQ(4u, arr2.Width());
    CHECK_EQ(3u, arr2.Height());

    for (size_t i = 0; i < 12; ++i)
    {
        CHECK_EQ(doctest::Approx(static_cast<double>(i) + 1.0), arr2[i]);
    }
}
