// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/CellCenteredVectorGrid2.hpp>

namespace CubbyFlow
{
CellCenteredVectorGrid2::CellCenteredVectorGrid2(
    size_t resolutionX, size_t resolutionY, double gridSpacingX,
    double gridSpacingY, double originX, double originY, double initialValueU,
    double initialValueV)
{
    Resize(resolutionX, resolutionY, gridSpacingX, gridSpacingY, originX,
           originY, initialValueU, initialValueV);
}

CellCenteredVectorGrid2::CellCenteredVectorGrid2(const Vector2UZ& resolution,
                                                 const Vector2D& gridSpacing,
                                                 const Vector2D& origin,
                                                 const Vector2D& initialValue)
{
    Resize(resolution, gridSpacing, origin, initialValue);
}

CellCenteredVectorGrid2::CellCenteredVectorGrid2(
    const CellCenteredVectorGrid2& other)
    : CollocatedVectorGrid2{ other }
{
    Set(other);
}

CellCenteredVectorGrid2& CellCenteredVectorGrid2::operator=(
    const CellCenteredVectorGrid2& other)
{
    Set(other);
    return *this;
}

Vector2UZ CellCenteredVectorGrid2::GetDataSize() const
{
    // The size of the data should be the same as the grid resolution.
    return Resolution();
}

Vector2D CellCenteredVectorGrid2::GetDataOrigin() const
{
    return Origin() + 0.5 * GridSpacing();
}

void CellCenteredVectorGrid2::Swap(Grid2* other)
{
    const auto sameType = dynamic_cast<CellCenteredVectorGrid2*>(other);
    if (sameType != nullptr)
    {
        SwapCollocatedVectorGrid(sameType);
    }
}

void CellCenteredVectorGrid2::Set(const CellCenteredVectorGrid2& other)
{
    SetCollocatedVectorGrid(other);
}

void CellCenteredVectorGrid2::Fill(const Vector2D& value,
                                   ExecutionPolicy policy)
{
    const Vector2UZ size = GetDataSize();
    auto acc = DataView();

    ParallelFor(
        ZERO_SIZE, size.x, ZERO_SIZE, size.y,
        [value, &acc](const size_t i, const size_t j) { acc(i, j) = value; },
        policy);
}

void CellCenteredVectorGrid2::Fill(
    const std::function<Vector2D(const Vector2D&)>& func,
    ExecutionPolicy policy)
{
    const Vector2UZ size = GetDataSize();
    ArrayView<Vector<double, 2>, 2> acc = DataView();
    DataPositionFunc pos = DataPosition();

    ParallelFor(
        ZERO_SIZE, size.x, ZERO_SIZE, size.y,
        [&func, &acc, &pos](const size_t i, const size_t j) {
            acc(i, j) = func(pos(i, j));
        },
        policy);
}

std::shared_ptr<VectorGrid2> CellCenteredVectorGrid2::Clone() const
{
    return std::shared_ptr<CellCenteredVectorGrid2>(
        new CellCenteredVectorGrid2{ *this },
        [](CellCenteredVectorGrid2* obj) { delete obj; });
}

CellCenteredVectorGrid2::Builder CellCenteredVectorGrid2::GetBuilder()
{
    return Builder{};
}

CellCenteredVectorGrid2::Builder&
CellCenteredVectorGrid2::Builder::WithResolution(const Vector2UZ& resolution)
{
    m_resolution = resolution;
    return *this;
}

CellCenteredVectorGrid2::Builder&
CellCenteredVectorGrid2::Builder::WithResolution(size_t resolutionX,
                                                 size_t resolutionY)
{
    m_resolution.x = resolutionX;
    m_resolution.y = resolutionY;
    return *this;
}

CellCenteredVectorGrid2::Builder&
CellCenteredVectorGrid2::Builder::WithGridSpacing(const Vector2D& gridSpacing)
{
    m_gridSpacing = gridSpacing;
    return *this;
}

CellCenteredVectorGrid2::Builder&
CellCenteredVectorGrid2::Builder::WithGridSpacing(double gridSpacingX,
                                                  double gridSpacingY)
{
    m_gridSpacing.x = gridSpacingX;
    m_gridSpacing.y = gridSpacingY;
    return *this;
}

CellCenteredVectorGrid2::Builder& CellCenteredVectorGrid2::Builder::WithOrigin(
    const Vector2D& gridOrigin)
{
    m_gridOrigin = gridOrigin;
    return *this;
}

CellCenteredVectorGrid2::Builder& CellCenteredVectorGrid2::Builder::WithOrigin(
    double gridOriginX, double gridOriginY)
{
    m_gridOrigin.x = gridOriginX;
    m_gridOrigin.y = gridOriginY;
    return *this;
}

CellCenteredVectorGrid2::Builder&
CellCenteredVectorGrid2::Builder::WithInitialValue(const Vector2D& initialVal)
{
    m_initialVal = initialVal;
    return *this;
}

CellCenteredVectorGrid2::Builder&
CellCenteredVectorGrid2::Builder::WithInitialValue(double initialValX,
                                                   double initialValY)
{
    m_initialVal.x = initialValX;
    m_initialVal.y = initialValY;
    return *this;
}

CellCenteredVectorGrid2 CellCenteredVectorGrid2::Builder::Build() const
{
    return CellCenteredVectorGrid2{ m_resolution, m_gridSpacing, m_gridOrigin,
                                    m_initialVal };
}

CellCenteredVectorGrid2Ptr CellCenteredVectorGrid2::Builder::MakeShared() const
{
    return std::shared_ptr<CellCenteredVectorGrid2>(
        new CellCenteredVectorGrid2{ m_resolution, m_gridSpacing, m_gridOrigin,
                                     m_initialVal },
        [](CellCenteredVectorGrid2* obj) { delete obj; });
}

VectorGrid2Ptr CellCenteredVectorGrid2::Builder::Build(
    const Vector2UZ& resolution, const Vector2D& gridSpacing,
    const Vector2D& gridOrigin, const Vector2D& initialVal) const
{
    return std::shared_ptr<CellCenteredVectorGrid2>(
        new CellCenteredVectorGrid2{ resolution, gridSpacing, gridOrigin,
                                     initialVal },
        [](CellCenteredVectorGrid2* obj) { delete obj; });
}
}  // namespace CubbyFlow