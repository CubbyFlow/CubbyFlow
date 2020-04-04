// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Field/CustomVectorField2.hpp>

namespace CubbyFlow
{
CustomVectorField2::CustomVectorField2(
    const std::function<Vector2D(const Vector2D&)>& customFunction,
    double derivativeResolution)
    : m_customFunction(customFunction), m_resolution(derivativeResolution)
{
    // Do nothing
}

CustomVectorField2::CustomVectorField2(
    const std::function<Vector2D(const Vector2D&)>& customFunction,
    const std::function<double(const Vector2D&)>& customDivergenceFunction,
    double derivativeResolution)
    : m_customFunction(customFunction),
      m_customDivergenceFunction(customDivergenceFunction),
      m_resolution(derivativeResolution)
{
    // Do nothing
}

CustomVectorField2::CustomVectorField2(
    const std::function<Vector2D(const Vector2D&)>& customFunction,
    const std::function<double(const Vector2D&)>& customDivergenceFunction,
    const std::function<double(const Vector2D&)>& customCurlFunction)
    : m_customFunction(customFunction),
      m_customDivergenceFunction(customDivergenceFunction),
      m_customCurlFunction(customCurlFunction)
{
    // Do nothing
}

Vector2D CustomVectorField2::Sample(const Vector2D& x) const
{
    return m_customFunction(x);
}

std::function<Vector2D(const Vector2D&)> CustomVectorField2::Sampler() const
{
    return m_customFunction;
}

double CustomVectorField2::Divergence(const Vector2D& x) const
{
    if (m_customDivergenceFunction)
    {
        return m_customDivergenceFunction(x);
    }

    double left = m_customFunction(x - Vector2D(0.5 * m_resolution, 0.0)).x;
    double right = m_customFunction(x + Vector2D(0.5 * m_resolution, 0.0)).x;
    double bottom = m_customFunction(x - Vector2D(0.0, 0.5 * m_resolution)).y;
    double top = m_customFunction(x + Vector2D(0.0, 0.5 * m_resolution)).y;

    return (right - left + top - bottom) / m_resolution;
}

double CustomVectorField2::Curl(const Vector2D& x) const
{
    if (m_customCurlFunction)
    {
        return m_customCurlFunction(x);
    }

    double left = m_customFunction(x - Vector2D(0.5 * m_resolution, 0.0)).y;
    double right = m_customFunction(x + Vector2D(0.5 * m_resolution, 0.0)).y;
    double bottom = m_customFunction(x - Vector2D(0.0, 0.5 * m_resolution)).x;
    double top = m_customFunction(x + Vector2D(0.0, 0.5 * m_resolution)).x;

    return (top - bottom - right + left) / m_resolution;
}

CustomVectorField2::Builder CustomVectorField2::GetBuilder()
{
    return Builder();
}

CustomVectorField2::Builder& CustomVectorField2::Builder::WithFunction(
    const std::function<Vector2D(const Vector2D&)>& func)
{
    m_customFunction = func;
    return *this;
}

CustomVectorField2::Builder&
CustomVectorField2::Builder::WithDivergenceFunction(
    const std::function<double(const Vector2D&)>& func)
{
    m_customDivergenceFunction = func;
    return *this;
}

CustomVectorField2::Builder& CustomVectorField2::Builder::WithCurlFunction(
    const std::function<double(const Vector2D&)>& func)
{
    m_customCurlFunction = func;
    return *this;
}

CustomVectorField2::Builder&
CustomVectorField2::Builder::WithDerivativeResolution(double resolution)
{
    m_resolution = resolution;
    return *this;
}

CustomVectorField2 CustomVectorField2::Builder::Build() const
{
    if (m_customCurlFunction)
    {
        return CustomVectorField2(m_customFunction, m_customDivergenceFunction,
                                  m_customCurlFunction);
    }

    return CustomVectorField2(m_customFunction, m_customDivergenceFunction,
                              m_resolution);
}

CustomVectorField2Ptr CustomVectorField2::Builder::MakeShared() const
{
    if (m_customCurlFunction)
    {
        return std::shared_ptr<CustomVectorField2>(
            new CustomVectorField2(m_customFunction, m_customDivergenceFunction,
                                   m_customCurlFunction),
            [](CustomVectorField2* obj) { delete obj; });
    }

    return std::shared_ptr<CustomVectorField2>(
        new CustomVectorField2(m_customFunction, m_customDivergenceFunction,
                               m_resolution),
        [](CustomVectorField2* obj) { delete obj; });
}
}  // namespace CubbyFlow