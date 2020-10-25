// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/LevelSet/LevelSetUtils.hpp>
#include <Core/Surface/ImplicitSurface3.hpp>

namespace CubbyFlow
{
ImplicitSurface3::ImplicitSurface3(const Transform3& _transform,
                                   bool _isNormalFlipped)
    : Surface3{ _transform, _isNormalFlipped }
{
    // Do nothing
}

double ImplicitSurface3::SignedDistance(const Vector3D& otherPoint) const
{
    const double sd = SignedDistanceLocal(transform.ToLocal(otherPoint));
    return (isNormalFlipped) ? -sd : sd;
}

double ImplicitSurface3::ClosestDistanceLocal(const Vector3D& otherPoint) const
{
    return std::fabs(SignedDistanceLocal(otherPoint));
}

bool ImplicitSurface3::IsInsideLocal(const Vector3D& otherPoint) const
{
    return IsInsideSDF(SignedDistanceLocal(otherPoint));
}
}  // namespace CubbyFlow