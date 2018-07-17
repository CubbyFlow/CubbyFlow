/*************************************************************************
> File Name: ImplicitSurface3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 3-D implicit surface.
> Created Time: 2017/04/18
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_IMPLICIT_SURFACE3_H
#define CUBBYFLOW_IMPLICIT_SURFACE3_H

#include <Surface/Surface3.h>

namespace CubbyFlow
{
	//! Abstract base class for 3-D implicit surface.
	class ImplicitSurface3 : public Surface3
	{
	public:
		//! Default constructor.
		ImplicitSurface3(
			const Transform3& transform = Transform3(),
			bool isNormalFlipped = false);
		
		//! Copy constructor.
		ImplicitSurface3(const ImplicitSurface3& other);

		//! Default destructor.
		virtual ~ImplicitSurface3();

		//! Returns signed distance from the given point \p otherPoint.
		double SignedDistance(const Vector3D& otherPoint) const;
		
	protected:
		//! Returns signed distance from the given point \p otherPoint in local space.
		virtual double SignedDistanceLocal(const Vector3D& otherPoint) const = 0;

	private:
		double ClosestDistanceLocal(const Vector3D& otherPoint) const override;
	};

	//! Shared pointer type for the ImplicitSurface3.
	using ImplicitSurface3Ptr = std::shared_ptr<ImplicitSurface3>;
}

#endif