// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/Grid3.hpp>
#include <Core/Utils/Serial.hpp>

namespace CubbyFlow
{
const Size3& Grid3::Resolution() const
{
    return m_resolution;
}

const Vector3D& Grid3::Origin() const
{
    return m_origin;
}

const Vector3D& Grid3::GridSpacing() const
{
    return m_gridSpacing;
}

const BoundingBox3D& Grid3::BoundingBox() const
{
    return m_boundingBox;
}

Grid3::DataPositionFunc Grid3::CellCenterPosition() const
{
    Vector3D h = m_gridSpacing;
    Vector3D o = m_origin;

    return [h, o](const size_t i, const size_t j, const size_t k) {
        return o + h * Vector3D{ static_cast<double>(i) + 0.5,
                                 static_cast<double>(j) + 0.5,
                                 static_cast<double>(k) + 0.5 };
    };
}

void Grid3::ForEachCellIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    SerialFor(ZERO_SIZE, m_resolution.x, ZERO_SIZE, m_resolution.y, ZERO_SIZE,
              m_resolution.z,
              [&func](const size_t i, const size_t j, const size_t k) {
                  func(i, j, k);
              });
}

void Grid3::ParallelForEachCellIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    ParallelFor(ZERO_SIZE, m_resolution.x, ZERO_SIZE, m_resolution.y, ZERO_SIZE,
                m_resolution.z,
                [&func](const size_t i, const size_t j, const size_t k) {
                    func(i, j, k);
                });
}

bool Grid3::HasSameShape(const Grid3& other) const
{
    return m_resolution.x == other.m_resolution.x &&
           m_resolution.y == other.m_resolution.y &&
           m_resolution.z == other.m_resolution.z &&
           Similar(m_gridSpacing.x, other.m_gridSpacing.x) &&
           Similar(m_gridSpacing.y, other.m_gridSpacing.y) &&
           Similar(m_gridSpacing.z, other.m_gridSpacing.z) &&
           Similar(m_origin.x, other.m_origin.x) &&
           Similar(m_origin.y, other.m_origin.y) &&
           Similar(m_origin.z, other.m_origin.z);
}

void Grid3::SetSizeParameters(const Size3& resolution,
                              const Vector3D& gridSpacing,
                              const Vector3D& origin)
{
    m_resolution = resolution;
    m_origin = origin;
    m_gridSpacing = gridSpacing;

    const Vector3D resolutionD = Vector3D{ static_cast<double>(resolution.x),
                                           static_cast<double>(resolution.y),
                                           static_cast<double>(resolution.z) };

    m_boundingBox = BoundingBox3D{ origin, origin + gridSpacing * resolutionD };
}

void Grid3::SwapGrid(Grid3* other)
{
    std::swap(m_resolution, other->m_resolution);
    std::swap(m_gridSpacing, other->m_gridSpacing);
    std::swap(m_origin, other->m_origin);
    std::swap(m_boundingBox, other->m_boundingBox);
}

void Grid3::SetGrid(const Grid3& other)
{
    m_resolution = other.m_resolution;
    m_gridSpacing = other.m_gridSpacing;
    m_origin = other.m_origin;
    m_boundingBox = other.m_boundingBox;
}
}  // namespace CubbyFlow