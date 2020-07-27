// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_BVH3_IMPL_HPP
#define CUBBYFLOW_BVH3_IMPL_HPP

#include <numeric>

namespace CubbyFlow
{
template <typename T>
BVH3<T>::Node::Node() : flags(0)
{
    child = std::numeric_limits<size_t>::max();
}

template <typename T>
void BVH3<T>::Node::InitLeaf(size_t it, const BoundingBox3D& b)
{
    flags = 3;
    item = it;
    bound = b;
}

template <typename T>
void BVH3<T>::Node::InitInternal(uint8_t axis, size_t c, const BoundingBox3D& b)
{
    flags = axis;
    child = c;
    bound = b;
}

template <typename T>
bool BVH3<T>::Node::IsLeaf() const
{
    return flags == 3;
}

template <typename T>
BVH3<T>::BVH3()
{
    // Do nothing
}

template <typename T>
void BVH3<T>::Build(const std::vector<T>& items,
                    const std::vector<BoundingBox3D>& itemsBounds)
{
    m_items = items;
    m_itemBounds = itemsBounds;

    if (m_items.empty())
    {
        return;
    }

    m_nodes.clear();
    m_bound = BoundingBox3D{};

    for (size_t i = 0; i < m_items.size(); ++i)
    {
        m_bound.Merge(m_itemBounds[i]);
    }

    std::vector<size_t> itemIndices(m_items.size());
    std::iota(std::begin(itemIndices), std::end(itemIndices), 0);

    Build(0, itemIndices.data(), m_items.size(), 0);
}

template <typename T>
void BVH3<T>::Clear()
{
    m_bound = BoundingBox3D();
    m_items.clear();
    m_itemBounds.clear();
    m_nodes.clear();
}

template <typename T>
inline NearestNeighborQueryResult3<T> BVH3<T>::GetNearestNeighbor(
    const Vector3D& pt,
    const NearestNeighborDistanceFunc3<T>& distanceFunc) const
{
    NearestNeighborQueryResult3<T> best;
    best.distance = std::numeric_limits<double>::max();
    best.item = nullptr;

    // Prepare to traverse BVH
    static const int maxTreeDepth = 8 * sizeof(size_t);
    const Node* todo[maxTreeDepth];
    size_t todoPos = 0;

    // Traverse BVH nodes
    const Node* node = m_nodes.data();
    while (node != nullptr)
    {
        if (node->IsLeaf())
        {
            double dist = distanceFunc(m_items[node->item], pt);
            if (dist < best.distance)
            {
                best.distance = dist;
                best.item = &m_items[node->item];
            }

            // Grab next node to process from todo stack
            if (todoPos > 0)
            {
                // Dequeue
                --todoPos;
                node = todo[todoPos];
            }
            else
            {
                break;
            }
        }
        else
        {
            const double bestDistSqr = best.distance * best.distance;

            const Node* left = node + 1;
            const Node* right = &m_nodes[node->child];

            // If pt is inside the box, then the closestLeft and Right will be
            // identical to pt. This will make distMinLeftSqr and
            // distMinRightSqr zero, meaning that such a box will have higher
            // priority.
            Vector3D closestLeft = left->bound.Clamp(pt);
            Vector3D closestRight = right->bound.Clamp(pt);

            double distMinLeftSqr = closestLeft.DistanceSquaredTo(pt);
            double distMinRightSqr = closestRight.DistanceSquaredTo(pt);

            bool shouldVisitLeft = distMinLeftSqr < bestDistSqr;
            bool shouldVisitRight = distMinRightSqr < bestDistSqr;

            const Node* firstChild;
            const Node* secondChild;

            if (shouldVisitLeft && shouldVisitRight)
            {
                if (distMinLeftSqr < distMinRightSqr)
                {
                    firstChild = left;
                    secondChild = right;
                }
                else
                {
                    firstChild = right;
                    secondChild = left;
                }

                // Enqueue secondChild in todo stack
                todo[todoPos] = secondChild;
                ++todoPos;
                node = firstChild;
            }
            else if (shouldVisitLeft)
            {
                node = left;
            }
            else if (shouldVisitRight)
            {
                node = right;
            }
            else
            {
                if (todoPos > 0)
                {
                    // Dequeue
                    --todoPos;
                    node = todo[todoPos];
                }
                else
                {
                    break;
                }
            }
        }
    }

    return best;
}

template <typename T>
inline bool BVH3<T>::IsIntersects(
    const BoundingBox3D& box, const BoxIntersectionTestFunc3<T>& testFunc) const
{
    if (!m_bound.Overlaps(box))
    {
        return false;
    }

    // prepare to traverse BVH for box
    static const int maxTreeDepth = 8 * sizeof(size_t);
    const Node* todo[maxTreeDepth];
    size_t todoPos = 0;

    // traverse BVH nodes for box
    const Node* node = m_nodes.data();

    while (node != nullptr)
    {
        if (node->IsLeaf())
        {
            if (testFunc(m_items[node->item], box))
            {
                return true;
            }

            // grab next node to process from todo stack
            if (todoPos > 0)
            {
                // Dequeue
                --todoPos;
                node = todo[todoPos];
            }
            else
            {
                break;
            }
        }
        else
        {
            // get node children pointers for box
            const Node* firstChild = node + 1;
            const Node* secondChild = const_cast<Node*>(&m_nodes[node->child]);

            // advance to next child node, possibly enqueue other child
            if (!firstChild->bound.Overlaps(box))
            {
                node = secondChild;
            }
            else if (!secondChild->bound.Overlaps(box))
            {
                node = firstChild;
            }
            else
            {
                // enqueue secondChild in todo stack
                todo[todoPos] = secondChild;
                ++todoPos;
                node = firstChild;
            }
        }
    }

    return false;
}

template <typename T>
inline bool BVH3<T>::IsIntersects(
    const Ray3D& ray, const RayIntersectionTestFunc3<T>& testFunc) const
{
    if (!m_bound.Intersects(ray))
    {
        return false;
    }

    // prepare to traverse BVH for ray
    static const int maxTreeDepth = 8 * sizeof(size_t);
    const Node* todo[maxTreeDepth];
    size_t todoPos = 0;

    // traverse BVH nodes for ray
    const Node* node = m_nodes.data();

    while (node != nullptr)
    {
        if (node->IsLeaf())
        {
            if (testFunc(m_items[node->item], ray))
            {
                return true;
            }

            // grab next node to process from todo stack
            if (todoPos > 0)
            {
                // Dequeue
                --todoPos;
                node = todo[todoPos];
            }
            else
            {
                break;
            }
        }
        else
        {
            // get node children pointers for ray
            const Node* firstChild;
            const Node* secondChild;

            if (ray.direction[node->flags] > 0.0)
            {
                firstChild = node + 1;
                secondChild = const_cast<Node*>(&m_nodes[node->child]);
            }
            else
            {
                firstChild = const_cast<Node*>(&m_nodes[node->child]);
                secondChild = node + 1;
            }

            // advance to next child node, possibly enqueue other child
            if (!firstChild->bound.Intersects(ray))
            {
                node = secondChild;
            }
            else if (!secondChild->bound.Intersects(ray))
            {
                node = firstChild;
            }
            else
            {
                // enqueue secondChild in todo stack
                todo[todoPos] = secondChild;
                ++todoPos;
                node = firstChild;
            }
        }
    }

    return false;
}

template <typename T>
inline void BVH3<T>::ForEachIntersectingItem(
    const BoundingBox3D& box, const BoxIntersectionTestFunc3<T>& testFunc,
    const IntersectionVisitorFunc3<T>& visitorFunc) const
{
    if (!m_bound.Overlaps(box))
    {
        return;
    }

    // prepare to traverse BVH for box
    static const int maxTreeDepth = 8 * sizeof(size_t);
    const Node* todo[maxTreeDepth];
    size_t todoPos = 0;

    // traverse BVH nodes for box
    const Node* node = m_nodes.data();

    while (node != nullptr)
    {
        if (node->IsLeaf())
        {
            if (testFunc(m_items[node->item], box))
            {
                visitorFunc(m_items[node->item]);
            }

            // grab next node to process from todo stack
            if (todoPos > 0)
            {
                // Dequeue
                --todoPos;
                node = todo[todoPos];
            }
            else
            {
                break;
            }
        }
        else
        {
            // get node children pointers for box
            const Node* firstChild = node + 1;
            const Node* secondChild = const_cast<Node*>(&m_nodes[node->child]);

            // advance to next child node, possibly enqueue other child
            if (!firstChild->bound.Overlaps(box))
            {
                node = secondChild;
            }
            else if (!secondChild->bound.Overlaps(box))
            {
                node = firstChild;
            }
            else
            {
                // enqueue secondChild in todo stack
                todo[todoPos] = secondChild;
                ++todoPos;
                node = firstChild;
            }
        }
    }
}

template <typename T>
inline void BVH3<T>::ForEachIntersectingItem(
    const Ray3D& ray, const RayIntersectionTestFunc3<T>& testFunc,
    const IntersectionVisitorFunc3<T>& visitorFunc) const
{
    if (!m_bound.Intersects(ray))
    {
        return;
    }

    // prepare to traverse BVH for ray
    static const int maxTreeDepth = 8 * sizeof(size_t);
    const Node* todo[maxTreeDepth];
    size_t todoPos = 0;

    // traverse BVH nodes for ray
    const Node* node = m_nodes.data();

    while (node != nullptr)
    {
        if (node->IsLeaf())
        {
            if (testFunc(m_items[node->item], ray))
            {
                visitorFunc(m_items[node->item]);
            }

            // grab next node to process from todo stack
            if (todoPos > 0)
            {
                // Dequeue
                --todoPos;
                node = todo[todoPos];
            }
            else
            {
                break;
            }
        }
        else
        {
            // get node children pointers for ray
            const Node* firstChild;
            const Node* secondChild;

            if (ray.direction[node->flags] > 0.0)
            {
                firstChild = node + 1;
                secondChild = const_cast<Node*>(&m_nodes[node->child]);
            }
            else
            {
                firstChild = const_cast<Node*>(&m_nodes[node->child]);
                secondChild = node + 1;
            }

            // advance to next child node, possibly enqueue other child
            if (!firstChild->bound.Intersects(ray))
            {
                node = secondChild;
            }
            else if (!secondChild->bound.Intersects(ray))
            {
                node = firstChild;
            }
            else
            {
                // enqueue secondChild in todo stack
                todo[todoPos] = secondChild;
                ++todoPos;
                node = firstChild;
            }
        }
    }
}

template <typename T>
inline ClosestIntersectionQueryResult3<T> BVH3<T>::GetClosestIntersection(
    const Ray3D& ray, const GetRayIntersectionFunc3<T>& testFunc) const
{
    ClosestIntersectionQueryResult3<T> best;
    best.distance = std::numeric_limits<double>::max();
    best.item = nullptr;

    if (!m_bound.Intersects(ray))
    {
        return best;
    }

    // prepare to traverse BVH for ray
    static const int maxTreeDepth = 8 * sizeof(size_t);
    const Node* todo[maxTreeDepth];
    size_t todoPos = 0;

    // traverse BVH nodes for ray
    const Node* node = m_nodes.data();

    while (node != nullptr)
    {
        if (node->IsLeaf())
        {
            double dist = testFunc(m_items[node->item], ray);
            if (dist < best.distance)
            {
                best.distance = dist;
                best.item = m_items.data() + node->item;
            }

            // grab next node to process from todo stack
            if (todoPos > 0)
            {
                // Dequeue
                --todoPos;
                node = todo[todoPos];
            }
            else
            {
                break;
            }
        }
        else
        {
            // get node children pointers for ray
            const Node* firstChild;
            const Node* secondChild;

            if (ray.direction[node->flags] > 0.0)
            {
                firstChild = node + 1;
                secondChild = const_cast<Node*>(&m_nodes[node->child]);
            }
            else
            {
                firstChild = const_cast<Node*>(&m_nodes[node->child]);
                secondChild = node + 1;
            }

            // advance to next child node, possibly enqueue other child
            if (!firstChild->bound.Intersects(ray))
            {
                node = secondChild;
            }
            else if (!secondChild->bound.Intersects(ray))
            {
                node = firstChild;
            }
            else
            {
                // enqueue secondChild in todo stack
                todo[todoPos] = secondChild;
                ++todoPos;
                node = firstChild;
            }
        }
    }

    return best;
}

template <typename T>
const BoundingBox3D& BVH3<T>::GetBoundingBox() const
{
    return m_bound;
}

template <typename T>
typename BVH3<T>::Iterator BVH3<T>::begin()
{
    return m_items.begin();
}

template <typename T>
typename BVH3<T>::Iterator BVH3<T>::end()
{
    return m_items.end();
}

template <typename T>
typename BVH3<T>::ConstIterator BVH3<T>::begin() const
{
    return m_items.begin();
}

template <typename T>
typename BVH3<T>::ConstIterator BVH3<T>::end() const
{
    return m_items.end();
}

template <typename T>
size_t BVH3<T>::GetNumberOfItems() const
{
    return m_items.size();
}

template <typename T>
const T& BVH3<T>::GetItem(size_t i) const
{
    return m_items[i];
}

template <typename T>
size_t BVH3<T>::GetNumberOfNodes() const
{
    return m_nodes.size();
}

template <typename T>
std::pair<size_t, size_t> BVH3<T>::GetChildren(size_t i) const
{
    if (IsLeaf(i))
    {
        return std::make_pair(std::numeric_limits<size_t>::max(),
                              std::numeric_limits<size_t>::max());
    }
    else
    {
        return std::make_pair(i + 1, m_nodes[i].child);
    }
}

template <typename T>
bool BVH3<T>::IsLeaf(size_t i) const
{
    return m_nodes[i].IsLeaf();
}

template <typename T>
const BoundingBox3D& BVH3<T>::GetNodeBound(size_t i) const
{
    return m_nodes[i].bound;
}

template <typename T>
typename BVH3<T>::Iterator BVH3<T>::GetItemOfNode(size_t i)
{
    if (IsLeaf(i))
    {
        return m_nodes[i].item + begin();
    }
    else
    {
        return end();
    }
}

template <typename T>
typename BVH3<T>::ConstIterator BVH3<T>::GetItemOfNode(size_t i) const
{
    if (IsLeaf(i))
    {
        return m_nodes[i].item + begin();
    }
    else
    {
        return end();
    }
}

template <typename T>
size_t BVH3<T>::Build(size_t nodeIndex, size_t* itemIndices, size_t nItems,
                      size_t currentDepth)
{
    // add a node
    m_nodes.push_back(Node());

    // Initialize leaf node if termination criteria met
    if (nItems == 1)
    {
        m_nodes[nodeIndex].InitLeaf(itemIndices[0],
                                    m_itemBounds[itemIndices[0]]);
        return currentDepth + 1;
    }

    // find the mid-point of the bounding box to use as a qsplit pivot
    BoundingBox3D nodeBound;
    for (size_t i = 0; i < nItems; ++i)
    {
        nodeBound.Merge(m_itemBounds[itemIndices[i]]);
    }

    Vector3D d = nodeBound.upperCorner - nodeBound.lowerCorner;

    // choose which axis to split along
    uint8_t axis;
    if (d.x > d.y && d.x > d.z)
    {
        axis = 0;
    }
    else
    {
        axis = (d.y > d.z) ? 1 : 2;
    }

    double pivot =
        0.5 * (nodeBound.upperCorner[axis] + nodeBound.lowerCorner[axis]);

    // classify primitives with respect to split
    size_t midPoint = QSplit(itemIndices, nItems, pivot, axis);

    // recursively Initialize children m_nodes
    size_t d0 = Build(nodeIndex + 1, itemIndices, midPoint, currentDepth + 1);
    m_nodes[nodeIndex].InitInternal(axis, m_nodes.size(), nodeBound);
    size_t d1 = Build(m_nodes[nodeIndex].child, itemIndices + midPoint,
                      nItems - midPoint, currentDepth + 1);

    return std::max(d0, d1);
}

template <typename T>
size_t BVH3<T>::QSplit(size_t* itemIndices, size_t numItems, double pivot,
                       uint8_t axis)
{
    size_t ret = 0;

    for (size_t i = 0; i < numItems; ++i)
    {
        BoundingBox3D b = m_itemBounds[itemIndices[i]];
        double centroid = 0.5f * (b.lowerCorner[axis] + b.upperCorner[axis]);

        if (centroid < pivot)
        {
            std::swap(itemIndices[i], itemIndices[ret]);
            ret++;
        }
    }

    if (ret == 0 || ret == numItems)
    {
        ret = numItems >> 1;
    }

    return ret;
}
}  // namespace CubbyFlow

#endif