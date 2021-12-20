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

TEST_CASE("[Array3] - Constructors")
{
    Array3<float> arr1;
    CHECK_EQ(0u, arr1.Width());
    CHECK_EQ(0u, arr1.Height());
    CHECK_EQ(0u, arr1.Depth());

    Array3<float> arr2(Vector3UZ(3, 7, 4));
    CHECK_EQ(3u, arr2.Width());
    CHECK_EQ(7u, arr2.Height());
    CHECK_EQ(4u, arr2.Depth());
    for (size_t i = 0; i < 84; ++i)
    {
        CHECK_EQ(doctest::Approx(0.0), arr2[i]);
    }

    Array3<float> arr3(Vector3UZ(1, 9, 5), 1.5f);
    CHECK_EQ(1u, arr3.Width());
    CHECK_EQ(9u, arr3.Height());
    CHECK_EQ(5u, arr3.Depth());
    for (size_t i = 0; i < 45; ++i)
    {
        CHECK_EQ(doctest::Approx(1.5), arr3[i]);
    }

    Array3<float> arr4(5, 2, 8);
    CHECK_EQ(5u, arr4.Width());
    CHECK_EQ(2u, arr4.Height());
    CHECK_EQ(8u, arr4.Depth());
    for (size_t i = 0; i < 80; ++i)
    {
        CHECK_EQ(doctest::Approx(0.0), arr4[i]);
    }

    Array3<float> arr5(3, 4, 2, 7.f);
    CHECK_EQ(3u, arr5.Width());
    CHECK_EQ(4u, arr5.Height());
    CHECK_EQ(2u, arr5.Depth());
    for (size_t i = 0; i < 24; ++i)
    {
        CHECK_EQ(doctest::Approx(7.0), arr5[i]);
    }

    Array3<float> arr6({ { { 1.f, 2.f, 3.f, 4.f },
                           { 5.f, 6.f, 7.f, 8.f },
                           { 9.f, 10.f, 11.f, 12.f } },
                         { { 13.f, 14.f, 15.f, 16.f },
                           { 17.f, 18.f, 19.f, 20.f },
                           { 21.f, 22.f, 23.f, 24.f } } });

    CHECK_EQ(4u, arr6.Width());
    CHECK_EQ(3u, arr6.Height());
    CHECK_EQ(2u, arr6.Depth());
    for (size_t i = 0; i < 24; ++i)
    {
        CHECK_EQ(doctest::Approx(static_cast<double>(i) + 1.0), arr6[i]);
    }

    Array3<float> arr7({ { { 1.f, 2.f, 3.f, 4.f },
                           { 5.f, 6.f, 7.f, 8.f },
                           { 9.f, 10.f, 11.f, 12.f } },
                         { { 13.f, 14.f, 15.f, 16.f },
                           { 17.f, 18.f, 19.f, 20.f },
                           { 21.f, 22.f, 23.f, 24.f } } });
    Array3<float> arr8(arr7);

    CHECK_EQ(4u, arr8.Width());
    CHECK_EQ(3u, arr8.Height());
    CHECK_EQ(2u, arr8.Depth());
    for (size_t i = 0; i < 24; ++i)
    {
        CHECK_EQ(doctest::Approx(static_cast<double>(i) + 1.0), arr8[i]);
    }
}

TEST_CASE("[Array3] - Clear")
{
    Array3<float> arr({ { { 1.f, 2.f, 3.f, 4.f },
                          { 5.f, 6.f, 7.f, 8.f },
                          { 9.f, 10.f, 11.f, 12.f } },
                        { { 13.f, 14.f, 15.f, 16.f },
                          { 17.f, 18.f, 19.f, 20.f },
                          { 21.f, 22.f, 23.f, 24.f } } });

    arr.Clear();
    CHECK_EQ(0u, arr.Width());
    CHECK_EQ(0u, arr.Height());
    CHECK_EQ(0u, arr.Depth());
}

