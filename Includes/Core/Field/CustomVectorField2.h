/*************************************************************************
> File Name: CustomVectorField2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D vector field with custom field function.
> Created Time: 2017/03/15
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CUSTOM_VECTOR_FIELD2_H
#define CUBBYFLOW_CUSTOM_VECTOR_FIELD2_H

#include <Core/Field/VectorField2.h>

namespace CubbyFlow
{
	//! 2-D vector field with custom field function.
	class CustomVectorField2 final : public VectorField2
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
		CustomVectorField2(
			const std::function<Vector2D(const Vector2D&)>& customFunction,
			double derivativeResolution = 1e-3);

		//!
		//! \brief Constructs a field with given field and gradient function.
		//!
		//! This constructor creates a field with user-provided field and gradient
		//! function objects. To compute Laplacian, finite differencing is used.
		//! Thus, the differencing resolution also can be provided as the last
		//! parameter.
		//!
		CustomVectorField2(
			const std::function<Vector2D(const Vector2D&)>& customFunction,
			const std::function<double(const Vector2D&)>& customDivergenceFunction,
			double derivativeResolution = 1e-3);

		//! Constructs a field with given field, gradient, and Laplacian function.
		CustomVectorField2(
			const std::function<Vector2D(const Vector2D&)>& customFunction,
			const std::function<double(const Vector2D&)>& customDivergenceFunction,
			const std::function<double(const Vector2D&)>& customCurlFunction);

		//! Returns the sampled value at given position \p x.
		Vector2D Sample(const Vector2D& x) const override;

		//! Returns the divergence at given position \p x.
		double Divergence(const Vector2D& x) const override;

		//! Returns the curl at given position \p x.
		double Curl(const Vector2D& x) const override;

		//! Returns the sampler function.
		std::function<Vector2D(const Vector2D&)> Sampler() const override;

		//! Returns builder fox CustomVectorField2.
		static Builder GetBuilder();

	private:
		std::function<Vector2D(const Vector2D&)> m_customFunction;
		std::function<double(const Vector2D&)> m_customDivergenceFunction;
		std::function<double(const Vector2D&)> m_customCurlFunction;
		double m_resolution = 1e-3;
	};

	//! Shared pointer type for the CustomVectorField2.
	using CustomVectorField2Ptr = std::shared_ptr<CustomVectorField2>;

	//!
	//! \brief Front-end to create CustomVectorField2 objects step by step.
	//!
	class CustomVectorField2::Builder final
	{
	public:
		//! Returns builder with field function.
		Builder& WithFunction(const std::function<Vector2D(const Vector2D&)>& func);

		//! Returns builder with divergence function.
		Builder& WithDivergenceFunction(const std::function<double(const Vector2D&)>& func);

		//! Returns builder with curl function.
		Builder& WithCurlFunction(const std::function<double(const Vector2D&)>& func);

		//! Returns builder with derivative resolution.
		Builder& WithDerivativeResolution(double resolution);

		//! Builds CustomVectorField2.
		CustomVectorField2 Build() const;

		//! Builds shared pointer of CustomVectorField2 instance.
		CustomVectorField2Ptr MakeShared() const;

	private:
		double m_resolution = 1e-3;
		std::function<Vector2D(const Vector2D&)> m_customFunction;
		std::function<double(const Vector2D&)> m_customDivergenceFunction;
		std::function<double(const Vector2D&)> m_customCurlFunction;
	};
}

#endif