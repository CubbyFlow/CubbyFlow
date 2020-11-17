// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_BOUNDING_BOX2_HPP
#define CUBBYFLOW_BOUNDING_BOX2_HPP

#include <Core/Geometry/BoundingBox.hpp>
#include <Core/Geometry/Ray2.hpp>
#include <Core/Vector/Vector2.hpp>

#include <limits>

namespace CubbyFlow
{
//!
//! \brief 2-D box-ray intersection result.
//!
//! \tparam T - The value type.
//!
template <typename T>
struct BoundingBoxRayIntersection2
{
    //! True if the box and ray intersects.
    bool isIntersecting = false;

    //! Distance to the first intersection point.
    T near = std::numeric_limits<T>::max();

    //! Distance to the second (and the last) intersection point.
    T far = std::numeric_limits<T>::max();
};

//!
//! \brief 2-D axis-aligned bounding box class.
//!
//! \tparam T - Real number type.
//!
template <typename T>
class BoundingBox<T, 2>
{
 public:
    //! Default constructor.
    BoundingBox();

    //! Constructs a box that tightly covers two points.
    BoundingBox(const Vector2<T>& point1, const Vector2<T>& point2);

    //! Copy constructor.
    BoundingBox(const BoundingBox& other);

    //! Move constructor.
    BoundingBox(BoundingBox&& other) noexcept;

    //! Default destructor.
    ~BoundingBox() = default;

    //! Copy assignment operator.
    BoundingBox& operator=(const BoundingBox& other);

    //! Move assignment operator.
    BoundingBox& operator=(BoundingBox&& other) noexcept;

    //! Returns width of the box.
    [[nodiscard]] T GetWidth() const;

    //! Returns height of the box.
    [[nodiscard]] T GetHeight() const;

    //! Returns length of the box in given axis.
    [[nodiscard]] T Length(size_t axis);

    //! Returns true of this box and other box overlaps.
    [[nodiscard]] bool Overlaps(const BoundingBox& other) const;

    //! Returns true if the input point is inside of this box.
    [[nodiscard]] bool Contains(const Vector2<T>& point) const;

    //! Returns true if the input ray is intersecting with this box.
    [[nodiscard]] bool Intersects(const Ray2<T>& ray) const;

    //! Returns intersection.isIntersecting = true if the input ray is
    //! intersecting with this box. If intersects, intersection.tNear is
    //! assigned with distant to the closest intersecting point, and
    //! intersection.tFar with furthest.
    [[nodiscard]] BoundingBoxRayIntersection2<T> ClosestIntersection(
        const Ray2<T>& ray) const;

    //! Returns the mid-point of this box.
    [[nodiscard]] Vector2<T> MidPoint() const;

    //! Returns diagonal length of this box.
    [[nodiscard]] T DiagonalLength() const;

    //! Returns squared diagonal length of this box.
    [[nodiscard]] T DiagonalLengthSquared() const;

    //! Resets this box to initial state (min=infinite, max=-infinite).
    void Reset();

    //! Merges this and other point.
    void Merge(const Vector2<T>& point);

    //! Merges this and other box.
    void Merge(const BoundingBox& other);

    //! Expands this box by given delta to all direction.
    //! If the width of the box was x, expand(y) will result a box with
    //! x+y+y width.
    void Expand(T delta);

    //! Returns corner position. Index starts from x-first order.
    [[nodiscard]] Vector2<T> Corner(size_t idx) const;

    //! Returns the clamped point.
    [[nodiscard]] Vector2<T> Clamp(const Vector2<T>& pt) const;

    //! Returns true if the box is empty.
    [[nodiscard]] bool IsEmpty() const;

    //! Lower corner of the bounding box.
    Vector2<T> lowerCorner;

    //! Upper corner of the bounding box.
    Vector2<T> upperCorner;
};

//! Type alias for 2-D BoundingBox.
template <typename T>
using BoundingBox2 = BoundingBox<T, 2>;

//! Float-type 2-D BoundingBox.
using BoundingBox2F = BoundingBox2<float>;

//! Double-type 2-D BoundingBox.
using BoundingBox2D = BoundingBox2<double>;

//! Float-type 2-D box-ray intersection result.
using BoundingBoxRayIntersection2F = BoundingBoxRayIntersection2<float>;

//! Double-type 2-D box-ray intersection result.
using BoundingBoxRayIntersection2D = BoundingBoxRayIntersection2<double>;
}  // namespace CubbyFlow

#include <Core/Geometry/BoundingBox2-Impl.hpp>

#endif