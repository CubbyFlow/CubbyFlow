// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_BVH_HPP
#define CUBBYFLOW_BVH_HPP

#include <Core/Array/Array.hpp>
#include <Core/Array/ArrayView.hpp>
#include <Core/QueryEngine/IntersectionQueryEngine.hpp>
#include <Core/QueryEngine/NearestNeighborQueryEngine.hpp>

namespace CubbyFlow
{
//!
//! \brief Bounding Volume Hierarchy (BVH) in N-D
//!
//! This class implements the classic bounding volume hierarchy structure in
//! N-D. It implements IntersectionQueryEngine in order to support box/ray
//! intersection tests. Also, NearestNeighborQueryEngine is implemented to
//! provide nearest neighbor query.
//!
template <typename T, size_t N>
class BVH final : public IntersectionQueryEngine<T, N>,
                  public NearestNeighborQueryEngine<T, N>
{
 public:
    using ContainerType = Array1<T>;
    using Iterator = typename ContainerType::Iterator;
    using ConstIterator = typename ContainerType::ConstIterator;

    //! Builds bounding volume hierarchy.
    void Build(const ConstArrayView1<T>& items,
               const ConstArrayView1<BoundingBox<double, N>>& itemsBounds);

    //! Clears all the contents of this instance.
    void Clear();

    //! Returns the nearest neighbor for given point and distance measure
    //! function.
    [[nodiscard]] NearestNeighborQueryResult<T, N> Nearest(
        const Vector<double, N>& pt,
        const NearestNeighborDistanceFunc<T, N>& distanceFunc) const override;

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

    //! Returns bounding box of every items.
    [[nodiscard]] const BoundingBox<double, N>& GetBoundingBox() const;

    //! Returns the begin Iterator of the item.
    [[nodiscard]] Iterator begin();

    //! Returns the end Iterator of the item.
    [[nodiscard]] Iterator end();

    //! Returns the immutable begin Iterator of the item.
    [[nodiscard]] ConstIterator begin() const;

    //! Returns the immutable end Iterator of the item.
    [[nodiscard]] ConstIterator end() const;

    //! Returns the number of items.
    [[nodiscard]] size_t NumberOfItems() const;

    //! Returns the item at \p i.
    [[nodiscard]] const T& Item(size_t i) const;

    //! Returns the number of nodes.
    [[nodiscard]] size_t NumberOfNodes() const;

    //! Returns the children indices of \p i-th node.
    [[nodiscard]] std::pair<size_t, size_t> Children(size_t i) const;

    //! Returns true if \p i-th node is a leaf node.
    [[nodiscard]] bool IsLeaf(size_t i) const;

    //! Returns bounding box of \p i-th node.
    [[nodiscard]] const BoundingBox<double, N>& NodeBound(size_t i) const;

    //! Returns item of \p i-th node.
    [[nodiscard]] Iterator ItemOfNode(size_t i);

    //! Returns item of \p i-th node.
    [[nodiscard]] ConstIterator ItemOfNode(size_t i) const;

 private:
    struct Node
    {
        Node();

        void InitLeaf(size_t it, const BoundingBox<double, N>& b);
        void InitInternal(uint8_t axis, size_t c,
                          const BoundingBox<double, N>& b);

        [[nodiscard]] bool IsLeaf() const;

        char flags;
        union
        {
            size_t child;
            size_t item{};
        };
        BoundingBox<double, N> bound;
    };

    size_t Build(size_t nodeIndex, size_t* itemIndices, size_t nItems,
                 size_t currentDepth);

    [[nodiscard]] size_t QSplit(size_t* itemIndices, size_t numItems,
                                double pivot, uint8_t axis);

    BoundingBox<double, N> m_bound;
    ContainerType m_items;
    Array1<BoundingBox<double, N>> m_itemBounds;
    Array1<Node> m_nodes;
};

//! 2-D BVH type.
template <typename T>
using BVH2 = BVH<T, 2>;

//! 3-D BVH type.
template <typename T>
using BVH3 = BVH<T, 3>;
}  // namespace CubbyFlow

#include <Core/Geometry/BVH-Impl.hpp>

#endif