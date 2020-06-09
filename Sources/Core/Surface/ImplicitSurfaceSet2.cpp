// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Surface/ImplicitSurfaceSet2.hpp>
#include <Core/Surface/SurfaceToImplicit2.hpp>

namespace CubbyFlow
{
ImplicitSurfaceSet2::ImplicitSurfaceSet2()
{
    // Do nothing
}

ImplicitSurfaceSet2::ImplicitSurfaceSet2(
    const std::vector<ImplicitSurface2Ptr>& surfaces,
    const Transform2& transform, bool isNormalFlipped)
    : ImplicitSurface2(transform, isNormalFlipped), m_surfaces(surfaces)
{
    for (const auto& surface : m_surfaces)
    {
        if (!surface->IsBounded())
        {
            m_unboundedSurfaces.push_back(surface);
        }
    }

    InvalidateBVH();
}

ImplicitSurfaceSet2::ImplicitSurfaceSet2(
    const std::vector<Surface2Ptr>& surfaces, const Transform2& transform,
    bool isNormalFlipped)
    : ImplicitSurface2(transform, isNormalFlipped)
{
    for (const auto& surface : surfaces)
    {
        AddExplicitSurface(surface);
    }
}

ImplicitSurfaceSet2::ImplicitSurfaceSet2(const ImplicitSurfaceSet2& other)
    : ImplicitSurface2(other),
      m_surfaces(other.m_surfaces),
      m_unboundedSurfaces(other.m_unboundedSurfaces)
{
    // Do nothing
}

void ImplicitSurfaceSet2::UpdateQueryEngine()
{
    BuildBVH();
}

bool ImplicitSurfaceSet2::IsValidGeometry() const
{
    // All surfaces should be valid.
    for (const auto& surface : m_surfaces)
    {
        if (!surface->IsValidGeometry())
        {
            return false;
        }
    }

    // Empty set is not valid.
    return !m_surfaces.empty();
}

size_t ImplicitSurfaceSet2::NumberOfSurfaces() const
{
    return m_surfaces.size();
}

const ImplicitSurface2Ptr& ImplicitSurfaceSet2::SurfaceAt(size_t i) const
{
    return m_surfaces[i];
}

void ImplicitSurfaceSet2::AddExplicitSurface(const Surface2Ptr& surface)
{
    AddSurface(std::make_shared<SurfaceToImplicit2>(surface));
}

void ImplicitSurfaceSet2::AddSurface(const ImplicitSurface2Ptr& surface)
{
    m_surfaces.push_back(surface);

    if (!surface->IsBounded())
    {
        m_unboundedSurfaces.push_back(surface);
    }

    InvalidateBVH();
}

Vector2D ImplicitSurfaceSet2::ClosestPointLocal(
    const Vector2D& otherPoint) const
{
    BuildBVH();

    const auto distanceFunc = [](const Surface2Ptr& surface,
                                 const Vector2D& pt) {
        return surface->ClosestDistance(pt);
    };

    Vector2D result{ std::numeric_limits<double>::max(),
                     std::numeric_limits<double>::max() };

    const auto queryResult = m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);
    if (queryResult.item != nullptr)
    {
        result = (*queryResult.item)->ClosestPoint(otherPoint);
    }

    double minDist = queryResult.distance;
    for (const auto& surface : m_unboundedSurfaces)
    {
        auto pt = surface->ClosestPoint(otherPoint);
        const double dist = pt.DistanceTo(otherPoint);

        if (dist < minDist)
        {
            minDist = dist;
            result = surface->ClosestPoint(otherPoint);
        }
    }

    return result;
}

double ImplicitSurfaceSet2::ClosestDistanceLocal(
    const Vector2D& otherPoint) const
{
    BuildBVH();

    const auto distanceFunc = [](const Surface2Ptr& surface,
                                 const Vector2D& pt) {
        return surface->ClosestDistance(pt);
    };

    const auto queryResult = m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);

    double minDist = queryResult.distance;
    for (const auto& surface : m_unboundedSurfaces)
    {
        auto pt = surface->ClosestPoint(otherPoint);
        const double dist = pt.DistanceTo(otherPoint);

        if (dist < minDist)
        {
            minDist = dist;
        }
    }

    return minDist;
}

