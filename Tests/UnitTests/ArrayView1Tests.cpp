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
#include <Core/Array/ArrayView.hpp>

using namespace CubbyFlow;

TEST_CASE("[ArrayView1] - Constructors")
{
    double data[5];
    for (int i = 0; i < 5; ++i)
    {
        data[i] = static_cast<double>(i);
    }

    ArrayView1<double> acc(data, 5);

    CHECK_EQ(5u, acc.Length());
    CHECK_EQ(data, acc.data());
}

TEST_CASE("[ArrayView1] - Iterators")
{
    Array1<float> arr1 = { 6.f, 4.f, 1.f, -5.f };
    auto acc = arr1.View();

    size_t i = 0;
    for (float& elem : acc)
    {
        CHECK_EQ(doctest::Approx(static_cast<double>(acc[i])), elem);
        ++i;
    }

    i = 0;
    for (const float& elem : acc)
    {
        CHECK_EQ(acc[i], elem);
        ++i;
    }
}

TEST_CASE("[ArrayView1] - ForEachIndex")
{
    Array1<float> arr1 = { 6.f, 4.f, 1.f, -5.f };
    const auto acc = arr1.View();

    size_t cnt = 0;
    ForEachIndex(acc.Size(), [&](size_t i) {
        CHECK_EQ(cnt, i);
        ++cnt;
    });
}

TEST_CASE("[ArrayView1] - ParallelForEachIndex")
{
    Array1<float> arr1(200);
    const auto acc = arr1.View();

    ForEachIndex(acc.Size(),
                 [&](size_t i) { arr1[i] = 200.f - static_cast<float>(i); });

    ParallelForEachIndex(acc.Size(), [&](size_t i) {
        const float ans = 200.f - static_cast<float>(i);
        CHECK_EQ(ans, arr1[i]);
    });
}

TEST_CASE("[ConstArrayView1] - Constructors")
{
    double data[5];
    for (int i = 0; i < 5; ++i)
    {
        data[i] = static_cast<double>(i);
    }

    // Construct with ArrayView1
    const ArrayView1<double> acc(data, 5);
    const ConstArrayView1<double> cacc(acc);

    CHECK_EQ(5u, cacc.Length());
    CHECK_EQ(data, cacc.data());
}

TEST_CASE("[ConstArrayView1] - Iterators")
{
    Array1<float> arr1 = { 6.f, 4.f, 1.f, -5.f };
    auto acc = arr1.View();

    size_t i = 0;
    for (const float& elem : acc)
    {
        CHECK_EQ(doctest::Approx(static_cast<double>(acc[i])), elem);
        ++i;
    }
}

TEST_CASE("[ConstArrayView1] - ForEachIndex")
{
    Array1<float> arr1 = { 6.f, 4.f, 1.f, -5.f };
    const auto acc = arr1.View();

    size_t cnt = 0;
    ForEachIndex(acc.Size(), [&](size_t i) {
        CHECK_EQ(cnt, i);
        ++cnt;
    });
}

TEST_CASE("[ConstArrayView1] - ParallelForEachIndex")
{
    Array1<float> arr1(200);
    const auto acc = arr1.View();

    ForEachIndex(acc.Size(),
                 [&](size_t i) { arr1[i] = 200.f - static_cast<float>(i); });

    ParallelForEachIndex(acc.Size(), [&](size_t i) {
        const float ans = 200.f - static_cast<float>(i);
        CHECK_EQ(ans, arr1[i]);
    });
}