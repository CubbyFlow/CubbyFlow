// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// Function fractionInside is originally from Christopher Batty's code:
//
// http://www.cs.ubc.ca/labs/imager/tr/2007/Batty_VariationalFluids/
//
// and
//
// https://github.com/christopherbatty/Fluid3D

#ifndef CUBBYFLOW_LEVEL_SET_UTILS_IMPL_HPP
#define CUBBYFLOW_LEVEL_SET_UTILS_IMPL_HPP

#include <Core/Utils/Constants.hpp>

#include <cmath>

namespace CubbyFlow
{
template <typename T>
bool IsInsideSDF(T phi)
{
    return phi < 0;
}

template <typename T>
T SmearedHeavisideSDF(T phi)
{
    if (phi > 1.5)
    {
        return 1;
    }

    if (phi < -1.5)
    {
        return 0;
    }

    return 0.5 + phi / 3.0 +
           0.5 * (1 / PI<T>()) * std::sin(PI<T>() * phi / 1.5);
}

template <typename T>
T SmearedDeltaSDF(T phi)
{
    if (std::fabs(phi) > 1.5)
    {
        return 0;
    }

    return 1.0 / 3.0 + 1.0 / 3.0 * std::cos(PI<T>() * phi / 1.5);
}

template <typename T>
T FractionInsideSDF(T phi0, T phi1)
{
    if (IsInsideSDF(phi0) && IsInsideSDF(phi1))
    {
        return 1;
    }

    if (IsInsideSDF(phi0) && !IsInsideSDF(phi1))
    {
        return phi0 / (phi0 - phi1);
    }

    if (!IsInsideSDF(phi0) && IsInsideSDF(phi1))
    {
        return phi1 / (phi1 - phi0);
    }

    return 0;
}

template <typename T>
void CycleArray(T* arr, int size)
{
    T t = arr[0];

    for (int i = 0; i < size - 1; ++i)
    {
        arr[i] = arr[i + 1];
    }

    arr[size - 1] = t;
}

template <typename T>
T FractionInside(T phiBottomLeft, T phiBottomRight, T phiTopLeft, T phiTopRight)
{
    const int insideCount =
        (phiBottomLeft < 0 ? 1 : 0) + (phiTopLeft < 0 ? 1 : 0) +
        (phiBottomRight < 0 ? 1 : 0) + (phiTopRight < 0 ? 1 : 0);
    T list[] = { phiBottomLeft, phiBottomRight, phiTopRight, phiTopLeft };

    if (insideCount == 4)
    {
        return 1;
    }

    if (insideCount == 3)
    {
        // Rotate until the positive value is in the first position
        while (list[0] < 0)
        {
            CycleArray(list, 4);
        }

        // Work out the area of the exterior triangle
        T side0 = 1 - FractionInsideSDF(list[0], list[3]);
        T side1 = 1 - FractionInsideSDF(list[0], list[1]);

        return 1 - 0.5f * side0 * side1;
    }

    if (insideCount == 2)
    {
        // Rotate until a negative value is in the first position, and the next
        // negative is in either slot 1 or 2.
        while (list[0] >= 0 || !(list[1] < 0 || list[2] < 0))
        {
            CycleArray(list, 4);
        }

        // The matching signs are adjacent
        if (list[1] < 0)
        {
            T side_left = FractionInsideSDF(list[0], list[3]);
            T side_right = FractionInsideSDF(list[1], list[2]);

            return 0.5f * (side_left + side_right);
        }

        // Matching signs are diagonally opposite
        // determine the center point's sign to disambiguate this case
        T middle_point = 0.25f * (list[0] + list[1] + list[2] + list[3]);
        if (middle_point < 0)
        {
            T area = 0;

            // First triangle (top left)
            T side1 = 1 - FractionInsideSDF(list[0], list[3]);
            T side3 = 1 - FractionInsideSDF(list[2], list[3]);

            area += 0.5f * side1 * side3;

            // Second triangle (top right)
            T side2 = 1 - FractionInsideSDF(list[2], list[1]);
            T side0 = 1 - FractionInsideSDF(list[0], list[1]);

            area += 0.5f * side0 * side2;

            return 1 - area;
        }

        T area = 0;

        // First triangle (bottom left)
        T side0 = FractionInsideSDF(list[0], list[1]);
        T side1 = FractionInsideSDF(list[0], list[3]);
        area += 0.5f * side0 * side1;

        // Second triangle (top right)
        T side2 = FractionInsideSDF(list[2], list[1]);
        T side3 = FractionInsideSDF(list[2], list[3]);
        area += 0.5f * side2 * side3;
        return area;
    }

    if (insideCount == 1)
    {
        // Rotate until the negative value is in the first position
        while (list[0] >= 0)
        {
            CycleArray(list, 4);
        }

        // Work out the area of the interior triangle, and subtract from 1.
        T side0 = FractionInsideSDF(list[0], list[3]);
        T side1 = FractionInsideSDF(list[0], list[1]);

        return 0.5f * side0 * side1;
    }

    return 0;
}

template <typename T>
T DistanceToZeroLevelSet(T phi0, T phi1)
{
    if (std::fabs(phi0) + std::fabs(phi1) >
        std::numeric_limits<double>::epsilon())
    {
        return std::fabs(phi0) / (std::fabs(phi0) + std::fabs(phi1));
    }

    return static_cast<T>(0.5);
}
}  // namespace CubbyFlow

#endif