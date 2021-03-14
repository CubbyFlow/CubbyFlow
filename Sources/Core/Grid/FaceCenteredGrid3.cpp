// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/FaceCenteredGrid3.hpp>

#include <array>

namespace CubbyFlow
{
FaceCenteredGrid3::FaceCenteredGrid3()
    : m_dataOriginU(0.0, 0.5, 0.5),
      m_dataOriginV(0.5, 0.0, 0.5),
      m_dataOriginW(0.5, 0.5, 0.0),
      m_uLinearSampler(LinearArraySampler3<double>{
          m_dataU, Vector3D{ 1, 1, 1 }, m_dataOriginU }),
      m_vLinearSampler(LinearArraySampler3<double>{
          m_dataV, Vector3D{ 1, 1, 1 }, m_dataOriginV }),
      m_wLinearSampler(LinearArraySampler3<double>{
          m_dataW, Vector3D{ 1, 1, 1 }, m_dataOriginW })
{
    // Do nothing
}

FaceCenteredGrid3::FaceCenteredGrid3(size_t resolutionX, size_t resolutionY,
                                     size_t resolutionZ, double gridSpacingX,
                                     double gridSpacingY, double gridSpacingZ,
                                     double originX, double originY,
                                     double originZ, double initialValueU,
                                     double initialValueV, double initialValueW)
    : FaceCenteredGrid3{ Vector3UZ{ resolutionX, resolutionY, resolutionZ },
                         Vector3D{ gridSpacingX, gridSpacingY, gridSpacingZ },
                         Vector3D{ originX, originY, originZ },
                         Vector3D{ initialValueU, initialValueV,
                                   initialValueW } }
{
    // Do nothing
}

FaceCenteredGrid3::FaceCenteredGrid3(const Vector3UZ& resolution,
                                     const Vector3D& gridSpacing,
                                     const Vector3D& origin,
                                     const Vector3D& initialValue)
    : m_uLinearSampler(LinearArraySampler3<double>{
          m_dataU, Vector3D{ 1, 1, 1 }, m_dataOriginU }),
      m_vLinearSampler(LinearArraySampler3<double>{
          m_dataV, Vector3D{ 1, 1, 1 }, m_dataOriginV }),
      m_wLinearSampler(LinearArraySampler3<double>{
          m_dataW, Vector3D{ 1, 1, 1 }, m_dataOriginW })
{
    Resize(resolution, gridSpacing, origin, initialValue);
}

FaceCenteredGrid3::FaceCenteredGrid3(const FaceCenteredGrid3& other)
    : VectorGrid3{ other },
      m_uLinearSampler(LinearArraySampler3<double>{
          m_dataU, Vector3D{ 1, 1, 1 }, m_dataOriginU }),
      m_vLinearSampler(LinearArraySampler3<double>{
          m_dataV, Vector3D{ 1, 1, 1 }, m_dataOriginV }),
      m_wLinearSampler(LinearArraySampler3<double>{
          m_dataW, Vector3D{ 1, 1, 1 }, m_dataOriginW })
{
    Set(other);
}

void FaceCenteredGrid3::Swap(Grid3* other)
{
    auto sameType = dynamic_cast<FaceCenteredGrid3*>(other);

    if (sameType != nullptr)
    {
        SwapGrid(sameType);

        m_dataU.Swap(sameType->m_dataU);
        m_dataV.Swap(sameType->m_dataV);
        m_dataW.Swap(sameType->m_dataW);
        std::swap(m_dataOriginU, sameType->m_dataOriginU);
        std::swap(m_dataOriginV, sameType->m_dataOriginV);
        std::swap(m_dataOriginW, sameType->m_dataOriginW);
        std::swap(m_uLinearSampler, sameType->m_uLinearSampler);
        std::swap(m_vLinearSampler, sameType->m_vLinearSampler);
        std::swap(m_wLinearSampler, sameType->m_wLinearSampler);
        std::swap(m_sampler, sameType->m_sampler);
    }
}

void FaceCenteredGrid3::Set(const FaceCenteredGrid3& other)
{
    SetGrid(other);

    m_dataU.CopyFrom(other.m_dataU);
    m_dataV.CopyFrom(other.m_dataV);
    m_dataW.CopyFrom(other.m_dataW);
    m_dataOriginU = other.m_dataOriginU;
    m_dataOriginV = other.m_dataOriginV;
    m_dataOriginW = other.m_dataOriginW;

    ResetSampler();
}

FaceCenteredGrid3& FaceCenteredGrid3::operator=(const FaceCenteredGrid3& other)
{
    Set(other);
    return *this;
}

double& FaceCenteredGrid3::GetU(size_t i, size_t j, size_t k)
{
    return m_dataU(i, j, k);
}

const double& FaceCenteredGrid3::GetU(size_t i, size_t j, size_t k) const
{
    return m_dataU(i, j, k);
}

double& FaceCenteredGrid3::GetV(size_t i, size_t j, size_t k)
{
    return m_dataV(i, j, k);
}

const double& FaceCenteredGrid3::GetV(size_t i, size_t j, size_t k) const
{
    return m_dataV(i, j, k);
}

double& FaceCenteredGrid3::GetW(size_t i, size_t j, size_t k)
{
    return m_dataW(i, j, k);
}

const double& FaceCenteredGrid3::GetW(size_t i, size_t j, size_t k) const
{
    return m_dataW(i, j, k);
}

Vector3D FaceCenteredGrid3::ValueAtCellCenter(size_t i, size_t j,
                                              size_t k) const
{
    assert(i < Resolution().x && j < Resolution().y && k < Resolution().z);

    return 0.5 * Vector3D{ m_dataU(i, j, k) + m_dataU(i + 1, j, k),
                           m_dataV(i, j, k) + m_dataV(i, j + 1, k),
                           m_dataW(i, j, k) + m_dataW(i, j, k + 1) };
}

double FaceCenteredGrid3::DivergenceAtCellCenter(size_t i, size_t j,
                                                 size_t k) const
{
    assert(i < Resolution().x && j < Resolution().y && k < Resolution().z);

    const Vector3D& gs = GridSpacing();

    const double leftU = m_dataU(i, j, k);
    const double rightU = m_dataU(i + 1, j, k);
    const double bottomV = m_dataV(i, j, k);
    const double topV = m_dataV(i, j + 1, k);
    const double backW = m_dataW(i, j, k);
    const double frontW = m_dataW(i, j, k + 1);

    return (rightU - leftU) / gs.x + (topV - bottomV) / gs.y +
           (frontW - backW) / gs.z;
}

Vector3D FaceCenteredGrid3::CurlAtCellCenter(size_t i, size_t j, size_t k) const
{
    const Vector3UZ& res = Resolution();
    const Vector3D& gs = GridSpacing();

    assert(i < res.x && j < res.y && k < res.z);

    const Vector3D left = ValueAtCellCenter((i > 0) ? i - 1 : i, j, k);
    const Vector3D right = ValueAtCellCenter((i + 1 < res.x) ? i + 1 : i, j, k);
    const Vector3D down = ValueAtCellCenter(i, (j > 0) ? j - 1 : j, k);
    const Vector3D up = ValueAtCellCenter(i, (j + 1 < res.y) ? j + 1 : j, k);
    const Vector3D back = ValueAtCellCenter(i, j, (k > 0) ? k - 1 : k);
    const Vector3D front = ValueAtCellCenter(i, j, (k + 1 < res.z) ? k + 1 : k);

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

FaceCenteredGrid3::ScalarDataView FaceCenteredGrid3::UView()
{
    return ScalarDataView(m_dataU);
}

FaceCenteredGrid3::ConstScalarDataView FaceCenteredGrid3::UView() const
{
    return ConstScalarDataView(m_dataU);
}

FaceCenteredGrid3::ScalarDataView FaceCenteredGrid3::VView()
{
    return ScalarDataView(m_dataV);
}

FaceCenteredGrid3::ConstScalarDataView FaceCenteredGrid3::VView() const
{
    return ConstScalarDataView(m_dataV);
}

FaceCenteredGrid3::ScalarDataView FaceCenteredGrid3::WView()
{
    return ScalarDataView(m_dataW);
}

FaceCenteredGrid3::ConstScalarDataView FaceCenteredGrid3::WView() const
{
    return ConstScalarDataView(m_dataW);
}

VectorGrid3::DataPositionFunc FaceCenteredGrid3::UPosition() const
{
    Vector3D h = GridSpacing();

    return
        [this, h](const size_t i, const size_t j, const size_t k) -> Vector3D {
            return m_dataOriginU +
                   ElemMul(h, Vector3D{ { static_cast<double>(i),
                                          static_cast<double>(j),
                                          static_cast<double>(k) } });
        };
}

VectorGrid3::DataPositionFunc FaceCenteredGrid3::VPosition() const
{
    Vector3D h = GridSpacing();

    return
        [this, h](const size_t i, const size_t j, const size_t k) -> Vector3D {
            return m_dataOriginV +
                   ElemMul(h, Vector3D{ { static_cast<double>(i),
                                          static_cast<double>(j),
                                          static_cast<double>(k) } });
        };
}

VectorGrid3::DataPositionFunc FaceCenteredGrid3::WPosition() const
{
    Vector3D h = GridSpacing();

    return
        [this, h](const size_t i, const size_t j, const size_t k) -> Vector3D {
            return m_dataOriginW +
                   ElemMul(h, Vector3D{ { static_cast<double>(i),
                                          static_cast<double>(j),
                                          static_cast<double>(k) } });
        };
}

Vector3UZ FaceCenteredGrid3::USize() const
{
    return m_dataU.Size();
}

Vector3UZ FaceCenteredGrid3::VSize() const
{
    return m_dataV.Size();
}

Vector3UZ FaceCenteredGrid3::WSize() const
{
    return m_dataW.Size();
}

Vector3D FaceCenteredGrid3::UOrigin() const
{
    return m_dataOriginU;
}

Vector3D FaceCenteredGrid3::VOrigin() const
{
    return m_dataOriginV;
}

Vector3D FaceCenteredGrid3::WOrigin() const
{
    return m_dataOriginW;
}

void FaceCenteredGrid3::Fill(const Vector3D& value, ExecutionPolicy policy)
{
    ParallelFor(
        ZERO_SIZE, m_dataU.Width(), ZERO_SIZE, m_dataU.Height(), ZERO_SIZE,
        m_dataU.Depth(),
        [this, value](const size_t i, const size_t j, const size_t k) {
            m_dataU(i, j, k) = value.x;
        },
        policy);

    ParallelFor(
        ZERO_SIZE, m_dataV.Width(), ZERO_SIZE, m_dataV.Height(), ZERO_SIZE,
        m_dataV.Depth(),
        [this, value](const size_t i, const size_t j, const size_t k) {
            m_dataV(i, j, k) = value.y;
        },
        policy);

    ParallelFor(
        ZERO_SIZE, m_dataW.Width(), ZERO_SIZE, m_dataW.Height(), ZERO_SIZE,
        m_dataW.Depth(),
        [this, value](const size_t i, const size_t j, const size_t k) {
            m_dataW(i, j, k) = value.z;
        },
        policy);
}

void FaceCenteredGrid3::Fill(
    const std::function<Vector3D(const Vector3D&)>& func,
    ExecutionPolicy policy)
{
    DataPositionFunc uPos = UPosition();
    ParallelFor(
        ZERO_SIZE, m_dataU.Width(), ZERO_SIZE, m_dataU.Height(), ZERO_SIZE,
        m_dataU.Depth(),
        [this, &func, &uPos](const size_t i, const size_t j, const size_t k) {
            m_dataU(i, j, k) = func(uPos(i, j, k)).x;
        },
        policy);

    DataPositionFunc vPos = VPosition();
    ParallelFor(
        ZERO_SIZE, m_dataV.Width(), ZERO_SIZE, m_dataV.Height(), ZERO_SIZE,
        m_dataV.Depth(),
        [this, &func, &vPos](const size_t i, const size_t j, const size_t k) {
            m_dataV(i, j, k) = func(vPos(i, j, k)).y;
        },
        policy);

    DataPositionFunc wPos = WPosition();
    ParallelFor(
        ZERO_SIZE, m_dataW.Width(), ZERO_SIZE, m_dataW.Height(), ZERO_SIZE,
        m_dataW.Depth(),
        [this, &func, &wPos](const size_t i, const size_t j, const size_t k) {
            m_dataW(i, j, k) = func(wPos(i, j, k)).z;
        },
        policy);
}

std::shared_ptr<VectorGrid3> FaceCenteredGrid3::Clone() const
{
    return std::shared_ptr<FaceCenteredGrid3>(
        new FaceCenteredGrid3(*this),
        [](FaceCenteredGrid3* obj) { delete obj; });
}

void FaceCenteredGrid3::ForEachUIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    ForEachIndex(m_dataU.Size(), func);
}

void FaceCenteredGrid3::ParallelForEachUIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    ParallelForEachIndex(m_dataU.Size(), func);
}

