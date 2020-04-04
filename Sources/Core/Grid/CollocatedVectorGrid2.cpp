// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/CollocatedVectorGrid2.hpp>

namespace CubbyFlow
{
CollocatedVectorGrid2::CollocatedVectorGrid2()
    : m_linearSampler(m_data.ConstAccessor(), Vector2D(1, 1), Vector2D())
{
    // Do nothing
}

CollocatedVectorGrid2::~CollocatedVectorGrid2()
{
    // Do nothing
}

const Vector2D& CollocatedVectorGrid2::operator()(size_t i, size_t j) const
{
    return m_data(i, j);
}

Vector2D& CollocatedVectorGrid2::operator()(size_t i, size_t j)
{
    return m_data(i, j);
}

double CollocatedVectorGrid2::DivergenceAtDataPoint(size_t i, size_t j) const
{
    const Size2 ds = m_data.size();
    const Vector2D& gs = GridSpacing();

    assert(i < ds.x && j < ds.y);

    double left = m_data((i > 0) ? i - 1 : i, j).x;
    double right = m_data((i + 1 < ds.x) ? i + 1 : i, j).x;
    double down = m_data(i, (j > 0) ? j - 1 : j).y;
    double up = m_data(i, (j + 1 < ds.y) ? j + 1 : j).y;

    return 0.5 * (right - left) / gs.x + 0.5 * (up - down) / gs.y;
}

double CollocatedVectorGrid2::CurlAtDataPoint(size_t i, size_t j) const
{
    const Size2 ds = m_data.size();
    const Vector2D& gs = GridSpacing();

    assert(i < ds.x && j < ds.y);

    Vector2D left = m_data((i > 0) ? i - 1 : i, j);
    Vector2D right = m_data((i + 1 < ds.x) ? i + 1 : i, j);
    Vector2D bottom = m_data(i, (j > 0) ? j - 1 : j);
    Vector2D top = m_data(i, (j + 1 < ds.y) ? j + 1 : j);

    double Fx_ym = bottom.x;
    double Fx_yp = top.x;

    double Fy_xm = left.y;
    double Fy_xp = right.y;

    return 0.5 * (Fy_xp - Fy_xm) / gs.x - 0.5 * (Fx_yp - Fx_ym) / gs.y;
}

Vector2D CollocatedVectorGrid2::Sample(const Vector2D& x) const
{
    return m_sampler(x);
}

double CollocatedVectorGrid2::Divergence(const Vector2D& x) const
{
    std::array<Point2UI, 4> indices;
    std::array<double, 4> weights;
    m_linearSampler.GetCoordinatesAndWeights(x, &indices, &weights);

    double result = 0.0;
    for (int i = 0; i < 4; ++i)
    {
        result +=
            weights[i] * DivergenceAtDataPoint(indices[i].x, indices[i].y);
    }

    return result;
}

double CollocatedVectorGrid2::Curl(const Vector2D& x) const
{
    std::array<Point2UI, 4> indices;
    std::array<double, 4> weights;
    m_linearSampler.GetCoordinatesAndWeights(x, &indices, &weights);

    double result = 0.0;
    for (int i = 0; i < 4; ++i)
    {
        result += weights[i] * CurlAtDataPoint(indices[i].x, indices[i].y);
    }

    return result;
}

std::function<Vector2D(const Vector2D&)> CollocatedVectorGrid2::Sampler() const
{
    return m_sampler;
}

VectorGrid2::VectorDataAccessor CollocatedVectorGrid2::GetDataAccessor()
{
    return m_data.Accessor();
}

VectorGrid2::ConstVectorDataAccessor
CollocatedVectorGrid2::GetConstDataAccessor() const
{
    return m_data.ConstAccessor();
}

VectorGrid2::DataPositionFunc CollocatedVectorGrid2::GetDataPosition() const
{
    Vector2D dataOrigin = GetDataOrigin();
    return [this, dataOrigin](size_t i, size_t j) -> Vector2D {
        return dataOrigin + GridSpacing() * Vector2D({ i, j });
    };
}

void CollocatedVectorGrid2::ForEachDataPointIndex(
    const std::function<void(size_t, size_t)>& func) const
{
    m_data.ForEachIndex(func);
}

void CollocatedVectorGrid2::ParallelForEachDataPointIndex(
    const std::function<void(size_t, size_t)>& func) const
{
    m_data.ParallelForEachIndex(func);
}

void CollocatedVectorGrid2::SwapCollocatedVectorGrid(
    CollocatedVectorGrid2* other)
{
    SwapGrid(other);

    m_data.Swap(other->m_data);
    std::swap(m_linearSampler, other->m_linearSampler);
    std::swap(m_sampler, other->m_sampler);
}

void CollocatedVectorGrid2::SetCollocatedVectorGrid(
    const CollocatedVectorGrid2& other)
{
    SetGrid(other);

    m_data.Set(other.m_data);
    ResetSampler();
}

void CollocatedVectorGrid2::OnResize(const Size2& resolution,
                                     const Vector2D& gridSpacing,
                                     const Vector2D& origin,
                                     const Vector2D& initialValue)
{
    UNUSED_VARIABLE(resolution);
    UNUSED_VARIABLE(gridSpacing);
    UNUSED_VARIABLE(origin);

    m_data.Resize(GetDataSize(), initialValue);
    ResetSampler();
}

void CollocatedVectorGrid2::ResetSampler()
{
    m_linearSampler = LinearArraySampler2<Vector2D, double>(
        m_data.ConstAccessor(), GridSpacing(), GetDataOrigin());
    m_sampler = m_linearSampler.Functor();
}

void CollocatedVectorGrid2::GetData(std::vector<double>* data) const
{
    size_t size = 2 * GetDataSize().x * GetDataSize().y;
    data->resize(size);
    size_t cnt = 0;

    m_data.ForEach([&](const Vector2D& value) {
        (*data)[cnt++] = value.x;
        (*data)[cnt++] = value.y;
    });
}

void CollocatedVectorGrid2::SetData(const std::vector<double>& data)
{
    assert(2 * GetDataSize().x * GetDataSize().y == data.size());

    size_t cnt = 0;

    m_data.ForEachIndex([&](size_t i, size_t j) {
        m_data(i, j).x = data[cnt++];
        m_data(i, j).y = data[cnt++];
    });
}
}  // namespace CubbyFlow