// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUSTOM_SCALAR_FIELD2_HPP
#define CUBBYFLOW_CUSTOM_SCALAR_FIELD2_HPP

#include <Core/Field/ScalarField2.hpp>

namespace CubbyFlow
{
//! 2-D scalar field with custom field function.
class CustomScalarField2 final : public ScalarField2
{
 public:
    class Builder;

    //!
    //! \brief Constructs a field with given function.
    //!
    //! This constructor creates a field with user-provided function object.
    //! To compute derivatives, such as gradient and Laplacian, finite
    //! differencing is used. Thus, the differencing resolution also can be
    //! provided as the last parameter.
    //!
    CustomScalarField2(
        const std::function<double(const Vector2D&)>& customFunction,
        double derivativeResolution = 1e-3);

    //!
    //! \brief Constructs a field with given field and gradient function.
    //!
    //! This constructor creates a field with user-provided field and gradient
    //! function objects. To compute Laplacian, finite differencing is used.
    //! Thus, the differencing resolution also can be provided as the last
    //! parameter.
    //!
    CustomScalarField2(
        const std::function<double(const Vector2D&)>& customFunction,
        const std::function<Vector2D(const Vector2D&)>& customGradientFunction,
        double derivativeResolution = 1e-3);

    //! Constructs a field with given field, gradient, and Laplacian function.
    CustomScalarField2(
        const std::function<double(const Vector2D&)>& customFunction,
        const std::function<Vector2D(const Vector2D&)>& customGradientFunction,
        const std::function<double(const Vector2D&)>& customLaplacianFunction);

    //! Returns the sampled value at given position \p x.
    double Sample(const Vector2D& x) const override;

    //! Returns the sampler function.
    std::function<double(const Vector2D&)> Sampler() const override;

    //! Returns the gradient vector at given position \p x.
    Vector2D Gradient(const Vector2D& x) const override;

    //! Returns the Laplacian at given position \p x.
    double Laplacian(const Vector2D& x) const override;

    //! Returns builder for CustomScalarField2.
    static Builder GetBuilder();

 private:
    std::function<double(const Vector2D&)> m_customFunction;
    std::function<Vector2D(const Vector2D&)> m_customGradientFunction;
    std::function<double(const Vector2D&)> m_customLaplacianFunction;
    double m_resolution = 1e-3;
};

//! Shared pointer type for the CustomScalarField2.
using CustomScalarField2Ptr = std::shared_ptr<CustomScalarField2>;

//!
//! \brief Front-end to create CustomScalarField2 objects step by step.
//!
class CustomScalarField2::Builder final
{
 public:
    //! Returns builder with field function.
    Builder& WithFunction(const std::function<double(const Vector2D&)>& func);

    //! Returns builder with divergence function.
    Builder& WithGradientFunction(
        const std::function<Vector2D(const Vector2D&)>& func);

    //! Returns builder with curl function.
    Builder& WithLaplacianFunction(
        const std::function<double(const Vector2D&)>& func);

    //! Returns builder with derivative resolution.
    Builder& WithDerivativeResolution(double resolution);

    //! Builds CustomScalarField2.
    CustomScalarField2 Build() const;

    //! Builds shared pointer of CustomScalarField2 instance.
    CustomScalarField2Ptr MakeShared() const;

 private:
    double m_resolution = 1e-3;
    std::function<double(const Vector2D&)> m_customFunction;
    std::function<Vector2D(const Vector2D&)> m_customGradientFunction;
    std::function<double(const Vector2D&)> m_customLaplacianFunction;
};
}  // namespace CubbyFlow

#endif