// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// Adopted from the sample code of:
// Bart Adams and Martin Wicke,
// "Meshless Approximation Methods and Applications in Physics Based Modeling
// and Animation", Eurographics 2009 Tutorial

#include <Core/SPH/SPHStdKernel2.hpp>
#include <Core/Utils/Constants.hpp>

namespace CubbyFlow
{
SPHStdKernel2::SPHStdKernel2() : h(0), h2(0), h3(0), h4(0)
{
    // Do nothing
}

SPHStdKernel2::SPHStdKernel2(double h_)
    : h(h_), h2(h * h), h3(h2 * h), h4(h2 * h2)
{
    // Do nothing
}

SPHStdKernel2::SPHStdKernel2(const SPHStdKernel2& other)
    : h(other.h), h2(other.h2), h3(other.h3), h4(other.h4)
{
    // Do nothing
}

double SPHStdKernel2::operator()(double distance) const
{
    double distanceSquared = distance * distance;

    if (distanceSquared >= h2)
    {
        return 0.0;
    }

    double x = 1.0 - distanceSquared / h2;
    return 4.0 / (PI_DOUBLE * h2) * x * x * x;
}

double SPHStdKernel2::FirstDerivative(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    double x = 1.0 - distance * distance / h2;
    return -24.0 * distance / (PI_DOUBLE * h4) * x * x;
}

Vector2D SPHStdKernel2::Gradient(const Vector2D& point) const
{
    double dist = point.Length();

    if (dist > 0.0)
    {
        return Gradient(dist, point / dist);
    }

    return Vector2D(0, 0);
}

Vector2D SPHStdKernel2::Gradient(double distance,
                                 const Vector2D& directionToCenter) const
{
    return -FirstDerivative(distance) * directionToCenter;
}

double SPHStdKernel2::SecondDerivative(double distance) const
{
    double distanceSquared = distance * distance;

    if (distanceSquared >= h2)
    {
        return 0.0;
    }

    double x = distanceSquared / h2;
    return 24.0 / (PI_DOUBLE * h4) * (1 - x) * (5 * x - 1);
}

SPHSpikyKernel2::SPHSpikyKernel2() : h(0), h2(0), h3(0), h4(0), h5(0)
{
    // Do nothing
}

SPHSpikyKernel2::SPHSpikyKernel2(double h_)
    : h(h_), h2(h * h), h3(h2 * h), h4(h2 * h2), h5(h3 * h2)
{
    // Do nothing
}

SPHSpikyKernel2::SPHSpikyKernel2(const SPHSpikyKernel2& other)
    : h(other.h), h2(other.h2), h3(other.h3), h4(other.h4), h5(other.h5)
{
    // Do nothing
}

double SPHSpikyKernel2::operator()(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    double x = 1.0 - distance / h;
    return 10.0 / (PI_DOUBLE * h2) * x * x * x;
}

double SPHSpikyKernel2::FirstDerivative(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    double x = 1.0 - distance / h;
    return -30.0 / (PI_DOUBLE * h3) * x * x;
}

Vector2D SPHSpikyKernel2::Gradient(const Vector2D& point) const
{
    double dist = point.Length();

    if (dist > 0.0)
    {
        return Gradient(dist, point / dist);
    }

    return Vector2D(0, 0);
}

Vector2D SPHSpikyKernel2::Gradient(double distance,
                                   const Vector2D& directionToCenter) const
{
    return -FirstDerivative(distance) * directionToCenter;
}

double SPHSpikyKernel2::SecondDerivative(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    double x = 1.0 - distance / h;
    return 60.0 / (PI_DOUBLE * h4) * x;
}
}  // namespace CubbyFlow