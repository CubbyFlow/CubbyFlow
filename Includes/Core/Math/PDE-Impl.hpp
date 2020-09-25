// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_PDE_IMPL_HPP
#define CUBBYFLOW_PDE_IMPL_HPP

#include <Core/Math/MathUtils.hpp>

namespace CubbyFlow
{
template <typename T>
std::array<T, 2> Upwind1(T* d0, T dx)
{
    const T invdx = 1 / dx;
    std::array<T, 2> dfx{};

    dfx[0] = invdx * (d0[1] - d0[0]);
    dfx[1] = invdx * (d0[2] - d0[1]);

    return dfx;
}

template <typename T>
T Upwind1(T* d0, T dx, bool isDirectionPositive)
{
    const T invdx = 1 / dx;
    return isDirectionPositive ? (invdx * (d0[1] - d0[0]))
                               : invdx * (d0[2] - d0[1]);
}

template <typename T>
T CD2(T* d0, T dx)
{
    const T hinvdx = 0.5f / dx;
    return hinvdx * (d0[2] - d0[0]);
}

template <typename T>
std::array<T, 2> ENO3(T* d0, T dx)
{
    const T invdx = 1 / dx;
    const T hinvdx = invdx / 2;
    const T tinvdx = invdx / 3;
    T d1[6], d2[5], d3[2];
    T c, cstar;
    int kstar;
    std::array<T, 2> dfx{};

    d1[0] = invdx * (d0[1] - d0[0]);
    d1[1] = invdx * (d0[2] - d0[1]);
    d1[2] = invdx * (d0[3] - d0[2]);
    d1[3] = invdx * (d0[4] - d0[3]);
    d1[4] = invdx * (d0[5] - d0[4]);
    d1[5] = invdx * (d0[6] - d0[5]);

    d2[0] = hinvdx * (d1[1] - d1[0]);
    d2[1] = hinvdx * (d1[2] - d1[1]);
    d2[2] = hinvdx * (d1[3] - d1[2]);
    d2[3] = hinvdx * (d1[4] - d1[3]);
    d2[4] = hinvdx * (d1[5] - d1[4]);

    for (int k = 0; k < 2; ++k)
    {
        if (std::fabs(d2[k + 1]) < std::fabs(d2[k + 2]))
        {
            c = d2[k + 1];
            kstar = k - 1;
            d3[0] = tinvdx * (d2[k + 1] - d2[k]);
            d3[1] = tinvdx * (d2[k + 2] - d2[k + 1]);
        }
        else
        {
            c = d2[k + 2];
            kstar = k;
            d3[0] = tinvdx * (d2[k + 2] - d2[k + 1]);
            d3[1] = tinvdx * (d2[k + 3] - d2[k + 2]);
        }

        if (std::fabs(d3[0]) < std::fabs(d3[1]))
        {
            cstar = d3[0];
        }
        else
        {
            cstar = d3[1];
        }

        T dq1 = d1[k + 2];
        T dq2 = c * (2 * (1 - k) - 1) * dx;
        T dq3 = cstar * (3 * Square(1 - kstar) - 6 * (1 - kstar) + 2) * dx * dx;

        dfx[k] = dq1 + dq2 + dq3;
    }

    return dfx;
}

template <typename T>
T ENO3(T* d0, T dx, bool isDirectionPositive)
{
    const T invdx = 1 / dx;
    const T hinvdx = invdx / 2;
    const T tinvdx = invdx / 3;
    T d1[6], d2[5], d3[2];
    T c, cstar;
    int kstar;

    d1[0] = invdx * (d0[1] - d0[0]);
    d1[1] = invdx * (d0[2] - d0[1]);
    d1[2] = invdx * (d0[3] - d0[2]);
    d1[3] = invdx * (d0[4] - d0[3]);
    d1[4] = invdx * (d0[5] - d0[4]);
    d1[5] = invdx * (d0[6] - d0[5]);

    d2[0] = hinvdx * (d1[1] - d1[0]);
    d2[1] = hinvdx * (d1[2] - d1[1]);
    d2[2] = hinvdx * (d1[3] - d1[2]);
    d2[3] = hinvdx * (d1[4] - d1[3]);
    d2[4] = hinvdx * (d1[5] - d1[4]);

    int k = isDirectionPositive ? 0 : 1;

    if (std::fabs(d2[k + 1]) < std::fabs(d2[k + 2]))
    {
        c = d2[k + 1];
        kstar = k - 1;
        d3[0] = tinvdx * (d2[k + 1] - d2[k]);
        d3[1] = tinvdx * (d2[k + 2] - d2[k + 1]);
    }
    else
    {
        c = d2[k + 2];
        kstar = k;
        d3[0] = tinvdx * (d2[k + 2] - d2[k + 1]);
        d3[1] = tinvdx * (d2[k + 3] - d2[k + 2]);
    }

    if (std::fabs(d3[0]) < std::fabs(d3[1]))
    {
        cstar = d3[0];
    }
    else
    {
        cstar = d3[1];
    }

    T dq1 = d1[k + 2];
    T dq2 = c * (2 * (1 - k) - 1) * dx;
    T dq3 = cstar * (3 * Square(1 - kstar) - 6 * (1 - kstar) + 2) * dx * dx;

    return dq1 + dq2 + dq3;
}

template <typename T>
std::array<T, 2> WENO5(T* v, T h, T eps)
{
    static const T c13 = T(1.0 / 3.0), c14 = T(0.25), c16 = T(1.0 / 6.0);
    static const T c56 = T(5.0 / 6.0), c76 = T(7.0 / 6.0), c116 = T(11.0 / 6.0);
    static const T c1312 = T(13.0 / 12.0);

    const T hinv = T(1) / h;
    std::array<T, 2> dfx{};
    T vdev[5];

    for (int k = 0; k < 2; ++k)
    {
        if (k == 0)
        {
            for (int m = 0; m < 5; ++m)
            {
                vdev[m] = (v[m + 1] - v[m]) * hinv;
            }
        }
        else
        {
            for (int m = 0; m < 5; ++m)
            {
                vdev[m] = (v[6 - m] - v[5 - m]) * hinv;
            }
        }

        const T phix1 = vdev[0] * c13 - vdev[1] * c76 + vdev[2] * c116;
        const T phix2 = -vdev[1] * c16 + vdev[2] * c56 + vdev[3] * c13;
        const T phix3 = vdev[2] * c13 + vdev[3] * c56 - vdev[4] * c16;

        T s1 = c1312 * Square(vdev[0] - 2 * vdev[1] + vdev[2]) +
               c14 * Square(vdev[0] - 4 * vdev[1] + 3 * vdev[2]);
        T s2 = c1312 * Square(vdev[1] - 2 * vdev[2] + vdev[3]) +
               c14 * Square(vdev[1] - vdev[3]);
        T s3 = c1312 * Square(vdev[2] - 2 * vdev[3] + vdev[4]) +
               c14 * Square(3 * vdev[2] - 4 * vdev[3] + vdev[4]);

        T alpha1 = T(0.1 / Square(s1 + eps));
        T alpha2 = T(0.6 / Square(s2 + eps));
        T alpha3 = T(0.3 / Square(s3 + eps));

        T sum = alpha1 + alpha2 + alpha3;

        dfx[k] = (alpha1 * phix1 + alpha2 * phix2 + alpha3 * phix3) / sum;
    }

    return dfx;
}

template <typename T>
T WENO5(T* v, T h, bool isDirectionPositive, T eps)
{
    static const T c13 = T(1.0 / 3.0), c14 = T(0.25), c16 = T(1.0 / 6.0);
    static const T c56 = T(5.0 / 6.0), c76 = T(7.0 / 6.0), c116 = T(11.0 / 6.0);
    static const T c1312 = T(13.0 / 12.0);

    const T hinv = T(1) / h;
    T vdev[5];

    if (isDirectionPositive)
    {
        for (int m = 0; m < 5; ++m)
        {
            vdev[m] = (v[m + 1] - v[m]) * hinv;
        }
    }
    else
    {
        for (int m = 0; m < 5; ++m)
        {
            vdev[m] = (v[6 - m] - v[5 - m]) * hinv;
        }
    }

    const T phix1 = vdev[0] * c13 - vdev[1] * c76 + vdev[2] * c116;
    const T phix2 = -vdev[1] * c16 + vdev[2] * c56 + vdev[3] * c13;
    const T phix3 = vdev[2] * c13 + vdev[3] * c56 - vdev[4] * c16;

    T s1 = c1312 * Square(vdev[0] - 2 * vdev[1] + vdev[2]) +
           c14 * Square(vdev[0] - 4 * vdev[1] + 3 * vdev[2]);
    T s2 = c1312 * Square(vdev[1] - 2 * vdev[2] + vdev[3]) +
           c14 * Square(vdev[1] - vdev[3]);
    T s3 = c1312 * Square(vdev[2] - 2 * vdev[3] + vdev[4]) +
           c14 * Square(3 * vdev[2] - 4 * vdev[3] + vdev[4]);

    T alpha1 = T(0.1 / Square(s1 + eps));
    T alpha2 = T(0.6 / Square(s2 + eps));
    T alpha3 = T(0.3 / Square(s3 + eps));

    T sum = alpha1 + alpha2 + alpha3;

    return (alpha1 * phix1 + alpha2 * phix2 + alpha3 * phix3) / sum;
}
}  // namespace CubbyFlow

#endif