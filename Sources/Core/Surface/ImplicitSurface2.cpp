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

namespace CubbyFlow
{
ImplicitSurface2::ImplicitSurface2(const Transform2& transform,
                                   bool isNormalFlipped)
    : Surface2(transform, isNormalFlipped)
{
    // Do nothing
}

ImplicitSurface2::ImplicitSurface2(const ImplicitSurface2& other)
    : Surface2(other)
{
    // Do nothing
}

ImplicitSurface2::~ImplicitSurface2()
{
    // Do nothing
}

double ImplicitSurface2::SignedDistance(const Vector2D& otherPoint) const
{
    return SignedDistanceLocal(transform.ToLocal(otherPoint));
}

double ImplicitSurface2::ClosestDistanceLocal(const Vector2D& otherPoint) const
{
    return std::fabs(SignedDistanceLocal(otherPoint));
}
}  // namespace CubbyFlow