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
#include <Core/Array/ArraySamplers.hpp>

using namespace CubbyFlow;

TEST_CASE("[NearestArraySampler1] - Sample")
{
    {
        Array1<double> grid({ 1.0, 2.0, 3.0, 4.0 });
        double gridSpacing = 1.0, gridOrigin = 0.0;
        NearestArraySampler1<double> sampler(grid.View(), gridSpacing,
                                             gridOrigin);

        double s0 = sampler(0.45);
        CHECK_LT(std::fabs(s0 - 1.0), 1e-9);

        double s1 = sampler(1.57);
        CHECK_LT(std::fabs(s1 - 3.0), 1e-9);

        double s2 = sampler(3.51);
        CHECK_LT(std::fabs(s2 - 4.0), 1e-9);
    }

    {
        Array1<double> grid({ 1.0, 2.0, 3.0, 4.0 });
        double gridSpacing = 0.5, gridOrigin = -1.0;
        NearestArraySampler1<double> sampler(grid.View(), gridSpacing,
                                             gridOrigin);

        double s0 = sampler(0.45);
        CHECK_LT(std::fabs(s0 - 4.0), 1e-9);

        double s1 = sampler(-0.05);
        CHECK_LT(std::fabs(s1 - 3.0), 1e-9);
    }
}

TEST_CASE("[LinearArraySampler1] - Sample")
{
    {
        Array1<double> grid({ 1.0, 2.0, 3.0, 4.0 });
        double gridSpacing = 1.0, gridOrigin = 0.0;
        LinearArraySampler1<double> sampler(grid.View(), gridSpacing,
                                            gridOrigin);

        double s0 = sampler(0.5);
        CHECK_LT(std::fabs(s0 - 1.5), 1e-9);

        double s1 = sampler(1.8);
        CHECK_LT(std::fabs(s1 - 2.8), 1e-9);

        double s2 = sampler(3.5);
        CHECK_EQ(doctest::Approx(4.0), s2);
    }

    {
        Array1<double> grid({ 1.0, 2.0, 3.0, 4.0 });
        double gridSpacing = 0.5, gridOrigin = -1.0;
        LinearArraySampler1<double> sampler(grid.View(), gridSpacing,
                                            gridOrigin);

        double s0 = sampler(0.2);
        CHECK_LT(std::fabs(s0 - 3.4), 1e-9);

        double s1 = sampler(-0.7);
        CHECK_LT(std::fabs(s1 - 1.6), 1e-9);
    }
}

TEST_CASE("[MonotonicCatmullRomArraySampler1] - Sample")
{
    Array1<double> grid({ 1.0, 2.0, 3.0, 4.0 });
    constexpr double gridSpacing = 1.0;
    constexpr double gridOrigin = 0.0;
    const MonotonicCatmullRomArraySampler1<double> sampler(
        grid.View(), gridSpacing, gridOrigin);

    const double s0 = sampler(1.25);
    CHECK_LT(2.0, s0);
    CHECK_GT(3.0, s0);
}

TEST_CASE("[NearestArraySampler2] - Sample")
{
    {
        Array2<double> grid({ { 1.0, 2.0, 3.0, 4.0 },
                              { 2.0, 3.0, 4.0, 5.0 },
                              { 3.0, 4.0, 5.0, 6.0 },
                              { 4.0, 5.0, 6.0, 7.0 },
                              { 5.0, 6.0, 7.0, 8.0 } });
        Vector2D gridSpacing(1.0, 1.0), gridOrigin;
        NearestArraySampler2<double> sampler(grid.View(), gridSpacing,
                                             gridOrigin);

        double s0 = sampler(Vector2D(0.45, 0.45));
        CHECK_LT(std::fabs(s0 - 1.0), 1e-9);

        double s1 = sampler(Vector2D(1.57, 4.01));
        CHECK_LT(std::fabs(s1 - 7.0), 1e-9);

        double s2 = sampler(Vector2D(3.50, 1.21));
        CHECK_LT(std::fabs(s2 - 5.0), 1e-9);
    }

    {
        Array2<double> grid({ { 1.0, 2.0, 3.0, 4.0 },
                              { 2.0, 3.0, 4.0, 5.0 },
                              { 3.0, 4.0, 5.0, 6.0 },
                              { 4.0, 5.0, 6.0, 7.0 },
                              { 5.0, 6.0, 7.0, 8.0 } });
        Vector2D gridSpacing(0.5, 0.25), gridOrigin(-1.0, -0.5);
        NearestArraySampler2<double> sampler(grid.View(), gridSpacing,
                                             gridOrigin);

        double s0 = sampler(Vector2D(0.45, 0.4));
        CHECK_LT(std::fabs(s0 - 8.0), 1e-9);

        double s1 = sampler(Vector2D(-0.05, 0.37));
        CHECK_LT(std::fabs(s1 - 6.0), 1e-9);
    }
}

