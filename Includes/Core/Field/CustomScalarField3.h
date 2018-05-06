/*************************************************************************
> File Name: CustomScalarField3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D scalar field with custom field function.
> Created Time: 2017/03/15
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CUSTOM_SCALAR_FIELD3_H
#define CUBBYFLOW_CUSTOM_SCALAR_FIELD3_H

#include <Core/Field/ScalarField3.h>

namespace CubbyFlow
{
	//! 3-D scalar field with custom field function.
	class CustomScalarField3 final : public ScalarField3
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
		CustomScalarField3(
			const std::function<double(const Vector3D&)>& customFunction,
			double derivativeResolution = 1e-3);

		//!
		//! \brief Constructs a field with given field and gradient function.
		//!
		//! This constructor creates a field with user-provided field and gradient
		//! function objects. To compute Laplacian, finite differencing is used.
		//! Thus, the differencing resolution also can be provided as the last
		//! parameter.
		//!
		CustomScalarField3(
			const std::function<double(const Vector3D&)>& customFunction,
			const std::function<Vector3D(const Vector3D&)>& customGradientFunction,
			double derivativeResolution = 1e-3);

		//! Constructs a field with given field, gradient, and Laplacian function.
		CustomScalarField3(
			const std::function<double(const Vector3D&)>& customFunction,
			const std::function<Vector3D(const Vector3D&)>& customGradientFunction,
			const std::function<double(const Vector3D&)>& customLaplacianFunction);

		//! Returns the sampled value at given position \p x.
		double Sample(const Vector3D& x) const override;

		//! Returns the sampler function.
		std::function<double(const Vector3D&)> Sampler() const override;

		//! Returns the gradient vector at given position \p x.
		Vector3D Gradient(const Vector3D& x) const override;

		//! Returns the Laplacian at given position \p x.
		double Laplacian(const Vector3D& x) const override;

		//! Returns builder fox CustomScalarField3.
		static Builder GetBuilder();

	private:
		std::function<double(const Vector3D&)> m_customFunction;
		std::function<Vector3D(const Vector3D&)> m_customGradientFunction;
		std::function<double(const Vector3D&)> m_customLaplacianFunction;
		double m_resolution = 1e-3;
	};

	//! Shared pointer type for the CustomScalarField3.
	using CustomScalarField3Ptr = std::shared_ptr<CustomScalarField3>;

	//!
	//! \brief Front-end to create CustomScalarField3 objects step by step.
	//!
	class CustomScalarField3::Builder final
	{
	public:
		//! Returns builder with field function.
		Builder& WithFunction(const std::function<double(const Vector3D&)>& func);

		//! Returns builder with divergence function.
		Builder& WithGradientFunction(const std::function<Vector3D(const Vector3D&)>& func);

		//! Returns builder with curl function.
		Builder& WithLaplacianFunction(const std::function<double(const Vector3D&)>& func);

		//! Returns builder with derivative resolution.
		Builder& WithDerivativeResolution(double resolution);

		//! Builds CustomScalarField3.
		CustomScalarField3 Build() const;

		//! Builds shared pointer of CustomScalarField3 instance.
		CustomScalarField3Ptr MakeShared() const;

	private:
		double m_resolution = 1e-3;
		std::function<double(const Vector3D&)> m_customFunction;
		std::function<Vector3D(const Vector3D&)> m_customGradientFunction;
		std::function<double(const Vector3D&)> m_customLaplacianFunction;
	};
}

#endif