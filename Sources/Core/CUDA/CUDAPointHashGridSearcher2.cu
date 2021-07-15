// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/CUDA/CUDAAlgorithms.hpp>
#include <Core/CUDA/CUDAPointHashGridSearcher2.hpp>

#include <thrust/device_ptr.h>
#include <thrust/for_each.h>
#include <thrust/sort.h>

using namespace CubbyFlow;

namespace
{
__global__ void InitializeIndexTables(uint32_t* startIndexTable,
                                      uint32_t* endIndexTable, size_t n)
{
    size_t i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
    {
        startIndexTable[i] = 0xffffffff;
        endIndexTable[i] = 0xffffffff;
    }
}

__global__ void InitializePointAndKeys(
    CUDAPointHashGridSearcher2::HashUtils hashUtils, const float2* points,
    size_t n, uint32_t* sortedIndices, uint32_t* keys)
{
    size_t i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
    {
        sortedIndices[i] = i;
        keys[i] = hashUtils.GetHashKeyFromPosition(points[i]);
    }
}

__global__ void BuildTables(const uint32_t* keys, size_t n,
                            uint32_t* startIndexTable, uint32_t* endIndexTable)
{
    size_t i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n && i > 0)
    {
        uint32_t k = keys[i];
        uint32_t kLeft = keys[i - 1];
        if (k > kLeft)
        {
            startIndexTable[k] = i;
            endIndexTable[kLeft] = i;
        }
    }
}
}  // namespace

CUDAPointHashGridSearcher2::CUDAPointHashGridSearcher2(uint32_t resolutionX,
                                                       uint32_t resolutionY,
                                                       float gridSpacing)
    : m_gridSpacing{ gridSpacing }
{
    m_resolution.x = std::max(resolutionX, 1u);
    m_resolution.y = std::max(resolutionY, 1u);

    m_startIndexTable.Resize(m_resolution.x * m_resolution.y, 0xffffffff);
    m_endIndexTable.Resize(m_resolution.x * m_resolution.y, 0xffffffff);
}

void CUDAPointHashGridSearcher2::Build(
    const ConstCUDAArrayView1<float2>& points)
{
    // Allocate/reset memory chuncks
    size_t numberOfPoints = points.Length();
    if (numberOfPoints == 0)
    {
        return;
    }

    m_points = points;

    // Initialize index tables
    size_t numberOfGrids = m_startIndexTable.Length();
    unsigned int numBlocks, numThreads;
    CUDAComputeGridSize(static_cast<unsigned int>(numberOfPoints), 256,
                        numBlocks, numThreads);

    InitializeIndexTables<<<numBlocks, numThreads>>>(
        m_startIndexTable.data(), m_endIndexTable.data(),
        m_startIndexTable.Length());

    // Initialize indices array and generate hash key for each point
    m_keys.Resize(numberOfPoints);
    m_sortedIndices.Resize(numberOfPoints);

    CUDAComputeGridSize(static_cast<unsigned int>(numberOfPoints), 256,
                        numBlocks, numThreads);

    CUDAPointHashGridSearcher2::HashUtils hashUtils(m_gridSpacing,
                                                    m_resolution);

    InitializePointAndKeys<<<numBlocks, numThreads>>>(
        hashUtils, m_points.data(), m_points.Length(), m_sortedIndices.data(),
        m_keys.data());

    // Sort indices/points/key based on hash key
    thrust::device_ptr<uint32_t> keysBegin(m_keys.data());
    thrust::device_ptr<uint32_t> keysEnd = keysBegin + m_keys.Length();
    thrust::device_ptr<float2> pointsBegin(m_points.data());
    thrust::device_ptr<uint32_t> sortedIndicesBegin(m_sortedIndices.data());
    thrust::sort_by_key(keysBegin, keysEnd,
                        thrust::make_zip_iterator(thrust::make_tuple(
                            pointsBegin, sortedIndicesBegin)));

    // Now _points and _keys are sorted by points' hash key values.
    // Let's fill in start/end index table with _keys.
    // Assume that _keys array looks like:
    // [5|8|8|10|10|10]
    // Then _startIndexTable and _endIndexTable should be like:
    // [.....|0|...|1|..|3|..]
    // [.....|1|...|3|..|6|..]
    //       ^5    ^8   ^10
    // So that _endIndexTable[i] - _startIndexTable[i] is the number points in
    // i-th table bucket.

    m_startIndexTable[m_keys[0]] = 0;
    m_endIndexTable[m_keys[numberOfPoints - 1]] =
        static_cast<uint32_t>(numberOfPoints);

    CUDAComputeGridSize(static_cast<unsigned int>(numberOfPoints), 256,
                        numBlocks, numThreads);

    BuildTables<<<numBlocks, numThreads>>>(m_keys.data(), numberOfPoints,
                                           m_startIndexTable.data(),
                                           m_endIndexTable.data());
}

void CUDAPointHashGridSearcher2::Set(const CUDAPointHashGridSearcher2& other)
{
    m_gridSpacing = other.m_gridSpacing;
    m_resolution = other.m_resolution;
    m_points = other.m_points;
    m_keys = other.m_keys;
    m_startIndexTable = other.m_startIndexTable;
    m_endIndexTable = other.m_endIndexTable;
    m_sortedIndices = other.m_sortedIndices;
}