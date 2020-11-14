// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Surface/ImplicitSurfaceSet3.hpp>
#include <Core/Surface/SurfaceToImplicit3.hpp>

#include <utility>

namespace CubbyFlow
{
ImplicitSurfaceSet3::ImplicitSurfaceSet3(
    std::vector<ImplicitSurface3Ptr> surfaces, const Transform3& _transform,
    bool _isNormalFlipped)
    : ImplicitSurface3{ _transform, _isNormalFlipped },
      m_surfaces{ std::move(surfaces) }
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

ImplicitSurfaceSet3::ImplicitSurfaceSet3(
    const std::vector<Surface3Ptr>& surfaces, const Transform3& _transform,
    bool _isNormalFlipped)
    : ImplicitSurface3{ _transform, _isNormalFlipped }
{
    for (const auto& surface : surfaces)
    {
        AddExplicitSurface(surface);
    }
}

ImplicitSurfaceSet3::ImplicitSurfaceSet3(const ImplicitSurfaceSet3& other)
    : ImplicitSurface3{ other },
      m_surfaces(other.m_surfaces),
      m_unboundedSurfaces(other.m_unboundedSurfaces)
{
    // Do nothing
}

ImplicitSurfaceSet3::ImplicitSurfaceSet3(ImplicitSurfaceSet3&& other) noexcept
    : ImplicitSurface3{ std::move(other) },
      m_surfaces(std::move(other.m_surfaces)),
      m_unboundedSurfaces(std::move(other.m_unboundedSurfaces))
{
    // Do nothing
}

ImplicitSurfaceSet3& ImplicitSurfaceSet3::operator=(
    const ImplicitSurfaceSet3& other)
{
    ImplicitSurface3::operator=(other);

    m_surfaces = other.m_surfaces;
    m_unboundedSurfaces = other.m_unboundedSurfaces;

    return *this;
}

void ImplicitSurfaceSet3::UpdateQueryEngine()
{
    InvalidateBVH();
    BuildBVH();
}

bool ImplicitSurfaceSet3::IsBounded() const
{
    // All surfaces should be bounded
    for (const auto& surface : m_surfaces)
    {
        if (!surface->IsBounded())
        {
            return false;
        }
    }

    // Empty set is not bounded
    return !m_surfaces.empty();
}

bool ImplicitSurfaceSet3::IsValidGeometry() const
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

size_t ImplicitSurfaceSet3::NumberOfSurfaces() const
{
    return m_surfaces.size();
}

const ImplicitSurface3Ptr& ImplicitSurfaceSet3::SurfaceAt(size_t i) const
{
    return m_surfaces[i];
}

void ImplicitSurfaceSet3::AddExplicitSurface(const Surface3Ptr& surface)
{
    AddSurface(std::make_shared<SurfaceToImplicit3>(surface));
}

void ImplicitSurfaceSet3::AddSurface(const ImplicitSurface3Ptr& surface)
{
    m_surfaces.push_back(surface);

    if (!surface->IsBounded())
    {
        m_unboundedSurfaces.push_back(surface);
    }

    InvalidateBVH();
}

Vector3D ImplicitSurfaceSet3::ClosestPointLocal(
    const Vector3D& otherPoint) const
{
    BuildBVH();

    const auto distanceFunc = [](const Surface3Ptr& surface,
                                 const Vector3D& pt) {
        return surface->ClosestDistance(pt);
    };

    Vector3D result{ std::numeric_limits<double>::max(),
                     std::numeric_limits<double>::max(),
                     std::numeric_limits<double>::max() };

    const NearestNeighborQueryResult3<ImplicitSurface3Ptr> queryResult =
        m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);
    if (queryResult.item != nullptr)
    {
        result = (*queryResult.item)->ClosestPoint(otherPoint);
    }

    double minDist = queryResult.distance;
    for (const auto& surface : m_unboundedSurfaces)
    {
        Vector3D pt = surface->ClosestPoint(otherPoint);
        const double dist = pt.DistanceTo(otherPoint);

        if (dist < minDist)
        {
            minDist = dist;
            result = surface->ClosestPoint(otherPoint);
        }
    }

    return result;
}

Vector3D ImplicitSurfaceSet3::ClosestNormalLocal(
    const Vector3D& otherPoint) const
{
    BuildBVH();

    const auto distanceFunc = [](const Surface3Ptr& surface,
                                 const Vector3D& pt) {
        return surface->ClosestDistance(pt);
    };

    Vector3D result{ 1.0, 0.0, 0.0 };

    const NearestNeighborQueryResult3<ImplicitSurface3Ptr> queryResult =
        m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);
    if (queryResult.item != nullptr)
    {
        result = (*queryResult.item)->ClosestNormal(otherPoint);
    }

    double minDist = queryResult.distance;
    for (const auto& surface : m_unboundedSurfaces)
    {
        Vector3D pt = surface->ClosestPoint(otherPoint);
        const double dist = pt.DistanceTo(otherPoint);

        if (dist < minDist)
        {
            minDist = dist;
            result = surface->ClosestNormal(otherPoint);
        }
    }

    return result;
}

