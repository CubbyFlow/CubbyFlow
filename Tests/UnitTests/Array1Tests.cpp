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

TEST_CASE("[Array1] - Constructors")
{
    Array1<float> arr1;
    CHECK_EQ(0u, arr1.Length());

    Array1<float> arr2(9, 1.5f);
    CHECK_EQ(9u, arr2.Length());
    for (size_t i = 0; i < 9; ++i)
    {
        CHECK_EQ(doctest::Approx(1.5), arr2[i]);
    }

    Array1<float> arr3({ 1.f, 2.f, 3.f, 4.f });
    CHECK_EQ(4u, arr3.Length());
    for (size_t i = 0; i < 4; ++i)
    {
        CHECK_EQ(doctest::Approx(static_cast<double>(i) + 1.0), arr3[i]);
    }

    Array1<float> arr4({ 1.f, 2.f, 3.f, 4.f });
    Array1<float> arr5(arr4);
    CHECK_EQ(4u, arr5.Length());
    for (size_t i = 0; i < 4; ++i)
    {
        CHECK_EQ(doctest::Approx(static_cast<double>(i) + 1.0), arr5[i]);
    }
}

TEST_CASE("[Array1] - SetMethods")
{
    Array1<float> arr1(12, -1.f);
    arr1.Fill(3.5f);
    for (float a : arr1)
    {
        CHECK_EQ(3.5f, a);
    }

    Array1<float> arr2;
    arr1.CopyFrom(arr2);
    CHECK_EQ(arr1.Length(), arr2.Length());
    for (size_t i = 0; i < arr2.Length(); ++i)
    {
        CHECK_EQ(arr1[i], arr2[i]);
    }

    arr2 = { 2.f, 5.f, 9.f, -1.f };
    CHECK_EQ(4u, arr2.Length());
    CHECK_EQ(2.f, arr2[0]);
    CHECK_EQ(5.f, arr2[1]);
    CHECK_EQ(9.f, arr2[2]);
    CHECK_EQ(-1.f, arr2[3]);
}

TEST_CASE("[Array1] - Clear")
{
    Array1<float> arr1 = { 2.f, 5.f, 9.f, -1.f };
    arr1.Clear();
    CHECK_EQ(0u, arr1.Length());
}

TEST_CASE("[Array1] - ResizeMethod")
{
    Array1<float> arr;
    arr.Resize(9);
    CHECK_EQ(9u, arr.Length());
    for (size_t i = 0; i < 9; ++i)
    {
        CHECK_EQ(doctest::Approx(0.0), arr[i]);
    }

    arr.Resize(12, 4.f);
    CHECK_EQ(12u, arr.Length());
    for (size_t i = 0; i < 8; ++i)
    {
        if (i < 9)
        {
            CHECK_EQ(doctest::Approx(0.0), arr[i]);
        }
        else
        {
            CHECK_EQ(doctest::Approx(4.0), arr[i]);
        }
    }
}

TEST_CASE("[Array1] - Iterators")
{
    Array1<float> arr1 = { 6.f, 4.f, 1.f, -5.f };

    size_t i = 0;
    for (float& elem : arr1)
    {
        CHECK_EQ(doctest::Approx(static_cast<double>(arr1[i])), elem);
        ++i;
    }

    i = 0;
    for (const float& elem : arr1)
    {
        CHECK_EQ(doctest::Approx(static_cast<double>(arr1[i])), elem);
        ++i;
    }
}

TEST_CASE("[Array1] - ForEach")
{
    Array1<float> arr1 = { 6.f, 4.f, 1.f, -5.f };
    size_t i = 0;
    std::for_each(arr1.begin(), arr1.end(), [&](float val) {
        CHECK_EQ(doctest::Approx(static_cast<double>(arr1[i])), val);
        ++i;
    });
}

TEST_CASE("[Array1] - ForEachIndex")
{
    const Array1<float> arr1 = { 6.f, 4.f, 1.f, -5.f };
    size_t cnt = 0;
    ForEachIndex(arr1.Length(), [&](size_t i) {
        CHECK_EQ(cnt, i);
        ++cnt;
    });
}

TEST_CASE("[Array1] - ParallelForEachIndex")
{
    Array1<float> arr1(200);
    ForEachIndex(arr1.Length(),
                 [&](size_t i) { arr1[i] = 200.f - static_cast<float>(i); });

    ParallelForEachIndex(arr1.Length(), [&](size_t i) {
        const auto ans = 200.f - static_cast<float>(i);
        CHECK_EQ(ans, arr1[i]);
    });
}