// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_LIST_QUERY_ENGINE_HPP
#define CUBBYFLOW_LIST_QUERY_ENGINE_HPP

#include <Core/Array/Array.hpp>
#include <Core/Array/ArrayView.hpp>
#include <Core/QueryEngine/IntersectionQueryEngine.hpp>
#include <Core/QueryEngine/NearestNeighborQueryEngine.hpp>

namespace CubbyFlow
{
//! Ad-hoc list-based N-D intersection/nearest-neighbor query engine.
template <typename T, size_t N>
class ListQueryEngine final : public IntersectionQueryEngine<T, N>,
                              public NearestNeighborQueryEngine<T, N>
{
 public:
    //! Adds an item to the container.
    void Add(const T& item);

    //! Adds items to the container.
    void Add(const ConstArrayView1<T>& items);

    //! Returns true if given \p box intersects with any of the stored items.
    [[nodiscard]] bool Intersects(
        const BoundingBox<double, N>& box,
        const BoxIntersectionTestFunc<T, N>& testFunc) const override;

    //! Returns true if given \p ray intersects with any of the stored items.
    [[nodiscard]] bool Intersects(
        const Ray<double, N>& ray,
        const RayIntersectionTestFunc<T, N>& testFunc) const override;

    //! Invokes \p visitorFunc for every intersecting items.
    void ForEachIntersectingItem(
        const BoundingBox<double, N>& box,
        const BoxIntersectionTestFunc<T, N>& testFunc,
        const IntersectionVisitorFunc<T>& visitorFunc) const override;

    //! Invokes \p visitorFunc for every intersecting items.
    void ForEachIntersectingItem(
        const Ray<double, N>& ray,
        const RayIntersectionTestFunc<T, N>& testFunc,
        const IntersectionVisitorFunc<T>& visitorFunc) const override;

    //! Returns the closest intersection for given \p ray.
    [[nodiscard]] ClosestIntersectionQueryResult<T, N> ClosestIntersection(
        const Ray<double, N>& ray,
        const GetRayIntersectionFunc<T, N>& testFunc) const override;

    //! Returns the nearest neighbor for given point and distance measure
    //! function.
    [[nodiscard]] NearestNeighborQueryResult<T, N> Nearest(
        const Vector<double, N>& pt,
        const NearestNeighborDistanceFunc<T, N>& distanceFunc) const override;

 private:
    Array1<T> m_items;
};

//! 2-D ListQueryEngine type.
template <typename T>
using ListQueryEngine2 = ListQueryEngine<T, 2>;

//! 3-D ListQueryEngine type.
template <typename T>
using ListQueryEngine3 = ListQueryEngine<T, 3>;
}  // namespace CubbyFlow

#include <Core/QueryEngine/ListQueryEngine-Impl.hpp>

#endif