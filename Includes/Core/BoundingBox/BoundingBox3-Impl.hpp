// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_BOUNDING_BOX3_IMPL_HPP
#define CUBBYFLOW_BOUNDING_BOX3_IMPL_HPP

namespace CubbyFlow
{
template <typename T>
BoundingBox<T, 3>::BoundingBox()
{
    Reset();
}

template <typename T>
BoundingBox<T, 3>::BoundingBox(const Vector3<T>& point1,
                               const Vector3<T>& point2)
{
    lowerCorner.x = std::min(point1.x, point2.x);
    lowerCorner.y = std::min(point1.y, point2.y);
    lowerCorner.z = std::min(point1.z, point2.z);
    upperCorner.x = std::max(point1.x, point2.x);
    upperCorner.y = std::max(point1.y, point2.y);
    upperCorner.z = std::max(point1.z, point2.z);
}

template <typename T>
BoundingBox<T, 3>::BoundingBox(const BoundingBox& other)
    : lowerCorner(other.lowerCorner), upperCorner(other.upperCorner)
{
    // Do nothing
}

template <typename T>
T BoundingBox<T, 3>::GetWidth() const
{
    return upperCorner.x - lowerCorner.x;
}

template <typename T>
T BoundingBox<T, 3>::GetHeight() const
{
    return upperCorner.y - lowerCorner.y;
}

template <typename T>
T BoundingBox<T, 3>::GetDepth() const
{
    return upperCorner.z - lowerCorner.z;
}

template <typename T>
T BoundingBox<T, 3>::Length(size_t axis)
{
    return upperCorner[axis] - lowerCorner[axis];
}

template <typename T>
bool BoundingBox<T, 3>::Overlaps(const BoundingBox& other) const
{
    if (upperCorner.x < other.lowerCorner.x ||
        lowerCorner.x > other.upperCorner.x)
    {
        return false;
    }

    if (upperCorner.y < other.lowerCorner.y ||
        lowerCorner.y > other.upperCorner.y)
    {
        return false;
    }

    if (upperCorner.z < other.lowerCorner.z ||
        lowerCorner.z > other.upperCorner.z)
    {
        return false;
    }

    return true;
}

template <typename T>
bool BoundingBox<T, 3>::Contains(const Vector3<T>& point) const
{
    if (upperCorner.x < point.x || lowerCorner.x > point.x)
    {
        return false;
    }

    if (upperCorner.y < point.y || lowerCorner.y > point.y)
    {
        return false;
    }

    if (upperCorner.z < point.z || lowerCorner.z > point.z)
    {
        return false;
    }

    return true;
}

template <typename T>
bool BoundingBox<T, 3>::Intersects(const Ray3<T>& ray) const
{
    T min = 0;
    T max = std::numeric_limits<T>::max();
    const Vector3<T>& rayInvDir = ray.direction.RDiv(1);

    for (size_t i = 0; i < 3; ++i)
    {
        T near = (lowerCorner[i] - ray.origin[i]) * rayInvDir[i];
        T far = (upperCorner[i] - ray.origin[i]) * rayInvDir[i];

        if (near > far)
        {
            std::swap(near, far);
        }

        min = std::max(near, min);
        max = std::min(far, max);

        if (min > max)
        {
            return false;
        }
    }

    return true;
}

template <typename T>
BoundingBoxRayIntersection3<T> BoundingBox<T, 3>::ClosestIntersection(
    const Ray3<T>& ray) const
{
    BoundingBoxRayIntersection3<T> intersection;

    T min = 0;
    T max = std::numeric_limits<T>::max();
    const Vector3<T>& rayInvDir = ray.direction.RDiv(1);

    for (size_t i = 0; i < 3; ++i)
    {
        T near = (lowerCorner[i] - ray.origin[i]) * rayInvDir[i];
        T far = (upperCorner[i] - ray.origin[i]) * rayInvDir[i];

        if (near > far)
        {
            std::swap(near, far);
        }

        min = std::max(near, min);
        max = std::min(far, max);

        if (min > max)
        {
            intersection.isIntersecting = false;
            return intersection;
        }
    }

    intersection.isIntersecting = true;

    if (Contains(ray.origin))
    {
        intersection.near = max;
        intersection.far = std::numeric_limits<T>::max();
    }
    else
    {
        intersection.near = min;
        intersection.far = max;
    }

    return intersection;
}

template <typename T>
Vector3<T> BoundingBox<T, 3>::MidPoint() const
{
    return (upperCorner + lowerCorner) / static_cast<T>(2);
}

template <typename T>
T BoundingBox<T, 3>::DiagonalLength() const
{
    return (upperCorner - lowerCorner).Length();
}

template <typename T>
T BoundingBox<T, 3>::DiagonalLengthSquared() const
{
    return (upperCorner - lowerCorner).LengthSquared();
}

template <typename T>
void BoundingBox<T, 3>::Reset()
{
    lowerCorner.x = std::numeric_limits<T>::max();
    lowerCorner.y = std::numeric_limits<T>::max();
    lowerCorner.z = std::numeric_limits<T>::max();
    upperCorner.x = -std::numeric_limits<T>::max();
    upperCorner.y = -std::numeric_limits<T>::max();
    upperCorner.z = -std::numeric_limits<T>::max();
}

template <typename T>
void BoundingBox<T, 3>::Merge(const Vector3<T>& point)
{
    lowerCorner.x = std::min(lowerCorner.x, point.x);
    lowerCorner.y = std::min(lowerCorner.y, point.y);
    lowerCorner.z = std::min(lowerCorner.z, point.z);
    upperCorner.x = std::max(upperCorner.x, point.x);
    upperCorner.y = std::max(upperCorner.y, point.y);
    upperCorner.z = std::max(upperCorner.z, point.z);
}

template <typename T>
void BoundingBox<T, 3>::Merge(const BoundingBox& other)
{
    lowerCorner.x = std::min(lowerCorner.x, other.lowerCorner.x);
    lowerCorner.y = std::min(lowerCorner.y, other.lowerCorner.y);
    lowerCorner.z = std::min(lowerCorner.z, other.lowerCorner.z);
    upperCorner.x = std::max(upperCorner.x, other.upperCorner.x);
    upperCorner.y = std::max(upperCorner.y, other.upperCorner.y);
    upperCorner.z = std::max(upperCorner.z, other.upperCorner.z);
}

template <typename T>
void BoundingBox<T, 3>::Expand(T delta)
{
    lowerCorner -= delta;
    upperCorner += delta;
}

template <typename T>
Vector3<T> BoundingBox<T, 3>::Corner(size_t idx) const
{
    static const T h = static_cast<T>(1) / 2;
    static const Vector3<T> offset[8] = { { -h, -h, -h }, { +h, -h, -h },
                                          { -h, +h, -h }, { +h, +h, -h },
                                          { -h, -h, +h }, { +h, -h, +h },
                                          { -h, +h, +h }, { +h, +h, +h } };

    return Vector3<T>(GetWidth(), GetHeight(), GetDepth()) * offset[idx] +
           MidPoint();
}

template <typename T>
Vector3<T> BoundingBox<T, 3>::Clamp(const Vector3<T>& pt) const
{
    return CubbyFlow::Clamp(pt, lowerCorner, upperCorner);
}

template <typename T>
bool BoundingBox<T, 3>::IsEmpty() const
{
    return (lowerCorner.x >= upperCorner.x || lowerCorner.y >= upperCorner.y ||
            lowerCorner.z >= upperCorner.z);
}
}  // namespace CubbyFlow

#endif