// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_LIST_QUERY_ENGINE3_IMPL_HPP
#define CUBBYFLOW_LIST_QUERY_ENGINE3_IMPL_HPP

namespace CubbyFlow
{
template <typename T>
void ListQueryEngine3<T>::Add(const T& item)
{
    m_items.push_back(item);
}

template <typename T>
void ListQueryEngine3<T>::Add(const std::vector<T>& items)
{
    m_items.insert(m_items.end(), items.begin(), items.end());
}

template <typename T>
bool ListQueryEngine3<T>::IsIntersects(
    const BoundingBox3D& box, const BoxIntersectionTestFunc3<T>& testFunc) const
{
    for (const auto& item : m_items)
    {
        if (testFunc(item, box))
        {
            return true;
        }
    }

    return false;
}

template <typename T>
bool ListQueryEngine3<T>::IsIntersects(
    const Ray3D& ray, const RayIntersectionTestFunc3<T>& testFunc) const
{
    for (const auto& item : m_items)
    {
        if (testFunc(item, ray))
        {
            return true;
        }
    }

    return false;
}

template <typename T>
void ListQueryEngine3<T>::ForEachIntersectingItem(
    const BoundingBox3D& box, const BoxIntersectionTestFunc3<T>& testFunc,
    const IntersectionVisitorFunc3<T>& visitorFunc) const
{
    for (const auto& item : m_items)
    {
        if (testFunc(item, box))
        {
            visitorFunc(item);
        }
    }
}

template <typename T>
void ListQueryEngine3<T>::ForEachIntersectingItem(
    const Ray3D& ray, const RayIntersectionTestFunc3<T>& testFunc,
    const IntersectionVisitorFunc3<T>& visitorFunc) const
{
    for (const auto& item : m_items)
    {
        if (testFunc(item, ray))
        {
            visitorFunc(item);
        }
    }
}

template <typename T>
ClosestIntersectionQueryResult3<T> ListQueryEngine3<T>::GetClosestIntersection(
    const Ray3D& ray, const GetRayIntersectionFunc3<T>& testFunc) const
{
    ClosestIntersectionQueryResult3<T> best;

    for (const auto& item : m_items)
    {
        double dist = testFunc(item, ray);
        if (dist < best.distance)
        {
            best.distance = dist;
            best.item = &item;
        }
    }

    return best;
}

template <typename T>
NearestNeighborQueryResult3<T> ListQueryEngine3<T>::GetNearestNeighbor(
    const Vector3D& pt,
    const NearestNeighborDistanceFunc3<T>& distanceFunc) const
{
    NearestNeighborQueryResult3<T> best;

    for (const auto& item : m_items)
    {
        double dist = distanceFunc(item, pt);
        if (dist < best.distance)
        {
            best.item = &item;
            best.distance = dist;
        }
    }

    return best;
}
}  // namespace CubbyFlow

#endif