/*************************************************************************
> File Name: VectorField2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Abstract base class for 2-D vector field.
> Created Time: 2017/03/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_VECTOR_FIELD2_H
#define CUBBYFLOW_VECTOR_FIELD2_H

#include <Core/Field/Field2.h>
#include <Core/Vector/Vector2.h>

#include <functional>
#include <memory>

namespace CubbyFlow
{
	//! Abstract base class for 2-D vector field.
	class VectorField2 : public Field2
	{
	public:
		//! Default constructor.
		VectorField2();

		//! Default destructor.
		virtual ~VectorField2();

		//! Returns sampled value at given position \p x.
		virtual Vector2D Sample(const Vector2D& x) const = 0;

		//! Returns divergence at given position \p x.
		virtual double Divergence(const Vector2D& x) const;

		//! Returns curl at given position \p x.
		virtual double Curl(const Vector2D& x) const;

		//! Returns sampler function object.
		virtual std::function<Vector2D(const Vector2D&)> Sampler() const;
	};

	//! Shared pointer for the VectorField2 type.
	using VectorField2Ptr = std::shared_ptr<VectorField2>;
}

#endif