// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_LIST_QUERY_ENGINE2_HPP
#define CUBBYFLOW_LIST_QUERY_ENGINE2_HPP

#include <Core/QueryEngine/IntersectionQueryEngine2.hpp>
#include <Core/QueryEngine/NearestNeighborQueryEngine2.hpp>

namespace CubbyFlow
{
//! Ad-hoc list-based 2-D intersection/nearest-neighbor query engine.
template <typename T>
class ListQueryEngine2 final : public IntersectionQueryEngine2<T>,
                               public NearestNeighborQueryEngine2<T>
{
 public:
    //! Adds an item to the container.
    void Add(const T& item);

    //! Adds items to the container.
    void Add(const std::vector<T>& items);

    //! Returns true if given \p box intersects with any of the stored items.
    bool IsIntersects(
        const BoundingBox2D& box,
        const BoxIntersectionTestFunc2<T>& testFunc) const override;

    //! Returns true if given \p ray intersects with any of the stored items.
    bool IsIntersects(
        const Ray2D& ray,
        const RayIntersectionTestFunc2<T>& testFunc) const override;

    //! Invokes \p visitorFunc for every intersecting items.
    void ForEachIntersectingItem(
        const BoundingBox2D& box, const BoxIntersectionTestFunc2<T>& testFunc,
        const IntersectionVisitorFunc2<T>& visitorFunc) const override;

    //! Invokes \p visitorFunc for every intersecting items.
    void ForEachIntersectingItem(
        const Ray2D& ray, const RayIntersectionTestFunc2<T>& testFunc,
        const IntersectionVisitorFunc2<T>& visitorFunc) const override;

    //! Returns the closest intersection for given \p ray.
    ClosestIntersectionQueryResult2<T> GetClosestIntersection(
        const Ray2D& ray,
        const GetRayIntersectionFunc2<T>& testFunc) const override;

    //! Returns the nearest neighbor for given point and distance measure
    //! function.
    NearestNeighborQueryResult2<T> GetNearestNeighbor(
        const Vector2D& pt,
        const NearestNeighborDistanceFunc2<T>& distanceFunc) const override;

 private:
    std::vector<T> m_items;
};
}  // namespace CubbyFlow

#include <Core/QueryEngine/ListQueryEngine2-Impl.hpp>

#endif