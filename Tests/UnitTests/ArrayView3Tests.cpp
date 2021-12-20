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

TEST_CASE("[ArrayView3] - Constructors")
{
    double data[60];
    for (int i = 0; i < 60; ++i)
    {
        data[i] = static_cast<double>(i);
    }

    ArrayView3<double> acc(data, Vector3UZ(5, 4, 3));

    CHECK_EQ(5u, acc.Size().x);
    CHECK_EQ(4u, acc.Size().y);
    CHECK_EQ(3u, acc.Size().z);
    CHECK_EQ(data, acc.data());
}

TEST_CASE("[ArrayView3] - Iterators")
{
    Array3<float> arr1({ { { 1.f, 2.f, 3.f, 4.f },
                           { 5.f, 6.f, 7.f, 8.f },
                           { 9.f, 10.f, 11.f, 12.f } },
                         { { 13.f, 14.f, 15.f, 16.f },
                           { 17.f, 18.f, 19.f, 20.f },
                           { 21.f, 22.f, 23.f, 24.f } } });
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

TEST_CASE("[ArrayView3] - ForEachIndex")
{
    Array3<float> arr1({ { { 1.f, 2.f, 3.f, 4.f },
                           { 5.f, 6.f, 7.f, 8.f },
                           { 9.f, 10.f, 11.f, 12.f } },
                         { { 13.f, 14.f, 15.f, 16.f },
                           { 17.f, 18.f, 19.f, 20.f },
                           { 21.f, 22.f, 23.f, 24.f } } });
    auto acc = arr1.View();

    ForEachIndex(acc.Size(), [&](size_t i, size_t j, size_t k) {
        const size_t idx = i + (4 * (j + 3 * k)) + 1;
        CHECK_EQ(doctest::Approx(static_cast<double>(idx)), acc(i, j, k));
    });
}

TEST_CASE("[ArrayView3] - ParallelForEachIndex")
{
    Array3<float> arr1({ { { 1.f, 2.f, 3.f, 4.f },
                           { 5.f, 6.f, 7.f, 8.f },
                           { 9.f, 10.f, 11.f, 12.f } },
                         { { 13.f, 14.f, 15.f, 16.f },
                           { 17.f, 18.f, 19.f, 20.f },
                           { 21.f, 22.f, 23.f, 24.f } } });
    auto acc = arr1.View();

    ParallelForEachIndex(acc.Size(), [&](size_t i, size_t j, size_t k) {
        const size_t idx = i + (4 * (j + 3 * k)) + 1;
        CHECK_EQ(doctest::Approx(static_cast<double>(idx)), acc(i, j, k));
    });
}

TEST_CASE("[ConstArrayView3] - Constructors")
{
    double data[60];
    for (int i = 0; i < 60; ++i)
    {
        data[i] = static_cast<double>(i);
    }

    // Construct with ArrayView3
    const ArrayView3<double> acc(data, Vector3UZ(5, 4, 3));
    const ConstArrayView3<double> cacc(acc);

    CHECK_EQ(5u, cacc.Size().x);
    CHECK_EQ(4u, cacc.Size().y);
    CHECK_EQ(3u, cacc.Size().z);
    CHECK_EQ(data, cacc.data());
}

TEST_CASE("[ConstArrayView3] - Iterators")
{
    Array3<float> arr1({ { { 1.f, 2.f, 3.f, 4.f },
                           { 5.f, 6.f, 7.f, 8.f },
                           { 9.f, 10.f, 11.f, 12.f } },
                         { { 13.f, 14.f, 15.f, 16.f },
                           { 17.f, 18.f, 19.f, 20.f },
                           { 21.f, 22.f, 23.f, 24.f } } });
    auto acc = arr1.View();

    double cnt = 1.0;
    for (const float& elem : acc)
    {
        CHECK_EQ(doctest::Approx(cnt), elem);
        cnt += 1.0;
    }
}

TEST_CASE("[ConstArrayView3] - ForEachIndex")
{
    Array3<float> arr1({ { { 1.f, 2.f, 3.f, 4.f },
                           { 5.f, 6.f, 7.f, 8.f },
                           { 9.f, 10.f, 11.f, 12.f } },
                         { { 13.f, 14.f, 15.f, 16.f },
                           { 17.f, 18.f, 19.f, 20.f },
                           { 21.f, 22.f, 23.f, 24.f } } });
    auto acc = arr1.View();

    ForEachIndex(acc.Size(), [&](size_t i, size_t j, size_t k) {
        const size_t idx = i + (4 * (j + 3 * k)) + 1;
        CHECK_EQ(doctest::Approx(static_cast<double>(idx)), acc(i, j, k));
    });
}

TEST_CASE("[ConstArrayView3] - ParallelForEachIndex")
{
    Array3<float> arr1({ { { 1.f, 2.f, 3.f, 4.f },
                           { 5.f, 6.f, 7.f, 8.f },
                           { 9.f, 10.f, 11.f, 12.f } },
                         { { 13.f, 14.f, 15.f, 16.f },
                           { 17.f, 18.f, 19.f, 20.f },
                           { 21.f, 22.f, 23.f, 24.f } } });
    auto acc = arr1.View();

    ParallelForEachIndex(acc.Size(), [&](size_t i, size_t j, size_t k) {
        const size_t idx = i + (4 * (j + 3 * k)) + 1;
        CHECK_EQ(doctest::Approx(static_cast<double>(idx)), acc(i, j, k));
    });
}