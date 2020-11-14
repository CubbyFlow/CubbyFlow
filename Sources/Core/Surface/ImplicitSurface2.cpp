// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Surface/ImplicitSurface2.hpp>
#include <Core/Utils/LevelSetUtils.hpp>

namespace CubbyFlow
{
ImplicitSurface2::ImplicitSurface2(const Transform2& _transform,
                                   bool _isNormalFlipped)
    : Surface2{ _transform, _isNormalFlipped }
{
    // Do nothing
}

double ImplicitSurface2::SignedDistance(const Vector2D& otherPoint) const
{
    const double sd = SignedDistanceLocal(transform.ToLocal(otherPoint));
    return isNormalFlipped ? -sd : sd;
}

double ImplicitSurface2::ClosestDistanceLocal(const Vector2D& otherPoint) const
{
    return std::fabs(SignedDistanceLocal(otherPoint));
}

bool ImplicitSurface2::IsInsideLocal(const Vector2D& otherPoint) const
{
    return IsInsideSDF(SignedDistanceLocal(otherPoint));
}
}  // namespace CubbyFlow