TEST_CASE("[Array3] - ResizeMethod")
{
    Array3<float> arr1;
    arr1.Resize(Vector3UZ(2, 9, 5));
    CHECK_EQ(2u, arr1.Width());
    CHECK_EQ(9u, arr1.Height());
    CHECK_EQ(5u, arr1.Depth());
    for (size_t i = 0; i < 90; ++i)
    {
        CHECK_EQ(doctest::Approx(0.0), arr1[i]);
    }

    arr1.Resize(Vector3UZ(8, 13, 7), 4.f);
    CHECK_EQ(8u, arr1.Width());
    CHECK_EQ(13u, arr1.Height());
    CHECK_EQ(7u, arr1.Depth());
    for (size_t k = 0; k < 7; ++k)
    {
        for (size_t j = 0; j < 13; ++j)
        {
            for (size_t i = 0; i < 8; ++i)
            {
                if (i < 2 && j < 9 && k < 5)
                {
                    CHECK_EQ(doctest::Approx(0.0), arr1(i, j, k));
                }
                else
                {
                    CHECK_EQ(doctest::Approx(4.0), arr1(i, j, k));
                }
            }
        }
    }

    Array3<float> arr2;
    arr2.Resize(7, 6, 3);
    CHECK_EQ(7u, arr2.Width());
    CHECK_EQ(6u, arr2.Height());
    CHECK_EQ(3u, arr2.Depth());
    for (size_t i = 0; i < 126; ++i)
    {
        CHECK_EQ(doctest::Approx(0.0), arr2[i]);
    }

    arr2.Resize(1, 9, 4, 3.f);
    CHECK_EQ(1u, arr2.Width());
    CHECK_EQ(9u, arr2.Height());
    CHECK_EQ(4u, arr2.Depth());
    for (size_t k = 0; k < 4; ++k)
    {
        for (size_t j = 0; j < 9; ++j)
        {
            for (size_t i = 0; i < 1; ++i)
            {
                if (j < 6 && k < 3)
                {
                    CHECK_EQ(doctest::Approx(0.0), arr2(i, j, k));
                }
                else
                {
                    CHECK_EQ(doctest::Approx(3.0), arr2(i, j, k));
                }
            }
        }
    }
}

TEST_CASE("[Array3] - Iterators")
{
    Array3<float> arr1({ { { 1.f, 2.f, 3.f, 4.f },
                           { 5.f, 6.f, 7.f, 8.f },
                           { 9.f, 10.f, 11.f, 12.f } },
                         { { 13.f, 14.f, 15.f, 16.f },
                           { 17.f, 18.f, 19.f, 20.f },
                           { 21.f, 22.f, 23.f, 24.f } } });

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

TEST_CASE("[Array3] - ForEach")
{
    Array3<float> arr1({ { { 1.f, 2.f, 3.f, 4.f },
                           { 5.f, 6.f, 7.f, 8.f },
                           { 9.f, 10.f, 11.f, 12.f } },
                         { { 13.f, 14.f, 15.f, 16.f },
                           { 17.f, 18.f, 19.f, 20.f },
                           { 21.f, 22.f, 23.f, 24.f } } });

    size_t i = 0;
    std::for_each(arr1.begin(), arr1.end(), [&](float val) {
        CHECK_EQ(doctest::Approx(static_cast<double>(arr1[i])), val);
        ++i;
    });
}

TEST_CASE("[Array3] - ForEachIndex")
{
    Array3<float> arr1({ { { 1.f, 2.f, 3.f, 4.f },
                           { 5.f, 6.f, 7.f, 8.f },
                           { 9.f, 10.f, 11.f, 12.f } },
                         { { 13.f, 14.f, 15.f, 16.f },
                           { 17.f, 18.f, 19.f, 20.f },
                           { 21.f, 22.f, 23.f, 24.f } } });

    ForEachIndex(arr1.Size(), [&](size_t i, size_t j, size_t k) {
        const size_t idx = i + (4 * (j + 3 * k)) + 1;
        CHECK_EQ(doctest::Approx(static_cast<double>(idx)), arr1(i, j, k));
    });
}

TEST_CASE("[Array3] - ParallelForEachIndex")
{
    Array3<float> arr1({ { { 1.f, 2.f, 3.f, 4.f },
                           { 5.f, 6.f, 7.f, 8.f },
                           { 9.f, 10.f, 11.f, 12.f } },
                         { { 13.f, 14.f, 15.f, 16.f },
                           { 17.f, 18.f, 19.f, 20.f },
                           { 21.f, 22.f, 23.f, 24.f } } });

    ParallelForEachIndex(arr1.Size(), [&](size_t i, size_t j, size_t k) {
        const size_t idx = i + (4 * (j + 3 * k)) + 1;
        CHECK_EQ(doctest::Approx(static_cast<double>(idx)), arr1(i, j, k));
    });
}

TEST_CASE("[Array3] - View")
{
    Array3<float> arr({ { { 1.f, 2.f, 3.f, 4.f },
                          { 5.f, 6.f, 7.f, 8.f },
                          { 9.f, 10.f, 11.f, 12.f } },
                        { { 13.f, 14.f, 15.f, 16.f },
                          { 17.f, 18.f, 19.f, 20.f },
                          { 21.f, 22.f, 23.f, 24.f } } });

    ArrayView3<float> arr2 = arr.View();
    CHECK_EQ(4u, arr2.Width());
    CHECK_EQ(3u, arr2.Height());

    for (size_t i = 0; i < 12; ++i)
    {
        CHECK_EQ(doctest::Approx(static_cast<double>(i) + 1.0), arr2[i]);
    }
}
