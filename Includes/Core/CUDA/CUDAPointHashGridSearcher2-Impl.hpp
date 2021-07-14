// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_POINT_HASH_GRID_SEARCHER2_IMPL_HPP
#define CUBBYFLOW_CUDA_POINT_HASH_GRID_SEARCHER2_IMPL_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/CUDA/CUDAUtils.hpp>

#include <thrust/for_each.h>
#include <thrust/iterator/counting_iterator.h>

namespace CubbyFlow
{
CUBBYFLOW_CUDA_HOST_DEVICE CUDAPointHashGridSearcher2::HashUtils::HashUtils(
    float gridSpacing, uint2 resolution)
    : m_gridSpacing{ gridSpacing }, m_resolution{ resolution }
{
    (void)m_dummy;
}

inline CUBBYFLOW_CUDA_HOST_DEVICE void
CUDAPointHashGridSearcher2::HashUtils::GetNearbyKeys(float2 position,
                                                     uint32_t* nearbyKeys) const
{
    int2 originIndex = GetBucketIndex(position), nearbyBucketIndices[8];

    for (int i = 0; i < 4; i++)
    {
        nearbyBucketIndices[i] = originIndex;
    }

    if ((originIndex.x + 0.5f) * m_gridSpacing <= position.x)
    {
        nearbyBucketIndices[2].x += 1;
        nearbyBucketIndices[3].x += 1;
    }
    else
    {
        nearbyBucketIndices[2].x -= 1;
        nearbyBucketIndices[3].x -= 1;
    }

    if ((originIndex.y + 0.5f) * m_gridSpacing <= position.y)
    {
        nearbyBucketIndices[1].y += 1;
        nearbyBucketIndices[3].y += 1;
    }
    else
    {
        nearbyBucketIndices[1].y -= 1;
        nearbyBucketIndices[3].y -= 1;
    }

    for (int i = 0; i < 4; i++)
    {
        nearbyKeys[i] = GetHashKeyFromBucketIndex(nearbyBucketIndices[i]);
    }
}

inline CUBBYFLOW_CUDA_HOST_DEVICE int2
CUDAPointHashGridSearcher2::HashUtils::GetBucketIndex(float2 position) const
{
    int2 bucketIndex;

    bucketIndex.x = static_cast<int>(floorf(position.x / m_gridSpacing));
    bucketIndex.y = static_cast<int>(floorf(position.y / m_gridSpacing));

    return bucketIndex;
}

inline CUBBYFLOW_CUDA_HOST_DEVICE uint32_t
CUDAPointHashGridSearcher2::HashUtils::GetHashKeyFromBucketIndex(
    int2 bucketIndex) const
{
    // Assumes m_resolution is power of two
    bucketIndex.x = bucketIndex.x & (m_resolution.x - 1);
    bucketIndex.y = bucketIndex.y & (m_resolution.y - 1);
    return bucketIndex.y * m_resolution.x + bucketIndex.x;
}

inline CUBBYFLOW_CUDA_HOST_DEVICE uint32_t
CUDAPointHashGridSearcher2::HashUtils::GetHashKeyFromPosition(
    float2 position) const
{
    int2 bucketIndex = GetBucketIndex(position);
    return GetHashKeyFromBucketIndex(bucketIndex);
}

template <typename Callback>
inline CUBBYFLOW_CUDA_HOST_DEVICE CUDAPointHashGridSearcher2::
    ForEachNearbyPointFunc<Callback>::ForEachNearbyPointFunc(
        float r, float gridSpacing, uint2 resolution, const uint32_t* sit,
        const uint32_t* eit, const uint32_t* si, const float2* p,
        const float2* o, Callback cb)
    : m_hashUtils(gridSpacing, resolution),
      m_radius(r),
      m_startIndexTable(sit),
      m_endIndexTable(eit),
      m_sortedIndices(si),
      m_points(p),
      m_origins(o),
      m_callback(cb)
{
    // Do nothing
}

template <typename Callback>
template <typename Index>
inline CUBBYFLOW_CUDA_HOST_DEVICE void
CUDAPointHashGridSearcher2::ForEachNearbyPointFunc<Callback>::operator()(
    Index idx)
{
    const float2 origin = m_origins[idx];

    uint32_t nearbyKeys[4];
    m_hashUtils.GetNearbyKeys(origin, nearbyKeys);

    const float queryRadiusSquared = m_radius * m_radius;

    for (int i = 0; i < 4; i++)
    {
        uint32_t nearbyKey = nearbyKeys[i];
        uint32_t start = m_startIndexTable[nearbyKey];

        // Empty bucket -- continue to next bucket
        if (start == 0xffffffff)
        {
            continue;
        }

        uint32_t end = m_endIndexTable[nearbyKey];

        for (uint32_t jj = start; jj < end; ++jj)
        {
            uint32_t j = m_sortedIndices[jj];
            float2 p = m_points[jj];
            float2 direction = p - origin;

            float distanceSquared = LengthSquared(direction);
            if (distanceSquared <= queryRadiusSquared)
            {
                if (distanceSquared > 0)
                {
                    float distance = sqrtf(distanceSquared);
                    direction /= distance;
                }

                m_callback(idx, origin, j, p);
            }
        }
    }
}

template <typename Callback>
void CUDAPointHashGridSearcher2::ForEachNearbyPoint(
    const ConstCUDAArrayView1<float2>& origins, float radius,
    Callback callback) const
{
    thrust::for_each(
        thrust::counting_iterator<size_t>(0),
        thrust::counting_iterator<size_t>(0) + origins.Length(),
        ForEachNearbyPointFunc<Callback>(
            radius, m_gridSpacing, m_resolution, m_startIndexTable.data(),
            m_endIndexTable.data(), m_sortedIndices.data(), m_points.data(),
            origins.data(), callback));
}
}  // namespace CubbyFlow

#endif

#endif