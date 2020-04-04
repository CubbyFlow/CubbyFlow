// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/VertexCenteredScalarGrid2.hpp>

namespace CubbyFlow
{
VertexCenteredScalarGrid2::VertexCenteredScalarGrid2()
{
    // Do nothing
}

VertexCenteredScalarGrid2::VertexCenteredScalarGrid2(
    size_t resolutionX, size_t resolutionY, double gridSpacingX,
    double gridSpacingY, double originX, double originY, double initialValue)
{
    Resize(resolutionX, resolutionY, gridSpacingX, gridSpacingY, originX,
           originY, initialValue);
}

VertexCenteredScalarGrid2::VertexCenteredScalarGrid2(
    const Size2& resolution, const Vector2D& gridSpacing,
    const Vector2D& origin, double initialValue)
{
    Resize(resolution, gridSpacing, origin, initialValue);
}

VertexCenteredScalarGrid2::VertexCenteredScalarGrid2(
    const VertexCenteredScalarGrid2& other)
{
    Set(other);
}

Size2 VertexCenteredScalarGrid2::GetDataSize() const
{
    if (Resolution() != Size2(0, 0))
    {
        return Resolution() + Size2(1, 1);
    }

    return Size2(0, 0);
}

Vector2D VertexCenteredScalarGrid2::GetDataOrigin() const
{
    return Origin();
}

std::shared_ptr<ScalarGrid2> VertexCenteredScalarGrid2::Clone() const
{
    return std::shared_ptr<VertexCenteredScalarGrid2>(
        new VertexCenteredScalarGrid2(*this),
        [](VertexCenteredScalarGrid2* obj) { delete obj; });
}

void VertexCenteredScalarGrid2::Swap(Grid2* other)
{
    VertexCenteredScalarGrid2* sameType =
        dynamic_cast<VertexCenteredScalarGrid2*>(other);
    if (sameType != nullptr)
    {
        SwapScalarGrid(sameType);
    }
}

void VertexCenteredScalarGrid2::Set(const VertexCenteredScalarGrid2& other)
{
    SetScalarGrid(other);
}

VertexCenteredScalarGrid2& VertexCenteredScalarGrid2::operator=(
    const VertexCenteredScalarGrid2& other)
{
    Set(other);
    return *this;
}

VertexCenteredScalarGrid2::Builder VertexCenteredScalarGrid2::GetBuilder()
{
    return Builder();
}

VertexCenteredScalarGrid2::Builder&
VertexCenteredScalarGrid2::Builder::WithResolution(const Size2& resolution)
{
    m_resolution = resolution;
    return *this;
}

VertexCenteredScalarGrid2::Builder&
VertexCenteredScalarGrid2::Builder::WithResolution(size_t resolutionX,
                                                   size_t resolutionY)
{
    m_resolution.x = resolutionX;
    m_resolution.y = resolutionY;
    return *this;
}

VertexCenteredScalarGrid2::Builder&
VertexCenteredScalarGrid2::Builder::WithGridSpacing(const Vector2D& gridSpacing)
{
    m_gridSpacing = gridSpacing;
    return *this;
}

VertexCenteredScalarGrid2::Builder&
VertexCenteredScalarGrid2::Builder::WithGridSpacing(double gridSpacingX,
                                                    double gridSpacingY)
{
    m_gridSpacing.x = gridSpacingX;
    m_gridSpacing.y = gridSpacingY;
    return *this;
}

VertexCenteredScalarGrid2::Builder&
VertexCenteredScalarGrid2::Builder::WithOrigin(const Vector2D& gridOrigin)
{
    m_gridOrigin = gridOrigin;
    return *this;
}

VertexCenteredScalarGrid2::Builder&
VertexCenteredScalarGrid2::Builder::WithOrigin(double gridOriginX,
                                               double gridOriginY)
{
    m_gridOrigin.x = gridOriginX;
    m_gridOrigin.y = gridOriginY;
    return *this;
}

VertexCenteredScalarGrid2::Builder&
VertexCenteredScalarGrid2::Builder::WithInitialValue(double initialVal)
{
    m_initialVal = initialVal;
    return *this;
}

VertexCenteredScalarGrid2 VertexCenteredScalarGrid2::Builder::Build() const
{
    return VertexCenteredScalarGrid2(m_resolution, m_gridSpacing, m_gridOrigin,
                                     m_initialVal);
}

VertexCenteredScalarGrid2Ptr VertexCenteredScalarGrid2::Builder::MakeShared()
    const
{
    return std::shared_ptr<VertexCenteredScalarGrid2>(
        new VertexCenteredScalarGrid2(m_resolution, m_gridSpacing, m_gridOrigin,
                                      m_initialVal),
        [](VertexCenteredScalarGrid2* obj) { delete obj; });
}

ScalarGrid2Ptr VertexCenteredScalarGrid2::Builder::Build(
    const Size2& resolution, const Vector2D& gridSpacing,
    const Vector2D& gridOrigin, double initialVal) const
{
    return std::shared_ptr<VertexCenteredScalarGrid2>(
        new VertexCenteredScalarGrid2(resolution, gridSpacing, gridOrigin,
                                      initialVal),
        [](VertexCenteredScalarGrid2* obj) { delete obj; });
}
}  // namespace CubbyFlow