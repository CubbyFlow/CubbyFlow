/*************************************************************************
> File Name: Sphere2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D sphere geometry.
> Created Time: 2017/04/04
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SPHERE2_H
#define CUBBYFLOW_SPHERE2_H

#include <Core/Surface/Surface2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D sphere geometry.
	//!
	//! This class represents 2-D sphere geometry which extends Surface2 by
	//! overriding surface-related queries.
	//!
	class Sphere2 final : public Surface2
	{
	public:
		class Builder;

		//! Center of the sphere.
		Vector2D center;

		//! Radius of the sphere.
		double radius = 1.0;

		//! Constructs a sphere with center at (0, 0) and radius of 1.
		Sphere2(
			const Transform2& transform = Transform2(),
			bool isNormalFlipped = false);

		//! Constructs a sphere with \p center and \p radius.
		Sphere2(
			const Vector2D& center,
			double radius,
			const Transform2& transform = Transform2(),
			bool isNormalFlipped = false);

		//! Copy constructor.
		Sphere2(const Sphere2& other);

		//! Returns builder fox Sphere2.
		static Builder GetBuilder();

	private:
		Vector2D ClosestPointLocal(const Vector2D& otherPoint) const override;

		double ClosestDistanceLocal(const Vector2D& otherPoint) const override;

		bool IntersectsLocal(const Ray2D& ray) const override;

		BoundingBox2D BoundingBoxLocal() const override;

		Vector2D ClosestNormalLocal(const Vector2D& otherPoint) const override;

		SurfaceRayIntersection2 ClosestIntersectionLocal(const Ray2D& ray) const override;
	};

	//! Shared pointer for the Sphere2 type.
	using Sphere2Ptr = std::shared_ptr<Sphere2>;

	//!
	//! \brief Front-end to create Sphere2 objects step by step.
	//!
	class Sphere2::Builder final : public SurfaceBuilderBase2<Sphere2::Builder>
	{
	public:
		//! Returns builder with sphere center.
		Builder& WithCenter(const Vector2D& center);

		//! Returns builder with sphere radius.
		Builder& WithRadius(double radius);

		//! Builds Sphere2.
		Sphere2 Build() const;

		//! Builds shared pointer of Sphere2 instance.
		Sphere2Ptr MakeShared() const;

	private:
		Vector2D m_center = { 0, 0 };
		double m_radius = 0.0;
	};
}

#endif