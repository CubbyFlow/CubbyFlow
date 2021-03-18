// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_LIST_QUERY_ENGINE3_HPP
#define CUBBYFLOW_LIST_QUERY_ENGINE3_HPP

#include <Core/QueryEngine/IntersectionQueryEngine.hpp>
#include <Core/QueryEngine/NearestNeighborQueryEngine.hpp>

namespace CubbyFlow
{
//! Ad-hoc list-based 3-D intersection/nearest-neighbor query engine.
template <typename T>
class ListQueryEngine3 final : public IntersectionQueryEngine3<T>,
                               public NearestNeighborQueryEngine3<T>
{
 public:
    //! Adds an item to the container.
    void Add(const T& item);

    //! Adds items to the container.
    void Add(const std::vector<T>& items);

    //! Returns true if given \p box intersects with any of the stored items.
    [[nodiscard]] bool Intersects(
        const BoundingBox3D& box,
        const BoxIntersectionTestFunc3<T>& testFunc) const override;

    //! Returns true if given \p ray intersects with any of the stored items.
    [[nodiscard]] bool Intersects(
        const Ray3D& ray,
        const RayIntersectionTestFunc3<T>& testFunc) const override;

    //! Invokes \p visitorFunc for every intersecting items.
    void ForEachIntersectingItem(
        const BoundingBox3D& box, const BoxIntersectionTestFunc3<T>& testFunc,
        const IntersectionVisitorFunc<T>& visitorFunc) const override;

    //! Invokes \p visitorFunc for every intersecting items.
    void ForEachIntersectingItem(
        const Ray3D& ray, const RayIntersectionTestFunc3<T>& testFunc,
        const IntersectionVisitorFunc<T>& visitorFunc) const override;

    //! Returns the closest intersection for given \p ray.
    [[nodiscard]] ClosestIntersectionQueryResult3<T> ClosestIntersection(
        const Ray3D& ray,
        const GetRayIntersectionFunc3<T>& testFunc) const override;

    //! Returns the nearest neighbor for given point and distance measure
    //! function.
    [[nodiscard]] NearestNeighborQueryResult3<T> Nearest(
        const Vector3D& pt,
        const NearestNeighborDistanceFunc3<T>& distanceFunc) const override;

 private:
    std::vector<T> m_items;
};
}  // namespace CubbyFlow

#include <Core/QueryEngine/ListQueryEngine3-Impl.hpp>

#endif