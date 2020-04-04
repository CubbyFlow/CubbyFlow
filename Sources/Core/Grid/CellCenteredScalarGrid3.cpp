// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/CellCenteredScalarGrid3.hpp>

namespace CubbyFlow
{
CellCenteredScalarGrid3::CellCenteredScalarGrid3()
{
    // Do nothing
}

CellCenteredScalarGrid3::CellCenteredScalarGrid3(
    size_t resolutionX, size_t resolutionY, size_t resolutionZ,
    double gridSpacingX, double gridSpacingY, double gridSpacingZ,
    double originX, double originY, double originZ, double initialValue)
{
    Resize(resolutionX, resolutionY, resolutionZ, gridSpacingX, gridSpacingY,
           gridSpacingZ, originX, originY, originZ, initialValue);
}

CellCenteredScalarGrid3::CellCenteredScalarGrid3(const Size3& resolution,
                                                 const Vector3D& gridSpacing,
                                                 const Vector3D& origin,
                                                 double initialValue)
{
    Resize(resolution, gridSpacing, origin, initialValue);
}

CellCenteredScalarGrid3::CellCenteredScalarGrid3(
    const CellCenteredScalarGrid3& other)
{
    Set(other);
}

Size3 CellCenteredScalarGrid3::GetDataSize() const
{
    // The size of the data should be the same as the grid resolution.
    return Resolution();
}

Vector3D CellCenteredScalarGrid3::GetDataOrigin() const
{
    return Origin() + 0.5 * GridSpacing();
}

std::shared_ptr<ScalarGrid3> CellCenteredScalarGrid3::Clone() const
{
    return std::shared_ptr<CellCenteredScalarGrid3>(
        new CellCenteredScalarGrid3(*this),
        [](CellCenteredScalarGrid3* obj) { delete obj; });
}

void CellCenteredScalarGrid3::Swap(Grid3* other)
{
    CellCenteredScalarGrid3* sameType =
        dynamic_cast<CellCenteredScalarGrid3*>(other);
    if (sameType != nullptr)
    {
        SwapScalarGrid(sameType);
    }
}

void CellCenteredScalarGrid3::Set(const CellCenteredScalarGrid3& other)
{
    SetScalarGrid(other);
}

CellCenteredScalarGrid3& CellCenteredScalarGrid3::operator=(
    const CellCenteredScalarGrid3& other)
{
    Set(other);
    return *this;
}

CellCenteredScalarGrid3::Builder CellCenteredScalarGrid3::GetBuilder()
{
    return Builder();
}

CellCenteredScalarGrid3::Builder&
CellCenteredScalarGrid3::Builder::WithResolution(const Size3& resolution)
{
    m_resolution = resolution;
    return *this;
}

CellCenteredScalarGrid3::Builder&
CellCenteredScalarGrid3::Builder::WithResolution(size_t resolutionX,
                                                 size_t resolutionY,
                                                 size_t resolutionZ)
{
    m_resolution.x = resolutionX;
    m_resolution.y = resolutionY;
    m_resolution.z = resolutionZ;
    return *this;
}

CellCenteredScalarGrid3::Builder&
CellCenteredScalarGrid3::Builder::WithGridSpacing(const Vector3D& gridSpacing)
{
    m_gridSpacing = gridSpacing;
    return *this;
}

CellCenteredScalarGrid3::Builder&
CellCenteredScalarGrid3::Builder::WithGridSpacing(double gridSpacingX,
                                                  double gridSpacingY,
                                                  double gridSpacingZ)
{
    m_gridSpacing.x = gridSpacingX;
    m_gridSpacing.y = gridSpacingY;
    m_gridSpacing.z = gridSpacingZ;
    return *this;
}

CellCenteredScalarGrid3::Builder& CellCenteredScalarGrid3::Builder::WithOrigin(
    const Vector3D& gridOrigin)
{
    m_gridOrigin = gridOrigin;
    return *this;
}

CellCenteredScalarGrid3::Builder& CellCenteredScalarGrid3::Builder::WithOrigin(
    double gridOriginX, double gridOriginY, double gridOriginZ)
{
    m_gridOrigin.x = gridOriginX;
    m_gridOrigin.y = gridOriginY;
    m_gridOrigin.z = gridOriginZ;
    return *this;
}

CellCenteredScalarGrid3::Builder&
CellCenteredScalarGrid3::Builder::WithInitialValue(double initialVal)
{
    m_initialVal = initialVal;
    return *this;
}

CellCenteredScalarGrid3 CellCenteredScalarGrid3::Builder::Build() const
{
    return CellCenteredScalarGrid3(m_resolution, m_gridSpacing, m_gridOrigin,
                                   m_initialVal);
}

CellCenteredScalarGrid3Ptr CellCenteredScalarGrid3::Builder::MakeShared() const
{
    return std::shared_ptr<CellCenteredScalarGrid3>(
        new CellCenteredScalarGrid3(m_resolution, m_gridSpacing, m_gridOrigin,
                                    m_initialVal),
        [](CellCenteredScalarGrid3* obj) { delete obj; });
}

ScalarGrid3Ptr CellCenteredScalarGrid3::Builder::Build(
    const Size3& resolution, const Vector3D& gridSpacing,
    const Vector3D& gridOrigin, double initialVal) const
{
    return std::shared_ptr<CellCenteredScalarGrid3>(
        new CellCenteredScalarGrid3(resolution, gridSpacing, gridOrigin,
                                    initialVal),
        [](CellCenteredScalarGrid3* obj) { delete obj; });
}
}  // namespace CubbyFlow