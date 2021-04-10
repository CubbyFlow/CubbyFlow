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
#include <Core/Grid/ScalarGrid2.hpp>
#include <Core/Utils/FlatbuffersHelper.hpp>

#include <Flatbuffers/generated/ScalarGrid2_generated.h>

#include <array>

namespace CubbyFlow
{
ScalarGrid2::ScalarGrid2()
    : m_linearSampler(
          LinearArraySampler2<double>{ m_data, Vector2D{ 1, 1 }, Vector2D{} })
{
    // Do nothing
}

void ScalarGrid2::Clear()
{
    Resize(Vector2UZ{}, GridSpacing(), Origin(), 0.0);
}

void ScalarGrid2::Resize(size_t resolutionX, size_t resolutionY,
                         double gridSpacingX, double gridSpacingY,
                         double originX, double originY, double initialValue)
{
    Resize(Vector2UZ{ resolutionX, resolutionY },
           Vector2D{ gridSpacingX, gridSpacingY }, Vector2D{ originX, originY },
           initialValue);
}

void ScalarGrid2::Resize(const Vector2UZ& resolution,
                         const Vector2D& gridSpacing, const Vector2D& origin,
                         double initialValue)
{
    SetSizeParameters(resolution, gridSpacing, origin);

    m_data.Resize(GetDataSize(), initialValue);
    ResetSampler();
}

void ScalarGrid2::Resize(double gridSpacingX, double gridSpacingY,
                         double originX, double originY)
{
    Resize(Vector2D{ gridSpacingX, gridSpacingY },
           Vector2D{ originX, originY });
}

void ScalarGrid2::Resize(const Vector2D& gridSpacing, const Vector2D& origin)
{
    Resize(Resolution(), gridSpacing, origin);
}

const double& ScalarGrid2::operator()(size_t i, size_t j) const
{
    return m_data(i, j);
}

double& ScalarGrid2::operator()(size_t i, size_t j)
{
    return m_data(i, j);
}

Vector2D ScalarGrid2::GradientAtDataPoint(size_t i, size_t j) const
{
    return Gradient2(m_data, GridSpacing(), i, j);
}

double ScalarGrid2::LaplacianAtDataPoint(size_t i, size_t j) const
{
    return Laplacian2(m_data, GridSpacing(), i, j);
}

double ScalarGrid2::Sample(const Vector2D& x) const
{
    return m_sampler(x);
}

std::function<double(const Vector2D&)> ScalarGrid2::Sampler() const
{
    return m_sampler;
}

Vector2D ScalarGrid2::Gradient(const Vector2D& x) const
{
    std::array<Vector2UZ, 4> indices;
    std::array<double, 4> weights{};
    m_linearSampler.GetCoordinatesAndWeights(x, indices, weights);

    Vector2D result;

    for (int i = 0; i < 4; ++i)
    {
        result += weights[i] * GradientAtDataPoint(indices[i].x, indices[i].y);
    }

    return result;
}

double ScalarGrid2::Laplacian(const Vector2D& x) const
{
    std::array<Vector2UZ, 4> indices;
    std::array<double, 4> weights{};
    m_linearSampler.GetCoordinatesAndWeights(x, indices, weights);

    double result = 0.0;

    for (int i = 0; i < 4; ++i)
    {
        result += weights[i] * LaplacianAtDataPoint(indices[i].x, indices[i].y);
    }

    return result;
}

ScalarGrid2::ScalarDataView ScalarGrid2::DataView()
{
    return ScalarDataView(m_data);
}

ScalarGrid2::ConstScalarDataView ScalarGrid2::DataView() const
{
    return ConstScalarDataView(m_data);
}

ScalarGrid2::DataPositionFunc ScalarGrid2::DataPosition() const
{
    Vector2D dataOrigin = GetDataOrigin();
    Vector2D gridSpacing = GridSpacing();

    return [dataOrigin, gridSpacing](const Vector2UZ& idx) -> Vector2D {
        return dataOrigin + ElemMul(gridSpacing, idx.CastTo<double>());
    };
}

void ScalarGrid2::Fill(double value, ExecutionPolicy policy)
{
    ParallelFor(
        ZERO_SIZE, m_data.Width(), ZERO_SIZE, m_data.Height(),
        [this, value](const size_t i, const size_t j) { m_data(i, j) = value; },
        policy);
}

void ScalarGrid2::Fill(const std::function<double(const Vector2D&)>& func,
                       ExecutionPolicy policy)
{
    auto pos = Unroll2(DataPosition());

    ParallelFor(
        ZERO_SIZE, m_data.Width(), ZERO_SIZE, m_data.Height(),
        [this, &func, &pos](const size_t i, const size_t j) {
            m_data(i, j) = func(pos(i, j));
        },
        policy);
}

void ScalarGrid2::ForEachDataPointIndex(
    const std::function<void(size_t, size_t)>& func) const
{
    ForEachIndex(m_data.Size(), func);
}

void ScalarGrid2::ParallelForEachDataPointIndex(
    const std::function<void(size_t, size_t)>& func) const
{
    ParallelForEachIndex(m_data.Size(), func);
}

void ScalarGrid2::Serialize(std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder{ 1024 };

    fbs::Vector2UZ fbsResolution = CubbyFlowToFlatbuffers(Resolution());
    fbs::Vector2D fbsGridSpacing = CubbyFlowToFlatbuffers(GridSpacing());
    fbs::Vector2D fbsOrigin = CubbyFlowToFlatbuffers(Origin());

    Array1<double> gridData;
    GetData(gridData);
    const flatbuffers::Offset<flatbuffers::Vector<double>> data =
        builder.CreateVector(gridData.data(), gridData.Length());

    const flatbuffers::Offset<fbs::ScalarGrid2> fbsGrid = CreateScalarGrid2(
        builder, &fbsResolution, &fbsGridSpacing, &fbsOrigin, data);

    builder.Finish(fbsGrid);

    uint8_t* buf = builder.GetBufferPointer();
    const size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void ScalarGrid2::Deserialize(const std::vector<uint8_t>& buffer)
{
    const fbs::ScalarGrid2* fbsGrid = fbs::GetScalarGrid2(buffer.data());

    Resize(FlatbuffersToCubbyFlow(*fbsGrid->resolution()),
           FlatbuffersToCubbyFlow(*fbsGrid->gridSpacing()),
           FlatbuffersToCubbyFlow(*fbsGrid->origin()));

    const flatbuffers::Vector<double>* data = fbsGrid->data();
    Array1<double> gridData(data->size());
    std::copy(data->begin(), data->end(), gridData.begin());

    SetData(gridData);
}

void ScalarGrid2::SwapScalarGrid(ScalarGrid2* other)
{
    SwapGrid(other);

    m_data.Swap(other->m_data);
    std::swap(m_linearSampler, other->m_linearSampler);
    std::swap(m_sampler, other->m_sampler);
}

void ScalarGrid2::SetScalarGrid(const ScalarGrid2& other)
{
    SetGrid(other);

    m_data.CopyFrom(other.m_data);
    ResetSampler();
}

void ScalarGrid2::ResetSampler()
{
    m_linearSampler =
        LinearArraySampler2<double>{ m_data, GridSpacing(), GetDataOrigin() };
    m_sampler = m_linearSampler.Functor();
}

void ScalarGrid2::GetData(Array1<double>& data) const
{
    const size_t size = GetDataSize().x * GetDataSize().y;
    data.Resize(size);
    std::copy(m_data.begin(), m_data.end(), data.begin());
}

void ScalarGrid2::SetData(const ConstArrayView1<double>& data)
{
    assert(GetDataSize().x * GetDataSize().y == data.Length());

    std::copy(data.begin(), data.end(), m_data.begin());
}
}  // namespace CubbyFlow