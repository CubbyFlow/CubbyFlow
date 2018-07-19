/*************************************************************************
> File Name: VectorField3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 3-D vector field.
> Created Time: 2017/03/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_VECTOR_FIELD3_H
#define CUBBYFLOW_VECTOR_FIELD3_H

#include <Field/Field3.h>
#include <Vector/Vector3.h>

#include <functional>
#include <memory>

namespace CubbyFlow
{
	//! Abstract base class for 3-D vector field.
	class VectorField3 : public Field3
	{
	public:
		//! Default constructor.
		VectorField3();

		//! Default destructor.
		virtual ~VectorField3();

		//! Returns sampled value at given position \p x.
		virtual Vector3D Sample(const Vector3D& x) const = 0;

		//! Returns divergence at given position \p x.
		virtual double Divergence(const Vector3D& x) const;

		//! Returns curl at given position \p x.
		virtual Vector3D Curl(const Vector3D& x) const;

		//! Returns sampler function object.
		virtual std::function<Vector3D(const Vector3D&)> Sampler() const;
	};

	//! Shared pointer for the VectorField3 type.
	using VectorField3Ptr = std::shared_ptr<VectorField3>;
}

#endif