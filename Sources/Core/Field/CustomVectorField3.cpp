// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Field/CustomVectorField3.hpp>

#include <utility>

namespace CubbyFlow
{
CustomVectorField3::CustomVectorField3(
    std::function<Vector3D(const Vector3D&)> customFunction,
    double derivativeResolution)
    : m_customFunction(std::move(customFunction)),
      m_resolution(derivativeResolution)
{
    // Do nothing
}

CustomVectorField3::CustomVectorField3(
    std::function<Vector3D(const Vector3D&)> customFunction,
    std::function<double(const Vector3D&)> customDivergenceFunction,
    double derivativeResolution)
    : m_customFunction(std::move(customFunction)),
      m_customDivergenceFunction(std::move(customDivergenceFunction)),
      m_resolution(derivativeResolution)
{
    // Do nothing
}

CustomVectorField3::CustomVectorField3(
    std::function<Vector3D(const Vector3D&)> customFunction,
    std::function<double(const Vector3D&)> customDivergenceFunction,
    std::function<Vector3D(const Vector3D&)> customCurlFunction)
    : m_customFunction(std::move(customFunction)),
      m_customDivergenceFunction(std::move(customDivergenceFunction)),
      m_customCurlFunction(std::move(customCurlFunction))
{
    // Do nothing
}

Vector3D CustomVectorField3::Sample(const Vector3D& x) const
{
    return m_customFunction(x);
}

std::function<Vector3D(const Vector3D&)> CustomVectorField3::Sampler() const
{
    return m_customFunction;
}

double CustomVectorField3::Divergence(const Vector3D& x) const
{
    if (m_customDivergenceFunction)
    {
        return m_customDivergenceFunction(x);
    }

    const double left =
        m_customFunction(x - Vector3D{ 0.5 * m_resolution, 0.0, 0.0 }).x;
    const double right =
        m_customFunction(x + Vector3D{ 0.5 * m_resolution, 0.0, 0.0 }).x;
    const double bottom =
        m_customFunction(x - Vector3D{ 0.0, 0.5 * m_resolution, 0.0 }).y;
    const double top =
        m_customFunction(x + Vector3D{ 0.0, 0.5 * m_resolution, 0.0 }).y;
    const double back =
        m_customFunction(x - Vector3D{ 0.0, 0.0, 0.5 * m_resolution }).z;
    const double front =
        m_customFunction(x + Vector3D{ 0.0, 0.0, 0.5 * m_resolution }).z;

    return (right - left + top - bottom + front - back) / m_resolution;
}

Vector3D CustomVectorField3::Curl(const Vector3D& x) const
{
    if (m_customCurlFunction)
    {
        return m_customCurlFunction(x);
    }

    const Vector3D left =
        m_customFunction(x - Vector3D{ 0.5 * m_resolution, 0.0, 0.0 });
    const Vector3D right =
        m_customFunction(x + Vector3D{ 0.5 * m_resolution, 0.0, 0.0 });
    const Vector3D bottom =
        m_customFunction(x - Vector3D{ 0.0, 0.5 * m_resolution, 0.0 });
    const Vector3D top =
        m_customFunction(x + Vector3D{ 0.0, 0.5 * m_resolution, 0.0 });
    const Vector3D back =
        m_customFunction(x - Vector3D{ 0.0, 0.0, 0.5 * m_resolution });
    const Vector3D front =
        m_customFunction(x + Vector3D{ 0.0, 0.0, 0.5 * m_resolution });

    const double Fx_ym = bottom.x;
    const double Fx_yp = top.x;
    const double Fx_zm = back.x;
    const double Fx_zp = front.x;

    const double Fy_xm = left.y;
    const double Fy_xp = right.y;
    const double Fy_zm = back.y;
    const double Fy_zp = front.y;

    const double Fz_xm = left.z;
    const double Fz_xp = right.z;
    const double Fz_ym = bottom.z;
    const double Fz_yp = top.z;

    return Vector3D{ (Fz_yp - Fz_ym) - (Fy_zp - Fy_zm),
                     (Fx_zp - Fx_zm) - (Fz_xp - Fz_xm),
                     (Fy_xp - Fy_xm) - (Fx_yp - Fx_ym) } /
           m_resolution;
}

CustomVectorField3::Builder CustomVectorField3::GetBuilder()
{
    return Builder{};
}

CustomVectorField3::Builder& CustomVectorField3::Builder::WithFunction(
    const std::function<Vector3D(const Vector3D&)>& func)
{
    m_customFunction = func;
    return *this;
}

CustomVectorField3::Builder&
CustomVectorField3::Builder::WithDivergenceFunction(
    const std::function<double(const Vector3D&)>& func)
{
    m_customDivergenceFunction = func;
    return *this;
}

CustomVectorField3::Builder& CustomVectorField3::Builder::WithCurlFunction(
    const std::function<Vector3D(const Vector3D&)>& func)
{
    m_customCurlFunction = func;
    return *this;
}

CustomVectorField3::Builder&
CustomVectorField3::Builder::WithDerivativeResolution(double resolution)
{
    m_resolution = resolution;
    return *this;
}

CustomVectorField3 CustomVectorField3::Builder::Build() const
{
    if (m_customCurlFunction)
    {
        return CustomVectorField3{ m_customFunction, m_customDivergenceFunction,
                                   m_customCurlFunction };
    }

    return CustomVectorField3{ m_customFunction, m_customDivergenceFunction,
                               m_resolution };
}

CustomVectorField3Ptr CustomVectorField3::Builder::MakeShared() const
{
    if (m_customCurlFunction)
    {
        return std::shared_ptr<CustomVectorField3>(
            new CustomVectorField3{ m_customFunction,
                                    m_customDivergenceFunction,
                                    m_customCurlFunction },
            [](CustomVectorField3* obj) { delete obj; });
    }

    return std::shared_ptr<CustomVectorField3>(
        new CustomVectorField3{ m_customFunction, m_customDivergenceFunction,
                                m_resolution },
        [](CustomVectorField3* obj) { delete obj; });
}
}  // namespace CubbyFlow