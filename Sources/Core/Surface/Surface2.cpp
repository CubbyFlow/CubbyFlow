// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Surface/Surface2.hpp>

#include <utility>

namespace CubbyFlow
{
Surface2::Surface2(Transform2 _transform, bool _isNormalFlipped)
    : transform{ std::move(_transform) }, isNormalFlipped{ _isNormalFlipped }
{
    // Do nothing
}

Vector2D Surface2::ClosestPoint(const Vector2D& otherPoint) const
{
    return transform.ToWorld(ClosestPointLocal(transform.ToLocal(otherPoint)));
}

BoundingBox2D Surface2::BoundingBox() const
{
    return transform.ToWorld(BoundingBoxLocal());
}

bool Surface2::Intersects(const Ray2D& ray) const
{
    return IntersectsLocal(transform.ToLocal(ray));
}

double Surface2::ClosestDistance(const Vector2D& otherPoint) const
{
    return ClosestDistanceLocal(transform.ToLocal(otherPoint));
}

SurfaceRayIntersection2 Surface2::ClosestIntersection(const Ray2D& ray) const
{
    SurfaceRayIntersection2 result =
        ClosestIntersectionLocal(transform.ToLocal(ray));

    result.point = transform.ToWorld(result.point);
    result.normal = transform.ToWorldDirection(result.normal);
    result.normal *= (isNormalFlipped) ? -1.0 : 1.0;

    return result;
}

Vector2D Surface2::ClosestNormal(const Vector2D& otherPoint) const
{
    Vector2D result = transform.ToWorldDirection(
        ClosestNormalLocal(transform.ToLocal(otherPoint)));
    result *= (isNormalFlipped) ? -1.0 : 1.0;
    return result;
}

void Surface2::UpdateQueryEngine()
{
    // Do nothing
}

bool Surface2::IsBounded() const
{
    return true;
}

bool Surface2::IsValidGeometry() const
{
    return true;
}

bool Surface2::IsInside(const Vector2D& otherPoint) const
{
    return isNormalFlipped == !IsInsideLocal(transform.ToLocal(otherPoint));
}

bool Surface2::IntersectsLocal(const Ray2D& ray) const
{
    const SurfaceRayIntersection2 result = ClosestIntersectionLocal(ray);
    return result.isIntersecting;
}

double Surface2::ClosestDistanceLocal(const Vector2D& otherPoint) const
{
    return otherPoint.DistanceTo(ClosestPointLocal(otherPoint));
}

bool Surface2::IsInsideLocal(const Vector2D& otherPointLocal) const
{
    const Vector2D cpLocal = ClosestPointLocal(otherPointLocal);
    const Vector2D normalLocal = ClosestNormalLocal(otherPointLocal);
    return (otherPointLocal - cpLocal).Dot(normalLocal) < 0.0;
}
}  // namespace CubbyFlow