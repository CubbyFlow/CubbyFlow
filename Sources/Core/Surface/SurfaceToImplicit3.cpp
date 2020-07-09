// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Geometry/TriangleMesh3.hpp>
#include <Core/Surface/SurfaceToImplicit3.hpp>
#include <Core/Utils/Logging.hpp>

namespace CubbyFlow
{
SurfaceToImplicit3::SurfaceToImplicit3(const Surface3Ptr& surface,
                                       const Transform3& transform,
                                       bool isNormalFlipped)
    : ImplicitSurface3(transform, isNormalFlipped), m_surface(surface)
{
    if (std::dynamic_pointer_cast<TriangleMesh3>(surface) != nullptr)
    {
        CUBBYFLOW_WARN
            << "Using TriangleMesh3 with SurfaceToImplicit3 can cause "
            << "undefined behavior. Use ImplicitTriangleMesh3 instead.";
    }
}

SurfaceToImplicit3::SurfaceToImplicit3(const SurfaceToImplicit3& other)
    : ImplicitSurface3(other), m_surface(other.m_surface)
{
    // Do nothing
}

bool SurfaceToImplicit3::IsBounded() const
{
    return m_surface->IsBounded();
}

bool SurfaceToImplicit3::IsValidGeometry() const
{
    return m_surface->IsValidGeometry();
}

Surface3Ptr SurfaceToImplicit3::GetSurface() const
{
    return m_surface;
}

SurfaceToImplicit3::Builder SurfaceToImplicit3::GetBuilder()
{
    return Builder();
}

Vector3D SurfaceToImplicit3::ClosestPointLocal(const Vector3D& otherPoint) const
{
    return m_surface->ClosestPoint(otherPoint);
}

double SurfaceToImplicit3::ClosestDistanceLocal(
    const Vector3D& otherPoint) const
{
    return m_surface->ClosestDistance(otherPoint);
}

bool SurfaceToImplicit3::IntersectsLocal(const Ray3D& ray) const
{
    return m_surface->Intersects(ray);
}

BoundingBox3D SurfaceToImplicit3::BoundingBoxLocal() const
{
    return m_surface->BoundingBox();
}

Vector3D SurfaceToImplicit3::ClosestNormalLocal(
    const Vector3D& otherPoint) const
{
    return m_surface->ClosestNormal(otherPoint);
}

double SurfaceToImplicit3::SignedDistanceLocal(const Vector3D& otherPoint) const
{
    Vector3D x = m_surface->ClosestPoint(otherPoint);
    Vector3D n = m_surface->ClosestNormal(otherPoint);

    n = (isNormalFlipped) ? -n : n;

    if (n.Dot(otherPoint - x) < 0.0)
    {
        return -x.DistanceTo(otherPoint);
    }

    return x.DistanceTo(otherPoint);
}

SurfaceRayIntersection3 SurfaceToImplicit3::ClosestIntersectionLocal(
    const Ray3D& ray) const
{
    return m_surface->ClosestIntersection(ray);
}

bool SurfaceToImplicit3::IsInsideLocal(const Vector3D& otherPoint) const
{
    return m_surface->IsInside(otherPoint);
}

SurfaceToImplicit3::Builder& SurfaceToImplicit3::Builder::WithSurface(
    const Surface3Ptr& surface)
{
    m_surface = surface;
    return *this;
}

SurfaceToImplicit3 SurfaceToImplicit3::Builder::Build() const
{
    return SurfaceToImplicit3(m_surface, m_transform, m_isNormalFlipped);
}

SurfaceToImplicit3Ptr SurfaceToImplicit3::Builder::MakeShared() const
{
    return std::shared_ptr<SurfaceToImplicit3>(
        new SurfaceToImplicit3(m_surface, m_transform, m_isNormalFlipped),
        [](SurfaceToImplicit3* obj) { delete obj; });
}
}  // namespace CubbyFlow
