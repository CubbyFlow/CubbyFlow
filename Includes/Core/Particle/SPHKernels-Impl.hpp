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

#ifndef CUBBYFLOW_SPH_KERNELS_IMPL_HPP
#define CUBBYFLOW_SPH_KERNELS_IMPL_HPP

namespace CubbyFlow
{
inline SPHStdKernel2::SPHStdKernel() : h(0), h2(0), h3(0), h4(0)
{
    // Do nothing
}

inline SPHStdKernel2::SPHStdKernel(double kernelRadius)
    : h(kernelRadius), h2(h * h), h3(h2 * h), h4(h2 * h2)
{
    // Do nothing
}

inline double SPHStdKernel2::operator()(double distance) const
{
    const double distanceSquared = distance * distance;

    if (distanceSquared >= h2)
    {
        return 0.0;
    }

    const double x = 1.0 - distanceSquared / h2;
    return 4.0 / (PI_DOUBLE * h2) * x * x * x;
}

inline double SPHStdKernel2::FirstDerivative(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    const double x = 1.0 - distance * distance / h2;
    return -24.0 * distance / (PI_DOUBLE * h4) * x * x;
}

inline double SPHStdKernel2::SecondDerivative(double distance) const
{
    const double distanceSquared = distance * distance;

    if (distanceSquared >= h2)
    {
        return 0.0;
    }

    const double x = distanceSquared / h2;
    return 24.0 / (PI_DOUBLE * h4) * (1 - x) * (5 * x - 1);
}

inline Vector2D SPHStdKernel2::Gradient(const Vector2D& point) const
{
    if (const double dist = point.Length(); dist > 0.0)
    {
        return Gradient(dist, point / dist);
    }

    return Vector2D{ 0, 0 };
}

inline Vector2D SPHStdKernel2::Gradient(double distance,
                                        const Vector2D& directionToCenter) const
{
    return -FirstDerivative(distance) * directionToCenter;
}

inline SPHStdKernel3::SPHStdKernel() : h(0), h2(0), h3(0), h5(0)
{
    // Do nothing
}

inline SPHStdKernel3::SPHStdKernel(double kernelRadius)
    : h(kernelRadius), h2(h * h), h3(h2 * h), h5(h2 * h3)
{
    // Do nothing
}

inline double SPHStdKernel3::operator()(double distance) const
{
    if (distance * distance >= h2)
    {
        return 0.0;
    }

    const double x = 1.0 - distance * distance / h2;
    return 315.0 / (64.0 * PI_DOUBLE * h3) * x * x * x;
}

inline double SPHStdKernel3::FirstDerivative(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    const double x = 1.0 - distance * distance / h2;
    return -945.0 / (32.0 * PI_DOUBLE * h5) * distance * x * x;
}

inline double SPHStdKernel3::SecondDerivative(double distance) const
{
    if (distance * distance >= h2)
    {
        return 0.0;
    }

    const double x = distance * distance / h2;
    return 945.0 / (32.0 * PI_DOUBLE * h5) * (1 - x) * (5 * x - 1);
}

inline Vector3D SPHStdKernel3::Gradient(const Vector3D& point) const
{
    if (const double dist = point.Length(); dist > 0.0)
    {
        return Gradient(dist, point / dist);
    }

    return Vector3D{ 0, 0, 0 };
}

inline Vector3D SPHStdKernel3::Gradient(double distance,
                                        const Vector3D& directionToCenter) const
{
    return -FirstDerivative(distance) * directionToCenter;
}

inline SPHSpikyKernel2::SPHSpikyKernel() : h(0), h2(0), h3(0), h4(0), h5(0)
{
    // Do nothing
}

inline SPHSpikyKernel2::SPHSpikyKernel(double kernelRadius)
    : h(kernelRadius), h2(h * h), h3(h2 * h), h4(h2 * h2), h5(h3 * h2)
{
    // Do nothing
}

inline double SPHSpikyKernel2::operator()(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    const double x = 1.0 - distance / h;
    return 10.0 / (PI_DOUBLE * h2) * x * x * x;
}

inline double SPHSpikyKernel2::FirstDerivative(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    const double x = 1.0 - distance / h;
    return -30.0 / (PI_DOUBLE * h3) * x * x;
}

inline double SPHSpikyKernel2::SecondDerivative(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    const double x = 1.0 - distance / h;
    return 60.0 / (PI_DOUBLE * h4) * x;
}

inline Vector2D SPHSpikyKernel2::Gradient(const Vector2D& point) const
{
    if (const double dist = point.Length(); dist > 0.0)
    {
        return Gradient(dist, point / dist);
    }

    return Vector2D{ 0, 0 };
}

inline Vector2D SPHSpikyKernel2::Gradient(
    double distance, const Vector2D& directionToCenter) const
{
    return -FirstDerivative(distance) * directionToCenter;
}

inline SPHSpikyKernel3::SPHSpikyKernel() : h(0), h2(0), h3(0), h4(0), h5(0)
{
    // Do nothing
}

inline SPHSpikyKernel3::SPHSpikyKernel(double kernelRadius)
    : h(kernelRadius), h2(h * h), h3(h2 * h), h4(h2 * h2), h5(h3 * h2)
{
    // Do nothing
}

inline double SPHSpikyKernel3::operator()(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    const double x = 1.0 - distance / h;
    return 15.0 / (PI_DOUBLE * h3) * x * x * x;
}

inline double SPHSpikyKernel3::FirstDerivative(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    const double x = 1.0 - distance / h;
    return -45.0 / (PI_DOUBLE * h4) * x * x;
}

inline double SPHSpikyKernel3::SecondDerivative(double distance) const
{
    if (distance >= h)
    {
        return 0.0;
    }

    const double x = 1.0 - distance / h;
    return 90.0 / (PI_DOUBLE * h5) * x;
}

inline Vector3D SPHSpikyKernel3::Gradient(const Vector3D& point) const
{
    if (const double dist = point.Length(); dist > 0.0)
    {
        return Gradient(dist, point / dist);
    }

    return Vector3D{ 0, 0, 0 };
}

inline Vector3D SPHSpikyKernel3::Gradient(
    double distance, const Vector3D& directionToCenter) const
{
    return -FirstDerivative(distance) * directionToCenter;
}
}  // namespace CubbyFlow

#endif