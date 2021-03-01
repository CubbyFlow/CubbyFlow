// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/FDM/FDMUtils.hpp>
#include <Core/Grid/ScalarGrid3.hpp>
#include <Core/Utils/FlatbuffersHelper.hpp>

#include <Flatbuffers/generated/ScalarGrid3_generated.h>

#include <array>

namespace CubbyFlow
{
ScalarGrid3::ScalarGrid3()
    : m_linearSampler(LinearArraySampler3<double>{ m_data, Vector3D{ 1, 1, 1 },
                                                   Vector3D{} })
{
    // Do nothing
}

void ScalarGrid3::Clear()
{
    Resize(Vector3UZ{}, GridSpacing(), Origin(), 0.0);
}

void ScalarGrid3::Resize(size_t resolutionX, size_t resolutionY,
                         size_t resolutionZ, double gridSpacingX,
                         double gridSpacingY, double gridSpacingZ,
                         double originX, double originY, double originZ,
                         double initialValue)
{
    Resize(Vector3UZ{ resolutionX, resolutionY, resolutionZ },
           Vector3D{ gridSpacingX, gridSpacingY, gridSpacingZ },
           Vector3D{ originX, originY, originZ }, initialValue);
}

void ScalarGrid3::Resize(const Vector3UZ& resolution,
                         const Vector3D& gridSpacing, const Vector3D& origin,
                         double initialValue)
{
    SetSizeParameters(resolution, gridSpacing, origin);

    m_data.Resize(GetDataSize(), initialValue);
    ResetSampler();
}

void ScalarGrid3::Resize(double gridSpacingX, double gridSpacingY,
                         double gridSpacingZ, double originX, double originY,
                         double originZ)
{
    Resize(Vector3D{ gridSpacingX, gridSpacingY, gridSpacingZ },
           Vector3D{ originX, originY, originZ });
}

void ScalarGrid3::Resize(const Vector3D& gridSpacing, const Vector3D& origin)
{
    Resize(Resolution(), gridSpacing, origin);
}

const double& ScalarGrid3::operator()(size_t i, size_t j, size_t k) const
{
    return m_data(i, j, k);
}

double& ScalarGrid3::operator()(size_t i, size_t j, size_t k)
{
    return m_data(i, j, k);
}

Vector3D ScalarGrid3::GradientAtDataPoint(size_t i, size_t j, size_t k) const
{
    return Gradient3(m_data, GridSpacing(), i, j, k);
}

double ScalarGrid3::LaplacianAtDataPoint(size_t i, size_t j, size_t k) const
{
    return Laplacian3(m_data, GridSpacing(), i, j, k);
}

double ScalarGrid3::Sample(const Vector3D& x) const
{
    return m_sampler(x);
}

std::function<double(const Vector3D&)> ScalarGrid3::Sampler() const
{
    return m_sampler;
}

Vector3D ScalarGrid3::Gradient(const Vector3D& x) const
{
    std::array<Vector3UZ, 8> indices;
    std::array<double, 8> weights{};
    m_linearSampler.GetCoordinatesAndWeights(x, indices, weights);

    Vector3D result;

    for (int i = 0; i < 8; ++i)
    {
        result += weights[i] *
                  GradientAtDataPoint(indices[i].x, indices[i].y, indices[i].z);
    }

    return result;
}

double ScalarGrid3::Laplacian(const Vector3D& x) const
{
    std::array<Vector3UZ, 8> indices;
    std::array<double, 8> weights{};
    m_linearSampler.GetCoordinatesAndWeights(x, indices, weights);

    double result = 0.0;

    for (int i = 0; i < 8; ++i)
    {
        result += weights[i] * LaplacianAtDataPoint(indices[i].x, indices[i].y,
                                                    indices[i].z);
    }

    return result;
}

ScalarGrid3::ScalarDataView ScalarGrid3::DataView()
{
    return ScalarDataView(m_data);
}

ScalarGrid3::ConstScalarDataView ScalarGrid3::DataView() const
{
    return ConstScalarDataView(m_data);
}

ScalarGrid3::DataPositionFunc ScalarGrid3::DataPosition() const
{
    Vector3D o = GetDataOrigin();

    return
        [this, o](const size_t i, const size_t j, const size_t k) -> Vector3D {
            return o + GridSpacing() * Vector3D{ { static_cast<double>(i),
                                                   static_cast<double>(j),
                                                   static_cast<double>(k) } };
        };
}

void ScalarGrid3::Fill(double value, ExecutionPolicy policy)
{
    ParallelFor(
        ZERO_SIZE, m_data.Width(), ZERO_SIZE, m_data.Height(), ZERO_SIZE,
        m_data.Depth(),
        [this, value](const size_t i, const size_t j, const size_t k) {
            m_data(i, j, k) = value;
        },
        policy);
}

void ScalarGrid3::Fill(const std::function<double(const Vector3D&)>& func,
                       ExecutionPolicy policy)
{
    DataPositionFunc pos = DataPosition();

    ParallelFor(
        ZERO_SIZE, m_data.Width(), ZERO_SIZE, m_data.Height(), ZERO_SIZE,
        m_data.Depth(),
        [this, &func, &pos](const size_t i, const size_t j, const size_t k) {
            m_data(i, j, k) = func(pos(i, j, k));
        },
        policy);
}

void ScalarGrid3::ForEachDataPointIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    ForEachIndex(m_data.Size(), func);
}

