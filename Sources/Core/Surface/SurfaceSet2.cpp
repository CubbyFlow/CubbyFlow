// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Surface/SurfaceSet2.hpp>

namespace CubbyFlow
{
SurfaceSet2::SurfaceSet2()
{
    // Do nothing
}

SurfaceSet2::SurfaceSet2(const std::vector<Surface2Ptr>& others,
                         const Transform2& transform, bool isNormalFlipped)
    : Surface2(transform, isNormalFlipped), m_surfaces(others)
{
    InvalidateBVH();
}

SurfaceSet2::SurfaceSet2(const SurfaceSet2& other)
    : Surface2(other), m_surfaces(other.m_surfaces)
{
    InvalidateBVH();
}

void SurfaceSet2::UpdateQueryEngine()
{
    BuildBVH();
}

size_t SurfaceSet2::NumberOfSurfaces() const
{
    return m_surfaces.size();
}

const Surface2Ptr& SurfaceSet2::SurfaceAt(size_t i) const
{
    return m_surfaces[i];
}

void SurfaceSet2::AddSurface(const Surface2Ptr& surface)
{
    m_surfaces.push_back(surface);
    InvalidateBVH();
}

Vector2D SurfaceSet2::ClosestPointLocal(const Vector2D& otherPoint) const
{
    BuildBVH();

    const auto distanceFunc = [](const Surface2Ptr& surface,
                                 const Vector2D& pt) {
        return surface->ClosestDistance(pt);
    };

    const auto queryResult = m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);
    if (queryResult.item != nullptr)
    {
        return (*queryResult.item)->ClosestPoint(otherPoint);
    }

    return Vector2D{ std::numeric_limits<double>::max(),
                     std::numeric_limits<double>::max() };
}

Vector2D SurfaceSet2::ClosestNormalLocal(const Vector2D& otherPoint) const
{
    BuildBVH();

    const auto distanceFunc = [](const Surface2Ptr& surface,
                                 const Vector2D& pt) {
        return surface->ClosestDistance(pt);
    };

    const auto queryResult = m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);
    if (queryResult.item != nullptr)
    {
        return (*queryResult.item)->ClosestNormal(otherPoint);
    }

    return Vector2D{ 1.0, 0.0 };
}

double SurfaceSet2::ClosestDistanceLocal(const Vector2D& otherPoint) const
{
    BuildBVH();

    const auto distanceFunc = [](const Surface2Ptr& surface,
                                 const Vector2D& pt) {
        return surface->ClosestDistance(pt);
    };

    const auto queryResult = m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);
    return queryResult.distance;
}

bool SurfaceSet2::IntersectsLocal(const Ray2D& ray) const
{
    BuildBVH();

    const auto testFunc = [](const Surface2Ptr& surface, const Ray2D& ray) {
        return surface->Intersects(ray);
    };

    return m_bvh.IsIntersects(ray, testFunc);
}

SurfaceRayIntersection2 SurfaceSet2::ClosestIntersectionLocal(
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

    return result;
}

BoundingBox2D SurfaceSet2::BoundingBoxLocal() const
{
    BuildBVH();

    return m_bvh.GetBoundingBox();
}

SurfaceSet2::Builder SurfaceSet2::GetBuilder()
{
    return Builder();
}

void SurfaceSet2::InvalidateBVH() const
{
    m_bvhInvalidated = true;
}

void SurfaceSet2::BuildBVH() const
{
    if (m_bvhInvalidated)
    {
        std::vector<BoundingBox2D> bounds(m_surfaces.size());
        for (size_t i = 0; i < m_surfaces.size(); ++i)
        {
            bounds[i] = m_surfaces[i]->BoundingBox();
        }

        m_bvh.Build(m_surfaces, bounds);
        m_bvhInvalidated = false;
    }
}

SurfaceSet2::Builder& SurfaceSet2::Builder::WithSurfaces(
    const std::vector<Surface2Ptr>& others)
{
    m_surfaces = others;
    return *this;
}

SurfaceSet2 SurfaceSet2::Builder::Build() const
{
    return SurfaceSet2(m_surfaces, m_transform, m_isNormalFlipped);
}

SurfaceSet2Ptr SurfaceSet2::Builder::MakeShared() const
{
    return std::shared_ptr<SurfaceSet2>(
        new SurfaceSet2(m_surfaces, m_transform, m_isNormalFlipped),
        [](SurfaceSet2* obj) { delete obj; });
}
}  // namespace CubbyFlow