void FaceCenteredGrid3::ForEachVIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    ForEachIndex(m_dataV.Size(), func);
}

void FaceCenteredGrid3::ParallelForEachVIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    ParallelForEachIndex(m_dataV.Size(), func);
}

void FaceCenteredGrid3::ForEachWIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    ForEachIndex(m_dataW.Size(), func);
}

void FaceCenteredGrid3::ParallelForEachWIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    ParallelForEachIndex(m_dataW.Size(), func);
}

Vector3D FaceCenteredGrid3::Sample(const Vector3D& x) const
{
    return m_sampler(x);
}

std::function<Vector3D(const Vector3D&)> FaceCenteredGrid3::Sampler() const
{
    return m_sampler;
}

double FaceCenteredGrid3::Divergence(const Vector3D& x) const
{
    const Vector3UZ res = Resolution();
    size_t i, j, k;
    double fx, fy, fz;
    const Vector3D cellCenterOrigin = GridOrigin() + 0.5 * GridSpacing();
    const Vector3D normalizedX = ElemDiv((x - cellCenterOrigin), GridSpacing());

    GetBarycentric(normalizedX.x, static_cast<size_t>(res.x), i, fx);
    GetBarycentric(normalizedX.y, static_cast<size_t>(res.y), j, fy);
    GetBarycentric(normalizedX.z, static_cast<size_t>(res.z), k, fz);

    std::array<Vector3UZ, 8> indices;
    std::array<double, 8> weights{};

    indices[0] = Vector3UZ{ i, j, k };
    indices[1] = Vector3UZ{ i + 1, j, k };
    indices[2] = Vector3UZ{ i, j + 1, k };
    indices[3] = Vector3UZ{ i + 1, j + 1, k };
    indices[4] = Vector3UZ{ i, j, k + 1 };
    indices[5] = Vector3UZ{ i + 1, j, k + 1 };
    indices[6] = Vector3UZ{ i, j + 1, k + 1 };
    indices[7] = Vector3UZ{ i + 1, j + 1, k + 1 };

    weights[0] = (1.0 - fx) * (1.0 - fy) * (1.0 - fz);
    weights[1] = fx * (1.0 - fy) * (1.0 - fz);
    weights[2] = (1.0 - fx) * fy * (1.0 - fz);
    weights[3] = fx * fy * (1.0 - fz);
    weights[4] = (1.0 - fx) * (1.0 - fy) * fz;
    weights[5] = fx * (1.0 - fy) * fz;
    weights[6] = (1.0 - fx) * fy * fz;
    weights[7] = fx * fy * fz;

    double result = 0.0;

    for (int n = 0; n < 8; ++n)
    {
        result += weights[n] * DivergenceAtCellCenter(
                                   indices[n].x, indices[n].y, indices[n].z);
    }

    return result;
}

