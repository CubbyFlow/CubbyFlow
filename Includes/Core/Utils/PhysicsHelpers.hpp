// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_PHYSICS_HELPERS_HPP
#define CUBBYFLOW_PHYSICS_HELPERS_HPP

#include <Core/Matrix/Matrix.hpp>

namespace CubbyFlow
{
inline Vector2D ComputeDragForce(double dragCoefficient, double radius,
                                 const Vector2D& velocity)
{
    // Stoke's drag force assuming our Reynolds number is very low.
    // http://en.wikipedia.org/wiki/Drag_(physics)#Very_low_Reynolds_numbers:_Stokes.27_drag
    return -6.0 * PI_DOUBLE * dragCoefficient * radius * velocity;
}

inline Vector3D ComputeDragForce(double dragCoefficient, double radius,
                                 const Vector3D& velocity)
{
    // Stoke's drag force assuming our Reynolds number is very low.
    // http://en.wikipedia.org/wiki/Drag_(physics)#Very_low_Reynolds_numbers:_Stokes.27_drag
    return -6.0 * PI_DOUBLE * dragCoefficient * radius * velocity;
}

template <size_t N>
Vector<double, N> ProjectAndApplyFriction(const Vector<double, N>& vel,
                                          const Vector<double, N>& normal,
                                          double frictionCoefficient)
{
    Vector<double, N> velt = vel.Projected(normal);
    if (velt.LengthSquared() > 0)
    {
        const double veln = std::max(-vel.Dot(normal), 0.0);
        velt *= std::max(1.0 - frictionCoefficient * veln / velt.Length(), 0.0);
    }

    return velt;
}

inline double ComputePressureFromEos(double density, double targetDensity,
                                     double eosScale, double eosExponent,
                                     double negativePressureScale)
{
    // See Murnaghan-Tait equation of state from
    // https://en.wikipedia.org/wiki/Tait_equation
    double p = eosScale / eosExponent *
               (std::pow((density / targetDensity), eosExponent) - 1.0);

    // Negative pressure scaling
    if (p < 0)
    {
        p *= negativePressureScale;
    }

    return p;
}
}  // namespace CubbyFlow

#endif