TEST_CASE("[LinearArraySampler2] - Sample")
{
    {
        Array2<double> grid({ { 1.0, 2.0, 3.0, 4.0 },
                              { 2.0, 3.0, 4.0, 5.0 },
                              { 3.0, 4.0, 5.0, 6.0 },
                              { 4.0, 5.0, 6.0, 7.0 },
                              { 5.0, 6.0, 7.0, 8.0 } });
        Vector2D gridSpacing(1.0, 1.0), gridOrigin;
        LinearArraySampler2<double> sampler(grid.View(), gridSpacing,
                                            gridOrigin);

        double s0 = sampler(Vector2D(0.5, 0.5));
        CHECK_LT(std::fabs(s0 - 2.0), 1e-9);

        double s1 = sampler(Vector2D(1.5, 4.0));
        CHECK_LT(std::fabs(s1 - 6.5), 1e-9);
    }

    {
        Array2<double> grid({ { 1.0, 2.0, 3.0, 4.0 },
                              { 2.0, 3.0, 4.0, 5.0 },
                              { 3.0, 4.0, 5.0, 6.0 },
                              { 4.0, 5.0, 6.0, 7.0 },
                              { 5.0, 6.0, 7.0, 8.0 } });
        Vector2D gridSpacing(0.5, 0.25), gridOrigin(-1.0, -0.5);
        LinearArraySampler2<double> sampler(grid.View(), gridSpacing,
                                            gridOrigin);

        double s0 = sampler(Vector2D(0.5, 0.5));
        CHECK_LT(std::fabs(s0 - 8.0), 1e-9);

        double s1 = sampler(Vector2D(-0.5, 0.375));
        CHECK_LT(std::fabs(s1 - 5.5), 1e-9);
    }
}

TEST_CASE("[MonotonicCatmullRomArraySampler2] - Sample")
{
    Array2<double> grid({ { 1.0, 2.0, 3.0, 4.0 },
                          { 2.0, 3.0, 4.0, 5.0 },
                          { 3.0, 4.0, 5.0, 6.0 },
                          { 4.0, 5.0, 6.0, 7.0 },
                          { 5.0, 6.0, 7.0, 8.0 } });
    constexpr Vector2D gridSpacing(1.0, 1.0);
    constexpr Vector2D gridOrigin;
    const MonotonicCatmullRomArraySampler2<double> sampler(
        grid.View(), gridSpacing, gridOrigin);

    const double s0 = sampler(Vector2D(1.5, 2.8));
    CHECK_LT(4.0, s0);
    CHECK_GT(6.0, s0);
}

TEST_CASE("[MonotonicCatmullRomArraySampler3] - Sample")
{
    Array3<double> grid(4, 4, 4);
    for (size_t k = 0; k < 4; ++k)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            for (size_t i = 0; i < 4; ++i)
            {
                grid(i, j, k) = static_cast<double>(i + j + k);
            }
        }
    }

    constexpr Vector3D gridSpacing(1.0, 1.0, 1.0);
    constexpr Vector3D gridOrigin;
    const MonotonicCatmullRomArraySampler3<double> sampler(
        grid.View(), gridSpacing, gridOrigin);

    const double s0 = sampler(Vector3D(1.5, 1.8, 1.2));
    CHECK_LT(3.0, s0);
    CHECK_GT(6.0, s0);
}