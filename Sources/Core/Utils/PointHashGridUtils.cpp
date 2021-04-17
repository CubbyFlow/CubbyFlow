// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Utils/PointHashGridUtils.hpp>

namespace CubbyFlow
{
template <size_t N>
template <size_t M>
std::enable_if_t<M == 2, size_t> PointHashGridUtils<N>::HashKey(
    const Vector<ssize_t, 2>& index, const Vector<ssize_t, 2>& resolution)
{
    return static_cast<size_t>(index.y * resolution.x + index.x);
}

template <size_t N>
template <size_t M>
std::enable_if_t<M == 3, size_t> PointHashGridUtils<N>::HashKey(
    const Vector<ssize_t, 3>& index, const Vector<ssize_t, 3>& resolution)
{
    return static_cast<size_t>(
        (index.z * resolution.y + index.y) * resolution.x + index.x);
}

template <size_t N>
size_t PointHashGridUtils<N>::GetHashKeyFromBucketIndex(
    const Vector<ssize_t, N>& bucketIndex, const Vector<ssize_t, N>& resolution)
{
    Vector<ssize_t, N> wrappedIndex = bucketIndex;

    for (size_t i = 0; i < N; ++i)
    {
        wrappedIndex[i] = bucketIndex[i] % resolution[i];

        if (wrappedIndex[i] < 0)
        {
            wrappedIndex[i] += resolution[i];
        }
    }

    return HashKey(wrappedIndex, resolution);
}

template <size_t N>
Vector<ssize_t, N> PointHashGridUtils<N>::GetBucketIndex(
    const Vector<double, N>& position, double gridSpacing)
{
    Vector<ssize_t, N> bucketIndex =
        Floor(position / gridSpacing).template CastTo<ssize_t>();
    return bucketIndex;
}

template <size_t N>
size_t PointHashGridUtils<N>::GetHashKeyFromPosition(
    const Vector<double, N>& position, double gridSpacing,
    const Vector<ssize_t, N>& resolution)
{
    Vector<ssize_t, N> bucketIndex = GetBucketIndex(position, gridSpacing);
    return GetHashKeyFromBucketIndex(bucketIndex, resolution);
}

template <size_t N>
void PointHashGridUtils<N>::GetNearbyKeys(const Vector<double, N>& position,
                                          double gridSpacing,
                                          const Vector<ssize_t, N>& resolution,
                                          size_t* nearbyKeys)
{
    constexpr int numKeys = 1 << N;

    Vector<ssize_t, N> originIndex = GetBucketIndex(position, gridSpacing);
    Vector<ssize_t, N> nearbyBucketIndices[numKeys];

    for (int i = 0; i < numKeys; i++)
    {
        nearbyBucketIndices[i] = originIndex;
    }

    for (size_t axis = 0; axis < N; axis++)
    {
        int offset =
            (static_cast<double>(originIndex[axis]) + 0.5) * gridSpacing <=
                    position[axis]
                ? 1
                : -1;

        for (int j = 0; j < numKeys; j++)
        {
            if (j & (1 << axis))
            {
                nearbyBucketIndices[j][axis] += offset;
            }
        }
    }

    for (int i = 0; i < numKeys; i++)
    {
        nearbyKeys[i] =
            GetHashKeyFromBucketIndex(nearbyBucketIndices[i], resolution);
    }
}

template class PointHashGridUtils<2>;

template class PointHashGridUtils<3>;
}  // namespace CubbyFlow