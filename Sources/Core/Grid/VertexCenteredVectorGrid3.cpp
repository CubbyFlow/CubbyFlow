// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/VertexCenteredVectorGrid3.hpp>

namespace CubbyFlow
{
VertexCenteredVectorGrid3::VertexCenteredVectorGrid3(
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

VertexCenteredVectorGrid3::VertexCenteredVectorGrid3(
    const Vector3UZ& resolution, const Vector3D& gridSpacing,
    const Vector3D& origin, const Vector3D& initialValue)
{
    Resize(resolution, gridSpacing, origin, initialValue);
}

VertexCenteredVectorGrid3::VertexCenteredVectorGrid3(
    const VertexCenteredVectorGrid3& other)
    : CollocatedVectorGrid3{ other }
{
    Set(other);
}

VertexCenteredVectorGrid3& VertexCenteredVectorGrid3::operator=(
    const VertexCenteredVectorGrid3& other)
{
    Set(other);
    return *this;
}

Vector3UZ VertexCenteredVectorGrid3::GetDataSize() const
{
    if (Resolution() != Vector3UZ{ 0, 0, 0 })
    {
        return Resolution() + Vector3UZ{ 1, 1, 1 };
    }

    return Vector3UZ{ 0, 0, 0 };
}

Vector3D VertexCenteredVectorGrid3::GetDataOrigin() const
{
    return Origin();
}

void VertexCenteredVectorGrid3::Swap(Grid3* other)
{
    auto sameType = dynamic_cast<VertexCenteredVectorGrid3*>(other);
    if (sameType != nullptr)
    {
        SwapCollocatedVectorGrid(sameType);
    }
}

void VertexCenteredVectorGrid3::Fill(const Vector3D& value,
                                     ExecutionPolicy policy)
{
    const Vector3UZ size = GetDataSize();
    ArrayView<Vector<double, 3>, 3> acc = DataView();

    ParallelFor(
        ZERO_SIZE, size.x, ZERO_SIZE, size.y, ZERO_SIZE, size.z,
        [value, &acc](const size_t i, const size_t j, const size_t k) {
            acc(i, j, k) = value;
        },
        policy);
}

void VertexCenteredVectorGrid3::Fill(
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

std::shared_ptr<VectorGrid3> VertexCenteredVectorGrid3::Clone() const
{
    return std::shared_ptr<VertexCenteredVectorGrid3>(
        new VertexCenteredVectorGrid3{ *this },
        [](VertexCenteredVectorGrid3* obj) { delete obj; });
}

void VertexCenteredVectorGrid3::Set(const VertexCenteredVectorGrid3& other)
{
    SetCollocatedVectorGrid(other);
}

VertexCenteredVectorGrid3::Builder VertexCenteredVectorGrid3::GetBuilder()
{
    return Builder();
}

VertexCenteredVectorGrid3::Builder&
VertexCenteredVectorGrid3::Builder::WithResolution(const Vector3UZ& resolution)
{
    m_resolution = resolution;
    return *this;
}

VertexCenteredVectorGrid3::Builder&
VertexCenteredVectorGrid3::Builder::WithResolution(size_t resolutionX,
                                                   size_t resolutionY,
                                                   size_t resolutionZ)
{
    m_resolution.x = resolutionX;
    m_resolution.y = resolutionY;
    m_resolution.z = resolutionZ;
    return *this;
}

VertexCenteredVectorGrid3::Builder&
VertexCenteredVectorGrid3::Builder::WithGridSpacing(const Vector3D& gridSpacing)
{
    m_gridSpacing = gridSpacing;
    return *this;
}

VertexCenteredVectorGrid3::Builder&
VertexCenteredVectorGrid3::Builder::WithGridSpacing(double gridSpacingX,
                                                    double gridSpacingY,
                                                    double gridSpacingZ)
{
    m_gridSpacing.x = gridSpacingX;
    m_gridSpacing.y = gridSpacingY;
    m_gridSpacing.z = gridSpacingZ;
    return *this;
}

VertexCenteredVectorGrid3::Builder&
VertexCenteredVectorGrid3::Builder::WithOrigin(const Vector3D& gridOrigin)
{
    m_gridOrigin = gridOrigin;
    return *this;
}

VertexCenteredVectorGrid3::Builder&
VertexCenteredVectorGrid3::Builder::WithOrigin(double gridOriginX,
                                               double gridOriginY,
                                               double gridOriginZ)
{
    m_gridOrigin.x = gridOriginX;
    m_gridOrigin.y = gridOriginY;
    m_gridOrigin.z = gridOriginZ;
    return *this;
}

VertexCenteredVectorGrid3::Builder&
VertexCenteredVectorGrid3::Builder::WithInitialValue(double initialValU,
                                                     double initialValV,
                                                     double initialValW)
{
    m_initialVal.x = initialValU;
    m_initialVal.y = initialValV;
    m_initialVal.z = initialValW;
    return *this;
}

VertexCenteredVectorGrid3 VertexCenteredVectorGrid3::Builder::Build() const
{
    return VertexCenteredVectorGrid3{ m_resolution, m_gridSpacing, m_gridOrigin,
                                      m_initialVal };
}

VertexCenteredVectorGrid3Ptr VertexCenteredVectorGrid3::Builder::MakeShared()
    const
{
    return std::shared_ptr<VertexCenteredVectorGrid3>(
        new VertexCenteredVectorGrid3{ m_resolution, m_gridSpacing,
                                       m_gridOrigin, m_initialVal },
        [](VertexCenteredVectorGrid3* obj) { delete obj; });
}

VectorGrid3Ptr VertexCenteredVectorGrid3::Builder::Build(
    const Vector3UZ& resolution, const Vector3D& gridSpacing,
    const Vector3D& gridOrigin, const Vector3D& initialVal) const
{
    return std::shared_ptr<VertexCenteredVectorGrid3>(
        new VertexCenteredVectorGrid3{ resolution, gridSpacing, gridOrigin,
                                       initialVal },
        [](VertexCenteredVectorGrid3* obj) { delete obj; });
}
}  // namespace CubbyFlow