Vector3D FaceCenteredGrid3::Curl(const Vector3D& x) const
{
    const Vector3UZ res = Resolution();
    size_t i, j, k;
    double fx, fy, fz;
    const Vector3D cellCenterOrigin = GridOrigin() + 0.5 * GridSpacing();
    const Vector3D normalizedX = ElemDiv((x - cellCenterOrigin), GridSpacing());

    GetBarycentric(normalizedX.x, static_cast<size_t>(res.x), i, fx);
    GetBarycentric(normalizedX.y, static_cast<size_t>(res.y), j, fy);
    GetBarycentric(normalizedX.z, static_cast<size_t>(res.z), k, fz);

    std::array<Vector3UZ, 8> indices;
    std::array<double, 8> weights{};

    indices[0] = Vector3UZ{ i, j, k };
    indices[1] = Vector3UZ{ i + 1, j, k };
    indices[2] = Vector3UZ{ i, j + 1, k };
    indices[3] = Vector3UZ{ i + 1, j + 1, k };
    indices[4] = Vector3UZ{ i, j, k + 1 };
    indices[5] = Vector3UZ{ i + 1, j, k + 1 };
    indices[6] = Vector3UZ{ i, j + 1, k + 1 };
    indices[7] = Vector3UZ{ i + 1, j + 1, k + 1 };

    weights[0] = (1.0 - fx) * (1.0 - fy) * (1.0 - fz);
    weights[1] = fx * (1.0 - fy) * (1.0 - fz);
    weights[2] = (1.0 - fx) * fy * (1.0 - fz);
    weights[3] = fx * fy * (1.0 - fz);
    weights[4] = (1.0 - fx) * (1.0 - fy) * fz;
    weights[5] = fx * (1.0 - fy) * fz;
    weights[6] = (1.0 - fx) * fy * fz;
    weights[7] = fx * fy * fz;

    Vector3D result;
    for (int n = 0; n < 8; ++n)
    {
        result += weights[n] *
                  CurlAtCellCenter(indices[n].x, indices[n].y, indices[n].z);
    }

    return result;
}

