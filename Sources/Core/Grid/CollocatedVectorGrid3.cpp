// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/CollocatedVectorGrid3.hpp>

#include <array>

namespace CubbyFlow
{
CollocatedVectorGrid3::CollocatedVectorGrid3()
    : m_linearSampler(m_data, Vector3D{ 1, 1, 1 }, Vector3D{})
{
    // Do nothing
}

const Vector3D& CollocatedVectorGrid3::operator()(size_t i, size_t j,
                                                  size_t k) const
{
    return m_data(i, j, k);
}

Vector3D& CollocatedVectorGrid3::operator()(size_t i, size_t j, size_t k)
{
    return m_data(i, j, k);
}

double CollocatedVectorGrid3::DivergenceAtDataPoint(size_t i, size_t j,
                                                    size_t k) const
{
    const Vector3UZ ds = m_data.Size();
    const Vector3D& gs = GridSpacing();

    assert(i < ds.x && j < ds.y && k < ds.z);

    const double left = m_data((i > 0) ? i - 1 : i, j, k).x;
    const double right = m_data((i + 1 < ds.x) ? i + 1 : i, j, k).x;
    const double down = m_data(i, (j > 0) ? j - 1 : j, k).y;
    const double up = m_data(i, (j + 1 < ds.y) ? j + 1 : j, k).y;
    const double back = m_data(i, j, (k > 0) ? k - 1 : k).z;
    const double front = m_data(i, j, (k + 1 < ds.z) ? k + 1 : k).z;

    return 0.5 * (right - left) / gs.x + 0.5 * (up - down) / gs.y +
           0.5 * (front - back) / gs.z;
}

Vector3D CollocatedVectorGrid3::CurlAtDataPoint(size_t i, size_t j,
                                                size_t k) const
{
    const Vector3UZ ds = m_data.Size();
    const Vector3D& gs = GridSpacing();

    assert(i < ds.x && j < ds.y && k < ds.z);

    const Vector3D left = m_data((i > 0) ? i - 1 : i, j, k);
    const Vector3D right = m_data((i + 1 < ds.x) ? i + 1 : i, j, k);
    const Vector3D down = m_data(i, (j > 0) ? j - 1 : j, k);
    const Vector3D up = m_data(i, (j + 1 < ds.y) ? j + 1 : j, k);
    const Vector3D back = m_data(i, j, (k > 0) ? k - 1 : k);
    const Vector3D front = m_data(i, j, (k + 1 < ds.z) ? k + 1 : k);

    const double Fx_ym = down.x;
    const double Fx_yp = up.x;
    const double Fx_zm = back.x;
    const double Fx_zp = front.x;

    const double Fy_xm = left.y;
    const double Fy_xp = right.y;
    const double Fy_zm = back.y;
    const double Fy_zp = front.y;

    const double Fz_xm = left.z;
    const double Fz_xp = right.z;
    const double Fz_ym = down.z;
    const double Fz_yp = up.z;

    return Vector3D{
        0.5 * (Fz_yp - Fz_ym) / gs.y - 0.5 * (Fy_zp - Fy_zm) / gs.z,
        0.5 * (Fx_zp - Fx_zm) / gs.z - 0.5 * (Fz_xp - Fz_xm) / gs.x,
        0.5 * (Fy_xp - Fy_xm) / gs.x - 0.5 * (Fx_yp - Fx_ym) / gs.y
    };
}

Vector3D CollocatedVectorGrid3::Sample(const Vector3D& x) const
{
    return m_sampler(x);
}

double CollocatedVectorGrid3::Divergence(const Vector3D& x) const
{
    std::array<Vector3UZ, 8> indices;
    std::array<double, 8> weights{};
    m_linearSampler.GetCoordinatesAndWeights(x, indices, weights);

    double result = 0.0;
    for (int i = 0; i < 8; ++i)
    {
        result += weights[i] * DivergenceAtDataPoint(indices[i].x, indices[i].y,
                                                     indices[i].z);
    }

    return result;
}

Vector3D CollocatedVectorGrid3::Curl(const Vector3D& x) const
{
    std::array<Vector3UZ, 8> indices;
    std::array<double, 8> weights{};
    m_linearSampler.GetCoordinatesAndWeights(x, indices, weights);

    Vector3D result;
    for (int i = 0; i < 8; ++i)
    {
        result += weights[i] *
                  CurlAtDataPoint(indices[i].x, indices[i].y, indices[i].z);
    }

    return result;
}

std::function<Vector3D(const Vector3D&)> CollocatedVectorGrid3::Sampler() const
{
    return m_sampler;
}

VectorGrid3::VectorDataView CollocatedVectorGrid3::DataView()
{
    return VectorDataView(m_data);
}

VectorGrid3::ConstVectorDataView CollocatedVectorGrid3::DataView() const
{
    return ConstVectorDataView(m_data);
}

VectorGrid3::DataPositionFunc CollocatedVectorGrid3::DataPosition() const
{
    Vector3D dataOrigin = GetDataOrigin();
    return [this, dataOrigin](const size_t i, const size_t j,
                              const size_t k) -> Vector3D {
        return dataOrigin +
               GridSpacing() *
                   Vector3D{ { static_cast<double>(i), static_cast<double>(j),
                               static_cast<double>(k) } };
    };
}

void CollocatedVectorGrid3::ForEachDataPointIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    ForEachIndex(m_data.Size(), func);
}

