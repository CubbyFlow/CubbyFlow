// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Geometry/SurfaceToImplicit2.hpp>

#include <utility>

namespace CubbyFlow
{
SurfaceToImplicit2::SurfaceToImplicit2(Surface2Ptr surface,
                                       const Transform2& _transform,
                                       bool _isNormalFlipped)
    : ImplicitSurface2{ _transform, _isNormalFlipped },
      m_surface{ std::move(surface) }
{
    // Do nothing
}

bool SurfaceToImplicit2::IsBounded() const
{
    return m_surface->IsBounded();
}

void SurfaceToImplicit2::UpdateQueryEngine()
{
    m_surface->UpdateQueryEngine();
}

bool SurfaceToImplicit2::IsValidGeometry() const
{
    return m_surface->IsValidGeometry();
}

Surface2Ptr SurfaceToImplicit2::GetSurface() const
{
    return m_surface;
}

SurfaceToImplicit2::Builder SurfaceToImplicit2::GetBuilder()
{
    return Builder{};
}

Vector2D SurfaceToImplicit2::ClosestPointLocal(const Vector2D& otherPoint) const
{
    return m_surface->ClosestPoint(otherPoint);
}

double SurfaceToImplicit2::ClosestDistanceLocal(
    const Vector2D& otherPoint) const
{
    return m_surface->ClosestDistance(otherPoint);
}

bool SurfaceToImplicit2::IntersectsLocal(const Ray2D& ray) const
{
    return m_surface->Intersects(ray);
}

BoundingBox2D SurfaceToImplicit2::BoundingBoxLocal() const
{
    return m_surface->BoundingBox();
}

Vector2D SurfaceToImplicit2::ClosestNormalLocal(
    const Vector2D& otherPoint) const
{
    return m_surface->ClosestNormal(otherPoint);
}

double SurfaceToImplicit2::SignedDistanceLocal(const Vector2D& otherPoint) const
{
    const Vector2D x = m_surface->ClosestPoint(otherPoint);
    const bool inside = m_surface->IsInside(otherPoint);
    return inside ? -x.DistanceTo(otherPoint) : x.DistanceTo(otherPoint);
}

SurfaceRayIntersection2 SurfaceToImplicit2::ClosestIntersectionLocal(
    const Ray2D& ray) const
{
    return m_surface->ClosestIntersection(ray);
}

bool SurfaceToImplicit2::IsInsideLocal(const Vector2D& otherPoint) const
{
    return m_surface->IsInside(otherPoint);
}

SurfaceToImplicit2::Builder& SurfaceToImplicit2::Builder::WithSurface(
    const Surface2Ptr& surface)
{
    m_surface = surface;
    return *this;
}

SurfaceToImplicit2 SurfaceToImplicit2::Builder::Build() const
{
    return SurfaceToImplicit2{ m_surface, m_transform, m_isNormalFlipped };
}

SurfaceToImplicit2Ptr SurfaceToImplicit2::Builder::MakeShared() const
{
    return std::shared_ptr<SurfaceToImplicit2>(
        new SurfaceToImplicit2{ m_surface, m_transform, m_isNormalFlipped },
        [](SurfaceToImplicit2* obj) { delete obj; });
}
}  // namespace CubbyFlow