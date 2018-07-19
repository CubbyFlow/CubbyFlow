/*************************************************************************
> File Name: ImplicitSurface2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 2-D implicit surface.
> Created Time: 2017/04/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_IMPLICIT_SURFACE2_H
#define CUBBYFLOW_IMPLICIT_SURFACE2_H

#include <Surface/Surface2.h>

namespace CubbyFlow
{
	//! Abstract base class for 2-D implicit surface.
	class ImplicitSurface2 : public Surface2
	{
	public:
		//! Default constructor.
		ImplicitSurface2(
			const Transform2& transform = Transform2(),
			bool isNormalFlipped = false);

		//! Copy constructor.
		ImplicitSurface2(const ImplicitSurface2& other);

		//! Default destructor.
		virtual ~ImplicitSurface2();

		//! Returns signed distance from the given point \p otherPoint.
		double SignedDistance(const Vector2D& otherPoint) const;

	protected:
		//! Returns signed distance from the given point \p otherPoint in local space.
		virtual double SignedDistanceLocal(const Vector2D& otherPoint) const = 0;

	private:
		double ClosestDistanceLocal(const Vector2D& otherPoint) const override;
	};

	//! Shared pointer type for the ImplicitSurface2.
	using ImplicitSurface2Ptr = std::shared_ptr<ImplicitSurface2>;
}

#endif