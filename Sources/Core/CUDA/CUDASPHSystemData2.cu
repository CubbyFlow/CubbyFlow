// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include "CUDASPHSystemData2Func.hpp"

#include <Core/CUDA/CUDAAlgorithms.hpp>
#include <Core/CUDA/CUDASPHSystemData2.hpp>
#include <Core/PointGenerator/TrianglePointGenerator.hpp>

#include <thrust/device_ptr.h>
#include <thrust/extrema.h>
#include <thrust/for_each.h>

using namespace CubbyFlow;

void CUDASPHSystemData2::UpdateDensities()
{
    NeighborSearcher()->ForEachNearbyPoint(
        Positions(), m_kernelRadius,
        UpdateDensity(m_kernelRadius, m_mass, Densities().data()));
}

void CUDASPHSystemData2::BuildNeighborListsAndUpdateDensities()
{
    size_t n = NumberOfParticles();

    m_neighborStarts.Resize(n);
    m_neighborEnds.Resize(n);

    auto neighborStarts = m_neighborStarts.View();

    // Count nearby points
    thrust::for_each(
        thrust::counting_iterator<size_t>(0),
        thrust::counting_iterator<size_t>(0) + NumberOfParticles(),
        ForEachNeighborFunc<NoOpFunc, CountNearbyPointsFunc>(
            *m_neighborSearcher, m_kernelRadius, Positions().data(), NoOpFunc(),
            CountNearbyPointsFunc(m_neighborStarts.data())));

    // Make start/end point of neighbor list, and allocate neighbor list.
    thrust::device_ptr<uint32_t> neighborStartsBegin(m_neighborStarts.data());
    thrust::device_ptr<uint32_t> neighborStartsEnd =
        neighborStartsBegin + m_neighborStarts.Length();
    thrust::device_ptr<uint32_t> neighborEndsBegin(m_neighborEnds.data());
    thrust::device_ptr<uint32_t> neighborEndsEnd =
        neighborEndsBegin + m_neighborEnds.Length();

    thrust::inclusive_scan(neighborStartsBegin, neighborStartsEnd,
                           neighborEndsBegin);
    thrust::transform(neighborEndsBegin, neighborEndsEnd, neighborStartsBegin,
                      neighborStartsBegin, thrust::minus<uint32_t>());
    size_t rbeginIdx =
        m_neighborEnds.Length() > 0 ? m_neighborEnds.Length() - 1 : 0;
    uint32_t m = m_neighborEnds[rbeginIdx];
    m_neighborLists.Resize(m, 0);

    // Build neighbor lists and update densities
    auto d = Densities();
    CUDAFill(d.data(), d.Length(), 0.0f);
    thrust::for_each(
        thrust::counting_iterator<size_t>(0),
        thrust::counting_iterator<size_t>(0) + NumberOfParticles(),
        ForEachNeighborFunc<BuildNeighborListsAndUpdateDensitiesFunc, NoOpFunc>(
            *m_neighborSearcher, m_kernelRadius, Positions().data(),
            BuildNeighborListsAndUpdateDensitiesFunc(
                m_neighborStarts.data(), m_neighborEnds.data(), m_kernelRadius,
                m_mass, m_neighborLists.data(), d.data()),
            NoOpFunc()));
}
