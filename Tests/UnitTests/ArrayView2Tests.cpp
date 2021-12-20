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

TEST_CASE("[ArrayView2] - Constructors")
{
    double data[20];
    for (int i = 0; i < 20; ++i)
    {
        data[i] = static_cast<double>(i);
    }

    ArrayView2<double> acc(data, Vector2UZ(5, 4));

    CHECK_EQ(5u, acc.Size().x);
    CHECK_EQ(4u, acc.Size().y);
    CHECK_EQ(data, acc.data());
}

TEST_CASE("[ArrayView2] - Iterators")
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });
    auto acc = arr1.View();

    double cnt = 1.0;
    for (float& elem : acc)
    {
        CHECK_EQ(doctest::Approx(cnt), elem);
        cnt += 1.0;
    }

    cnt = 1.0;
    for (const float& elem : acc)
    {
        CHECK_EQ(doctest::Approx(cnt), elem);
        cnt += 1.0;
    }
}

TEST_CASE("[ArrayView2] - ForEachIndex")
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });

    ForEachIndex(arr1.Size(), [&](size_t i, size_t j) {
        const size_t idx = i + (4 * j) + 1;
        CHECK_EQ(doctest::Approx(static_cast<double>(idx)), arr1(i, j));
    });
}

TEST_CASE("[ArrayView2] - ParallelForEachIndex")
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });

    ParallelForEachIndex(arr1.Size(), [&](size_t i, size_t j) {
        const size_t idx = i + (4 * j) + 1;
        CHECK_EQ(doctest::Approx(static_cast<double>(idx)), arr1(i, j));
    });
}

TEST_CASE("[ConstArrayView2] - Constructors")
{
    double data[20];
    for (int i = 0; i < 20; ++i)
    {
        data[i] = static_cast<double>(i);
    }

    // Construct with ArrayView2
    const ArrayView2<double> acc(data, Vector2UZ(5, 4));
    const ConstArrayView2<double> cacc(acc);

    CHECK_EQ(5u, cacc.Size().x);
    CHECK_EQ(4u, cacc.Size().y);
    CHECK_EQ(data, cacc.data());
}

TEST_CASE("[ConstArrayView2] - Iterators")
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });
    auto acc = arr1.View();

    double cnt = 1.0;
    for (const float& elem : acc)
    {
        CHECK_EQ(doctest::Approx(cnt), elem);
        cnt += 1.0;
    }
}

TEST_CASE("[ConstArrayView2] - ForEach")
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });
    auto acc = arr1.View();

    size_t i = 0;
    std::for_each(acc.begin(), acc.end(), [&](float val) {
        CHECK_EQ(doctest::Approx(static_cast<double>(acc[i])), val);
        ++i;
    });
}

TEST_CASE("[ConstArrayView2] - ForEachIndex")
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });
    auto acc = arr1.View();

    ForEachIndex(acc.Size(), [&](size_t i, size_t j) {
        const size_t idx = i + (4 * j) + 1;
        CHECK_EQ(doctest::Approx(static_cast<double>(idx)), acc(i, j));
    });
}

TEST_CASE("[ConstArrayView2] - ParallelForEachIndex")
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });
    auto acc = arr1.View();

    ParallelForEachIndex(acc.Size(), [&](size_t i, size_t j) {
        const size_t idx = i + (4 * j) + 1;
        CHECK_EQ(doctest::Approx(static_cast<double>(idx)), acc(i, j));
    });
}