/*************************************************************************
> File Name: SurfaceToImplicit2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D implicit surface wrapper for generic Surface2 instance.
> Created Time: 2017/04/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SURFACE_TO_IMPLICIT2_H
#define CUBBYFLOW_SURFACE_TO_IMPLICIT2_H

#include <Core/Surface/ImplicitSurface2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D implicit surface wrapper for generic Surface2 instance.
	//!
	//! This class represents 2-D implicit surface that converts Surface2 instance
	//! to an ImplicitSurface2 object. The conversion is made by evaluating closest
	//! point and normal from a given point for the given (explicit) surface. Thus,
	//! this conversion won't work for every single surfaces. Use this class only
	//! for the basic primitives such as Sphere2 or Box2.
	//!
	class SurfaceToImplicit2 final : public ImplicitSurface2
	{
	public:
		class Builder;

		//! Constructs an instance with generic Surface2 instance.
		SurfaceToImplicit2(
			const Surface2Ptr& surface,
			const Transform2& transform = Transform2(),
			bool isNormalFlipped = false);

		//! Copy constructor.
		SurfaceToImplicit2(const SurfaceToImplicit2& other);

		//! Returns the raw surface instance.
		Surface2Ptr GetSurface() const;

		//! Returns builder for SurfaceToImplicit2.
		static Builder GetBuilder();

	protected:
		Vector2D ClosestPointLocal(const Vector2D& otherPoint) const override;

		double ClosestDistanceLocal(const Vector2D& otherPoint) const override;

		bool IntersectsLocal(const Ray2D& ray) const override;

		BoundingBox2D BoundingBoxLocal() const override;

		Vector2D ClosestNormalLocal(const Vector2D& otherPoint) const override;

		double SignedDistanceLocal(const Vector2D& otherPoint) const override;

		SurfaceRayIntersection2 ClosestIntersectionLocal(const Ray2D& ray) const override;

	private:
		Surface2Ptr m_surface;
	};

	//! Shared pointer for the SurfaceToImplicit2 type.
	using SurfaceToImplicit2Ptr = std::shared_ptr<SurfaceToImplicit2>;
	
	//!
	//! \brief Front-end to create SurfaceToImplicit2 objects step by step.
	//!
	class SurfaceToImplicit2::Builder final	: public SurfaceBuilderBase2<SurfaceToImplicit2::Builder>
	{
	public:
		//! Returns builder with surface.
		Builder& WithSurface(const Surface2Ptr& surface);

		//! Builds SurfaceToImplicit2.
		SurfaceToImplicit2 Build() const;

		//! Builds shared pointer of SurfaceToImplicit2 instance.
		SurfaceToImplicit2Ptr MakeShared() const;

	private:
		Surface2Ptr m_surface;
	};
}

#endif