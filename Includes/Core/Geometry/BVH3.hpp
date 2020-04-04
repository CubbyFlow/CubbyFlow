// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_BVH3_HPP
#define CUBBYFLOW_BVH3_HPP

#include <Core/QueryEngine/IntersectionQueryEngine3.hpp>
#include <Core/QueryEngine/NearestNeighborQueryEngine3.hpp>

namespace CubbyFlow
{
//!
//! \brief Bounding Volume Hierarchy (BVH) in 3D
//!
//! This class implements the classic bounding volume hierarchy structure in 3D.
//! It implements IntersectionQueryEngine3 in order to support box/ray
//! intersection tests. Also, NearestNeighborQueryEngine3 is implemented to
//! provide nearest neighbor query.
//!
template <typename T>
class BVH3 final : public IntersectionQueryEngine3<T>,
                   public NearestNeighborQueryEngine3<T>
{
 public:
    using ContainerType = std::vector<T>;
    using Iterator = typename ContainerType::iterator;
    using ConstIterator = typename ContainerType::const_iterator;

    //! Default constructor.
    BVH3();

    //! Builds bounding volume hierarchy.
    void Build(const std::vector<T>& items,
               const std::vector<BoundingBox3D>& itemsBounds);

    //! Clears all the contents of this instance.
    void Clear();

    //! Returns the nearest neighbor for given point and distance measure
    //! function.
    NearestNeighborQueryResult3<T> GetNearestNeighbor(
        const Vector3D& pt,
        const NearestNeighborDistanceFunc3<T>& distanceFunc) const override;

    //! Returns true if given \p box intersects with any of the stored items.
    bool IsIntersects(
        const BoundingBox3D& box,
        const BoxIntersectionTestFunc3<T>& testFunc) const override;

    //! Returns true if given \p ray intersects with any of the stored items.
    bool IsIntersects(
        const Ray3D& ray,
        const RayIntersectionTestFunc3<T>& testFunc) const override;

    //! Invokes \p visitorFunc for every intersecting items.
    void ForEachIntersectingItem(
        const BoundingBox3D& box, const BoxIntersectionTestFunc3<T>& testFunc,
        const IntersectionVisitorFunc3<T>& visitorFunc) const override;

    //! Invokes \p visitorFunc for every intersecting items.
    void ForEachIntersectingItem(
        const Ray3D& ray, const RayIntersectionTestFunc3<T>& testFunc,
        const IntersectionVisitorFunc3<T>& visitorFunc) const override;

    //! Returns the closest intersection for given \p ray.
    ClosestIntersectionQueryResult3<T> GetClosestIntersection(
        const Ray3D& ray,
        const GetRayIntersectionFunc3<T>& testFunc) const override;

    //! Returns bounding box of every items.
    const BoundingBox3D& GetBoundingBox() const;

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

 private:
    struct Node
    {
        char flags;
        union
        {
            size_t child;
            size_t item;
        };
        BoundingBox3D bound;

        Node();
        void InitLeaf(size_t it, const BoundingBox3D& b);
        void InitInternal(uint8_t axis, size_t c, const BoundingBox3D& b);
        bool IsLeaf() const;
    };

    BoundingBox3D m_bound;
    ContainerType m_items;
    std::vector<BoundingBox3D> m_itemBounds;
    std::vector<Node> m_nodes;

    size_t Build(size_t nodeIndex, size_t* itemIndices, size_t nItems,
                 size_t currentDepth);

    size_t QSplit(size_t* itemIndices, size_t numItems, double pivot,
                  uint8_t axis);
};
}  // namespace CubbyFlow

#include <Core/Geometry/BVH3-Impl.hpp>

#endif