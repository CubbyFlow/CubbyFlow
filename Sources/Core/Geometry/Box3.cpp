// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Geometry/Box3.hpp>
#include <Core/Geometry/Plane3.hpp>

namespace CubbyFlow
{
Box3::Box3(const Transform3& transform, bool isNormalFlipped)
    : Surface3(transform, isNormalFlipped)
{
    // Do nothing
}

Box3::Box3(const Vector3D& lowerCorner, const Vector3D& upperCorner,
           const Transform3& transform, bool isNormalFlipped)
    : Box3(BoundingBox3D(lowerCorner, upperCorner), transform, isNormalFlipped)
{
    // Do nothing
}

Box3::Box3(const BoundingBox3D& boundingBox, const Transform3& transform,
           bool isNormalFlipped)
    : Surface3(transform, isNormalFlipped), bound(boundingBox)
{
    // Do nothing
}

Box3::Box3(const Box3& other) : Surface3(other), bound(other.bound)
{
    // Do nothing
}

Vector3D Box3::ClosestPointLocal(const Vector3D& otherPoint) const
{
    if (bound.Contains(otherPoint))
    {
        Plane3 planes[6] = { Plane3(Vector3D(1, 0, 0), bound.upperCorner),
                             Plane3(Vector3D(0, 1, 0), bound.upperCorner),
                             Plane3(Vector3D(0, 0, 1), bound.upperCorner),
                             Plane3(Vector3D(-1, 0, 0), bound.lowerCorner),
                             Plane3(Vector3D(0, -1, 0), bound.lowerCorner),
                             Plane3(Vector3D(0, 0, -1), bound.lowerCorner) };

        Vector3D result = planes[0].ClosestPoint(otherPoint);
        double distanceSquared = result.DistanceSquaredTo(otherPoint);

        for (int i = 1; i < 6; ++i)
        {
            Vector3D localResult = planes[i].ClosestPoint(otherPoint);
            double localDistanceSquared =
                localResult.DistanceSquaredTo(otherPoint);

            if (localDistanceSquared < distanceSquared)
            {
                result = localResult;
                distanceSquared = localDistanceSquared;
            }
        }

        return result;
    }

    return Clamp(otherPoint, bound.lowerCorner, bound.upperCorner);
}

Vector3D Box3::ClosestNormalLocal(const Vector3D& otherPoint) const
{
    Plane3 planes[6] = { Plane3(Vector3D(1, 0, 0), bound.upperCorner),
                         Plane3(Vector3D(0, 1, 0), bound.upperCorner),
                         Plane3(Vector3D(0, 0, 1), bound.upperCorner),
                         Plane3(Vector3D(-1, 0, 0), bound.lowerCorner),
                         Plane3(Vector3D(0, -1, 0), bound.lowerCorner),
                         Plane3(Vector3D(0, 0, -1), bound.lowerCorner) };

    if (bound.Contains(otherPoint))
    {
        Vector3D closestNormal = planes[0].normal;
        Vector3D closestPoint = planes[0].ClosestPoint(otherPoint);
        double minDistanceSquared = (closestPoint - otherPoint).LengthSquared();

        for (int i = 1; i < 6; ++i)
        {
            Vector3D localClosestPoint = planes[i].ClosestPoint(otherPoint);
            double localDistanceSquared =
                (localClosestPoint - otherPoint).LengthSquared();

            if (localDistanceSquared < minDistanceSquared)
            {
                closestNormal = planes[i].normal;
                minDistanceSquared = localDistanceSquared;
            }
        }

        return closestNormal;
    }

    Vector3D closestPoint =
        Clamp(otherPoint, bound.lowerCorner, bound.upperCorner);
    Vector3D closestPointToInputPoint = otherPoint - closestPoint;
    Vector3D closestNormal = planes[0].normal;
    double maxCosineAngle = closestNormal.Dot(closestPointToInputPoint);

    for (int i = 1; i < 6; ++i)
    {
        double cosineAngle = planes[i].normal.Dot(closestPointToInputPoint);

        if (cosineAngle > maxCosineAngle)
        {
            closestNormal = planes[i].normal;
            maxCosineAngle = cosineAngle;
        }
    }

    return closestNormal;
}

bool Box3::IntersectsLocal(const Ray3D& ray) const
{
    return bound.Intersects(ray);
}

SurfaceRayIntersection3 Box3::ClosestIntersectionLocal(const Ray3D& ray) const
{
    SurfaceRayIntersection3 intersection;
    BoundingBoxRayIntersection3D bbRayIntersection =
        bound.ClosestIntersection(ray);
    intersection.isIntersecting = bbRayIntersection.isIntersecting;

    if (intersection.isIntersecting)
    {
        intersection.distance = bbRayIntersection.near;
        intersection.point = ray.PointAt(bbRayIntersection.near);
        intersection.normal = Box3::ClosestNormal(intersection.point);
    }

    return intersection;
}

BoundingBox3D Box3::BoundingBoxLocal() const
{
    return bound;
}

Box3::Builder Box3::GetBuilder()
{
    return Builder();
}

Box3::Builder& Box3::Builder::WithLowerCorner(const Vector3D& pt)
{
    m_lowerCorner = pt;
    return *this;
}

Box3::Builder& Box3::Builder::WithUpperCorner(const Vector3D& pt)
{
    m_upperCorner = pt;
    return *this;
}

Box3::Builder& Box3::Builder::WithBoundingBox(const BoundingBox3D& bbox)
{
    m_lowerCorner = bbox.lowerCorner;
    m_upperCorner = bbox.upperCorner;
    return *this;
}

Box3 Box3::Builder::Build() const
{
    return Box3(m_lowerCorner, m_upperCorner, m_transform, m_isNormalFlipped);
}

Box3Ptr Box3::Builder::MakeShared() const
{
    return std::shared_ptr<Box3>(
        new Box3(m_lowerCorner, m_upperCorner, m_transform, m_isNormalFlipped),
        [](Box3* obj) { delete obj; });
}
}  // namespace CubbyFlow