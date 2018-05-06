/*************************************************************************
> File Name: CustomVectorField3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D vector field with custom field function.
> Created Time: 2017/03/15
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CUSTOM_VECTOR_FIELD3_H
#define CUBBYFLOW_CUSTOM_VECTOR_FIELD3_H

#include <Core/Field/VectorField3.h>

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
		CustomVectorField3(
			const std::function<Vector3D(const Vector3D&)>& customFunction,
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
			const std::function<Vector3D(const Vector3D&)>& customFunction,
			const std::function<double(const Vector3D&)>& customDivergenceFunction,
			double derivativeResolution = 1e-3);

		//! Constructs a field with given field, gradient, and Laplacian function.
		CustomVectorField3(
			const std::function<Vector3D(const Vector3D&)>& customFunction,
			const std::function<double(const Vector3D&)>& customDivergenceFunction,
			const std::function<Vector3D(const Vector3D&)>& customCurlFunction);

		//! Returns the sampled value at given position \p x.
		Vector3D Sample(const Vector3D& x) const override;

		//! Returns the divergence at given position \p x.
		double Divergence(const Vector3D& x) const override;

		//! Returns the curl at given position \p x.
		Vector3D Curl(const Vector3D& x) const override;

		//! Returns the sampler function.
		std::function<Vector3D(const Vector3D&)> Sampler() const override;

		//! Returns builder fox CustomVectorField2.
		static Builder GetBuilder();

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
		Builder& WithFunction(const std::function<Vector3D(const Vector3D&)>& func);

		//! Returns builder with divergence function.
		Builder& WithDivergenceFunction(const std::function<double(const Vector3D&)>& func);

		//! Returns builder with curl function.
		Builder& WithCurlFunction(const std::function<Vector3D(const Vector3D&)>& func);

		//! Returns builder with derivative resolution.
		Builder& WithDerivativeResolution(double resolution);

		//! Builds CustomVectorField3.
		CustomVectorField3 Build() const;

		//! Builds shared pointer of CustomVectorField3 instance.
		CustomVectorField3Ptr MakeShared() const;

	private:
		double m_resolution = 1e-3;
		std::function<Vector3D(const Vector3D&)> m_customFunction;
		std::function<double(const Vector3D&)> m_customDivergenceFunction;
		std::function<Vector3D(const Vector3D&)> m_customCurlFunction;
	};
}

#endif