// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SURFACE3_HPP
#define CUBBYFLOW_SURFACE3_HPP

#include <Core/BoundingBox/BoundingBox3.hpp>
#include <Core/Ray/Ray3.hpp>
#include <Core/Transform/Transform3.hpp>
#include <Core/Vector/Vector3.hpp>

#include <limits>
#include <memory>

namespace CubbyFlow
{
//! Structure that represents ray-surface intersection point.
struct SurfaceRayIntersection3
{
    bool isIntersecting = false;
    double distance = std::numeric_limits<double>::max();
    Vector3D point;
    Vector3D normal;
};

//! Abstract base class for 3-D surface.
class Surface3
{
 public:
    //! Local-to-world transform.
    Transform3 transform;

    //! Flips normal when calling Surface3::closestNormal(...).
    bool isNormalFlipped = false;

    //! Constructs a surface with normal direction.
    Surface3(const Transform3& transform = Transform3(),
             bool isNormalFlipped = false);

    //! Copy constructor.
    Surface3(const Surface3& other);

    //! Default destructor.
    virtual ~Surface3();

    //! Returns the closest point from the given point \p otherPoint to the
    //! surface.
    Vector3D ClosestPoint(const Vector3D& otherPoint) const;

    //! Returns the bounding box of this surface object.
    BoundingBox3D BoundingBox() const;

    //! Returns true if the given \p ray intersects with this surface object.
    bool Intersects(const Ray3D& ray) const;

    //! Returns the closest distance from the given point \p otherPoint to the
    //! point on the surface.
    double ClosestDistance(const Vector3D& otherPoint) const;

    //! Returns the closest intersection point for given \p ray.
    SurfaceRayIntersection3 ClosestIntersection(const Ray3D& ray) const;

    //! Returns the normal to the closest point on the surface from the given
    //! point \p otherPoint.
    Vector3D ClosestNormal(const Vector3D& otherPoint) const;

    //! Updates internal spatial query engine.
    virtual void UpdateQueryEngine();

    //! Returns true if bounding box can be defined.
    virtual bool IsBounded() const;

    //! Returns true if the surface is a valid geometry.
    virtual bool IsValidGeometry() const;

    //! Returns true if \p otherPoint is inside the volume defined by the
    //! surface.
    bool IsInside(const Vector3D& otherPoint) const;

 protected:
    //! Returns the closest point from the given point \p otherPoint to the
    //! surface in local frame.
    virtual Vector3D ClosestPointLocal(const Vector3D& otherPoint) const = 0;

    //! Returns the bounding box of this surface object in local frame.
    virtual BoundingBox3D BoundingBoxLocal() const = 0;

    //! Returns the closest intersection point for given \p ray in local frame.
    virtual SurfaceRayIntersection3 ClosestIntersectionLocal(
        const Ray3D& ray) const = 0;

    //! Returns the normal to the closest point on the surface from the given
    //! point \p otherPoint in local frame.
    virtual Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const = 0;

    //! Returns true if the given \p ray intersects with this surface object
    //! in local frame.
    virtual bool IntersectsLocal(const Ray3D& ray) const;

    //! Returns the closest distance from the given point \p otherPoint to the
    //! point on the surface in local frame.
    virtual double ClosestDistanceLocal(const Vector3D& otherPoint) const;

    //! Returns true if \p otherPoint is inside by given \p depth the volume
    //! defined by the surface in local frame.
    virtual bool IsInsideLocal(const Vector3D& otherPoint) const;
};

//! Shared pointer for the Surface3 type.
using Surface3Ptr = std::shared_ptr<Surface3>;

//!
//! \brief Base class for 3-D surface builder.
//!
template <typename DerivedBuilder>
class SurfaceBuilderBase3
{
 public:
    //! Returns builder with flipped normal flag.
    DerivedBuilder& WithIsNormalFlipped(bool isNormalFlipped);

    //! Returns builder with translation.
    DerivedBuilder& WithTranslation(const Vector3D& translation);

    //! Returns builder with orientation.
    DerivedBuilder& WithOrientation(const QuaternionD& orientation);

    //! Returns builder with transform.
    DerivedBuilder& WithTransform(const Transform3& transform);

 protected:
    bool m_isNormalFlipped = false;
    Transform3 m_transform;
};

template <typename T>
T& SurfaceBuilderBase3<T>::WithIsNormalFlipped(bool isNormalFlipped)
{
    m_isNormalFlipped = isNormalFlipped;
    return static_cast<T&>(*this);
}

template <typename T>
T& SurfaceBuilderBase3<T>::WithTranslation(const Vector3D& translation)
{
    m_transform.SetTranslation(translation);
    return static_cast<T&>(*this);
}

template <typename T>
T& SurfaceBuilderBase3<T>::WithOrientation(const QuaternionD& orientation)
{
    m_transform.SetOrientation(orientation);
    return static_cast<T&>(*this);
}

template <typename T>
T& SurfaceBuilderBase3<T>::WithTransform(const Transform3& transform)
{
    m_transform = transform;
    return static_cast<T&>(*this);
}
}  // namespace CubbyFlow

#endif