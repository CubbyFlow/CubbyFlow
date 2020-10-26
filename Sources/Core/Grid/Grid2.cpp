// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/Grid2.hpp>
#include <Core/Utils/Serial.hpp>

namespace CubbyFlow
{
const Size2& Grid2::Resolution() const
{
    return m_resolution;
}

const Vector2D& Grid2::Origin() const
{
    return m_origin;
}

const Vector2D& Grid2::GridSpacing() const
{
    return m_gridSpacing;
}

const BoundingBox2D& Grid2::BoundingBox() const
{
    return m_boundingBox;
}

Grid2::DataPositionFunc Grid2::CellCenterPosition() const
{
    Vector2D h = m_gridSpacing;
    Vector2D o = m_origin;

    return [h, o](const size_t i, const size_t j) {
        return o + h * Vector2D{ static_cast<double>(i) + 0.5,
                                 static_cast<double>(j) + 0.5 };
    };
}

void Grid2::ForEachCellIndex(
    const std::function<void(size_t, size_t)>& func) const
{
    SerialFor(ZERO_SIZE, m_resolution.x, ZERO_SIZE, m_resolution.y,
              [&func](const size_t i, const size_t j) { func(i, j); });
}

void Grid2::ParallelForEachCellIndex(
    const std::function<void(size_t, size_t)>& func) const
{
    ParallelFor(ZERO_SIZE, m_resolution.x, ZERO_SIZE, m_resolution.y,
                [&func](const size_t i, const size_t j) { func(i, j); });
}

bool Grid2::HasSameShape(const Grid2& other) const
{
    return m_resolution.x == other.m_resolution.x &&
           m_resolution.y == other.m_resolution.y &&
           Similar(m_gridSpacing.x, other.m_gridSpacing.x) &&
           Similar(m_gridSpacing.y, other.m_gridSpacing.y) &&
           Similar(m_origin.x, other.m_origin.x) &&
           Similar(m_origin.y, other.m_origin.y);
}

void Grid2::SetSizeParameters(const Size2& resolution,
                              const Vector2D& gridSpacing,
                              const Vector2D& origin)
{
    m_resolution = resolution;
    m_origin = origin;
    m_gridSpacing = gridSpacing;

    const Vector2D resolutionD = Vector2D{ static_cast<double>(resolution.x),
                                           static_cast<double>(resolution.y) };

    m_boundingBox = BoundingBox2D{ origin, origin + gridSpacing * resolutionD };
}

void Grid2::SwapGrid(Grid2* other)
{
    std::swap(m_resolution, other->m_resolution);
    std::swap(m_gridSpacing, other->m_gridSpacing);
    std::swap(m_origin, other->m_origin);
    std::swap(m_boundingBox, other->m_boundingBox);
}

void Grid2::SetGrid(const Grid2& other)
{
    m_resolution = other.m_resolution;
    m_gridSpacing = other.m_gridSpacing;
    m_origin = other.m_origin;
    m_boundingBox = other.m_boundingBox;
}
}  // namespace CubbyFlow