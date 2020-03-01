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

namespace CubbyFlow
{
FaceCenteredGrid3::FaceCenteredGrid3()
    : m_dataOriginU(0.0, 0.5, 0.5),
      m_dataOriginV(0.5, 0.0, 0.5),
      m_dataOriginW(0.5, 0.5, 0.0),
      m_uLinearSampler(LinearArraySampler3<double, double>(
          m_dataU.ConstAccessor(), Vector3D(1, 1, 1), m_dataOriginU)),
      m_vLinearSampler(LinearArraySampler3<double, double>(
          m_dataV.ConstAccessor(), Vector3D(1, 1, 1), m_dataOriginV)),
      m_wLinearSampler(LinearArraySampler3<double, double>(
          m_dataW.ConstAccessor(), Vector3D(1, 1, 1), m_dataOriginW))
{
    // Do nothing
}

FaceCenteredGrid3::FaceCenteredGrid3(size_t ResolutionX, size_t ResolutionY,
                                     size_t ResolutionZ, double GridSpacingX,
                                     double GridSpacingY, double GridSpacingZ,
                                     double OriginX, double OriginY,
                                     double OriginZ, double initialValueU,
                                     double initialValueV, double initialValueW)
    : FaceCenteredGrid3(Size3(ResolutionX, ResolutionY, ResolutionZ),
                        Vector3D(GridSpacingX, GridSpacingY, GridSpacingZ),
                        Vector3D(OriginX, OriginY, OriginZ),
                        Vector3D(initialValueU, initialValueV, initialValueW))
{
    // Do nothing
}

FaceCenteredGrid3::FaceCenteredGrid3(const Size3& Resolution,
                                     const Vector3D& GridSpacing,
                                     const Vector3D& Origin,
                                     const Vector3D& initialValue)
    : m_uLinearSampler(LinearArraySampler3<double, double>(
          m_dataU.ConstAccessor(), Vector3D(1, 1, 1), m_dataOriginU)),
      m_vLinearSampler(LinearArraySampler3<double, double>(
          m_dataV.ConstAccessor(), Vector3D(1, 1, 1), m_dataOriginV)),
      m_wLinearSampler(LinearArraySampler3<double, double>(
          m_dataW.ConstAccessor(), Vector3D(1, 1, 1), m_dataOriginW))
{
    Resize(Resolution, GridSpacing, Origin, initialValue);
}

FaceCenteredGrid3::FaceCenteredGrid3(const FaceCenteredGrid3& other)
    : m_uLinearSampler(LinearArraySampler3<double, double>(
          m_dataU.ConstAccessor(), Vector3D(1, 1, 1), m_dataOriginU)),
      m_vLinearSampler(LinearArraySampler3<double, double>(
          m_dataV.ConstAccessor(), Vector3D(1, 1, 1), m_dataOriginV)),
      m_wLinearSampler(LinearArraySampler3<double, double>(
          m_dataW.ConstAccessor(), Vector3D(1, 1, 1), m_dataOriginW))
{
    Set(other);
}

void FaceCenteredGrid3::Swap(Grid3* other)
{
    FaceCenteredGrid3* sameType = dynamic_cast<FaceCenteredGrid3*>(other);

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

    m_dataU.Set(other.m_dataU);
    m_dataV.Set(other.m_dataV);
    m_dataW.Set(other.m_dataW);
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

    return 0.5 * Vector3D(m_dataU(i, j, k) + m_dataU(i + 1, j, k),
                          m_dataV(i, j, k) + m_dataV(i, j + 1, k),
                          m_dataW(i, j, k) + m_dataW(i, j, k + 1));
}

double FaceCenteredGrid3::DivergenceAtCellCenter(size_t i, size_t j,
                                                 size_t k) const
{
    assert(i < Resolution().x && j < Resolution().y && k < Resolution().z);

    const Vector3D& gs = GridSpacing();

    double leftU = m_dataU(i, j, k);
    double rightU = m_dataU(i + 1, j, k);
    double bottomV = m_dataV(i, j, k);
    double topV = m_dataV(i, j + 1, k);
    double backW = m_dataW(i, j, k);
    double frontW = m_dataW(i, j, k + 1);

    return (rightU - leftU) / gs.x + (topV - bottomV) / gs.y +
           (frontW - backW) / gs.z;
}

Vector3D FaceCenteredGrid3::CurlAtCellCenter(size_t i, size_t j, size_t k) const
{
    const Size3& res = Resolution();
    const Vector3D& gs = GridSpacing();

    assert(i < res.x && j < res.y && k < res.z);

    Vector3D left = ValueAtCellCenter((i > 0) ? i - 1 : i, j, k);
    Vector3D right = ValueAtCellCenter((i + 1 < res.x) ? i + 1 : i, j, k);
    Vector3D down = ValueAtCellCenter(i, (j > 0) ? j - 1 : j, k);
    Vector3D up = ValueAtCellCenter(i, (j + 1 < res.y) ? j + 1 : j, k);
    Vector3D back = ValueAtCellCenter(i, j, (k > 0) ? k - 1 : k);
    Vector3D front = ValueAtCellCenter(i, j, (k + 1 < res.z) ? k + 1 : k);

    double Fx_ym = down.x;
    double Fx_yp = up.x;
    double Fx_zm = back.x;
    double Fx_zp = front.x;

    double Fy_xm = left.y;
    double Fy_xp = right.y;
    double Fy_zm = back.y;
    double Fy_zp = front.y;

    double Fz_xm = left.z;
    double Fz_xp = right.z;
    double Fz_ym = down.z;
    double Fz_yp = up.z;

    return Vector3D(
        0.5 * (Fz_yp - Fz_ym) / gs.y - 0.5 * (Fy_zp - Fy_zm) / gs.z,
        0.5 * (Fx_zp - Fx_zm) / gs.z - 0.5 * (Fz_xp - Fz_xm) / gs.x,
        0.5 * (Fy_xp - Fy_xm) / gs.x - 0.5 * (Fx_yp - Fx_ym) / gs.y);
}

FaceCenteredGrid3::ScalarDataAccessor FaceCenteredGrid3::GetUAccessor()
{
    return m_dataU.Accessor();
}

FaceCenteredGrid3::ConstScalarDataAccessor
FaceCenteredGrid3::GetUConstAccessor() const
{
    return m_dataU.ConstAccessor();
}

FaceCenteredGrid3::ScalarDataAccessor FaceCenteredGrid3::GetVAccessor()
{
    return m_dataV.Accessor();
}

FaceCenteredGrid3::ConstScalarDataAccessor
FaceCenteredGrid3::GetVConstAccessor() const
{
    return m_dataV.ConstAccessor();
}

FaceCenteredGrid3::ScalarDataAccessor FaceCenteredGrid3::GetWAccessor()
{
    return m_dataW.Accessor();
}

FaceCenteredGrid3::ConstScalarDataAccessor
FaceCenteredGrid3::GetWConstAccessor() const
{
    return m_dataW.ConstAccessor();
}

VectorGrid3::DataPositionFunc FaceCenteredGrid3::GetUPosition() const
{
    Vector3D h = GridSpacing();

    return [this, h](size_t i, size_t j, size_t k) -> Vector3D {
        return m_dataOriginU + h * Vector3D({ i, j, k });
    };
}

VectorGrid3::DataPositionFunc FaceCenteredGrid3::GetVPosition() const
{
    Vector3D h = GridSpacing();

    return [this, h](size_t i, size_t j, size_t k) -> Vector3D {
        return m_dataOriginV + h * Vector3D({ i, j, k });
    };
}

VectorGrid3::DataPositionFunc FaceCenteredGrid3::GetWPosition() const
{
    Vector3D h = GridSpacing();

    return [this, h](size_t i, size_t j, size_t k) -> Vector3D {
        return m_dataOriginW + h * Vector3D({ i, j, k });
    };
}

Size3 FaceCenteredGrid3::GetUSize() const
{
    return m_dataU.size();
}

Size3 FaceCenteredGrid3::GetVSize() const
{
    return m_dataV.size();
}

Size3 FaceCenteredGrid3::GetWSize() const
{
    return m_dataW.size();
}

Vector3D FaceCenteredGrid3::GetUOrigin() const
{
    return m_dataOriginU;
}

Vector3D FaceCenteredGrid3::GetVOrigin() const
{
    return m_dataOriginV;
}

Vector3D FaceCenteredGrid3::GetWOrigin() const
{
    return m_dataOriginW;
}

void FaceCenteredGrid3::Fill(const Vector3D& value, ExecutionPolicy policy)
{
    ParallelFor(
        ZERO_SIZE, m_dataU.Width(), ZERO_SIZE, m_dataU.Height(), ZERO_SIZE,
        m_dataU.Depth(),
        [this, value](size_t i, size_t j, size_t k) {
            m_dataU(i, j, k) = value.x;
        },
        policy);

    ParallelFor(
        ZERO_SIZE, m_dataV.Width(), ZERO_SIZE, m_dataV.Height(), ZERO_SIZE,
        m_dataV.Depth(),
        [this, value](size_t i, size_t j, size_t k) {
            m_dataV(i, j, k) = value.y;
        },
        policy);

    ParallelFor(
        ZERO_SIZE, m_dataW.Width(), ZERO_SIZE, m_dataW.Height(), ZERO_SIZE,
        m_dataW.Depth(),
        [this, value](size_t i, size_t j, size_t k) {
            m_dataW(i, j, k) = value.z;
        },
        policy);
}

void FaceCenteredGrid3::Fill(
    const std::function<Vector3D(const Vector3D&)>& func,
    ExecutionPolicy policy)
{
    DataPositionFunc uPos = GetUPosition();
    ParallelFor(
        ZERO_SIZE, m_dataU.Width(), ZERO_SIZE, m_dataU.Height(), ZERO_SIZE,
        m_dataU.Depth(),
        [this, &func, &uPos](size_t i, size_t j, size_t k) {
            m_dataU(i, j, k) = func(uPos(i, j, k)).x;
        },
        policy);

    DataPositionFunc vPos = GetVPosition();
    ParallelFor(
        ZERO_SIZE, m_dataV.Width(), ZERO_SIZE, m_dataV.Height(), ZERO_SIZE,
        m_dataV.Depth(),
        [this, &func, &vPos](size_t i, size_t j, size_t k) {
            m_dataV(i, j, k) = func(vPos(i, j, k)).y;
        },
        policy);

    DataPositionFunc wPos = GetWPosition();
    ParallelFor(
        ZERO_SIZE, m_dataW.Width(), ZERO_SIZE, m_dataW.Height(), ZERO_SIZE,
        m_dataW.Depth(),
        [this, &func, &wPos](size_t i, size_t j, size_t k) {
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
    m_dataU.ForEachIndex(func);
}

void FaceCenteredGrid3::ParallelForEachUIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    m_dataU.ParallelForEachIndex(func);
}

void FaceCenteredGrid3::ForEachVIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    m_dataV.ForEachIndex(func);
}

void FaceCenteredGrid3::ParallelForEachVIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    m_dataV.ParallelForEachIndex(func);
}

