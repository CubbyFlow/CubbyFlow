// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/VertexCenteredVectorGrid2.hpp>

namespace CubbyFlow
{
VertexCenteredVectorGrid2::VertexCenteredVectorGrid2(
    size_t resolutionX, size_t resolutionY, double gridSpacingX,
    double gridSpacingY, double originX, double originY, double initialValueU,
    double initialValueV)
{
    Resize({ resolutionX, resolutionY }, { gridSpacingX, gridSpacingY },
           { originX, originY }, { initialValueU, initialValueV });
}

VertexCenteredVectorGrid2::VertexCenteredVectorGrid2(
    const Vector2UZ& resolution, const Vector2D& gridSpacing,
    const Vector2D& origin, const Vector2D& initialValue)
{
    Resize(resolution, gridSpacing, origin, initialValue);
}

VertexCenteredVectorGrid2::VertexCenteredVectorGrid2(
    const VertexCenteredVectorGrid2& other)
    : CollocatedVectorGrid2{ other }
{
    Set(other);
}

VertexCenteredVectorGrid2& VertexCenteredVectorGrid2::operator=(
    const VertexCenteredVectorGrid2& other)
{
    Set(other);
    return *this;
}

Vector2UZ VertexCenteredVectorGrid2::DataSize() const
{
    if (Resolution() != Vector2UZ{ 0, 0 })
    {
        return Resolution() + Vector2UZ{ 1, 1 };
    }

    return Vector2UZ{ 0, 0 };
}

Vector2D VertexCenteredVectorGrid2::DataOrigin() const
{
    return Origin();
}

void VertexCenteredVectorGrid2::Swap(Grid2* other)
{
    auto sameType = dynamic_cast<VertexCenteredVectorGrid2*>(other);
    if (sameType != nullptr)
    {
        SwapCollocatedVectorGrid(sameType);
    }
}

void VertexCenteredVectorGrid2::Fill(const Vector2D& value,
                                     ExecutionPolicy policy)
{
    const Vector2UZ size = DataSize();
    ArrayView<Vector<double, 2>, 2> acc = DataView();

    ParallelFor(
        ZERO_SIZE, size.x, ZERO_SIZE, size.y,
        [value, &acc](const size_t i, const size_t j) { acc(i, j) = value; },
        policy);
}

void VertexCenteredVectorGrid2::Fill(
    const std::function<Vector2D(const Vector2D&)>& func,
    ExecutionPolicy policy)
{
    const Vector2UZ size = DataSize();
    ArrayView<Vector<double, 2>, 2> acc = DataView();
    auto pos = Unroll2(DataPosition());

    ParallelFor(
        ZERO_SIZE, size.x, ZERO_SIZE, size.y,
        [&func, &acc, &pos](const size_t i, const size_t j) {
            acc(i, j) = func(pos(i, j));
        },
        policy);
}

std::shared_ptr<VectorGrid2> VertexCenteredVectorGrid2::Clone() const
{
    return std::shared_ptr<VertexCenteredVectorGrid2>(
        new VertexCenteredVectorGrid2{ *this },
        [](VertexCenteredVectorGrid2* obj) { delete obj; });
}

void VertexCenteredVectorGrid2::Set(const VertexCenteredVectorGrid2& other)
{
    SetCollocatedVectorGrid(other);
}

VertexCenteredVectorGrid2::Builder VertexCenteredVectorGrid2::GetBuilder()
{
    return Builder{};
}

VertexCenteredVectorGrid2::Builder&
VertexCenteredVectorGrid2::Builder::WithResolution(const Vector2UZ& resolution)
{
    m_resolution = resolution;
    return *this;
}

VertexCenteredVectorGrid2::Builder&
VertexCenteredVectorGrid2::Builder::WithResolution(size_t resolutionX,
                                                   size_t resolutionY)
{
    m_resolution.x = resolutionX;
    m_resolution.y = resolutionY;
    return *this;
}

VertexCenteredVectorGrid2::Builder&
VertexCenteredVectorGrid2::Builder::WithGridSpacing(const Vector2D& gridSpacing)
{
    m_gridSpacing = gridSpacing;
    return *this;
}

VertexCenteredVectorGrid2::Builder&
VertexCenteredVectorGrid2::Builder::WithGridSpacing(double gridSpacingX,
                                                    double gridSpacingY)
{
    m_gridSpacing.x = gridSpacingX;
    m_gridSpacing.y = gridSpacingY;
    return *this;
}

VertexCenteredVectorGrid2::Builder&
VertexCenteredVectorGrid2::Builder::WithOrigin(const Vector2D& gridOrigin)
{
    m_gridOrigin = gridOrigin;
    return *this;
}

VertexCenteredVectorGrid2::Builder&
VertexCenteredVectorGrid2::Builder::WithOrigin(double gridOriginX,
                                               double gridOriginY)
{
    m_gridOrigin.x = gridOriginX;
    m_gridOrigin.y = gridOriginY;
    return *this;
}

VertexCenteredVectorGrid2::Builder&
VertexCenteredVectorGrid2::Builder::WithInitialValue(double initialValU,
                                                     double initialValV)
{
    m_initialVal.x = initialValU;
    m_initialVal.y = initialValV;
    return *this;
}

VertexCenteredVectorGrid2 VertexCenteredVectorGrid2::Builder::Build() const
{
    return VertexCenteredVectorGrid2{ m_resolution, m_gridSpacing, m_gridOrigin,
                                      m_initialVal };
}

VertexCenteredVectorGrid2Ptr VertexCenteredVectorGrid2::Builder::MakeShared()
    const
{
    return std::shared_ptr<VertexCenteredVectorGrid2>(
        new VertexCenteredVectorGrid2{ m_resolution, m_gridSpacing,
                                       m_gridOrigin, m_initialVal },
        [](VertexCenteredVectorGrid2* obj) { delete obj; });
}

VectorGrid2Ptr VertexCenteredVectorGrid2::Builder::Build(
    const Vector2UZ& resolution, const Vector2D& gridSpacing,
    const Vector2D& gridOrigin, const Vector2D& initialVal) const
{
    return std::shared_ptr<VertexCenteredVectorGrid2>(
        new VertexCenteredVectorGrid2{ resolution, gridSpacing, gridOrigin,
                                       initialVal },
        [](VertexCenteredVectorGrid2* obj) { delete obj; });
}
}  // namespace CubbyFlow