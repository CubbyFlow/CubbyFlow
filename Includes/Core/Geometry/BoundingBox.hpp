// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_BOUNDING_BOX_HPP
#define CUBBYFLOW_BOUNDING_BOX_HPP

#include <Core/Geometry/Ray.hpp>
#include <Core/Matrix/Matrix.hpp>

#include <limits>

namespace CubbyFlow
{
//!
//! \brief  Box-ray intersection result.
//!
//! \tparam T   The value type.
//!
template <typename T>
struct BoundingBoxRayIntersection
{
    //! True if the box and ray intersects.
    bool isIntersecting = false;

    //! Distance to the first intersection point.
    T near = std::numeric_limits<T>::max();

    //! Distance to the second (and the last) intersection point.
    T far = std::numeric_limits<T>::max();
};

//!
//! \brief  N-D axis-aligned bounding box class.
//!
//! \tparam T   Real number type.
//! \tparam N   Dimension.
//!
template <typename T, size_t N>
class BoundingBox
{
 public:
    static_assert(N > 0, "Dimension should be greater than 0");
    static_assert(
        std::is_floating_point<T>::value,
        "BoundingBox only can be instantiated with floating point types");

    using VectorType = Vector<T, N>;
    using RayType = Ray<T, N>;

    //! Default constructor.
    BoundingBox();

    //! Constructs a box that tightly covers two points.
    BoundingBox(const VectorType& point1, const VectorType& point2);

    //! Default destructor.
    ~BoundingBox() = default;

    //! Constructs a box with other box instance.
    BoundingBox(const BoundingBox& other);

    //! Constructs a box with other box instance.
    BoundingBox(BoundingBox&& other) noexcept;

    //! Copy assignment operator.
    BoundingBox& operator=(const BoundingBox& other);

    //! Move assignment operator.
    BoundingBox& operator=(BoundingBox&& other) noexcept;

    //! Returns width of the box.
    [[nodiscard]] T Width() const;

    //! Returns height of the box.
    template <typename U = T>
    [[nodiscard]] std::enable_if_t<(N > 1), U> Height() const;

    //! Returns depth of the box.
    template <typename U = T>
    [[nodiscard]] std::enable_if_t<(N > 2), U> Depth() const;

    //! Returns length of the box in given axis.
    [[nodiscard]] T Length(size_t axis);

    //! Returns true of this box and other box overlaps.
    [[nodiscard]] bool Overlaps(const BoundingBox& other) const;

    //! Returns true if the input vector is inside of this box.
    [[nodiscard]] bool Contains(const VectorType& point) const;

    //! Returns true if the input ray is intersecting with this box.
    [[nodiscard]] bool Intersects(const RayType& ray) const;

    //! Returns intersection.isIntersecting = true if the input ray is
    //! intersecting with this box. If intersects, intersection.tNear is
    //! assigned with distant to the closest intersecting point, and
    //! intersection.tFar with furthest.
    [[nodiscard]] BoundingBoxRayIntersection<T> ClosestIntersection(
        const RayType& ray) const;

    //! Returns the mid-point of this box.
    [[nodiscard]] VectorType MidPoint() const;

    //! Returns diagonal length of this box.
    [[nodiscard]] T DiagonalLength() const;

    //! Returns squared diagonal length of this box.
    [[nodiscard]] T DiagonalLengthSquared() const;

    //! Resets this box to initial state (min=infinite, max=-infinite).
    void Reset();

    //! Merges this and other point.
    void Merge(const VectorType& point);

    //! Merges this and other box.
    void Merge(const BoundingBox& other);

    //! Expands this box by given delta to all direction.
    //! If the width of the box was x, expand(y) will result a box with
    //! x+y+y width.
    void Expand(T delta);

    //! Returns corner position. Index starts from x-first order.
    [[nodiscard]] VectorType Corner(size_t idx) const;

    //! Returns the clamped point.
    [[nodiscard]] VectorType Clamp(const VectorType& point) const;

    //! Returns true if the box is empty.
    [[nodiscard]] bool IsEmpty() const;

    //! Returns box with different value type.
    template <typename U>
    [[nodiscard]] BoundingBox<U, N> CastTo() const;

    //! Lower corner of the bounding box.
    VectorType lowerCorner;

    //! Upper corner of the bounding box.
    VectorType upperCorner;
};

template <typename T>
using BoundingBox2 = BoundingBox<T, 2>;

template <typename T>
using BoundingBox3 = BoundingBox<T, 3>;

using BoundingBox2F = BoundingBox2<float>;

using BoundingBox2D = BoundingBox2<double>;

using BoundingBox3F = BoundingBox3<float>;

using BoundingBox3D = BoundingBox3<double>;

using BoundingBoxRayIntersectionF = BoundingBoxRayIntersection<float>;

using BoundingBoxRayIntersectionD = BoundingBoxRayIntersection<double>;
}  // namespace CubbyFlow

#include <Core/Geometry/BoundingBox-Impl.hpp>

#endif