// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_BOUNDING_BOX3_HPP
#define CUBBYFLOW_BOUNDING_BOX3_HPP

#include <Core/Geometry/BoundingBox.hpp>
#include <Core/Ray/Ray3.hpp>
#include <Core/Vector/Vector3.hpp>

#include <limits>

namespace CubbyFlow
{
//!
//! \brief 3-D box-ray intersection result.
//!
//! \tparam T - The value type.
//!
template <typename T>
struct BoundingBoxRayIntersection3
{
    //! True if the box and ray intersects.
    bool isIntersecting = false;

    //! Distance to the first intersection point.
    T near = std::numeric_limits<T>::max();

    //! Distance to the second (and the last) intersection point.
    T far = std::numeric_limits<T>::max();
};

//!
//! \brief 3-D axis-aligned bounding box class.
//!
//! \tparam T - Real number type.
//!
template <typename T>
class BoundingBox<T, 3>
{
 public:
    //! Default constructor.
    BoundingBox();

    //! Constructs a box that tightly covers two points.
    BoundingBox(const Vector3<T>& point1, const Vector3<T>& point2);

    //! Copy constructor.
    BoundingBox(const BoundingBox& other);

    //! Move constructor.
    BoundingBox(BoundingBox&& other) noexcept;

    //! Default destructor.
    ~BoundingBox() = default;

    //! Copy assignment operator.
    BoundingBox& operator=(const BoundingBox& other);

    //! Copy assignment operator.
    BoundingBox& operator=(BoundingBox&& other) noexcept;

    //! Returns width of the box.
    [[nodiscard]] T GetWidth() const;

    //! Returns height of the box.
    [[nodiscard]] T GetHeight() const;

    //! Returns depth of the box.
    [[nodiscard]] T GetDepth() const;

    //! Returns length of the box in given axis.
    [[nodiscard]] T Length(size_t axis);

    //! Returns true of this box and other box overlaps.
    [[nodiscard]] bool Overlaps(const BoundingBox& other) const;

    //! Returns true if the input vector is inside of this box.
    [[nodiscard]] bool Contains(const Vector3<T>& point) const;

    //! Returns true if the input ray is intersecting with this box.
    [[nodiscard]] bool Intersects(const Ray3<T>& ray) const;

    //! Returns intersection.isIntersecting = true if the input ray is
    //! intersecting with this box. If intersects, intersection.tNear is
    //! assigned with distant to the closest intersecting point, and
    //! intersection.tFar with furthest.
    [[nodiscard]] BoundingBoxRayIntersection3<T> ClosestIntersection(
        const Ray3<T>& ray) const;

    //! Returns the mid-point of this box.
    [[nodiscard]] Vector3<T> MidPoint() const;

    //! Returns diagonal length of this box.
    [[nodiscard]] T DiagonalLength() const;

    //! Returns squared diagonal length of this box.
    [[nodiscard]] T DiagonalLengthSquared() const;

    //! Resets this box to initial state (min=infinite, max=-infinite).
    void Reset();

    //! Merges this and other point.
    void Merge(const Vector3<T>& point);

    //! Merges this and other box.
    void Merge(const BoundingBox& other);

    //! Expands this box by given delta to all direction.
    //! If the width of the box was x, expand(y) will result a box with
    //! x+y+y width.
    void Expand(T delta);

    //! Returns corner position. Index starts from x-first order.
    [[nodiscard]] Vector3<T> Corner(size_t idx) const;

    //! Returns the clamped point.
    [[nodiscard]] Vector3<T> Clamp(const Vector3<T>& pt) const;

    //! Returns true if the box is empty.
    [[nodiscard]] bool IsEmpty() const;

    //! Lower corner of the bounding box.
    Vector3<T> lowerCorner;

    //! Upper corner of the bounding box.
    Vector3<T> upperCorner;
};

//! Type alias for 3-D BoundingBox.
template <typename T>
using BoundingBox3 = BoundingBox<T, 3>;

//! Float-type 3-D BoundingBox.
using BoundingBox3F = BoundingBox3<float>;

//! Double-type 3-D BoundingBox.
using BoundingBox3D = BoundingBox3<double>;

//! Float-type 3-D box-ray intersection result.
using BoundingBoxRayIntersection3F = BoundingBoxRayIntersection3<float>;

//! Double-type 3-D box-ray intersection result.
using BoundingBoxRayIntersection3D = BoundingBoxRayIntersection3<double>;
}  // namespace CubbyFlow

#include <Core/Geometry/BoundingBox3-Impl.hpp>

#endif