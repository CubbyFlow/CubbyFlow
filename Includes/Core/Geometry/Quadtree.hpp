// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_QUADTREE_HPP
#define CUBBYFLOW_QUADTREE_HPP

#include <Core/QueryEngine/IntersectionQueryEngine.hpp>
#include <Core/QueryEngine/NearestNeighborQueryEngine.hpp>

namespace CubbyFlow
{
//!
//! \brief      Generic quadtree data structure.
//!
//! This class is a generic quadtree representation to store arbitrary spatial
//! data. The quadtree supports closest neighbor search, overlapping test, and
//! ray intersection test.
//!
//! \tparam     T     Value type.
//!
template <typename T>
class Quadtree final : public IntersectionQueryEngine2<T>,
                       public NearestNeighborQueryEngine2<T>
{
 public:
    using ContainerType = std::vector<T>;
    using Iterator = typename ContainerType::iterator;
    using ConstIterator = typename ContainerType::const_iterator;

    //! Default constructor.
    Quadtree() = default;

    //! Default copy constructor.
    Quadtree(const Quadtree&) = default;

    //! Default move constructor.
    Quadtree(Quadtree&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~Quadtree() = default;

    //! Default copy assignment operator.
    Quadtree& operator=(const Quadtree&) = default;

    //! Default move assignment operator.
    Quadtree& operator=(Quadtree&&) noexcept = default;

    //! Builds an quadtree with given list of items, bounding box of the items,
    //! overlapping test function, and max depth of the tree.
    void Build(const std::vector<T>& items, const BoundingBox2D& bound,
               const BoxIntersectionTestFunc2<T>& testFunc, size_t maxDepth);

    //! Clears all the contents of this instance.
    void Clear();

    //! Returns the nearest neighbor for given point and distance measure
    //! function.
    [[nodiscard]] NearestNeighborQueryResult2<T> Nearest(
        const Vector2D& pt,
        const NearestNeighborDistanceFunc2<T>& distanceFunc) const override;

    //! Returns true if given \p box intersects with any of the stored items.
    [[nodiscard]] bool Intersects(
        const BoundingBox2D& box,
        const BoxIntersectionTestFunc2<T>& testFunc) const override;

    //! Returns true if given \p ray intersects with any of the stored items.
    [[nodiscard]] bool Intersects(
        const Ray2D& ray,
        const RayIntersectionTestFunc2<T>& testFunc) const override;

    //! Invokes \p visitorFunc for every intersecting items.
    void ForEachIntersectingItem(
        const BoundingBox2D& box, const BoxIntersectionTestFunc2<T>& testFunc,
        const IntersectionVisitorFunc<T>& visitorFunc) const override;

    //! Invokes \p visitorFunc for every intersecting items.
    void ForEachIntersectingItem(
        const Ray2D& ray, const RayIntersectionTestFunc2<T>& testFunc,
        const IntersectionVisitorFunc<T>& visitorFunc) const override;

    //! Returns the closest intersection for given \p ray.
    [[nodiscard]] ClosestIntersectionQueryResult2<T> ClosestIntersection(
        const Ray2D& ray,
        const GetRayIntersectionFunc2<T>& testFunc) const override;

    //! Returns the begin iterator of the item.
    [[nodiscard]] Iterator begin();

    //! Returns the end iterator of the item.
    [[nodiscard]] Iterator end();

    //! Returns the immutable begin iterator of the item.
    [[nodiscard]] ConstIterator begin() const;

    //! Returns the immutable end iterator of the item.
    [[nodiscard]] ConstIterator end() const;

    //! Returns the number of items.
    [[nodiscard]] size_t GetNumberOfItems() const;

    //! Returns the item at \p i.
    [[nodiscard]] const T& GetItem(size_t i) const;

    //! Returns the number of quadtree nodes.
    [[nodiscard]] size_t GetNumberOfNodes() const;

    //! Returns the list of the items for given node index.
    [[nodiscard]] const std::vector<size_t>& GetItemsAtNode(
        size_t nodeIdx) const;

    //!
    //! \brief      Returns a child's index for given node.
    //!
    //! For a given node, its children is stored continuously, such that if the
    //! node's first child's index is i, then i + 1, i + 2, ... , i + 7 are the
    //! indices for its children. The order of octant is x-major.
    //!
    //! \param[in]  nodeIdx The node index.
    //! \param[in]  childIdx The child index (0 to 7).
    //!
    //! \return     Index of the selected child.
    //!
    [[nodiscard]] size_t GetChildIndex(size_t nodeIdx, size_t childIdx) const;

    //! Returns the bounding box of this quadtree.
    [[nodiscard]] const BoundingBox2D& GetBoundingBox() const;

    //! Returns the maximum depth of the tree.
    [[nodiscard]] size_t GetMaxDepth() const;

 private:
    struct Node
    {
        [[nodiscard]] bool IsLeaf() const;

        size_t firstChild = std::numeric_limits<size_t>::max();
        std::vector<size_t> items;
    };

    void Build(size_t nodeIdx, size_t Depth, const BoundingBox2D& Bound,
               const BoxIntersectionTestFunc2<T>& testFunc);

    [[nodiscard]] bool Intersects(const BoundingBox2D& box,
                                  const BoxIntersectionTestFunc2<T>& testFunc,
                                  size_t nodeIdx,
                                  const BoundingBox2D& Bound) const;

    [[nodiscard]] bool Intersects(const Ray2D& ray,
                                  const RayIntersectionTestFunc2<T>& testFunc,
                                  size_t nodeIdx,
                                  const BoundingBox2D& Bound) const;

    void ForEachIntersectingItem(const BoundingBox2D& box,
                                 const BoxIntersectionTestFunc2<T>& testFunc,
                                 const IntersectionVisitorFunc<T>& visitorFunc,
                                 size_t nodeIdx,
                                 const BoundingBox2D& Bound) const;

    void ForEachIntersectingItem(const Ray2D& ray,
                                 const RayIntersectionTestFunc2<T>& testFunc,
                                 const IntersectionVisitorFunc<T>& visitorFunc,
                                 size_t nodeIdx,
                                 const BoundingBox2D& Bound) const;

    [[nodiscard]] ClosestIntersectionQueryResult2<T> ClosestIntersection(
        const Ray2D& ray, const GetRayIntersectionFunc2<T>& testFunc,
        size_t nodeIdx, const BoundingBox2D& Bound,
        ClosestIntersectionQueryResult2<T> best) const;

    size_t m_maxDepth = 1;
    BoundingBox2D m_bbox;
    std::vector<T> m_items;
    std::vector<Node> m_nodes;
};
}  // namespace CubbyFlow

#include <Core/Geometry/Quadtree-Impl.hpp>

#endif