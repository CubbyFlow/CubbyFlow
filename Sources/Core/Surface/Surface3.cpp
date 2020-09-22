// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Surface/Surface3.hpp>

#include <utility>

namespace CubbyFlow
{
Surface3::Surface3(Transform3 _transform, bool _isNormalFlipped)
    : transform(std::move(_transform)), isNormalFlipped(_isNormalFlipped)
{
    // Do nothing
}

Vector3D Surface3::ClosestPoint(const Vector3D& otherPoint) const
{
    return transform.ToWorld(ClosestPointLocal(transform.ToLocal(otherPoint)));
}

BoundingBox3D Surface3::BoundingBox() const
{
    return transform.ToWorld(BoundingBoxLocal());
}

bool Surface3::Intersects(const Ray3D& ray) const
{
    return IntersectsLocal(transform.ToLocal(ray));
}

double Surface3::ClosestDistance(const Vector3D& otherPoint) const
{
    return ClosestDistanceLocal(transform.ToLocal(otherPoint));
}

SurfaceRayIntersection3 Surface3::ClosestIntersection(const Ray3D& ray) const
{
    SurfaceRayIntersection3 result =
        ClosestIntersectionLocal(transform.ToLocal(ray));

    result.point = transform.ToWorld(result.point);
    result.normal = transform.ToWorldDirection(result.normal);
    result.normal *= (isNormalFlipped) ? -1.0 : 1.0;

    return result;
}

Vector3D Surface3::ClosestNormal(const Vector3D& otherPoint) const
{
    Vector3D result = transform.ToWorldDirection(
        ClosestNormalLocal(transform.ToLocal(otherPoint)));
    result *= (isNormalFlipped) ? -1.0 : 1.0;
    return result;
}

bool Surface3::IntersectsLocal(const Ray3D& ray) const
{
    const SurfaceRayIntersection3 result = ClosestIntersectionLocal(ray);
    return result.isIntersecting;
}

void Surface3::UpdateQueryEngine()
{
    // Do nothing
}

bool Surface3::IsBounded() const
{
    return true;
}

bool Surface3::IsValidGeometry() const
{
    return true;
}

bool Surface3::IsInside(const Vector3D& otherPoint) const
{
    return isNormalFlipped == !IsInsideLocal(transform.ToLocal(otherPoint));
}

double Surface3::ClosestDistanceLocal(const Vector3D& otherPoint) const
{
    return otherPoint.DistanceTo(ClosestPointLocal(otherPoint));
}

bool Surface3::IsInsideLocal(const Vector3D& otherPointLocal) const
{
    const Vector3D cpLocal = ClosestPointLocal(otherPointLocal);
    const Vector3D normalLocal = ClosestNormalLocal(otherPointLocal);
    return (otherPointLocal - cpLocal).Dot(normalLocal) < 0.0;
}
}  // namespace CubbyFlow