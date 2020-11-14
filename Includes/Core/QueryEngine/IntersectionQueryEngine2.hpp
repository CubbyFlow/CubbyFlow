// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_INTERSECTION_QUERY_ENGINE2_HPP
#define CUBBYFLOW_INTERSECTION_QUERY_ENGINE2_HPP

#include <Core/BoundingBox/BoundingBox2.hpp>
#include <Core/Vector/Vector2.hpp>

namespace CubbyFlow
{
//! Closest intersection query result.
template <typename T>
struct ClosestIntersectionQueryResult2
{
    const T* item = nullptr;
    double distance = std::numeric_limits<double>::max();
};

//! Closest intersection distance measure function.
template <typename T>
using ClosestIntersectionDistanceFunc2 =
    std::function<double(const T&, const Vector2D&)>;

//! Box-item intersection test function.
template <typename T>
using BoxIntersectionTestFunc2 =
    std::function<bool(const T&, const BoundingBox2D&)>;

//! Ray-item intersection test function.
template <typename T>
using RayIntersectionTestFunc2 = std::function<bool(const T&, const Ray2D&)>;

//! Ray-item closest intersection evaluation function.
template <typename T>
using GetRayIntersectionFunc2 = std::function<double(const T&, const Ray2D&)>;

//! Visitor function which is invoked for each intersecting item.
template <typename T>
using IntersectionVisitorFunc2 = std::function<void(const T&)>;

//! Abstract base class for 2-D intersection test query engine.
template <typename T>
class IntersectionQueryEngine2
{
 public:
    //! Default constructor.
    IntersectionQueryEngine2() = default;

    //! Default copy constructor.
    IntersectionQueryEngine2(const IntersectionQueryEngine2&) = default;

    //! Default move constructor.
    IntersectionQueryEngine2(IntersectionQueryEngine2&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~IntersectionQueryEngine2() = default;

    //! Default copy assignment operator.
    IntersectionQueryEngine2& operator=(const IntersectionQueryEngine2&) =
        default;

    //! Default move assignment operator.
    IntersectionQueryEngine2& operator=(IntersectionQueryEngine2&&) noexcept =
        default;

    //! Returns true if given \p box intersects with any of the stored items.
    [[nodiscard]] virtual bool IsIntersects(
        const BoundingBox2D& box,
        const BoxIntersectionTestFunc2<T>& testFunc) const = 0;

    //! Returns true if given \p ray intersects with any of the stored items.
    [[nodiscard]] virtual bool IsIntersects(
        const Ray2D& ray,
        const RayIntersectionTestFunc2<T>& testFunc) const = 0;

    //! Invokes \p visitorFunc for every intersecting items.
    virtual void ForEachIntersectingItem(
        const BoundingBox2D& box, const BoxIntersectionTestFunc2<T>& testFunc,
        const IntersectionVisitorFunc2<T>& visitorFunc) const = 0;

    //! Invokes \p visitorFunc for every intersecting items.
    virtual void ForEachIntersectingItem(
        const Ray2D& ray, const RayIntersectionTestFunc2<T>& testFunc,
        const IntersectionVisitorFunc2<T>& visitorFunc) const = 0;

    //! Returns the closest intersection for given \p ray.
    [[nodiscard]] virtual ClosestIntersectionQueryResult2<T>
    GetClosestIntersection(
        const Ray2D& ray, const GetRayIntersectionFunc2<T>& testFunc) const = 0;
};
}  // namespace CubbyFlow

#endif