// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_BVH2_HPP
#define CUBBYFLOW_BVH2_HPP

#include <Core/QueryEngine/IntersectionQueryEngine2.hpp>
#include <Core/QueryEngine/NearestNeighborQueryEngine2.hpp>

namespace CubbyFlow
{
//!
//! \brief Bounding Volume Hierarchy (BVH) in 2D
//!
//! This class implements the classic bounding volume hierarchy structure in 2D.
//! It implements IntersectionQueryEngine2 in order to support box/ray
//! intersection tests. Also, NearestNeighborQueryEngine2 is implemented to
//! provide nearest neighbor query.
//!
template <typename T>
class BVH2 final : public IntersectionQueryEngine2<T>,
                   public NearestNeighborQueryEngine2<T>
{
 public:
    using ContainerType = std::vector<T>;
    using Iterator = typename ContainerType::iterator;
    using ConstIterator = typename ContainerType::const_iterator;

    //! Default constructor.
    BVH2();

    //! Builds bounding volume hierarchy.
    void Build(const std::vector<T>& items,
               const std::vector<BoundingBox2D>& itemsBounds);

    //! Clears all the contents of this instance.
    void Clear();

    //! Returns the nearest neighbor for given point and distance measure
    //! function.
    NearestNeighborQueryResult2<T> GetNearestNeighbor(
        const Vector2D& pt,
        const NearestNeighborDistanceFunc2<T>& distanceFunc) const override;

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

    //! Returns bounding box of every items.
    const BoundingBox2D& GetBoundingBox() const;

    //! Returns the begin iterator of the item.
    Iterator begin();

    //! Returns the end iterator of the item.
    Iterator end();

    //! Returns the immutable begin iterator of the item.
    ConstIterator begin() const;

    //! Returns the immutable end iterator of the item.
    ConstIterator end() const;

    //! Returns the number of items.
    size_t GetNumberOfItems() const;

    //! Returns the item at \p i.
    const T& GetItem(size_t i) const;

    //! Returns the number of nodes.
    size_t GetNumberOfNodes() const;

    //! Returns the children indices of \p i-th node.
    std::pair<size_t, size_t> GetChildren(size_t i) const;

    //! Returns true if \p i-th node is a leaf node.
    bool IsLeaf(size_t i) const;

    //! Returns bounding box of \p i-th node.
    const BoundingBox2D& GetNodeBound(size_t i) const;

    //! Returns item of \p i-th node.
    Iterator GetItemOfNode(size_t i);

    //! Returns item of \p i-th node.
    ConstIterator GetItemOfNode(size_t i) const;

 private:
    struct Node
    {
        char flags;
        union
        {
            size_t child;
            size_t item;
        };
        BoundingBox2D bound;

        Node();
        void InitLeaf(size_t it, const BoundingBox2D& b);
        void InitInternal(uint8_t axis, size_t c, const BoundingBox2D& b);
        bool IsLeaf() const;
    };

    BoundingBox2D m_bound;
    ContainerType m_items;
    std::vector<BoundingBox2D> m_itemBounds;
    std::vector<Node> m_nodes;

    size_t Build(size_t nodeIndex, size_t* itemIndices, size_t nItems,
                 size_t currentDepth);

    size_t QSplit(size_t* itemIndices, size_t numItems, double pivot,
                  uint8_t axis);
};
}  // namespace CubbyFlow

#include <Core/Geometry/BVH2-Impl.hpp>

#endif