void FaceCenteredGrid3::OnResize(const Vector3UZ& resolution,
                                 const Vector3D& gridSpacing,
                                 const Vector3D& origin,
                                 const Vector3D& initialValue)
{
    if (resolution != Vector3UZ{ 0, 0, 0 })
    {
        m_dataU.Resize(resolution + Vector3UZ{ 1, 0, 0 }, initialValue.x);
        m_dataV.Resize(resolution + Vector3UZ{ 0, 1, 0 }, initialValue.y);
        m_dataW.Resize(resolution + Vector3UZ{ 0, 0, 1 }, initialValue.z);
    }
    else
    {
        m_dataU.Resize(Vector3UZ{ 0, 0, 0 });
        m_dataV.Resize(Vector3UZ{ 0, 0, 0 });
        m_dataW.Resize(Vector3UZ{ 0, 0, 0 });
    }

    m_dataOriginU =
        origin + 0.5 * Vector3D{ 0.0, gridSpacing.y, gridSpacing.z };
    m_dataOriginV =
        origin + 0.5 * Vector3D{ gridSpacing.x, 0.0, gridSpacing.z };
    m_dataOriginW =
        origin + 0.5 * Vector3D{ gridSpacing.x, gridSpacing.y, 0.0 };

    ResetSampler();
}