void FaceCenteredGrid3::ForEachWIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    m_dataW.ForEachIndex(func);
}

void FaceCenteredGrid3::ParallelForEachWIndex(
    const std::function<void(size_t, size_t, size_t)>& func) const
{
    m_dataW.ParallelForEachIndex(func);
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
    Size3 res = Resolution();
    ssize_t i, j, k;
    double fx, fy, fz;
    Vector3D cellCenterOrigin = Origin() + 0.5 * GridSpacing();
    Vector3D normalizedX = (x - cellCenterOrigin) / GridSpacing();

    GetBarycentric(normalizedX.x, 0, static_cast<ssize_t>(res.x) - 1, &i, &fx);
    GetBarycentric(normalizedX.y, 0, static_cast<ssize_t>(res.y) - 1, &j, &fy);
    GetBarycentric(normalizedX.z, 0, static_cast<ssize_t>(res.z) - 1, &k, &fz);

    std::array<Point3UI, 8> indices;
    std::array<double, 8> weights;

    indices[0] = Point3UI(i, j, k);
    indices[1] = Point3UI(i + 1, j, k);
    indices[2] = Point3UI(i, j + 1, k);
    indices[3] = Point3UI(i + 1, j + 1, k);
    indices[4] = Point3UI(i, j, k + 1);
    indices[5] = Point3UI(i + 1, j, k + 1);
    indices[6] = Point3UI(i, j + 1, k + 1);
    indices[7] = Point3UI(i + 1, j + 1, k + 1);

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
    Size3 res = Resolution();
    ssize_t i, j, k;
    double fx, fy, fz;
    Vector3D cellCenterOrigin = Origin() + 0.5 * GridSpacing();
    Vector3D normalizedX = (x - cellCenterOrigin) / GridSpacing();

    GetBarycentric(normalizedX.x, 0, static_cast<ssize_t>(res.x) - 1, &i, &fx);
    GetBarycentric(normalizedX.y, 0, static_cast<ssize_t>(res.y) - 1, &j, &fy);
    GetBarycentric(normalizedX.z, 0, static_cast<ssize_t>(res.z) - 1, &k, &fz);

    std::array<Point3UI, 8> indices;
    std::array<double, 8> weights;

    indices[0] = Point3UI(i, j, k);
    indices[1] = Point3UI(i + 1, j, k);
    indices[2] = Point3UI(i, j + 1, k);
    indices[3] = Point3UI(i + 1, j + 1, k);
    indices[4] = Point3UI(i, j, k + 1);
    indices[5] = Point3UI(i + 1, j, k + 1);
    indices[6] = Point3UI(i, j + 1, k + 1);
    indices[7] = Point3UI(i + 1, j + 1, k + 1);

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

void FaceCenteredGrid3::OnResize(const Size3& Resolution,
                                 const Vector3D& GridSpacing,
                                 const Vector3D& Origin,
                                 const Vector3D& initialValue)
{
    if (Resolution != Size3(0, 0, 0))
    {
        m_dataU.Resize(Resolution + Size3(1, 0, 0), initialValue.x);
        m_dataV.Resize(Resolution + Size3(0, 1, 0), initialValue.y);
        m_dataW.Resize(Resolution + Size3(0, 0, 1), initialValue.z);
    }
    else
    {
        m_dataU.Resize(Size3(0, 0, 0));
        m_dataV.Resize(Size3(0, 0, 0));
        m_dataW.Resize(Size3(0, 0, 0));
    }

    m_dataOriginU = Origin + 0.5 * Vector3D(0.0, GridSpacing.y, GridSpacing.z);
    m_dataOriginV = Origin + 0.5 * Vector3D(GridSpacing.x, 0.0, GridSpacing.z);
    m_dataOriginW = Origin + 0.5 * Vector3D(GridSpacing.x, GridSpacing.y, 0.0);

    ResetSampler();
}

void FaceCenteredGrid3::ResetSampler()
{
    LinearArraySampler3<double, double> uSampler(m_dataU.ConstAccessor(),
                                                 GridSpacing(), m_dataOriginU);
    LinearArraySampler3<double, double> vSampler(m_dataV.ConstAccessor(),
                                                 GridSpacing(), m_dataOriginV);
    LinearArraySampler3<double, double> wSampler(m_dataW.ConstAccessor(),
                                                 GridSpacing(), m_dataOriginW);

    m_uLinearSampler = uSampler;
    m_vLinearSampler = vSampler;
    m_wLinearSampler = wSampler;

    m_sampler = [uSampler, vSampler, wSampler](const Vector3D& x) -> Vector3D {
        double u = uSampler(x);
        double v = vSampler(x);
        double w = wSampler(x);
        return Vector3D(u, v, w);
    };
}

FaceCenteredGrid3::Builder FaceCenteredGrid3::GetBuilder()
{
    return Builder();
}

void FaceCenteredGrid3::GetData(std::vector<double>* data) const
{
    size_t size = GetUSize().x * GetUSize().y * GetUSize().z +
                  GetVSize().x * GetVSize().y * GetVSize().z +
                  GetWSize().x * GetWSize().y * GetWSize().z;
    data->resize(size);
    size_t cnt = 0;

    m_dataU.ForEach([&](double value) { (*data)[cnt++] = value; });
    m_dataV.ForEach([&](double value) { (*data)[cnt++] = value; });
    m_dataW.ForEach([&](double value) { (*data)[cnt++] = value; });
}

void FaceCenteredGrid3::SetData(const std::vector<double>& data)
{
    assert(GetUSize().x * GetUSize().y * GetUSize().z +
               GetVSize().x * GetVSize().y * GetVSize().z +
               GetWSize().x * GetWSize().y * GetWSize().z ==
           data.size());

    size_t cnt = 0;

    m_dataU.ForEachIndex(
        [&](size_t i, size_t j, size_t k) { m_dataU(i, j, k) = data[cnt++]; });
    m_dataV.ForEachIndex(
        [&](size_t i, size_t j, size_t k) { m_dataV(i, j, k) = data[cnt++]; });
    m_dataW.ForEachIndex(
        [&](size_t i, size_t j, size_t k) { m_dataW(i, j, k) = data[cnt++]; });
}

FaceCenteredGrid3::Builder& FaceCenteredGrid3::Builder::WithResolution(
    const Size3& resolution)
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
    return FaceCenteredGrid3(m_resolution, m_gridSpacing, m_gridOrigin,
                             m_initialVal);
}

FaceCenteredGrid3Ptr FaceCenteredGrid3::Builder::MakeShared() const
{
    return std::shared_ptr<FaceCenteredGrid3>(
        new FaceCenteredGrid3(m_resolution, m_gridSpacing, m_gridOrigin,
                              m_initialVal),
        [](FaceCenteredGrid3* obj) { delete obj; });
}

VectorGrid3Ptr FaceCenteredGrid3::Builder::Build(
    const Size3& resolution, const Vector3D& gridSpacing,
    const Vector3D& gridOrigin, const Vector3D& initialVal) const
{
    return std::shared_ptr<FaceCenteredGrid3>(
        new FaceCenteredGrid3(resolution, gridSpacing, gridOrigin, initialVal),
        [](FaceCenteredGrid3* obj) { delete obj; });
}
}  // namespace CubbyFlow