double ImplicitSurfaceSet3::ClosestDistanceLocal(
    const Vector3D& otherPoint) const
{
    BuildBVH();

    const auto distanceFunc = [](const Surface3Ptr& surface,
                                 const Vector3D& pt) {
        return surface->ClosestDistance(pt);
    };

    const NearestNeighborQueryResult3<ImplicitSurface3Ptr> queryResult =
        m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);

    double minDist = queryResult.distance;
    for (const auto& surface : m_unboundedSurfaces)
    {
        Vector3D pt = surface->ClosestPoint(otherPoint);
        const double dist = pt.DistanceTo(otherPoint);

        if (dist < minDist)
        {
            minDist = dist;
        }
    }

    return minDist;
}

bool ImplicitSurfaceSet3::IntersectsLocal(const Ray3D& ray) const
{
    BuildBVH();

    const auto testFunc = [](const Surface3Ptr& surface, const Ray3D& _ray) {
        return surface->Intersects(_ray);
    };

    bool result = m_bvh.IsIntersects(ray, testFunc);
    for (const auto& surface : m_unboundedSurfaces)
    {
        result |= surface->Intersects(ray);
    }

    return result;
}

SurfaceRayIntersection3 ImplicitSurfaceSet3::ClosestIntersectionLocal(
    const Ray3D& ray) const
{
    BuildBVH();

    const auto testFunc = [](const Surface3Ptr& surface, const Ray3D& _ray) {
        const SurfaceRayIntersection3 result =
            surface->ClosestIntersection(_ray);
        return result.distance;
    };

    const ClosestIntersectionQueryResult3<ImplicitSurface3Ptr> queryResult =
        m_bvh.GetClosestIntersection(ray, testFunc);

    SurfaceRayIntersection3 result;
    result.distance = queryResult.distance;
    result.isIntersecting = queryResult.item != nullptr;

    if (queryResult.item != nullptr)
    {
        result.point = ray.PointAt(queryResult.distance);
        result.normal = (*queryResult.item)->ClosestNormal(result.point);
    }

    for (const auto& surface : m_unboundedSurfaces)
    {
        const SurfaceRayIntersection3 localResult =
            surface->ClosestIntersection(ray);
        if (localResult.distance < result.distance)
        {
            result = localResult;
        }
    }

    return result;
}

BoundingBox3D ImplicitSurfaceSet3::BoundingBoxLocal() const
{
    BuildBVH();

    return m_bvh.GetBoundingBox();
}

bool ImplicitSurfaceSet3::IsInsideLocal(const Vector3D& otherPoint) const
{
    for (const auto& surface : m_surfaces)
    {
        if (surface->IsInside(otherPoint))
        {
            return true;
        }
    }

    return false;
}

double ImplicitSurfaceSet3::SignedDistanceLocal(
    const Vector3D& otherPoint) const
{
    double sdf = std::numeric_limits<double>::max();

    for (const auto& surface : m_surfaces)
    {
        sdf = std::min(sdf, surface->SignedDistance(otherPoint));
    }

    return sdf;
}

void ImplicitSurfaceSet3::InvalidateBVH() const
{
    m_bvhInvalidated = true;
}

void ImplicitSurfaceSet3::BuildBVH() const
{
    if (m_bvhInvalidated)
    {
        std::vector<ImplicitSurface3Ptr> surfs;
        std::vector<BoundingBox3D> bounds;

        for (const auto& surface : m_surfaces)
        {
            if (surface->IsBounded())
            {
                surfs.push_back(surface);
                bounds.push_back(surface->BoundingBox());
            }
        }

        m_bvh.Build(surfs, bounds);
        m_bvhInvalidated = false;
    }
}

ImplicitSurfaceSet3::Builder ImplicitSurfaceSet3::GetBuilder()
{
    return Builder{};
}

ImplicitSurfaceSet3::Builder& ImplicitSurfaceSet3::Builder::WithSurfaces(
    const std::vector<ImplicitSurface3Ptr>& surfaces)
{
    m_surfaces = surfaces;
    return *this;
}

ImplicitSurfaceSet3::Builder&
ImplicitSurfaceSet3::Builder::WithExplicitSurfaces(
    const std::vector<Surface3Ptr>& surfaces)
{
    m_surfaces.clear();

    for (const auto& surface : surfaces)
    {
        m_surfaces.push_back(std::make_shared<SurfaceToImplicit3>(surface));
    }

    return *this;
}

ImplicitSurfaceSet3 ImplicitSurfaceSet3::Builder::Build() const
{
    return ImplicitSurfaceSet3{ m_surfaces, m_transform, m_isNormalFlipped };
}

ImplicitSurfaceSet3Ptr ImplicitSurfaceSet3::Builder::MakeShared() const
{
    return std::shared_ptr<ImplicitSurfaceSet3>(
        new ImplicitSurfaceSet3{ m_surfaces, m_transform, m_isNormalFlipped },
        [](ImplicitSurfaceSet3* obj) { delete obj; });
}
}  // namespace CubbyFlow