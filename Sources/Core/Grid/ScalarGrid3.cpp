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

namespace CubbyFlow
{
ScalarGrid3::ScalarGrid3()
    : m_linearSampler(LinearArraySampler3<double, double>(
          m_data.ConstAccessor(), Vector3D(1, 1, 1), Vector3D()))
{
    // Do nothing
}

ScalarGrid3::~ScalarGrid3()
{
    // Do nothing
}

void ScalarGrid3::Clear()
{
    Resize(Size3(), GridSpacing(), Origin(), 0.0);
}

void ScalarGrid3::Resize(size_t resolutionX, size_t resolutionY,
                         size_t resolutionZ, double gridSpacingX,
                         double gridSpacingY, double gridSpacingZ,
                         double originX, double originY, double originZ,
                         double initialValue)
{
    Resize(Size3(resolutionX, resolutionY, resolutionZ),
           Vector3D(gridSpacingX, gridSpacingY, gridSpacingZ),
           Vector3D(originX, originY, originZ), initialValue);
}

void ScalarGrid3::Resize(const Size3& resolution, const Vector3D& gridSpacing,
                         const Vector3D& origin, double initialValue)
{
    SetSizeParameters(resolution, gridSpacing, origin);

    m_data.Resize(GetDataSize(), initialValue);
    ResetSampler();
}

void ScalarGrid3::Resize(double gridSpacingX, double gridSpacingY,
                         double gridSpacingZ, double originX, double originY,
                         double originZ)
{
    Resize(Vector3D(gridSpacingX, gridSpacingY, gridSpacingZ),
           Vector3D(originX, originY, originZ));
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
    return Gradient3(m_data.ConstAccessor(), GridSpacing(), i, j, k);
}

double ScalarGrid3::LaplacianAtDataPoint(size_t i, size_t j, size_t k) const
{
    return Laplacian3(m_data.ConstAccessor(), GridSpacing(), i, j, k);
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
    std::array<Point3UI, 8> indices;
    std::array<double, 8> weights;
    m_linearSampler.GetCoordinatesAndWeights(x, &indices, &weights);

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
    std::array<Point3UI, 8> indices;
    std::array<double, 8> weights;
    m_linearSampler.GetCoordinatesAndWeights(x, &indices, &weights);

    double result = 0.0;

    for (int i = 0; i < 8; ++i)
    {
        result += weights[i] * LaplacianAtDataPoint(indices[i].x, indices[i].y,
                                                    indices[i].z);
    }

    return result;
}

ScalarGrid3::ScalarDataAccessor ScalarGrid3::GetDataAccessor()
{
    return m_data.Accessor();
}

ScalarGrid3::ConstScalarDataAccessor ScalarGrid3::GetConstDataAccessor() const
{
    return m_data.ConstAccessor();
}

ScalarGrid3::DataPositionFunc ScalarGrid3::GetDataPosition() const
{
    Vector3D o = GetDataOrigin();

    return [this, o](size_t i, size_t j, size_t k) -> Vector3D {
        return o + GridSpacing() * Vector3D({ i, j, k });
    };
}

void ScalarGrid3::Fill(double value, ExecutionPolicy policy)
{
    ParallelFor(
        ZERO_SIZE, m_data.Width(), ZERO_SIZE, m_data.Height(), ZERO_SIZE,
        m_data.Depth(),
        [this, value](size_t i, size_t j, size_t k) {
            m_data(i, j, k) = value;
        },
        policy);
}

void ScalarGrid3::Fill(const std::function<double(const Vector3D&)>& func,
                       ExecutionPolicy policy)
{
    DataPositionFunc pos = GetDataPosition();

    ParallelFor(
        ZERO_SIZE, m_data.Width(), ZERO_SIZE, m_data.Height(), ZERO_SIZE,
        m_data.Depth(),
        [this, &func, &pos](size_t i, size_t j, size_t k) {
            m_data(i, j, k) = func(pos(i, j, k));
        },
        policy);
}

void ScalarGrid3::ForEachDataPointIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    m_data.ForEachIndex(func);
}

void ScalarGrid3::ParallelForEachDataPointIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    m_data.ParallelForEachIndex(func);
}

void ScalarGrid3::Serialize(std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder(1024);

    auto fbsResolution = CubbyFlowToFlatbuffers(Resolution());
    auto fbsGridSpacing = CubbyFlowToFlatbuffers(GridSpacing());
    auto fbsOrigin = CubbyFlowToFlatbuffers(Origin());

    std::vector<double> gridData;
    GetData(&gridData);
    auto data = builder.CreateVector(gridData.data(), gridData.size());

    auto fbsGrid = fbs::CreateScalarGrid3(builder, &fbsResolution,
                                          &fbsGridSpacing, &fbsOrigin, data);

    builder.Finish(fbsGrid);

    uint8_t* buf = builder.GetBufferPointer();
    size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void ScalarGrid3::Deserialize(const std::vector<uint8_t>& buffer)
{
    auto fbsGrid = fbs::GetScalarGrid3(buffer.data());

    Resize(FlatbuffersToCubbyFlow(*fbsGrid->resolution()),
           FlatbuffersToCubbyFlow(*fbsGrid->gridSpacing()),
           FlatbuffersToCubbyFlow(*fbsGrid->origin()));

    auto data = fbsGrid->data();
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

    m_data.Set(other.m_data);
    ResetSampler();
}

void ScalarGrid3::ResetSampler()
{
    m_linearSampler = LinearArraySampler3<double, double>(
        m_data.ConstAccessor(), GridSpacing(), GetDataOrigin());
    m_sampler = m_linearSampler.Functor();
}

void ScalarGrid3::GetData(std::vector<double>* data) const
{
    size_t size = GetDataSize().x * GetDataSize().y * GetDataSize().z;
    data->resize(size);
    std::copy(m_data.begin(), m_data.end(), data->begin());
}

void ScalarGrid3::SetData(const std::vector<double>& data)
{
    assert(GetDataSize().x * GetDataSize().y * GetDataSize().z == data.size());

    std::copy(data.begin(), data.end(), m_data.begin());
}

ScalarGridBuilder3::ScalarGridBuilder3()
{
    // Do nothing
}

ScalarGridBuilder3::~ScalarGridBuilder3()
{
    // Do nothing
}
}  // namespace CubbyFlow