void FaceCenteredGrid3::ResetSampler()
{
    LinearArraySampler3<double> uSampler{ m_dataU, GridSpacing(),
                                          m_dataOriginU };
    LinearArraySampler3<double> vSampler{ m_dataV, GridSpacing(),
                                          m_dataOriginV };
    LinearArraySampler3<double> wSampler{ m_dataW, GridSpacing(),
                                          m_dataOriginW };

    m_uLinearSampler = uSampler;
    m_vLinearSampler = vSampler;
    m_wLinearSampler = wSampler;

    m_sampler = [uSampler, vSampler, wSampler](const Vector3D& x) -> Vector3D {
        const double u = uSampler(x);
        const double v = vSampler(x);
        const double w = wSampler(x);
        return Vector3D{ u, v, w };
    };
}

FaceCenteredGrid3::Builder FaceCenteredGrid3::GetBuilder()
{
    return Builder{};
}

void FaceCenteredGrid3::GetData(std::vector<double>* data) const
{
    const size_t size = USize().x * USize().y * USize().z +
                        VSize().x * VSize().y * VSize().z +
                        WSize().x * WSize().y * WSize().z;
    data->resize(size);
    size_t cnt = 0;

    std::for_each(m_dataU.begin(), m_dataU.end(),
                  [&](double value) { (*data)[cnt++] = value; });
    std::for_each(m_dataV.begin(), m_dataV.end(),
                  [&](double value) { (*data)[cnt++] = value; });
    std::for_each(m_dataW.begin(), m_dataW.end(),
                  [&](double value) { (*data)[cnt++] = value; });
}

