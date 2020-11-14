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

#include <Core/SPH/SPHStdKernel3.hpp>
#include <Core/Utils/Constants.hpp>

namespace CubbyFlow
{
SPHStdKernel3::SPHStdKernel3() : h{ 0 }, h2{ 0 }, h3{ 0 }, h5{ 0 }
{
    // Do nothing
}

SPHStdKernel3::SPHStdKernel3(double kernelRadius)
    : h{ kernelRadius }, h2{ h * h }, h3{ h2 * h }, h5{ h2 * h2 * h }
{
    // Do nothing
}

double SPHStdKernel3::operator()(double distance) const
{
    const double distanceSquared = distance * distance;

    if (distanceSquared >= h2)
    {
        return 0.0;
    }

    const double x = 1.0 - distanceSquared / h2;
    return 315.0 / (64.0 * PI_DOUBLE * h3) * x * x * x;
}

double SPHStdKernel3::FirstDerivative(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    const double x = 1.0 - distance * distance / h2;
    return -945.0 * distance / (32.0 * PI_DOUBLE * h5) * x * x;
}

Vector3D SPHStdKernel3::Gradient(const Vector3D& point) const
{
    const double dist = point.Length();

    if (dist > 0.0)
    {
        return Gradient(dist, point / dist);
    }

    return Vector3D{ 0, 0, 0 };
}

Vector3D SPHStdKernel3::Gradient(double distance,
                                 const Vector3D& directionToCenter) const
{
    return -FirstDerivative(distance) * directionToCenter;
}

double SPHStdKernel3::SecondDerivative(double distance) const
{
    const double distanceSquared = distance * distance;

    if (distanceSquared >= h2)
    {
        return 0.0;
    }

    const double x = distanceSquared / h2;
    return 945.0 / (32.0 * PI_DOUBLE * h5) * (1 - x) * (5 * x - 1);
}

SPHSpikyKernel3::SPHSpikyKernel3() : h{ 0 }, h2{ 0 }, h3{ 0 }, h4{ 0 }, h5{ 0 }
{
    // Do nothing
}

SPHSpikyKernel3::SPHSpikyKernel3(double kernelRadius)
    : h{ kernelRadius }, h2{ h * h }, h3{ h2 * h }, h4{ h2 * h2 }, h5{ h3 * h2 }
{
    // Do nothing
}

double SPHSpikyKernel3::operator()(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    const double x = 1.0 - distance / h;
    return 15.0 / (PI_DOUBLE * h3) * x * x * x;
}

double SPHSpikyKernel3::FirstDerivative(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    const double x = 1.0 - distance / h;
    return -45.0 / (PI_DOUBLE * h4) * x * x;
}

Vector3D SPHSpikyKernel3::Gradient(const Vector3D& point) const
{
    const double dist = point.Length();

    if (dist > 0.0)
    {
        return Gradient(dist, point / dist);
    }

    return Vector3D{ 0, 0, 0 };
}

Vector3D SPHSpikyKernel3::Gradient(double distance,
                                   const Vector3D& directionToCenter) const
{
    return -FirstDerivative(distance) * directionToCenter;
}

double SPHSpikyKernel3::SecondDerivative(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    const double x = 1.0 - distance / h;
    return 90.0 / (PI_DOUBLE * h5) * x;
}
}  // namespace CubbyFlow