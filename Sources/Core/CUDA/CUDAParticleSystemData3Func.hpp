// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_PARTICLE_SYSTEM_DATA3_FUNC_HPP
#define CUBBYFLOW_CUDA_PARTICLE_SYSTEM_DATA3_FUNC_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/CUDA/CUDAPointHashGridSearcher3.hpp>
#include <Core/CUDA/CUDAUtils.hpp>
#include <Core/Utils/Macros.hpp>

namespace CubbyFlow
{
template <typename NeighborCallback, typename NeighborCounterCallback>
class ForEachNeighborFunc
{
 public:
    inline CUBBYFLOW_CUDA_HOST ForEachNeighborFunc(
        const CUDAPointHashGridSearcher3& searcher, float radius,
        const float4* origins, NeighborCallback insideCallback,
        NeighborCounterCallback cntCallback)
        : m_neighborCallback(insideCallback),
          m_neighborNeighborCounterCallback(cntCallback),
          m_hashUtils(searcher.GridSpacing(), ToUInt3(searcher.Resolution())),
          m_radius(radius),
          m_startIndexTable(searcher.StartIndexTable().data()),
          m_endIndexTable(searcher.EndIndexTable().data()),
          m_sortedIndices(searcher.SortedIndices().data()),
          m_points(searcher.SortedPoints().data()),
          m_origins(origins)
    {
        // Do nothing
    }

    template <typename Index>
    inline CUBBYFLOW_CUDA_HOST_DEVICE void operator()(Index i)
    {
        const float4 origin = m_origins[i];

        uint32_t nearbyKeys[8];
        m_hashUtils.GetNearbyKeys(origin, nearbyKeys);

        const float queryRadiusSquared = m_radius * m_radius;

        uint32_t cnt = 0;
        for (int c = 0; c < 8; c++)
        {
            uint32_t nearbyKey = nearbyKeys[c];
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
                float4 r = m_points[jj] - origin;

                float distanceSquared = LengthSquared(r);
                if (distanceSquared <= queryRadiusSquared)
                {
                    m_neighborCallback(i, j, cnt, distanceSquared);

                    if (i != j)
                    {
                        ++cnt;
                    }
                }
            }
        }

        m_neighborNeighborCounterCallback(i, cnt);
    }

 private:
    NeighborCallback m_neighborCallback;
    NeighborCounterCallback m_neighborNeighborCounterCallback;
    CUDAPointHashGridSearcher3::HashUtils m_hashUtils;
    float m_radius;
    const uint32_t* m_startIndexTable;
    const uint32_t* m_endIndexTable;
    const uint32_t* m_sortedIndices;
    const float4* m_points;
    const float4* m_origins;
};

class NoOpFunc
{
 public:
    template <typename Index>
    inline CUBBYFLOW_CUDA_HOST_DEVICE void operator()(size_t, Index)
    {
        // Do nothing
    }

    template <typename Index>
    inline CUBBYFLOW_CUDA_HOST_DEVICE void operator()(size_t, Index, Index,
                                                      float)
    {
        // Do nothing
    }
};

class BuildNeighborListsFunc
{
 public:
    inline CUBBYFLOW_CUDA_HOST_DEVICE BuildNeighborListsFunc(
        const uint32_t* neighborStarts, const uint32_t* neighborEnds,
        uint32_t* neighborLists)
        : m_neighborStarts(neighborStarts),
          m_neighborEnds(neighborEnds),
          m_neighborLists(neighborLists)
    {
        // Do nothing
    }

    template <typename Index>
    inline CUBBYFLOW_CUDA_HOST_DEVICE void operator()(size_t i, Index j,
                                                      Index cnt, float)
    {
        if (i != j)
        {
            m_neighborLists[m_neighborStarts[i] + cnt] = j;
        }
    }

 private:
    const uint32_t* m_neighborStarts;
    const uint32_t* m_neighborEnds;
    uint32_t* m_neighborLists;
};

class CountNearbyPointsFunc
{
 public:
    inline CUBBYFLOW_CUDA_HOST_DEVICE CountNearbyPointsFunc(uint32_t* cnt)
        : m_counts(cnt)
    {
        // Do nothing
    }

    template <typename Index>
    inline CUBBYFLOW_CUDA_HOST_DEVICE void operator()(size_t idx, Index cnt)
    {
        m_counts[idx] = cnt;
    }

 private:
    uint32_t* m_counts;
};
}  // namespace CubbyFlow

#endif

#endif