void FaceCenteredGrid3::SetData(const std::vector<double>& data)
{
    assert(USize().x * USize().y * USize().z +
               VSize().x * VSize().y * VSize().z +
               WSize().x * WSize().y * WSize().z ==
           data.size());

    size_t cnt = 0;

    ForEachIndex(m_dataU.Size(), [&](size_t i, size_t j, size_t k) {
        m_dataU(i, j, k) = data[cnt++];
    });
    ForEachIndex(m_dataV.Size(), [&](size_t i, size_t j, size_t k) {
        m_dataV(i, j, k) = data[cnt++];
    });
    ForEachIndex(m_dataW.Size(), [&](size_t i, size_t j, size_t k) {
        m_dataW(i, j, k) = data[cnt++];
    });
}

FaceCenteredGrid3::Builder& FaceCenteredGrid3::Builder::WithResolution(
    const Vector3UZ& resolution)
{
    m_resolution = resolution;
    return *this;
}

FaceCenteredGrid3::Builder& FaceCenteredGrid3::Builder::WithResolution(
    size_t resolutionX, size_t resolutionY, size_t resolutionZ)
{
    m_resolution.x = resolutionX;
    m_resolution.y = resolutionY;
    m_resolution.z = resolutionZ;
    return *this;
}

FaceCenteredGrid3::Builder& FaceCenteredGrid3::Builder::WithGridSpacing(
    const Vector3D& gridSpacing)
{
    m_gridSpacing = gridSpacing;
    return *this;
}

FaceCenteredGrid3::Builder& FaceCenteredGrid3::Builder::WithGridSpacing(
    double gridSpacingX, double gridSpacingY, double gridSpacingZ)
{
    m_gridSpacing.x = gridSpacingX;
    m_gridSpacing.y = gridSpacingY;
    m_gridSpacing.z = gridSpacingZ;
    return *this;
}

FaceCenteredGrid3::Builder& FaceCenteredGrid3::Builder::WithOrigin(
    const Vector3D& gridOrigin)
{
    m_gridOrigin = gridOrigin;
    return *this;
}

FaceCenteredGrid3::Builder& FaceCenteredGrid3::Builder::WithOrigin(
    double gridOriginX, double gridOriginY, double gridOriginZ)
{
    m_gridOrigin.x = gridOriginX;
    m_gridOrigin.y = gridOriginY;
    m_gridOrigin.z = gridOriginZ;
    return *this;
}

FaceCenteredGrid3::Builder& FaceCenteredGrid3::Builder::WithInitialValue(
    const Vector3D& initialVal)
{
    m_initialVal = initialVal;
    return *this;
}

FaceCenteredGrid3::Builder& FaceCenteredGrid3::Builder::WithInitialValue(
    double initialValX, double initialValY, double initialValZ)
{
    m_initialVal.x = initialValX;
    m_initialVal.y = initialValY;
    m_initialVal.z = initialValZ;
    return *this;
}

FaceCenteredGrid3 FaceCenteredGrid3::Builder::Build() const
{
    return FaceCenteredGrid3{ m_resolution, m_gridSpacing, m_gridOrigin,
                              m_initialVal };
}

FaceCenteredGrid3Ptr FaceCenteredGrid3::Builder::MakeShared() const
{
    return std::shared_ptr<FaceCenteredGrid3>(
        new FaceCenteredGrid3{ m_resolution, m_gridSpacing, m_gridOrigin,
                               m_initialVal },
        [](FaceCenteredGrid3* obj) { delete obj; });
}

VectorGrid3Ptr FaceCenteredGrid3::Builder::Build(
    const Vector3UZ& resolution, const Vector3D& gridSpacing,
    const Vector3D& gridOrigin, const Vector3D& initialVal) const
{
    return std::shared_ptr<FaceCenteredGrid3>(
        new FaceCenteredGrid3{ resolution, gridSpacing, gridOrigin,
                               initialVal },
        [](FaceCenteredGrid3* obj) { delete obj; });
}
}  // namespace CubbyFlow
