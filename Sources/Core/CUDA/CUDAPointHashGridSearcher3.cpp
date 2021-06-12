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

#include <Core/CUDA/CUDAPointHashGridSearcher3.hpp>

using namespace CubbyFlow;

CUDAPointHashGridSearcher3::CUDAPointHashGridSearcher3(const uint3& resolution,
                                                       float gridSpacing)
    : CUDAPointHashGridSearcher3(resolution.x, resolution.y, resolution.z,
                                 gridSpacing)
{
    // Do nothing
}

CUDAPointHashGridSearcher3::CUDAPointHashGridSearcher3(
    const CUDAPointHashGridSearcher3& other)
{
    Set(other);
}

CUDAPointHashGridSearcher3::CUDAPointHashGridSearcher3(
    CUDAPointHashGridSearcher3&& other) noexcept
{
    m_gridSpacing = std::move(other.m_gridSpacing);
    m_resolution = std::move(other.m_resolution);
    m_points = std::move(other.m_points);
    m_keys = std::move(other.m_keys);
    m_startIndexTable = std::move(other.m_startIndexTable);
    m_endIndexTable = std::move(other.m_endIndexTable);
    m_sortedIndices = std::move(other.m_sortedIndices);
}

CUDAPointHashGridSearcher3& CUDAPointHashGridSearcher3::operator=(
    const CUDAPointHashGridSearcher3& other)
{
    Set(other);
    return (*this);
}

CUDAPointHashGridSearcher3& CUDAPointHashGridSearcher3::operator=(
    CUDAPointHashGridSearcher3&& other) noexcept
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

float CUDAPointHashGridSearcher3::GridSpacing() const
{
    return m_gridSpacing;
}

Vector3UZ CUDAPointHashGridSearcher3::Resolution() const
{
    return Vector3UZ{ static_cast<uint32_t>(m_resolution.x),
                      static_cast<uint32_t>(m_resolution.y),
                      static_cast<uint32_t>(m_resolution.z) };
}

ConstCUDAArrayView1<float4> CUDAPointHashGridSearcher3::SortedPoints() const
{
    return m_points;
}

ConstCUDAArrayView1<uint32_t> CUDAPointHashGridSearcher3::Keys() const
{
    return m_keys.View();
}

ConstCUDAArrayView1<uint32_t> CUDAPointHashGridSearcher3::StartIndexTable()
    const
{
    return m_startIndexTable.View();
}

ConstCUDAArrayView1<uint32_t> CUDAPointHashGridSearcher3::EndIndexTable() const
{
    return m_endIndexTable.View();
}

ConstCUDAArrayView1<uint32_t> CUDAPointHashGridSearcher3::SortedIndices() const
{
    return m_sortedIndices.View();
}

#endif