Vector2D ImplicitSurfaceSet2::ClosestNormalLocal(
    const Vector2D& otherPoint) const
{
    BuildBVH();

    const auto distanceFunc = [](const Surface2Ptr& surface,
                                 const Vector2D& pt) {
        return surface->ClosestDistance(pt);
    };

    Vector2D result{ 1.0, 0.0 };

    const auto queryResult = m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);
    if (queryResult.item != nullptr)
    {
        result = (*queryResult.item)->ClosestNormal(otherPoint);
    }

    double minDist = queryResult.distance;
    for (const auto& surface : m_unboundedSurfaces)
    {
        auto pt = surface->ClosestPoint(otherPoint);
        const double dist = pt.DistanceTo(otherPoint);

        if (dist < minDist)
        {
            minDist = dist;
            result = surface->ClosestNormal(otherPoint);
        }
    }

    return result;
}

bool ImplicitSurfaceSet2::IntersectsLocal(const Ray2D& ray) const
{
    BuildBVH();

    const auto testFunc = [](const Surface2Ptr& surface, const Ray2D& ray) {
        return surface->Intersects(ray);
    };

    bool result = m_bvh.IsIntersects(ray, testFunc);
    for (const auto& surface : m_unboundedSurfaces)
    {
        result |= surface->Intersects(ray);
    }

    return result;
}

SurfaceRayIntersection2 ImplicitSurfaceSet2::ClosestIntersectionLocal(
    const Ray2D& ray) const
{
    BuildBVH();

    const auto testFunc = [](const Surface2Ptr& surface, const Ray2D& ray) {
        SurfaceRayIntersection2 result = surface->ClosestIntersection(ray);
        return result.distance;
    };

    const auto queryResult = m_bvh.GetClosestIntersection(ray, testFunc);

    SurfaceRayIntersection2 result;
    result.distance = queryResult.distance;
    result.isIntersecting = queryResult.item != nullptr;

    if (queryResult.item != nullptr)
    {
        result.point = ray.PointAt(queryResult.distance);
        result.normal = (*queryResult.item)->ClosestNormal(result.point);
    }

    for (const auto& surface : m_unboundedSurfaces)
    {
        const SurfaceRayIntersection2 localResult =
            surface->ClosestIntersection(ray);
        if (localResult.distance < result.distance)
        {
            result = localResult;
        }
    }

    return result;
}

BoundingBox2D ImplicitSurfaceSet2::BoundingBoxLocal() const
{
    BuildBVH();

    return m_bvh.GetBoundingBox();
}

double ImplicitSurfaceSet2::SignedDistanceLocal(
    const Vector2D& otherPoint) const
{
    double sdf = std::numeric_limits<double>::max();

    for (const auto& surface : m_surfaces)
    {
        sdf = std::min(sdf, surface->SignedDistance(otherPoint));
    }

    return sdf;
}

void ImplicitSurfaceSet2::InvalidateBVH() const
{
    m_bvhInvalidated = true;
}

void ImplicitSurfaceSet2::BuildBVH() const
{
    if (m_bvhInvalidated)
    {
        std::vector<BoundingBox2D> bounds;

        for (const auto& surface : m_surfaces)
        {
            if (surface->IsBounded())
            {
                bounds.push_back(surface->BoundingBox());
            }
        }

        m_bvh.Build(m_surfaces, bounds);
        m_bvhInvalidated = false;
    }
}

ImplicitSurfaceSet2::Builder ImplicitSurfaceSet2::GetBuilder()
{
    return Builder();
}

ImplicitSurfaceSet2::Builder& ImplicitSurfaceSet2::Builder::WithSurfaces(
    const std::vector<ImplicitSurface2Ptr>& surfaces)
{
    m_surfaces = surfaces;
    return *this;
}

ImplicitSurfaceSet2::Builder&
ImplicitSurfaceSet2::Builder::WithExplicitSurfaces(
    const std::vector<Surface2Ptr>& surfaces)
{
    m_surfaces.clear();

    for (const auto& surface : surfaces)
    {
        m_surfaces.push_back(std::make_shared<SurfaceToImplicit2>(surface));
    }

    return *this;
}

ImplicitSurfaceSet2 ImplicitSurfaceSet2::Builder::Build() const
{
    return ImplicitSurfaceSet2(m_surfaces, m_transform, m_isNormalFlipped);
}

ImplicitSurfaceSet2Ptr ImplicitSurfaceSet2::Builder::MakeShared() const
{
    return std::shared_ptr<ImplicitSurfaceSet2>(
        new ImplicitSurfaceSet2(m_surfaces, m_transform, m_isNormalFlipped),
        [](ImplicitSurfaceSet2* obj) { delete obj; });
}
}  // namespace CubbyFlow