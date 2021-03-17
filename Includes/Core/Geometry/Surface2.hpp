// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SURFACE2_HPP
#define CUBBYFLOW_SURFACE2_HPP

#include <Core/Geometry/BoundingBox.hpp>
#include <Core/Geometry/Ray.hpp>
#include <Core/Geometry/Transform.hpp>
#include <Core/Matrix/Matrix.hpp>

#include <limits>
#include <memory>

namespace CubbyFlow
{
//! Structure that represents ray-surface intersection point.
struct SurfaceRayIntersection2
{
    bool isIntersecting = false;
    double distance = std::numeric_limits<double>::max();
    Vector2D point;
    Vector2D normal;
};

//! Abstract base class for 2-D surface.
class Surface2
{
 public:
    //! Constructs a surface with normal direction.
    Surface2(Transform2 _transform = Transform2{},
             bool _isNormalFlipped = false);

    //! Default copy constructor.
    Surface2(const Surface2&) = default;

    //! Default move constructor.
    Surface2(Surface2&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~Surface2() = default;

    //! Default copy assignment operator.
    Surface2& operator=(const Surface2&) = default;

    //! Default move assignment operator.
    Surface2& operator=(Surface2&&) noexcept = default;

    //! Returns the closest point from the given point \p otherPoint to the
    //! surface.
    [[nodiscard]] Vector2D ClosestPoint(const Vector2D& otherPoint) const;

    //! Returns the bounding box of this surface object.
    [[nodiscard]] BoundingBox2D BoundingBox() const;

    //! Returns true if the given \p ray intersects with this surface object.
    [[nodiscard]] bool Intersects(const Ray2D& ray) const;

    //! Returns the closest distance from the given point \p otherPoint to the
    //! point on the surface.
    [[nodiscard]] double ClosestDistance(const Vector2D& otherPoint) const;

    //! Returns the closest intersection point for given \p ray.
    [[nodiscard]] SurfaceRayIntersection2 ClosestIntersection(
        const Ray2D& ray) const;

    //! Returns the normal to the closest point on the surface from the given
    //! point \p otherPoint.
    [[nodiscard]] Vector2D ClosestNormal(const Vector2D& otherPoint) const;

    //! Updates internal spatial query engine.
    virtual void UpdateQueryEngine();

    //! Returns true if bounding box can be defined.
    [[nodiscard]] virtual bool IsBounded() const;

    //! Returns true if the surface is a valid geometry.
    [[nodiscard]] virtual bool IsValidGeometry() const;

    //! Returns true if \p otherPoint is inside the volume defined by the
    //! surface.
    [[nodiscard]] bool IsInside(const Vector2D& otherPoint) const;

    //! Local-to-world transform.
    Transform2 transform;

    //! Flips normal.
    bool isNormalFlipped = false;

 protected:
    //! Returns the closest point from the given point \p otherPoint to the
    //! surface in local frame.
    [[nodiscard]] virtual Vector2D ClosestPointLocal(
        const Vector2D& otherPoint) const = 0;

    //! Returns the bounding box of this surface object in local frame.
    [[nodiscard]] virtual BoundingBox2D BoundingBoxLocal() const = 0;

    //! Returns the closest intersection point for given \p ray in local frame.
    [[nodiscard]] virtual SurfaceRayIntersection2 ClosestIntersectionLocal(
        const Ray2D& ray) const = 0;

    //! Returns the normal to the closest point on the surface from the given
    //! point \p otherPoint in local frame.
    [[nodiscard]] virtual Vector2D ClosestNormalLocal(
        const Vector2D& otherPoint) const = 0;

    //! Returns true if the given \p ray intersects with this surface object
    //! in local frame.
    [[nodiscard]] virtual bool IntersectsLocal(const Ray2D& ray) const;

    //! Returns the closest distance from the given point \p otherPoint to the
    //! point on the surface in local frame.
    [[nodiscard]] virtual double ClosestDistanceLocal(
        const Vector2D& otherPoint) const;

    //! Returns true if \p otherPoint is inside by given \p depth the volume
    //! defined by the surface in local frame.
    [[nodiscard]] virtual bool IsInsideLocal(const Vector2D& otherPoint) const;
};

//! Shared pointer for the Surface2 type.
using Surface2Ptr = std::shared_ptr<Surface2>;

//!
//! \brief Base class for 2-D surface builder.
//!
template <typename DerivedBuilder>
class SurfaceBuilderBase2
{
 public:
    //! Returns builder with flipped normal flag.
    [[nodiscard]] DerivedBuilder& WithIsNormalFlipped(bool isNormalFlipped);

    //! Returns builder with translation.
    [[nodiscard]] DerivedBuilder& WithTranslation(const Vector2D& translation);

    //! Returns builder with orientation.
    [[nodiscard]] DerivedBuilder& WithOrientation(double orientation);

    //! Returns builder with transform.
    [[nodiscard]] DerivedBuilder& WithTransform(const Transform2& transform);

 protected:
    bool m_isNormalFlipped = false;
    Transform2 m_transform;
};

template <typename T>
T& SurfaceBuilderBase2<T>::WithIsNormalFlipped(bool isNormalFlipped)
{
    m_isNormalFlipped = isNormalFlipped;
    return static_cast<T&>(*this);
}

template <typename T>
T& SurfaceBuilderBase2<T>::WithTranslation(const Vector2D& translation)
{
    m_transform.SetTranslation(translation);
    return static_cast<T&>(*this);
}

template <typename T>
T& SurfaceBuilderBase2<T>::WithOrientation(double orientation)
{
    m_transform.SetOrientation(orientation);
    return static_cast<T&>(*this);
}

template <typename T>
T& SurfaceBuilderBase2<T>::WithTransform(const Transform2& transform)
{
    m_transform = transform;
    return static_cast<T&>(*this);
}
}  // namespace CubbyFlow

#endif