// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Geometry/Sphere3.hpp>
#include <Core/Math/MathUtils.hpp>

namespace CubbyFlow
{
Sphere3::Sphere3(const Transform3& _transform, bool _isNormalFlipped)
    : Surface3{ _transform, _isNormalFlipped }
{
    // Do nothing
}

Sphere3::Sphere3(const Vector3D& _center, double _radius,
                 const Transform3& _transform, bool _isNormalFlipped)
    : Surface3{ _transform, _isNormalFlipped }, center(_center), radius(_radius)
{
    // Do nothing
}

Vector3D Sphere3::ClosestPointLocal(const Vector3D& otherPoint) const
{
    return radius * ClosestNormalLocal(otherPoint) + center;
}

double Sphere3::ClosestDistanceLocal(const Vector3D& otherPoint) const
{
    return std::fabs(center.DistanceTo(otherPoint) - radius);
}

Vector3D Sphere3::ClosestNormalLocal(const Vector3D& otherPoint) const
{
    if (center.IsSimilar(otherPoint))
    {
        return Vector3D{ 1, 0, 0 };
    }

    return (otherPoint - center).Normalized();
}

bool Sphere3::IntersectsLocal(const Ray3D& ray) const
{
    const Vector3D r = ray.origin - center;
    const double b = ray.direction.Dot(r);
    const double c = r.LengthSquared() - Square(radius);
    double d = b * b - c;

    if (d > 0.0)
    {
        d = std::sqrt(d);
        double min = -b - d;
        const double max = -b + d;

        if (min < 0.0)
        {
            min = max;
        }

        if (min >= 0.0)
        {
            return true;
        }
    }

    return false;
}

SurfaceRayIntersection3 Sphere3::ClosestIntersectionLocal(
    const Ray3D& ray) const
{
    SurfaceRayIntersection3 intersection;
    const Vector3D r = ray.origin - center;
    const double b = ray.direction.Dot(r);
    const double c = r.LengthSquared() - Square(radius);
    double d = b * b - c;

    if (d > 0.0)
    {
        d = std::sqrt(d);
        double min = -b - d;
        const double max = -b + d;

        if (min < 0.0)
        {
            min = max;
        }

        if (min < 0.0)
        {
            intersection.isIntersecting = false;
        }
        else
        {
            intersection.isIntersecting = true;
            intersection.distance = min;
            intersection.point = ray.origin + min * ray.direction;
            intersection.normal = (intersection.point - center).Normalized();
        }
    }
    else
    {
        intersection.isIntersecting = false;
    }

    return intersection;
}

BoundingBox3D Sphere3::BoundingBoxLocal() const
{
    const Vector3D r{ radius, radius, radius };
    return BoundingBox3D{ center - r, center + r };
}

Sphere3::Builder Sphere3::GetBuilder()
{
    return Builder{};
}

Sphere3::Builder& Sphere3::Builder::WithCenter(const Vector3D& _center)
{
    m_center = _center;
    return *this;
}

Sphere3::Builder& Sphere3::Builder::WithRadius(double _radius)
{
    m_radius = _radius;
    return *this;
}

Sphere3 Sphere3::Builder::Build() const
{
    return Sphere3{ m_center, m_radius, m_transform, m_isNormalFlipped };
}

Sphere3Ptr Sphere3::Builder::MakeShared() const
{
    return std::shared_ptr<Sphere3>(
        new Sphere3{ m_center, m_radius, m_transform, m_isNormalFlipped },
        [](Sphere3* obj) { delete obj; });
}
}  // namespace CubbyFlow