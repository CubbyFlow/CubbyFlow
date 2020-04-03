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

namespace CubbyFlow
{
ScalarGrid2::ScalarGrid2()
    : m_linearSampler(LinearArraySampler2<double, double>(
          m_data.ConstAccessor(), Vector2D(1, 1), Vector2D()))
{
    // Do nothing
}

ScalarGrid2::~ScalarGrid2()
{
    // Do nothing
}

void ScalarGrid2::Clear()
{
    Resize(Size2(), GridSpacing(), Origin(), 0.0);
}

void ScalarGrid2::Resize(size_t resolutionX, size_t resolutionY,
                         double gridSpacingX, double gridSpacingY,
                         double originX, double originY, double initialValue)
{
    Resize(Size2(resolutionX, resolutionY),
           Vector2D(gridSpacingX, gridSpacingY), Vector2D(originX, originY),
           initialValue);
}

void ScalarGrid2::Resize(const Size2& resolution, const Vector2D& gridSpacing,
                         const Vector2D& origin, double initialValue)
{
    SetSizeParameters(resolution, gridSpacing, origin);

    m_data.Resize(GetDataSize(), initialValue);
    ResetSampler();
}

void ScalarGrid2::Resize(double gridSpacingX, double gridSpacingY,
                         double originX, double originY)
{
    Resize(Vector2D(gridSpacingX, gridSpacingY), Vector2D(originX, originY));
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
    return Gradient2(m_data.ConstAccessor(), GridSpacing(), i, j);
}

double ScalarGrid2::LaplacianAtDataPoint(size_t i, size_t j) const
{
    return Laplacian2(m_data.ConstAccessor(), GridSpacing(), i, j);
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
    std::array<Point2UI, 4> indices;
    std::array<double, 4> weights;
    m_linearSampler.GetCoordinatesAndWeights(x, &indices, &weights);

    Vector2D result;

    for (int i = 0; i < 4; ++i)
    {
        result += weights[i] * GradientAtDataPoint(indices[i].x, indices[i].y);
    }

    return result;
}

double ScalarGrid2::Laplacian(const Vector2D& x) const
{
    std::array<Point2UI, 4> indices;
    std::array<double, 4> weights;
    m_linearSampler.GetCoordinatesAndWeights(x, &indices, &weights);

    double result = 0.0;

    for (int i = 0; i < 4; ++i)
    {
        result += weights[i] * LaplacianAtDataPoint(indices[i].x, indices[i].y);
    }

    return result;
}

ScalarGrid2::ScalarDataAccessor ScalarGrid2::GetDataAccessor()
{
    return m_data.Accessor();
}

ScalarGrid2::ConstScalarDataAccessor ScalarGrid2::GetConstDataAccessor() const
{
    return m_data.ConstAccessor();
}

ScalarGrid2::DataPositionFunc ScalarGrid2::GetDataPosition() const
{
    Vector2D o = GetDataOrigin();

    return [this, o](size_t i, size_t j) -> Vector2D {
        return o + GridSpacing() * Vector2D({ i, j });
    };
}

void ScalarGrid2::Fill(double value, ExecutionPolicy policy)
{
    ParallelFor(
        ZERO_SIZE, m_data.Width(), ZERO_SIZE, m_data.Height(),
        [this, value](size_t i, size_t j) { m_data(i, j) = value; }, policy);
}

void ScalarGrid2::Fill(const std::function<double(const Vector2D&)>& func,
                       ExecutionPolicy policy)
{
    DataPositionFunc pos = GetDataPosition();

    ParallelFor(
        ZERO_SIZE, m_data.Width(), ZERO_SIZE, m_data.Height(),
        [this, &func, &pos](size_t i, size_t j) {
            m_data(i, j) = func(pos(i, j));
        },
        policy);
}

void ScalarGrid2::ForEachDataPointIndex(
    const std::function<void(size_t, size_t)>& func) const
{
    m_data.ForEachIndex(func);
}

void ScalarGrid2::ParallelForEachDataPointIndex(
    const std::function<void(size_t, size_t)>& func) const
{
    m_data.ParallelForEachIndex(func);
}

void ScalarGrid2::Serialize(std::vector<uint8_t>* buffer) const
{
    flatbuffers::FlatBufferBuilder builder(1024);

    auto fbsResolution = CubbyFlowToFlatbuffers(Resolution());
    auto fbsGridSpacing = CubbyFlowToFlatbuffers(GridSpacing());
    auto fbsOrigin = CubbyFlowToFlatbuffers(Origin());

    std::vector<double> gridData;
    GetData(&gridData);
    auto data = builder.CreateVector(gridData.data(), gridData.size());

    auto fbsGrid = fbs::CreateScalarGrid2(builder, &fbsResolution,
                                          &fbsGridSpacing, &fbsOrigin, data);

    builder.Finish(fbsGrid);

    uint8_t* buf = builder.GetBufferPointer();
    size_t size = builder.GetSize();

    buffer->resize(size);
    memcpy(buffer->data(), buf, size);
}

void ScalarGrid2::Deserialize(const std::vector<uint8_t>& buffer)
{
    auto fbsGrid = fbs::GetScalarGrid2(buffer.data());

    Resize(FlatbuffersToCubbyFlow(*fbsGrid->resolution()),
           FlatbuffersToCubbyFlow(*fbsGrid->gridSpacing()),
           FlatbuffersToCubbyFlow(*fbsGrid->origin()));

    auto data = fbsGrid->data();
    std::vector<double> gridData(data->size());
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

    m_data.Set(other.m_data);
    ResetSampler();
}

void ScalarGrid2::ResetSampler()
{
    m_linearSampler = LinearArraySampler2<double, double>(
        m_data.ConstAccessor(), GridSpacing(), GetDataOrigin());
    m_sampler = m_linearSampler.Functor();
}

void ScalarGrid2::GetData(std::vector<double>* data) const
{
    size_t size = GetDataSize().x * GetDataSize().y;
    data->resize(size);
    std::copy(m_data.begin(), m_data.end(), data->begin());
}

void ScalarGrid2::SetData(const std::vector<double>& data)
{
    assert(GetDataSize().x * GetDataSize().y == data.size());

    std::copy(data.begin(), data.end(), m_data.begin());
}

ScalarGridBuilder2::ScalarGridBuilder2()
{
    // Do nothing
}

ScalarGridBuilder2::~ScalarGridBuilder2()
{
    // Do nothing
}
}  // namespace CubbyFlow