void CollocatedVectorGrid3::ParallelForEachDataPointIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    ParallelForEachIndex(m_data.Size(), func);
}

void CollocatedVectorGrid3::SwapCollocatedVectorGrid(
    CollocatedVectorGrid3* other)
{
    SwapGrid(other);

    m_data.Swap(other->m_data);
    std::swap(m_linearSampler, other->m_linearSampler);
    std::swap(m_sampler, other->m_sampler);
}

void CollocatedVectorGrid3::SetCollocatedVectorGrid(
    const CollocatedVectorGrid3& other)
{
    SetGrid(other);

    m_data.CopyFrom(other.m_data);
    ResetSampler();
}

void CollocatedVectorGrid3::OnResize(const Vector3UZ& resolution,
                                     const Vector3D& gridSpacing,
                                     const Vector3D& origin,
                                     const Vector3D& initialValue)
{
    UNUSED_VARIABLE(resolution);
    UNUSED_VARIABLE(gridSpacing);
    UNUSED_VARIABLE(origin);

    m_data.Resize(GetDataSize(), initialValue);
    ResetSampler();
}

void CollocatedVectorGrid3::ResetSampler()
{
    m_linearSampler =
        LinearArraySampler3<Vector3D>{ m_data, GridSpacing(), GetDataOrigin() };
    m_sampler = m_linearSampler.Functor();
}

void CollocatedVectorGrid3::GetData(std::vector<double>* data) const
{
    const size_t size = 3 * GetDataSize().x * GetDataSize().y * GetDataSize().z;
    data->resize(size);
    size_t cnt = 0;

    ForEachIndex(m_data.Size(), [&](size_t i, size_t j, size_t k) {
        const Vector3D& value = m_data(i, j, k);

        (*data)[cnt++] = value.x;
        (*data)[cnt++] = value.y;
        (*data)[cnt++] = value.z;
    });
}

void CollocatedVectorGrid3::SetData(const std::vector<double>& data)
{
    assert(3 * GetDataSize().x * GetDataSize().y * GetDataSize().z ==
           data.size());

    size_t cnt = 0;

    ForEachIndex(m_data.Size(),
                 [&](const size_t i, const size_t j, const size_t k) {
                     m_data(i, j, k).x = data[cnt++];
                     m_data(i, j, k).y = data[cnt++];
                     m_data(i, j, k).z = data[cnt++];
                 });
}
}  // namespace CubbyFlow