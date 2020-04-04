// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_KDTREE_HPP
#define CUBBYFLOW_KDTREE_HPP

#include <Core/BoundingBox/BoundingBox.hpp>
#include <Core/Vector/Vector.hpp>

namespace CubbyFlow
{
//! Generic k-d tree structure.
template <typename T, size_t K>
class KdTree final
{
 public:
    using Point = Vector<T, K>;
    using BBox = BoundingBox<T, K>;

    //! Simple K-d tree node.
    struct Node
    {
        //! Split axis if flags < K, leaf indicator if flags == K.
        size_t flags = 0;

        //! \brief Right child index.
        //! Note that left child index is this node index + 1.
        size_t child = std::numeric_limits<size_t>::max();

        //! Item index.
        size_t item = std::numeric_limits<size_t>::max();

        //! Point stored in the node.
        Point point;

        //! Default contructor.
        Node();

        //! Initializes leaf node.
        void InitLeaf(size_t it, const Point& pt);

        //! Initializes internal node.
        void InitInternal(size_t axis, size_t it, size_t c, const Point& pt);

        //! Returns true if leaf.
        bool IsLeaf() const;
    };

    using ContainerType = std::vector<Point>;
    using Iterator = typename ContainerType::iterator;
    using ConstIterator = typename ContainerType::const_iterator;

    using NodeContainerType = std::vector<Node>;
    using NodeIterator = typename NodeContainerType::iterator;
    using ConstNodeIterator = typename NodeContainerType::const_iterator;

    //! Constructs an empty kD-tree instance.
    KdTree();

    //! Builds internal acceleration structure for given points list.
    void Build(const ConstArrayAccessor1<Point>& points);

    //!
    //! Invokes the callback function for each nearby point around the origin
    //! within given radius.
    //!
    //! \param[in]  origin   The origin position.
    //! \param[in]  radius   The search radius.
    //! \param[in]  callback The callback function.
    //!
    void ForEachNearbyPoint(
        const Point& origin, T radius,
        const std::function<void(size_t, const Point&)>& callback) const;

    //!
    //! Returns true if there are any nearby points for given origin within
    //! radius.
    //!
    //! \param[in]  origin The origin.
    //! \param[in]  radius The radius.
    //!
    //! \return     True if has nearby point, false otherwise.
    //!
    bool HasNearbyPoint(const Point& origin, T radius) const;

    //! Returns index of the nearest point.
    size_t GetNearestPoint(const Point& origin) const;

    //! Returns the mutable begin iterator of the item.
    Iterator begin();

    //! Returns the mutable end iterator of the item.
    Iterator end();

    //! Returns the immutable begin iterator of the item.
    ConstIterator begin() const;

    //! Returns the immutable end iterator of the item.
    ConstIterator end() const;

    //! Returns the mutable begin iterator of the node.
    NodeIterator BeginNode();

    //! Returns the mutable end iterator of the node.
    NodeIterator EndNode();

    //! Returns the immutable begin iterator of the node.
    ConstNodeIterator BeginNode() const;

    //! Returns the immutable end iterator of the node.
    ConstNodeIterator EndNode() const;

    //! Reserves memory space for this tree.
    void Reserve(size_t numPoints, size_t numNodes);

 private:
    std::vector<Point> m_points;
    std::vector<Node> m_nodes;

    size_t Build(size_t nodeIndex, size_t* itemIndices, size_t nItems,
                 size_t currentDepth);
};
}  // namespace CubbyFlow

#include <Core/Geometry/KdTree-Impl.hpp>

#endif