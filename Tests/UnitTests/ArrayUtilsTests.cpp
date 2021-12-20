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
#include <Core/Array/ArrayUtils.hpp>

using namespace CubbyFlow;

TEST_CASE("[ArrayUtils] - Fill")
{
    Array1<double> array0(5);

    Fill(array0.View(), 3.4);

    for (size_t i = 0; i < 5; ++i)
    {
        CHECK_EQ(3.4, array0[i]);
    }

    Fill(array0.View(), 2, 4, 4.2);

    for (size_t i = 2; i < 4; ++i)
    {
        CHECK_EQ(4.2, array0[i]);
    }
}

TEST_CASE("[ArrayUtils] - Copy1")
{
    Array1<double> array0({ 1.0, 2.0, 3.0, 4.0, 5.0 });
    Array1<double> array1(5);

    Copy(array0.View(), 1, 3, array1.View());

    for (size_t i = 1; i < 3; ++i)
    {
        CHECK_EQ(array0[i], array1[i]);
    }

    Copy(array0.View(), 0, 5, array1.View());

    for (size_t i = 0; i < 5; ++i)
    {
        CHECK_EQ(array0[i], array1[i]);
    }
}

TEST_CASE("[ArrayUtils] - Copy2")
{
    Array2<double> array0({ { 1.0, 2.0 }, { 3.0, 4.0 }, { 5.0, 6.0 } });
    Array2<double> array1(2, 3);

    Copy(array0.View(), { 0, 1 }, { 2, 3 }, array1.View());

    for (size_t j = 2; j < 3; ++j)
    {
        for (size_t i = 0; i < 1; ++i)
        {
            CHECK_EQ(array0(i, j), array1(i, j));
        }
    }

    Copy(array0.View(), {}, { 2, 3 }, array1.View());

    for (size_t j = 0; j < 3; ++j)
    {
        for (size_t i = 0; i < 2; ++i)
        {
            CHECK_EQ(array0(i, j), array1(i, j));
        }
    }
}

TEST_CASE("[ArrayUtils] - Copy3")
{
    Array3<double> array0({ { { 1.0, 2.0 }, { 3.0, 4.0 }, { 5.0, 6.0 } },
                            { { 7.0, 8.0 }, { 9.0, 10.0 }, { 11.0, 12.0 } } });
    Array3<double> array1(2, 3, 2);

    Copy(array0.View(), { 0, 2, 1 }, { 1, 3, 2 }, array1.View());

    for (size_t k = 1; k < 2; ++k)
    {
        for (size_t j = 2; j < 3; ++j)
        {
            for (size_t i = 0; i < 1; ++i)
            {
                CHECK_EQ(array0(i, j, k), array1(i, j, k));
            }
        }
    }

    Copy(array0.View(), {}, { 2, 3, 2 }, array1.View());

    for (size_t k = 0; k < 2; ++k)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            for (size_t i = 0; i < 2; ++i)
            {
                CHECK_EQ(array0(i, j, k), array1(i, j, k));
            }
        }
    }
}

TEST_CASE("[ArrayUtils] - ExtrapolateToRegion2")
{
    Array2<double> data(10, 12, 0.0);
    Array2<char> valid(10, 12, static_cast<char>(0));

    for (size_t j = 3; j < 10; ++j)
    {
        for (size_t i = 2; i < 6; ++i)
        {
            data(i, j) = static_cast<double>(i) + static_cast<double>(j) * 10.0;
            valid(i, j) = 1;
        }
    }

    ExtrapolateToRegion(data.View(), valid.View(), 6, data.View());

    Array2<double> dataAnswer(
        { { 32.0, 32.0, 32.0, 33.0, 34.0, 35.0, 35.0, 35.0, 35.0, 0.0 },
          { 32.0, 32.0, 32.0, 33.0, 34.0, 35.0, 35.0, 35.0, 35.0, 35.0 },
          { 32.0, 32.0, 32.0, 33.0, 34.0, 35.0, 35.0, 35.0, 35.0, 35.0 },
          { 32.0, 32.0, 32.0, 33.0, 34.0, 35.0, 35.0, 35.0, 35.0, 35.0 },
          { 42.0, 42.0, 42.0, 43.0, 44.0, 45.0, 45.0, 45.0, 45.0, 45.0 },
          { 52.0, 52.0, 52.0, 53.0, 54.0, 55.0, 55.0, 55.0, 55.0, 55.0 },
          { 62.0, 62.0, 62.0, 63.0, 64.0, 65.0, 65.0, 65.0, 65.0, 65.0 },
          { 72.0, 72.0, 72.0, 73.0, 74.0, 75.0, 75.0, 75.0, 75.0, 75.0 },
          { 82.0, 82.0, 82.0, 83.0, 84.0, 85.0, 85.0, 85.0, 85.0, 85.0 },
          { 92.0, 92.0, 92.0, 93.0, 94.0, 95.0, 95.0, 95.0, 95.0, 95.0 },
          { 92.0, 92.0, 92.0, 93.0, 94.0, 95.0, 95.0, 95.0, 95.0, 95.0 },
          { 92.0, 92.0, 92.0, 93.0, 94.0, 95.0, 95.0, 95.0, 95.0, 95.0 } });

    for (size_t j = 0; j < 12; ++j)
    {
        for (size_t i = 0; i < 10; ++i)
        {
            CHECK_EQ(doctest::Approx(dataAnswer(i, j)), data(i, j));
        }
    }
}

TEST_CASE("[ArrayUtils] - ExtrapolateToRegion3")
{
    // TODO: Need better testing
    Array3<double> data(3, 4, 5, 0.0);
    Array3<char> valid(3, 4, 5, static_cast<char>(0));

    for (size_t k = 1; k < 4; ++k)
    {
        for (size_t j = 2; j < 3; ++j)
        {
            for (size_t i = 1; i < 2; ++i)
            {
                data(i, j, k) = 42.0;
                valid(i, j, k) = 1;
            }
        }
    }

    ExtrapolateToRegion(data.View(), valid.View(), 5, data.View());

    for (size_t k = 0; k < 5; ++k)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            for (size_t i = 0; i < 3; ++i)
            {
                CHECK_EQ(doctest::Approx(42.0), data(i, j, k));
            }
        }
    }
}