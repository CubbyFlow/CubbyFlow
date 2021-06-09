// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#if CUBBYFLOW_USE_CUDA

#include <Core/CUDA/CUDAPointHashGridSearcher2.hpp>

using namespace CubbyFlow;

CUDAPointHashGridSearcher2::CUDAPointHashGridSearcher2(const uint2& resolution,
                                                       float gridSpacing)
    : CUDAPointHashGridSearcher2(resolution.x, resolution.y, gridSpacing)
{
    // Do nothing
}

CUDAPointHashGridSearcher2::CUDAPointHashGridSearcher2(
    const CUDAPointHashGridSearcher2& other)
{
    Set(other);
}

CUDAPointHashGridSearcher2::CUDAPointHashGridSearcher2(
    CUDAPointHashGridSearcher2&& other) noexcept
{
    m_gridSpacing = std::move(other.m_gridSpacing);
    m_resolution = std::move(other.m_resolution);
    m_points = std::move(other.m_points);
    m_keys = std::move(other.m_keys);
    m_startIndexTable = std::move(other.m_startIndexTable);
    m_endIndexTable = std::move(other.m_endIndexTable);
    m_sortedIndices = std::move(other.m_sortedIndices);
}

CUDAPointHashGridSearcher2& CUDAPointHashGridSearcher2::operator=(
    const CUDAPointHashGridSearcher2& other)
{
    Set(other);
    return *this;
}

CUDAPointHashGridSearcher2& CUDAPointHashGridSearcher2::operator=(
    CUDAPointHashGridSearcher2&& other) noexcept
{
    m_gridSpacing = std::move(other.m_gridSpacing);
    m_resolution = std::move(other.m_resolution);
    m_points = std::move(other.m_points);
    m_keys = std::move(other.m_keys);
    m_startIndexTable = std::move(other.m_startIndexTable);
    m_endIndexTable = std::move(other.m_endIndexTable);
    m_sortedIndices = std::move(other.m_sortedIndices);
    return *this;
}

float CUDAPointHashGridSearcher2::GridSpacing() const
{
    return m_gridSpacing;
}

Vector2UZ CUDAPointHashGridSearcher2::Resolution() const
{
    return Vector2UZ{ static_cast<uint32_t>(m_resolution.x),
                      static_cast<uint32_t>(m_resolution.y) };
}

ConstCUDAArrayView1<float2> CUDAPointHashGridSearcher2::SortedPoints() const
{
    return m_points;
}

ConstCUDAArrayView1<uint32_t> CUDAPointHashGridSearcher2::Keys() const
{
    return m_keys.View();
}

ConstCUDAArrayView1<uint32_t> CUDAPointHashGridSearcher2::StartIndexTable()
    const
{
    return m_startIndexTable.View();
}

ConstCUDAArrayView1<uint32_t> CUDAPointHashGridSearcher2::EndIndexTable() const
{
    return m_endIndexTable.View();
}

ConstCUDAArrayView1<uint32_t> CUDAPointHashGridSearcher2::SortedIndices() const
{
    return m_sortedIndices.View();
}

#endif
