/*************************************************************************
> File Name: ScalarField2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 2-D scalar field.
> Created Time: 2017/03/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SCALAR_FIELD2_H
#define CUBBYFLOW_SCALAR_FIELD2_H

#include <Field/Field2.h>
#include <Vector/Vector2.h>

#include <functional>
#include <memory>

namespace CubbyFlow
{
	//! Abstract base class for 2-D scalar field.
	class ScalarField2 : public Field2
	{
	public:
		//! Default constructor.
		ScalarField2();

		//! Default destructor.
		virtual ~ScalarField2();

		//! Returns sampled value at given position \p x.
		virtual double Sample(const Vector2D& x) const = 0;

		//! Returns gradient vector at given position \p x.
		virtual Vector2D Gradient(const Vector2D& x) const;

		//! Returns Laplacian at given position \p x.
		virtual double Laplacian(const Vector2D& x) const;

		//! Returns sampler function object.
		virtual std::function<double(const Vector2D&)> Sampler() const;
	};

	//! Shared pointer for the ScalarField2 type.
	using ScalarField2Ptr = std::shared_ptr<ScalarField2>;
}

#endif