void ScalarGrid3::ParallelForEachDataPointIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    ParallelForEachIndex(m_data.Size(), func);
}

void ScalarGrid3::Serialize(std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder{ 1024 };

    fbs::Vector3UZ fbsResolution = CubbyFlowToFlatbuffers(Resolution());
    fbs::Vector3D fbsGridSpacing = CubbyFlowToFlatbuffers(GridSpacing());
    fbs::Vector3D fbsOrigin = CubbyFlowToFlatbuffers(Origin());

    std::vector<double> gridData;
    GetData(&gridData);
    const flatbuffers::Offset<flatbuffers::Vector<double>> data =
        builder.CreateVector(gridData.data(), gridData.size());

    const flatbuffers::Offset<fbs::ScalarGrid3> fbsGrid = CreateScalarGrid3(
        builder, &fbsResolution, &fbsGridSpacing, &fbsOrigin, data);

    builder.Finish(fbsGrid);

    uint8_t* buf = builder.GetBufferPointer();
    const size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void ScalarGrid3::Deserialize(const std::vector<uint8_t>& buffer)
{
    const fbs::ScalarGrid3* fbsGrid = fbs::GetScalarGrid3(buffer.data());

    Resize(FlatbuffersToCubbyFlow(*fbsGrid->resolution()),
           FlatbuffersToCubbyFlow(*fbsGrid->gridSpacing()),
           FlatbuffersToCubbyFlow(*fbsGrid->origin()));

    const flatbuffers::Vector<double>* data = fbsGrid->data();
    std::vector<double> gridData(data->size());
    std::copy(data->begin(), data->end(), gridData.begin());

    SetData(gridData);
}

void ScalarGrid3::SwapScalarGrid(ScalarGrid3* other)
{
    SwapGrid(other);

    m_data.Swap(other->m_data);
    std::swap(m_linearSampler, other->m_linearSampler);
    std::swap(m_sampler, other->m_sampler);
}

void ScalarGrid3::SetScalarGrid(const ScalarGrid3& other)
{
    SetGrid(other);

    m_data.CopyFrom(other.m_data);
    ResetSampler();
}

void ScalarGrid3::ResetSampler()
{
    m_linearSampler =
        LinearArraySampler3<double>{ m_data, GridSpacing(), GetDataOrigin() };
    m_sampler = m_linearSampler.Functor();
}

void ScalarGrid3::GetData(std::vector<double>* data) const
{
    const size_t size = GetDataSize().x * GetDataSize().y * GetDataSize().z;
    data->resize(size);
    std::copy(m_data.begin(), m_data.end(), data->begin());
}

void ScalarGrid3::SetData(const std::vector<double>& data)
{
    assert(GetDataSize().x * GetDataSize().y * GetDataSize().z == data.size());

    std::copy(data.begin(), data.end(), m_data.begin());
}
}  // namespace CubbyFlow