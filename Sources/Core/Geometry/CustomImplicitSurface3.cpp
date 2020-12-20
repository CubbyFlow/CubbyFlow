// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Geometry/CustomImplicitSurface3.hpp>
#include <Core/Utils/LevelSetUtils.hpp>

#include <utility>

#include <utility>

namespace CubbyFlow
{
CustomImplicitSurface3::CustomImplicitSurface3(
    std::function<double(const Vector3D&)> func, BoundingBox3D domain,
    double resolution, double rayMarchingResolution,
    unsigned int maxNumberOfIterations, const Transform3& _transform,
    bool _isNormalFlipped)
    : ImplicitSurface3{ _transform, _isNormalFlipped },
      m_func{ std::move(func) },
      m_domain{ std::move(domain) },
      m_resolution{ resolution },
      m_rayMarchingResolution{ rayMarchingResolution },
      m_maxNumberOfIterations{ maxNumberOfIterations }
{
    // Do nothing
}

Vector3D CustomImplicitSurface3::ClosestPointLocal(
    const Vector3D& otherPoint) const
{
    Vector3D pt = Clamp(otherPoint, m_domain.lowerCorner, m_domain.upperCorner);

    for (unsigned int iter = 0; iter < m_maxNumberOfIterations; ++iter)
    {
        const double sdf = SignedDistanceLocal(pt);

        if (std::fabs(sdf) < std::numeric_limits<double>::epsilon())
        {
            break;
        }

        Vector3D g = GradientLocal(pt);
        pt = pt - sdf * g;
    }

    return pt;
}

bool CustomImplicitSurface3::IntersectsLocal(const Ray3D& ray) const
{
    const BoundingBoxRayIntersection3D intersection =
        m_domain.ClosestIntersection(ray);

    if (intersection.isIntersecting)
    {
        double start, end;
        if (std::abs(intersection.far - std::numeric_limits<double>::max()) <
            std::numeric_limits<double>::epsilon())
        {
            start = 0.0;
            end = intersection.near;
        }
        else
        {
            start = intersection.near;
            end = intersection.far;
        }

        double t = start;
        Vector3D pt = ray.PointAt(t);
        double prevPhi = m_func(pt);

        while (t <= end)
        {
            pt = ray.PointAt(t);
            const double newPhi = m_func(pt);
            const double newPhiAbs = std::fabs(newPhi);

            if (newPhi * prevPhi < 0.0)
            {
                return true;
            }

            t += std::max(newPhiAbs, m_rayMarchingResolution);
            prevPhi = newPhi;
        }
    }

    return false;
}

BoundingBox3D CustomImplicitSurface3::BoundingBoxLocal() const
{
    return m_domain;
}

double CustomImplicitSurface3::SignedDistanceLocal(
    const Vector3D& otherPoint) const
{
    if (m_func)
    {
        return m_func(otherPoint);
    }

    return std::numeric_limits<double>::max();
}

Vector3D CustomImplicitSurface3::ClosestNormalLocal(
    const Vector3D& otherPoint) const
{
    const Vector3D pt = ClosestPointLocal(otherPoint);
    Vector3D g = GradientLocal(pt);

    if (g.LengthSquared() > 0.0)
    {
        return g.Normalized();
    }

    return g;
}

SurfaceRayIntersection3 CustomImplicitSurface3::ClosestIntersectionLocal(
    const Ray3D& ray) const
{
    SurfaceRayIntersection3 result;
    const BoundingBoxRayIntersection3D intersection =
        m_domain.ClosestIntersection(ray);

    if (intersection.isIntersecting)
    {
        double start, end;
        if (std::abs(intersection.far - std::numeric_limits<double>::max()) <
            std::numeric_limits<double>::epsilon())
        {
            start = 0.0;
            end = intersection.near;
        }
        else
        {
            start = intersection.near;
            end = intersection.far;
        }

        double t = start;
        double tPrev = t;
        Vector3D pt = ray.PointAt(t);
        double prevPhi = m_func(pt);

        while (t <= end)
        {
            pt = ray.PointAt(t);
            const double newPhi = m_func(pt);
            const double newPhiAbs = std::fabs(newPhi);

            if (newPhi * prevPhi < 0.0)
            {
                const double frac = prevPhi / (prevPhi - newPhi);
                const double tSub = tPrev + m_rayMarchingResolution * frac;

                result.isIntersecting = true;
                result.distance = tSub;
                result.point = ray.PointAt(tSub);
                result.normal = GradientLocal(result.point);

                if (result.normal.Length() > 0.0)
                {
                    result.normal.Normalize();
                }

                return result;
            }

            tPrev = t;
            t += std::max(newPhiAbs, m_rayMarchingResolution);
            prevPhi = newPhi;
        }
    }

    return result;
}

Vector3D CustomImplicitSurface3::GradientLocal(const Vector3D& x) const
{
    const double left = m_func(x - Vector3D{ 0.5 * m_resolution, 0.0, 0.0 });
    const double right = m_func(x + Vector3D{ 0.5 * m_resolution, 0.0, 0.0 });
    const double bottom = m_func(x - Vector3D{ 0.0, 0.5 * m_resolution, 0.0 });
    const double top = m_func(x + Vector3D{ 0.0, 0.5 * m_resolution, 0.0 });
    const double back = m_func(x - Vector3D{ 0.0, 0.0, 0.5 * m_resolution });
    const double front = m_func(x + Vector3D{ 0.0, 0.0, 0.5 * m_resolution });

    return Vector3D{ (right - left) / m_resolution,
                     (top - bottom) / m_resolution,
                     (front - back) / m_resolution };
}

CustomImplicitSurface3::Builder CustomImplicitSurface3::GetBuilder()
{
    return Builder{};
}

CustomImplicitSurface3::Builder&
CustomImplicitSurface3::Builder::WithSignedDistanceFunction(
    const std::function<double(const Vector3D&)>& func)
{
    m_func = func;
    return *this;
}

CustomImplicitSurface3::Builder& CustomImplicitSurface3::Builder::WithDomain(
    const BoundingBox3D& domain)
{
    m_domain = domain;
    return *this;
}

CustomImplicitSurface3::Builder&
CustomImplicitSurface3::Builder::WithResolution(double resolution)
{
    m_resolution = resolution;
    return *this;
}

CustomImplicitSurface3::Builder&
CustomImplicitSurface3::Builder::WithRayMarchingResolution(double resolution)
{
    m_rayMarchingResolution = resolution;
    return *this;
}

CustomImplicitSurface3::Builder&
CustomImplicitSurface3::Builder::WithMaxNumberOfIterations(unsigned int numIter)
{
    m_maxNumberOfIterations = numIter;
    return *this;
}

CustomImplicitSurface3 CustomImplicitSurface3::Builder::Build() const
{
    return CustomImplicitSurface3{ m_func,
                                   m_domain,
                                   m_resolution,
                                   m_rayMarchingResolution,
                                   m_maxNumberOfIterations,
                                   m_transform,
                                   m_isNormalFlipped };
}

CustomImplicitSurface3Ptr CustomImplicitSurface3::Builder::MakeShared() const
{
    return std::shared_ptr<CustomImplicitSurface3>(
        new CustomImplicitSurface3{
            m_func, m_domain, m_resolution, m_rayMarchingResolution,
            m_maxNumberOfIterations, m_transform, m_isNormalFlipped },
        [](CustomImplicitSurface3* obj) { delete obj; });
}
}  // namespace CubbyFlow