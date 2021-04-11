// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/CellCenteredVectorGrid3.hpp>

namespace CubbyFlow
{
CellCenteredVectorGrid3::CellCenteredVectorGrid3(
    size_t resolutionX, size_t resolutionY, size_t resolutionZ,
    double gridSpacingX, double gridSpacingY, double gridSpacingZ,
    double originX, double originY, double originZ, double initialValueU,
    double initialValueV, double initialValueW)
{
    Resize({ resolutionX, resolutionY, resolutionZ },
           { gridSpacingX, gridSpacingY, gridSpacingZ },
           { originX, originY, originZ },
           { initialValueU, initialValueV, initialValueW });
}

CellCenteredVectorGrid3::CellCenteredVectorGrid3(const Vector3UZ& resolution,
                                                 const Vector3D& gridSpacing,
                                                 const Vector3D& origin,
                                                 const Vector3D& initialValue)
{
    Resize(resolution, gridSpacing, origin, initialValue);
}

CellCenteredVectorGrid3::CellCenteredVectorGrid3(
    const CellCenteredVectorGrid3& other)
    : CollocatedVectorGrid3{ other }
{
    Set(other);
}

CellCenteredVectorGrid3& CellCenteredVectorGrid3::operator=(
    const CellCenteredVectorGrid3& other)
{
    Set(other);
    return *this;
}

Vector3UZ CellCenteredVectorGrid3::GetDataSize() const
{
    // The size of the data should be the same as the grid resolution.
    return Resolution();
}

Vector3D CellCenteredVectorGrid3::GetDataOrigin() const
{
    return Origin() + 0.5 * GridSpacing();
}

void CellCenteredVectorGrid3::Swap(Grid3* other)
{
    const auto sameType = dynamic_cast<CellCenteredVectorGrid3*>(other);
    if (sameType != nullptr)
    {
        SwapCollocatedVectorGrid(sameType);
    }
}

void CellCenteredVectorGrid3::Set(const CellCenteredVectorGrid3& other)
{
    SetCollocatedVectorGrid(other);
}

void CellCenteredVectorGrid3::Fill(const Vector3D& value,
                                   ExecutionPolicy policy)
{
    const Vector3UZ size = GetDataSize();
    auto acc = DataView();

    ParallelFor(
        ZERO_SIZE, size.x, ZERO_SIZE, size.y, ZERO_SIZE, size.z,
        [value, &acc](const size_t i, const size_t j, const size_t k) {
            acc(i, j, k) = value;
        },
        policy);
}

void CellCenteredVectorGrid3::Fill(
    const std::function<Vector3D(const Vector3D&)>& func,
    ExecutionPolicy policy)
{
    const Vector3UZ size = GetDataSize();
    ArrayView<Vector<double, 3>, 3> acc = DataView();
    auto pos = Unroll3(DataPosition());

    ParallelFor(
        ZERO_SIZE, size.x, ZERO_SIZE, size.y, ZERO_SIZE, size.z,
        [&func, &acc, &pos](const size_t i, const size_t j, const size_t k) {
            acc(i, j, k) = func(pos(i, j, k));
        },
        policy);
}

std::shared_ptr<VectorGrid3> CellCenteredVectorGrid3::Clone() const
{
    return std::shared_ptr<CellCenteredVectorGrid3>(
        new CellCenteredVectorGrid3{ *this },
        [](CellCenteredVectorGrid3* obj) { delete obj; });
}

CellCenteredVectorGrid3::Builder CellCenteredVectorGrid3::GetBuilder()
{
    return Builder{};
}

CellCenteredVectorGrid3::Builder&
CellCenteredVectorGrid3::Builder::WithResolution(const Vector3UZ& resolution)
{
    m_resolution = resolution;
    return *this;
}

CellCenteredVectorGrid3::Builder&
CellCenteredVectorGrid3::Builder::WithResolution(size_t resolutionX,
                                                 size_t resolutionY,
                                                 size_t resolutionZ)
{
    m_resolution.x = resolutionX;
    m_resolution.y = resolutionY;
    m_resolution.z = resolutionZ;
    return *this;
}

CellCenteredVectorGrid3::Builder&
CellCenteredVectorGrid3::Builder::WithGridSpacing(const Vector3D& gridSpacing)
{
    m_gridSpacing = gridSpacing;
    return *this;
}

CellCenteredVectorGrid3::Builder&
CellCenteredVectorGrid3::Builder::WithGridSpacing(double gridSpacingX,
                                                  double gridSpacingY,
                                                  double gridSpacingZ)
{
    m_gridSpacing.x = gridSpacingX;
    m_gridSpacing.y = gridSpacingY;
    m_gridSpacing.z = gridSpacingZ;
    return *this;
}

CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithOrigin(
    const Vector3D& gridOrigin)
{
    m_gridOrigin = gridOrigin;
    return *this;
}

CellCenteredVectorGrid3::Builder& CellCenteredVectorGrid3::Builder::WithOrigin(
    double gridOriginX, double gridOriginY, double gridOriginZ)
{
    m_gridOrigin.x = gridOriginX;
    m_gridOrigin.y = gridOriginY;
    m_gridOrigin.z = gridOriginZ;
    return *this;
}

CellCenteredVectorGrid3::Builder&
CellCenteredVectorGrid3::Builder::WithInitialValue(const Vector3D& initialVal)
{
    m_initialVal = initialVal;
    return *this;
}

CellCenteredVectorGrid3::Builder&
CellCenteredVectorGrid3::Builder::WithInitialValue(double initialValX,
                                                   double initialValY,
                                                   double initialValZ)
{
    m_initialVal.x = initialValX;
    m_initialVal.y = initialValY;
    m_initialVal.z = initialValZ;
    return *this;
}

CellCenteredVectorGrid3 CellCenteredVectorGrid3::Builder::Build() const
{
    return CellCenteredVectorGrid3{ m_resolution, m_gridSpacing, m_gridOrigin,
                                    m_initialVal };
}

CellCenteredVectorGrid3Ptr CellCenteredVectorGrid3::Builder::MakeShared() const
{
    return std::shared_ptr<CellCenteredVectorGrid3>(
        new CellCenteredVectorGrid3{ m_resolution, m_gridSpacing, m_gridOrigin,
                                     m_initialVal },
        [](CellCenteredVectorGrid3* obj) { delete obj; });
}

VectorGrid3Ptr CellCenteredVectorGrid3::Builder::Build(
    const Vector3UZ& resolution, const Vector3D& gridSpacing,
    const Vector3D& gridOrigin, const Vector3D& initialVal) const
{
    return std::shared_ptr<CellCenteredVectorGrid3>(
        new CellCenteredVectorGrid3{ resolution, gridSpacing, gridOrigin,
                                     initialVal },
        [](CellCenteredVectorGrid3* obj) { delete obj; });
}
}  // namespace CubbyFlow
