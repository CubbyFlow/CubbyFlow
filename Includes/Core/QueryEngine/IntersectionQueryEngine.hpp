// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_INTERSECTION_QUERY_ENGINE_HPP
#define CUBBYFLOW_INTERSECTION_QUERY_ENGINE_HPP

#include <Core/Geometry/BoundingBox.hpp>
#include <Core/Geometry/Ray.hpp>
#include <Core/Matrix/Matrix.hpp>

#include <functional>
#include <limits>

namespace CubbyFlow
{
//! N-D closest intersection query result.
template <typename T, size_t N>
struct ClosestIntersectionQueryResult
{
    const T* item = nullptr;
    double distance = std::numeric_limits<double>::max();
};

//! 2-D closest intersection query result.
template <typename T>
using ClosestIntersectionQueryResult2 = ClosestIntersectionQueryResult<T, 2>;

//! 3-D closest intersection query result.
template <typename T>
using ClosestIntersectionQueryResult3 = ClosestIntersectionQueryResult<T, 3>;

//! N-D closest intersection distance measure function.
template <typename T, size_t N>
using ClosestIntersectionDistanceFunc =
    std::function<double(const T&, const Vector<double, N>&)>;

//! 2-D closestIntersectionDistanceFunc.
template <typename T>
using ClosestIntersectionDistanceFunc2 = ClosestIntersectionDistanceFunc<T, 2>;

//! 3-D closestIntersectionDistanceFunc.
template <typename T>
using ClosestIntersectionDistanceFunc3 = ClosestIntersectionDistanceFunc<T, 3>;

//! N-D box-item intersection test function.
template <typename T, size_t N>
using BoxIntersectionTestFunc =
    std::function<bool(const T&, const BoundingBox<double, N>&)>;

//! 2-D box-item intersection test function.
template <typename T>
using BoxIntersectionTestFunc2 = BoxIntersectionTestFunc<T, 2>;

//! 3-D box-item intersection test function.
template <typename T>
using BoxIntersectionTestFunc3 = BoxIntersectionTestFunc<T, 3>;

//! N-D ray-item intersection test function.
template <typename T, size_t N>
using RayIntersectionTestFunc =
    std::function<bool(const T&, const Ray<double, N>&)>;

//! 2-D ray-item intersection test function.
template <typename T>
using RayIntersectionTestFunc2 = RayIntersectionTestFunc<T, 2>;

//! 3-D ray-item intersection test function.
template <typename T>
using RayIntersectionTestFunc3 = RayIntersectionTestFunc<T, 3>;

//! N-D ray-item closest intersection evaluation function.
template <typename T, size_t N>
using GetRayIntersectionFunc =
    std::function<double(const T&, const Ray<double, N>&)>;

//! 2-D ray-item closest intersection evaluation function.
template <typename T>
using GetRayIntersectionFunc2 = GetRayIntersectionFunc<T, 2>;

//! 3-D ray-item closest intersection evaluation function.
template <typename T>
using GetRayIntersectionFunc3 = GetRayIntersectionFunc<T, 3>;

//! Visitor function which is invoked for each intersecting item.
template <typename T>
using IntersectionVisitorFunc = std::function<void(const T&)>;

//! Abstract base class for N-D intersection test query engine.
template <typename T, size_t N>
class IntersectionQueryEngine
{
 public:
    //! Default constructor.
    IntersectionQueryEngine() = default;

    //! Default virtual destructor.
    virtual ~IntersectionQueryEngine() = default;

    //! Default copy constructor.
    IntersectionQueryEngine(const IntersectionQueryEngine& other) = default;

    //! Default move constructor.
    IntersectionQueryEngine(IntersectionQueryEngine&& other) noexcept = default;

    //! Default copy assignment operator.
    IntersectionQueryEngine& operator=(const IntersectionQueryEngine& other) =
        default;

    //! Default move assignment operator.
    IntersectionQueryEngine& operator=(
        IntersectionQueryEngine&& other) noexcept = default;

    //! Returns true if given \p box intersects with any of the stored items.
    [[nodiscard]] virtual bool Intersects(
        const BoundingBox<double, N>& box,
        const BoxIntersectionTestFunc<T, N>& testFunc) const = 0;

    //! Returns true if given \p ray intersects with any of the stored items.
    [[nodiscard]] virtual bool Intersects(
        const Ray<double, N>& ray,
        const RayIntersectionTestFunc<T, N>& testFunc) const = 0;

    //! Invokes \p visitorFunc for every intersecting items.
    virtual void ForEachIntersectingItem(
        const BoundingBox<double, N>& box,
        const BoxIntersectionTestFunc<T, N>& testFunc,
        const IntersectionVisitorFunc<T>& visitorFunc) const = 0;

    //! Invokes \p visitorFunc for every intersecting items.
    virtual void ForEachIntersectingItem(
        const Ray<double, N>& ray,
        const RayIntersectionTestFunc<T, N>& testFunc,
        const IntersectionVisitorFunc<T>& visitorFunc) const = 0;

    //! Returns the closest intersection for given \p ray.
    [[nodiscard]] virtual ClosestIntersectionQueryResult<T, N>
    ClosestIntersection(const Ray<double, N>& ray,
                        const GetRayIntersectionFunc<T, N>& testFunc) const = 0;
};

//! Abstract base class for 2-D intersection test query engine.
template <typename T>
using IntersectionQueryEngine2 = IntersectionQueryEngine<T, 2>;

//! Abstract base class for 3-D intersection test query engine.
template <typename T>
using IntersectionQueryEngine3 = IntersectionQueryEngine<T, 3>;

}  // namespace CubbyFlow

#endif