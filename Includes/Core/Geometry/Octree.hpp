// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_OCTREE_HPP
#define CUBBYFLOW_OCTREE_HPP

#include <Core/QueryEngine/IntersectionQueryEngine.hpp>
#include <Core/QueryEngine/NearestNeighborQueryEngine.hpp>

namespace CubbyFlow
{
//!
//! \brief      Generic octree data structure.
//!
//! This class is a generic octree representation to store arbitrary spatial
//! data. The octree supports closest neighbor search, overlapping test, and
//! ray intersection test.
//!
//! \tparam     T     Value type.
//!
template <typename T>
class Octree final : public IntersectionQueryEngine3<T>,
                     public NearestNeighborQueryEngine3<T>
{
 public:
    using ContainerType = std::vector<T>;
    using Iterator = typename ContainerType::iterator;
    using ConstIterator = typename ContainerType::const_iterator;

    //! Default constructor.
    Octree() = default;

    //! Default copy constructor.
    Octree(const Octree&) = default;

    //! Default move constructor.
    Octree(Octree&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~Octree() = default;

    //! Default copy assignment operator.
    Octree& operator=(const Octree&) = default;

    //! Default move assignment operator.
    Octree& operator=(Octree&&) noexcept = default;

    //! Builds an octree with given list of items, bounding box of the items,
    //! overlapping test function, and max depth of the tree.
    void Build(const std::vector<T>& items, const BoundingBox3D& bound,
               const BoxIntersectionTestFunc3<T>& testFunc, size_t maxDepth);

    //! Clears all the contents of this instance.
    void Clear();

    //! Returns the nearest neighbor for given point and distance measure
    //! function.
    [[nodiscard]] NearestNeighborQueryResult3<T> Nearest(
        const Vector3D& pt,
        const NearestNeighborDistanceFunc3<T>& distanceFunc) const override;

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

    //! Returns the number of octree nodes.
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

    //! Returns the bounding box of this octree.
    [[nodiscard]] const BoundingBox3D& GetBoundingBox() const;

    //! Returns the maximum depth of the tree.
    [[nodiscard]] size_t GetMaxDepth() const;

 private:
    struct Node
    {
        [[nodiscard]] bool IsLeaf() const;

        size_t firstChild = std::numeric_limits<size_t>::max();
        std::vector<size_t> items;
    };

    void Build(size_t nodeIdx, size_t Depth, const BoundingBox3D& Bound,
               const BoxIntersectionTestFunc3<T>& testFunc);

    bool Intersects(const BoundingBox3D& box,
                      const BoxIntersectionTestFunc3<T>& testFunc,
                      size_t nodeIdx, const BoundingBox3D& Bound) const;

    [[nodiscard]] bool Intersects(const Ray3D& ray,
                                    const RayIntersectionTestFunc3<T>& testFunc,
                                    size_t nodeIdx,
                                    const BoundingBox3D& Bound) const;

    void ForEachIntersectingItem(const BoundingBox3D& box,
                                 const BoxIntersectionTestFunc3<T>& testFunc,
                                 const IntersectionVisitorFunc<T>& visitorFunc,
                                 size_t nodeIdx,
                                 const BoundingBox3D& Bound) const;

    void ForEachIntersectingItem(const Ray3D& ray,
                                 const RayIntersectionTestFunc3<T>& testFunc,
                                 const IntersectionVisitorFunc<T>& visitorFunc,
                                 size_t nodeIdx,
                                 const BoundingBox3D& Bound) const;

    [[nodiscard]] ClosestIntersectionQueryResult3<T> ClosestIntersection(
        const Ray3D& ray, const GetRayIntersectionFunc3<T>& testFunc,
        size_t nodeIdx, const BoundingBox3D& Bound,
        ClosestIntersectionQueryResult3<T> best) const;

    size_t m_maxDepth = 1;
    BoundingBox3D m_bbox;
    std::vector<T> m_items;
    std::vector<Node> m_nodes;
};
}  // namespace CubbyFlow

#include <Core/Geometry/Octree-Impl.hpp>

#endif