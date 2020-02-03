// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Field/CustomScalarField2.hpp>

namespace CubbyFlow
{
CustomScalarField2::CustomScalarField2(
    const std::function<double(const Vector2D&)>& customFunction,
    double derivativeResolution)
    : m_customFunction(customFunction), m_resolution(derivativeResolution)
{
    // Do nothing
}

CustomScalarField2::CustomScalarField2(
    const std::function<double(const Vector2D&)>& customFunction,
    const std::function<Vector2D(const Vector2D&)>& customGradientFunction,
    double derivativeResolution)
    : m_customFunction(customFunction),
      m_customGradientFunction(customGradientFunction),
      m_resolution(derivativeResolution)
{
    // Do nothing
}

CustomScalarField2::CustomScalarField2(
    const std::function<double(const Vector2D&)>& customFunction,
    const std::function<Vector2D(const Vector2D&)>& customGradientFunction,
    const std::function<double(const Vector2D&)>& customLaplacianFunction)
    : m_customFunction(customFunction),
      m_customGradientFunction(customGradientFunction),
      m_customLaplacianFunction(customLaplacianFunction),
      m_resolution(1e-3)
{
    // Do nothing
}

double CustomScalarField2::Sample(const Vector2D& x) const
{
    return m_customFunction(x);
}

std::function<double(const Vector2D&)> CustomScalarField2::Sampler() const
{
    return m_customFunction;
}

Vector2D CustomScalarField2::Gradient(const Vector2D& x) const
{
    if (m_customGradientFunction)
    {
        return m_customGradientFunction(x);
    }

    double left = m_customFunction(x - Vector2D(0.5 * m_resolution, 0.0));
    double right = m_customFunction(x + Vector2D(0.5 * m_resolution, 0.0));
    double bottom = m_customFunction(x - Vector2D(0.0, 0.5 * m_resolution));
    double top = m_customFunction(x + Vector2D(0.0, 0.5 * m_resolution));

    return Vector2D((right - left) / m_resolution,
                    (top - bottom) / m_resolution);
}

double CustomScalarField2::Laplacian(const Vector2D& x) const
{
    if (m_customLaplacianFunction)
    {
        return m_customLaplacianFunction(x);
    }

    double center = m_customFunction(x);
    double left = m_customFunction(x - Vector2D(0.5 * m_resolution, 0.0));
    double right = m_customFunction(x + Vector2D(0.5 * m_resolution, 0.0));
    double bottom = m_customFunction(x - Vector2D(0.0, 0.5 * m_resolution));
    double top = m_customFunction(x + Vector2D(0.0, 0.5 * m_resolution));

    return (left + right + bottom + top - 4.0 * center) /
           (m_resolution * m_resolution);
}

CustomScalarField2::Builder CustomScalarField2::GetBuilder()
{
    return Builder();
}

CustomScalarField2::Builder& CustomScalarField2::Builder::WithFunction(
    const std::function<double(const Vector2D&)>& func)
{
    m_customFunction = func;
    return *this;
}

CustomScalarField2::Builder& CustomScalarField2::Builder::WithGradientFunction(
    const std::function<Vector2D(const Vector2D&)>& func)
{
    m_customGradientFunction = func;
    return *this;
}

CustomScalarField2::Builder& CustomScalarField2::Builder::WithLaplacianFunction(
    const std::function<double(const Vector2D&)>& func)
{
    m_customLaplacianFunction = func;
    return *this;
}

CustomScalarField2::Builder&
CustomScalarField2::Builder::WithDerivativeResolution(double resolution)
{
    m_resolution = resolution;
    return *this;
}

CustomScalarField2 CustomScalarField2::Builder::Build() const
{
    if (m_customLaplacianFunction)
    {
        return CustomScalarField2(m_customFunction, m_customGradientFunction,
                                  m_customLaplacianFunction);
    }

    return CustomScalarField2(m_customFunction, m_customGradientFunction,
                              m_resolution);
}

CustomScalarField2Ptr CustomScalarField2::Builder::MakeShared() const
{
    if (m_customLaplacianFunction)
    {
        return std::shared_ptr<CustomScalarField2>(
            new CustomScalarField2(m_customFunction, m_customGradientFunction,
                                   m_customLaplacianFunction),
            [](CustomScalarField2* obj) { delete obj; });
    }

    return std::shared_ptr<CustomScalarField2>(
        new CustomScalarField2(m_customFunction, m_customGradientFunction,
                               m_resolution),
        [](CustomScalarField2* obj) { delete obj; });
}
}  // namespace CubbyFlow