// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Geometry/Plane3.hpp>

namespace CubbyFlow
{
Plane3::Plane3(const Transform3& transform_, bool isNormalFlipped_)
    : Surface3(transform_, isNormalFlipped_)
{
    // Do nothing
}

Plane3::Plane3(const Vector3D& normal, const Vector3D& point,
               const Transform3& transform_, bool isNormalFlipped_)
    : Surface3(transform_, isNormalFlipped_), normal(normal), point(point)
{
    // Do nothing
}

Plane3::Plane3(const Vector3D& point0, const Vector3D& point1,
               const Vector3D& point2, const Transform3& transform_,
               bool isNormalFlipped_)
    : Surface3(transform_, isNormalFlipped_)
{
    normal = (point1 - point0).Cross(point2 - point0).Normalized();
    point = point0;
}

Plane3::Plane3(const Plane3& other)
    : Surface3(other), normal(other.normal), point(other.point)
{
    // Do nothing
}

Vector3D Plane3::ClosestPointLocal(const Vector3D& otherPoint) const
{
    Vector3D r = otherPoint - point;
    return r - normal.Dot(r) * normal + point;
}

Vector3D Plane3::ClosestNormalLocal(const Vector3D& otherPoint) const
{
    UNUSED_VARIABLE(otherPoint);

    return normal;
}

bool Plane3::IntersectsLocal(const Ray3D& ray) const
{
    return std::fabs(ray.direction.Dot(normal)) > 0;
}

SurfaceRayIntersection3 Plane3::ClosestIntersectionLocal(const Ray3D& ray) const
{
    SurfaceRayIntersection3 intersection;

    double dDotN = ray.direction.Dot(normal);

    // Check if not parallel
    if (std::fabs(dDotN) > 0)
    {
        double t = normal.Dot(point - ray.origin) / dDotN;
        if (t >= 0.0)
        {
            intersection.isIntersecting = true;
            intersection.distance = t;
            intersection.point = ray.PointAt(t);
            intersection.normal = normal;
        }
    }

    return intersection;
}

BoundingBox3D Plane3::BoundingBoxLocal() const
{
    static const double eps = std::numeric_limits<double>::epsilon();
    static const double dmax = std::numeric_limits<double>::max();

    if (std::fabs(normal.Dot(Vector3D(1, 0, 0)) - 1.0) < eps)
    {
        return BoundingBox3D(point - Vector3D(0, dmax, dmax),
                             point + Vector3D(0, dmax, dmax));
    }

    if (std::fabs(normal.Dot(Vector3D(0, 1, 0)) - 1.0) < eps)
    {
        return BoundingBox3D(point - Vector3D(dmax, 0, dmax),
                             point + Vector3D(dmax, 0, dmax));
    }

    if (std::fabs(normal.Dot(Vector3D(0, 0, 1)) - 1.0) < eps)
    {
        return BoundingBox3D(point - Vector3D(dmax, dmax, 0),
                             point + Vector3D(dmax, dmax, 0));
    }

    return BoundingBox3D(Vector3D(dmax, dmax, dmax),
                         Vector3D(dmax, dmax, dmax));
}

Plane3::Builder Plane3::GetBuilder()
{
    return Builder();
}

Plane3::Builder& Plane3::Builder::WithNormal(const Vector3D& normal)
{
    m_normal = normal;
    return *this;
}

Plane3::Builder& Plane3::Builder::WithPoint(const Vector3D& point)
{
    m_point = point;
    return *this;
}

Plane3 Plane3::Builder::Build() const
{
    return Plane3(m_normal, m_point, m_transform, m_isNormalFlipped);
}

Plane3Ptr Plane3::Builder::MakeShared() const
{
    return std::shared_ptr<Plane3>(
        new Plane3(m_normal, m_point, m_transform, m_isNormalFlipped),
        [](Plane3* obj) { delete obj; });
}
}  // namespace CubbyFlow