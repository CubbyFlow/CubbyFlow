// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Grid/FaceCenteredGrid2.hpp>

#include <array>

namespace CubbyFlow
{
FaceCenteredGrid2::FaceCenteredGrid2()
    : m_dataOriginU(0.0, 0.5),
      m_dataOriginV(0.5, 0.0),
      m_uLinearSampler(LinearArraySampler2<double>{ m_dataU, Vector2D{ 1, 1 },
                                                    m_dataOriginU }),
      m_vLinearSampler(LinearArraySampler2<double>{ m_dataV, Vector2D{ 1, 1 },
                                                    m_dataOriginV })
{
    // Do nothing
}

FaceCenteredGrid2::FaceCenteredGrid2(size_t resolutionX, size_t resolutionY,
                                     double gridSpacingX, double gridSpacingY,
                                     double originX, double originY,
                                     double initialValueU, double initialValueV)
    : FaceCenteredGrid2{ Vector2UZ{ resolutionX, resolutionY },
                         Vector2D{ gridSpacingX, gridSpacingY },
                         Vector2D{ originX, originY },
                         Vector2D{ initialValueU, initialValueV } }
{
    // Do nothing
}

FaceCenteredGrid2::FaceCenteredGrid2(const Vector2UZ& resolution,
                                     const Vector2D& gridSpacing,
                                     const Vector2D& origin,
                                     const Vector2D& initialValue)
    : m_uLinearSampler(LinearArraySampler2<double>{ m_dataU, Vector2D{ 1, 1 },
                                                    m_dataOriginU }),
      m_vLinearSampler(LinearArraySampler2<double>{ m_dataV, Vector2D{ 1, 1 },
                                                    m_dataOriginV })
{
    Resize(resolution, gridSpacing, origin, initialValue);
}

FaceCenteredGrid2::FaceCenteredGrid2(const FaceCenteredGrid2& other)
    : VectorGrid2{ other },
      m_uLinearSampler(LinearArraySampler2<double>{ m_dataU, Vector2D{ 1, 1 },
                                                    m_dataOriginU }),
      m_vLinearSampler(LinearArraySampler2<double>{ m_dataV, Vector2D{ 1, 1 },
                                                    m_dataOriginV })
{
    Set(other);
}

FaceCenteredGrid2& FaceCenteredGrid2::operator=(const FaceCenteredGrid2& other)
{
    Set(other);
    return *this;
}

void FaceCenteredGrid2::Swap(Grid2* other)
{
    auto sameType = dynamic_cast<FaceCenteredGrid2*>(other);

    if (sameType != nullptr)
    {
        SwapGrid(sameType);

        m_dataU.Swap(sameType->m_dataU);
        m_dataV.Swap(sameType->m_dataV);
        std::swap(m_dataOriginU, sameType->m_dataOriginU);
        std::swap(m_dataOriginV, sameType->m_dataOriginV);
        std::swap(m_uLinearSampler, sameType->m_uLinearSampler);
        std::swap(m_vLinearSampler, sameType->m_vLinearSampler);
        std::swap(m_sampler, sameType->m_sampler);
    }
}

void FaceCenteredGrid2::Set(const FaceCenteredGrid2& other)
{
    SetGrid(other);

    m_dataU.CopyFrom(other.m_dataU);
    m_dataV.CopyFrom(other.m_dataV);
    m_dataOriginU = other.m_dataOriginU;
    m_dataOriginV = other.m_dataOriginV;

    ResetSampler();
}

double& FaceCenteredGrid2::GetU(size_t i, size_t j)
{
    return m_dataU(i, j);
}

const double& FaceCenteredGrid2::GetU(size_t i, size_t j) const
{
    return m_dataU(i, j);
}

double& FaceCenteredGrid2::GetV(size_t i, size_t j)
{
    return m_dataV(i, j);
}

const double& FaceCenteredGrid2::GetV(size_t i, size_t j) const
{
    return m_dataV(i, j);
}

Vector2D FaceCenteredGrid2::ValueAtCellCenter(size_t i, size_t j) const
{
    assert(i < Resolution().x && j < Resolution().y);

    return 0.5 * Vector2D{ m_dataU(i, j) + m_dataU(i + 1, j),
                           m_dataV(i, j) + m_dataV(i, j + 1) };
}

double FaceCenteredGrid2::DivergenceAtCellCenter(size_t i, size_t j) const
{
    assert(i < Resolution().x && j < Resolution().y);

    const Vector2D& gs = GridSpacing();

    const double leftU = m_dataU(i, j);
    const double rightU = m_dataU(i + 1, j);
    const double bottomV = m_dataV(i, j);
    const double topV = m_dataV(i, j + 1);

    return (rightU - leftU) / gs.x + (topV - bottomV) / gs.y;
}

double FaceCenteredGrid2::CurlAtCellCenter(size_t i, size_t j) const
{
    const Vector2UZ& res = Resolution();

    assert(i < res.x && j < res.y);

    const Vector2D gs = GridSpacing();

    const Vector2D left = ValueAtCellCenter((i > 0) ? i - 1 : i, j);
    const Vector2D right = ValueAtCellCenter((i + 1 < res.x) ? i + 1 : i, j);
    const Vector2D bottom = ValueAtCellCenter(i, (j > 0) ? j - 1 : j);
    const Vector2D top = ValueAtCellCenter(i, (j + 1 < res.y) ? j + 1 : j);

    const double Fx_ym = bottom.x;
    const double Fx_yp = top.x;

    const double Fy_xm = left.y;
    const double Fy_xp = right.y;

    return 0.5 * (Fy_xp - Fy_xm) / gs.x - 0.5 * (Fx_yp - Fx_ym) / gs.y;
}

FaceCenteredGrid2::ScalarDataView FaceCenteredGrid2::UView()
{
    return m_dataU;
}

FaceCenteredGrid2::ConstScalarDataView FaceCenteredGrid2::UView() const
{
    return m_dataU;
}

FaceCenteredGrid2::ScalarDataView FaceCenteredGrid2::VView()
{
    return m_dataV;
}

FaceCenteredGrid2::ConstScalarDataView FaceCenteredGrid2::VView() const
{
    return m_dataV;
}

VectorGrid2::DataPositionFunc FaceCenteredGrid2::UPosition() const
{
    Vector2D h = GridSpacing();

    return [this, h](const size_t i, const size_t j) -> Vector2D {
        return m_dataOriginU +
               ElemMul(h, Vector2D{ { static_cast<double>(i),
                                      static_cast<double>(j) } });
    };
}

VectorGrid2::DataPositionFunc FaceCenteredGrid2::VPosition() const
{
    Vector2D h = GridSpacing();

    return [this, h](const size_t i, const size_t j) -> Vector2D {
        return m_dataOriginV +
               ElemMul(h, Vector2D{ { static_cast<double>(i),
                                      static_cast<double>(j) } });
    };
}

Vector2UZ FaceCenteredGrid2::USize() const
{
    return m_dataU.Size();
}

Vector2UZ FaceCenteredGrid2::VSize() const
{
    return m_dataV.Size();
}

Vector2D FaceCenteredGrid2::UOrigin() const
{
    return m_dataOriginU;
}

Vector2D FaceCenteredGrid2::VOrigin() const
{
    return m_dataOriginV;
}

void FaceCenteredGrid2::Fill(const Vector2D& value, ExecutionPolicy policy)
{
    ParallelFor(
        ZERO_SIZE, m_dataU.Width(), ZERO_SIZE, m_dataU.Height(),
        [this, value](const size_t i, const size_t j) {
            m_dataU(i, j) = value.x;
        },
        policy);

    ParallelFor(
        ZERO_SIZE, m_dataV.Width(), ZERO_SIZE, m_dataV.Height(),
        [this, value](const size_t i, const size_t j) {
            m_dataV(i, j) = value.y;
        },
        policy);
}

void FaceCenteredGrid2::Fill(
    const std::function<Vector2D(const Vector2D&)>& func,
    ExecutionPolicy policy)
{
    DataPositionFunc uPos = UPosition();
    ParallelFor(
        ZERO_SIZE, m_dataU.Width(), ZERO_SIZE, m_dataU.Height(),
        [this, &func, &uPos](const size_t i, const size_t j) {
            m_dataU(i, j) = func(uPos(i, j)).x;
        },
        policy);

    DataPositionFunc vPos = VPosition();
    ParallelFor(
        ZERO_SIZE, m_dataV.Width(), ZERO_SIZE, m_dataV.Height(),
        [this, &func, &vPos](const size_t i, const size_t j) {
            m_dataV(i, j) = func(vPos(i, j)).y;
        },
        policy);
}

std::shared_ptr<VectorGrid2> FaceCenteredGrid2::Clone() const
{
    return std::shared_ptr<FaceCenteredGrid2>(
        new FaceCenteredGrid2{ *this },
        [](FaceCenteredGrid2* obj) { delete obj; });
}

void FaceCenteredGrid2::ForEachUIndex(
    const std::function<void(size_t, size_t)>& func) const
{
    ForEachIndex(m_dataU.Size(), func);
}

void FaceCenteredGrid2::ParallelForEachUIndex(
    const std::function<void(size_t, size_t)>& func) const
{
    ParallelForEachIndex(m_dataU.Size(), func);
}

void FaceCenteredGrid2::ForEachVIndex(
    const std::function<void(size_t, size_t)>& func) const
{
    ForEachIndex(m_dataV.Size(), func);
}

void FaceCenteredGrid2::ParallelForEachVIndex(
    const std::function<void(size_t, size_t)>& func) const
{
    ParallelForEachIndex(m_dataV.Size(), func);
}

Vector2D FaceCenteredGrid2::Sample(const Vector2D& x) const
{
    return m_sampler(x);
}

std::function<Vector2D(const Vector2D&)> FaceCenteredGrid2::Sampler() const
{
    return m_sampler;
}

double FaceCenteredGrid2::Divergence(const Vector2D& x) const
{
    size_t i, j;
    double fx, fy;
    const Vector2D cellCenterOrigin = Origin() + 0.5 * GridSpacing();
    const Vector2D normalizedX = ElemDiv((x - cellCenterOrigin), GridSpacing());

    GetBarycentric(normalizedX.x, static_cast<size_t>(Resolution().x), i, fx);
    GetBarycentric(normalizedX.y, static_cast<size_t>(Resolution().y), j, fy);

    std::array<Vector2UZ, 4> indices;
    std::array<double, 4> weights{};

    indices[0] = Vector2UZ{ i, j };
    indices[1] = Vector2UZ{ i + 1, j };
    indices[2] = Vector2UZ{ i, j + 1 };
    indices[3] = Vector2UZ{ i + 1, j + 1 };

    weights[0] = (1.0 - fx) * (1.0 - fy);
    weights[1] = fx * (1.0 - fy);
    weights[2] = (1.0 - fx) * fy;
    weights[3] = fx * fy;

    double result = 0.0;

    for (int n = 0; n < 4; ++n)
    {
        result +=
            weights[n] * DivergenceAtCellCenter(indices[n].x, indices[n].y);
    }

    return result;
}

double FaceCenteredGrid2::Curl(const Vector2D& x) const
{
    size_t i, j;
    double fx, fy;
    const Vector2D cellCenterOrigin = Origin() + 0.5 * GridSpacing();
    const Vector2D normalizedX = ElemDiv((x - cellCenterOrigin), GridSpacing());

    GetBarycentric(normalizedX.x, static_cast<size_t>(Resolution().x), i, fx);
    GetBarycentric(normalizedX.y, static_cast<size_t>(Resolution().y), j, fy);

    std::array<Vector2UZ, 4> indices;
    std::array<double, 4> weights{};

    indices[0] = Vector2UZ{ i, j };
    indices[1] = Vector2UZ{ i + 1, j };
    indices[2] = Vector2UZ{ i, j + 1 };
    indices[3] = Vector2UZ{ i + 1, j + 1 };

    weights[0] = (1.0 - fx) * (1.0 - fy);
    weights[1] = fx * (1.0 - fy);
    weights[2] = (1.0 - fx) * fy;
    weights[3] = fx * fy;

    double result = 0.0;
    for (int n = 0; n < 4; ++n)
    {
        result += weights[n] * CurlAtCellCenter(indices[n].x, indices[n].y);
    }

    return result;
}

void FaceCenteredGrid2::OnResize(const Vector2UZ& resolution,
                                 const Vector2D& gridSpacing,
                                 const Vector2D& origin,
                                 const Vector2D& initialValue)
{
    if (resolution != Vector2UZ{ 0, 0 })
    {
        m_dataU.Resize(resolution + Vector2UZ{ 1, 0 }, initialValue.x);
        m_dataV.Resize(resolution + Vector2UZ{ 0, 1 }, initialValue.y);
    }
    else
    {
        m_dataU.Resize(Vector2UZ{ 0, 0 });
        m_dataV.Resize(Vector2UZ{ 0, 0 });
    }

    m_dataOriginU = origin + 0.5 * Vector2D{ 0.0, gridSpacing.y };
    m_dataOriginV = origin + 0.5 * Vector2D{ gridSpacing.x, 0.0 };

    ResetSampler();
}

void FaceCenteredGrid2::ResetSampler()
{
    LinearArraySampler2<double> uSampler{ m_dataU, GridSpacing(),
                                          m_dataOriginU };
    LinearArraySampler2<double> vSampler{ m_dataV, GridSpacing(),
                                          m_dataOriginV };

    m_uLinearSampler = uSampler;
    m_vLinearSampler = vSampler;

    m_sampler = [uSampler, vSampler](const Vector2D& x) -> Vector2D {
        const double u = uSampler(x);
        const double v = vSampler(x);
        return Vector2D{ u, v };
    };
}

FaceCenteredGrid2::Builder FaceCenteredGrid2::GetBuilder()
{
    return Builder{};
}

void FaceCenteredGrid2::GetData(std::vector<double>* data) const
{
    const size_t size = USize().x * USize().y + VSize().x * VSize().y;
    data->resize(size);
    size_t cnt = 0;

    std::for_each(m_dataU.begin(), m_dataU.end(),
                  [&](double value) { (*data)[cnt++] = value; });
    std::for_each(m_dataV.begin(), m_dataV.end(),
                  [&](double value) { (*data)[cnt++] = value; });
}

void FaceCenteredGrid2::SetData(const std::vector<double>& data)
{
    assert(USize().x * USize().y + VSize().x * VSize().y == data.size());

    size_t cnt = 0;

    ForEachIndex(m_dataU.Size(),
                 [&](size_t i, size_t j) { m_dataU(i, j) = data[cnt++]; });
    ForEachIndex(m_dataV.Size(),
                 [&](size_t i, size_t j) { m_dataV(i, j) = data[cnt++]; });
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::WithResolution(
    const Vector2UZ& resolution)
{
    m_resolution = resolution;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::WithResolution(
    size_t resolutionX, size_t resolutionY)
{
    m_resolution.x = resolutionX;
    m_resolution.y = resolutionY;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::WithGridSpacing(
    const Vector2D& gridSpacing)
{
    m_gridSpacing = gridSpacing;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::WithGridSpacing(
    double gridSpacingX, double gridSpacingY)
{
    m_gridSpacing.x = gridSpacingX;
    m_gridSpacing.y = gridSpacingY;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::WithOrigin(
    const Vector2D& gridOrigin)
{
    m_gridOrigin = gridOrigin;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::WithOrigin(
    double gridOriginX, double gridOriginY)
{
    m_gridOrigin.x = gridOriginX;
    m_gridOrigin.y = gridOriginY;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::WithInitialValue(
    const Vector2D& initialVal)
{
    m_initialVal = initialVal;
    return *this;
}

FaceCenteredGrid2::Builder& FaceCenteredGrid2::Builder::WithInitialValue(
    double initialValX, double initialValY)
{
    m_initialVal.x = initialValX;
    m_initialVal.y = initialValY;
    return *this;
}

FaceCenteredGrid2 FaceCenteredGrid2::Builder::Build() const
{
    return FaceCenteredGrid2{ m_resolution, m_gridSpacing, m_gridOrigin,
                              m_initialVal };
}

FaceCenteredGrid2Ptr FaceCenteredGrid2::Builder::MakeShared() const
{
    return std::shared_ptr<FaceCenteredGrid2>(
        new FaceCenteredGrid2{ m_resolution, m_gridSpacing, m_gridOrigin,
                               m_initialVal },
        [](FaceCenteredGrid2* obj) { delete obj; });
}

VectorGrid2Ptr FaceCenteredGrid2::Builder::Build(
    const Vector2UZ& resolution, const Vector2D& gridSpacing,
    const Vector2D& gridOrigin, const Vector2D& initialVal) const
{
    return std::shared_ptr<FaceCenteredGrid2>(
        new FaceCenteredGrid2{ resolution, gridSpacing, gridOrigin,
                               initialVal },
        [](FaceCenteredGrid2* obj) { delete obj; });
}
}  // namespace CubbyFlow