// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/VertexCenteredScalarGrid3.hpp>

namespace CubbyFlow
{
VertexCenteredScalarGrid3::VertexCenteredScalarGrid3(
    size_t resolutionX, size_t resolutionY, size_t resolutionZ,
    double gridSpacingX, double gridSpacingY, double gridSpacingZ,
    double originX, double originY, double originZ, double initialValue)
{
    Resize(resolutionX, resolutionY, resolutionZ, gridSpacingX, gridSpacingY,
           gridSpacingZ, originX, originY, originZ, initialValue);
}

VertexCenteredScalarGrid3::VertexCenteredScalarGrid3(
    const Vector3UZ& resolution, const Vector3D& gridSpacing,
    const Vector3D& origin, double initialValue)
{
    Resize(resolution, gridSpacing, origin, initialValue);
}

VertexCenteredScalarGrid3::VertexCenteredScalarGrid3(
    const VertexCenteredScalarGrid3& other)
    : ScalarGrid3{ other }
{
    Set(other);
}

VertexCenteredScalarGrid3& VertexCenteredScalarGrid3::operator=(
    const VertexCenteredScalarGrid3& other)
{
    Set(other);
    return *this;
}

Vector3UZ VertexCenteredScalarGrid3::GetDataSize() const
{
    if (Resolution() != Vector3UZ{ 0, 0, 0 })
    {
        return Resolution() + Vector3UZ{ 1, 1, 1 };
    }

    return Vector3UZ{ 0, 0, 0 };
}

Vector3D VertexCenteredScalarGrid3::GetDataOrigin() const
{
    return GridOrigin();
}

std::shared_ptr<ScalarGrid3> VertexCenteredScalarGrid3::Clone() const
{
    return std::shared_ptr<VertexCenteredScalarGrid3>(
        new VertexCenteredScalarGrid3{ *this },
        [](VertexCenteredScalarGrid3* obj) { delete obj; });
}

void VertexCenteredScalarGrid3::Swap(Grid3* other)
{
    auto sameType = dynamic_cast<VertexCenteredScalarGrid3*>(other);
    if (sameType != nullptr)
    {
        SwapScalarGrid(sameType);
    }
}

void VertexCenteredScalarGrid3::Set(const VertexCenteredScalarGrid3& other)
{
    SetScalarGrid(other);
}

VertexCenteredScalarGrid3::Builder VertexCenteredScalarGrid3::GetBuilder()
{
    return Builder{};
}

VertexCenteredScalarGrid3::Builder&
VertexCenteredScalarGrid3::Builder::WithResolution(const Vector3UZ& resolution)
{
    m_resolution = resolution;
    return *this;
}

VertexCenteredScalarGrid3::Builder&
VertexCenteredScalarGrid3::Builder::WithResolution(size_t resolutionX,
                                                   size_t resolutionY,
                                                   size_t resolutionZ)
{
    m_resolution.x = resolutionX;
    m_resolution.y = resolutionY;
    m_resolution.z = resolutionZ;
    return *this;
}

VertexCenteredScalarGrid3::Builder&
VertexCenteredScalarGrid3::Builder::WithGridSpacing(const Vector3D& gridSpacing)
{
    m_gridSpacing = gridSpacing;
    return *this;
}

VertexCenteredScalarGrid3::Builder&
VertexCenteredScalarGrid3::Builder::WithGridSpacing(double gridSpacingX,
                                                    double gridSpacingY,
                                                    double gridSpacingZ)
{
    m_gridSpacing.x = gridSpacingX;
    m_gridSpacing.y = gridSpacingY;
    m_gridSpacing.z = gridSpacingZ;
    return *this;
}

VertexCenteredScalarGrid3::Builder&
VertexCenteredScalarGrid3::Builder::WithOrigin(const Vector3D& gridOrigin)
{
    m_gridOrigin = gridOrigin;
    return *this;
}

VertexCenteredScalarGrid3::Builder&
VertexCenteredScalarGrid3::Builder::WithOrigin(double gridOriginX,
                                               double gridOriginY,
                                               double gridOriginZ)
{
    m_gridOrigin.x = gridOriginX;
    m_gridOrigin.y = gridOriginY;
    m_gridOrigin.z = gridOriginZ;
    return *this;
}

VertexCenteredScalarGrid3::Builder&
VertexCenteredScalarGrid3::Builder::WithInitialValue(double initialVal)
{
    m_initialVal = initialVal;
    return *this;
}

VertexCenteredScalarGrid3 VertexCenteredScalarGrid3::Builder::Build() const
{
    return VertexCenteredScalarGrid3{ m_resolution, m_gridSpacing, m_gridOrigin,
                                      m_initialVal };
}

VertexCenteredScalarGrid3Ptr VertexCenteredScalarGrid3::Builder::MakeShared()
    const
{
    return std::shared_ptr<VertexCenteredScalarGrid3>(
        new VertexCenteredScalarGrid3{ m_resolution, m_gridSpacing,
                                       m_gridOrigin, m_initialVal },
        [](VertexCenteredScalarGrid3* obj) { delete obj; });
}

ScalarGrid3Ptr VertexCenteredScalarGrid3::Builder::Build(
    const Vector3UZ& resolution, const Vector3D& gridSpacing,
    const Vector3D& gridOrigin, double initialVal) const
{
    return std::shared_ptr<VertexCenteredScalarGrid3>(
        new VertexCenteredScalarGrid3{ resolution, gridSpacing, gridOrigin,
                                       initialVal },
        [](VertexCenteredScalarGrid3* obj) { delete obj; });
}
}  // namespace CubbyFlow