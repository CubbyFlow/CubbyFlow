// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUSTOM_VECTOR_FIELD3_HPP
#define CUBBYFLOW_CUSTOM_VECTOR_FIELD3_HPP

#include <Core/Field/VectorField.hpp>

namespace CubbyFlow
{
//! 3-D vector field with custom field function.
class CustomVectorField3 final : public VectorField3
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
    explicit CustomVectorField3(
        std::function<Vector3D(const Vector3D&)> customFunction,
        double derivativeResolution = 1e-3);

    //!
    //! \brief Constructs a field with given field and gradient function.
    //!
    //! This constructor creates a field with user-provided field and gradient
    //! function objects. To compute Laplacian, finite differencing is used.
    //! Thus, the differencing resolution also can be provided as the last
    //! parameter.
    //!
    CustomVectorField3(
        std::function<Vector3D(const Vector3D&)> customFunction,
        std::function<double(const Vector3D&)> customDivergenceFunction,
        double derivativeResolution = 1e-3);

    //! Constructs a field with given field, gradient, and Laplacian function.
    CustomVectorField3(
        std::function<Vector3D(const Vector3D&)> customFunction,
        std::function<double(const Vector3D&)> customDivergenceFunction,
        std::function<Vector3D(const Vector3D&)> customCurlFunction);

    //! Returns the sampled value at given position \p x.
    [[nodiscard]] Vector3D Sample(const Vector3D& x) const override;

    //! Returns the divergence at given position \p x.
    [[nodiscard]] double Divergence(const Vector3D& x) const override;

    //! Returns the curl at given position \p x.
    [[nodiscard]] Vector3D Curl(const Vector3D& x) const override;

    //! Returns the sampler function.
    [[nodiscard]] std::function<Vector3D(const Vector3D&)> Sampler()
        const override;

    //! Returns builder fox CustomVectorField2.
    [[nodiscard]] static Builder GetBuilder();

 private:
    std::function<Vector3D(const Vector3D&)> m_customFunction;
    std::function<double(const Vector3D&)> m_customDivergenceFunction;
    std::function<Vector3D(const Vector3D&)> m_customCurlFunction;
    double m_resolution = 1e-3;
};

//! Shared pointer type for the CustomVectorField3.
using CustomVectorField3Ptr = std::shared_ptr<CustomVectorField3>;

//!
//! \brief Front-end to create CustomVectorField3 objects step by step.
//!
class CustomVectorField3::Builder final
{
 public:
    //! Returns builder with field function.
    [[nodiscard]] Builder& WithFunction(
        const std::function<Vector3D(const Vector3D&)>& func);

    //! Returns builder with divergence function.
    [[nodiscard]] Builder& WithDivergenceFunction(
        const std::function<double(const Vector3D&)>& func);

    //! Returns builder with curl function.
    [[nodiscard]] Builder& WithCurlFunction(
        const std::function<Vector3D(const Vector3D&)>& func);

    //! Returns builder with derivative resolution.
    [[nodiscard]] Builder& WithDerivativeResolution(double resolution);

    //! Builds CustomVectorField3.
    [[nodiscard]] CustomVectorField3 Build() const;

    //! Builds shared pointer of CustomVectorField3 instance.
    [[nodiscard]] CustomVectorField3Ptr MakeShared() const;

 private:
    double m_resolution = 1e-3;
    std::function<Vector3D(const Vector3D&)> m_customFunction;
    std::function<double(const Vector3D&)> m_customDivergenceFunction;
    std::function<Vector3D(const Vector3D&)> m_customCurlFunction;
};
}  // namespace CubbyFlow

#endif