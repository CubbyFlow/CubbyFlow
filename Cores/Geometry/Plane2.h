/*************************************************************************
> File Name: Plane2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D plane geometry.
> Created Time: 2017/06/18
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PLANE2_H
#define CUBBYFLOW_PLANE2_H

#include <Surface/Surface2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D plane geometry.
	//!
	//! This class represents 2-D plane geometry which extends Surface2 by
	//! overriding surface-related queries.
	//!
	class Plane2 final : public Surface2
	{
	public:
		class Builder;

		//! Plane normal.
		Vector2D normal = Vector2D(0, 1);

		//! Point that lies on the plane.
		Vector2D point;

		//! Constructs a plane that crosses (0, 0) with surface normal (0, 1).
		Plane2(const Transform2& transform = Transform2(), bool isNormalFlipped = false);

		//! Constructs a plane that cross \p point with surface normal \p normal.
		Plane2(const Vector2D& normal, const Vector2D& point,
			const Transform2& transform = Transform2(), bool isNormalFlipped = false);

		//! Copy constructor.
		Plane2(const Plane2& other);

		//! Returns builder fox Plane2.
		static Builder GetBuilder();

	private:
		Vector2D ClosestPointLocal(const Vector2D& otherPoint) const override;

		bool IntersectsLocal(const Ray2D& ray) const override;

		BoundingBox2D BoundingBoxLocal() const override;

		Vector2D ClosestNormalLocal(const Vector2D& otherPoint) const override;

		SurfaceRayIntersection2 ClosestIntersectionLocal(const Ray2D& ray) const override;
	};

	//! Shared pointer for the Plane2 type.
	using Plane2Ptr = std::shared_ptr<Plane2>;
	
	//!
	//! \brief Front-end to create Plane2 objects step by step.
	//!
	class Plane2::Builder final : public SurfaceBuilderBase2<Plane2::Builder>
	{
	public:
		//! Returns builder with plane normal.
		Builder& WithNormal(const Vector2D& normal);

		//! Returns builder with point on the plane.
		Builder& WithPoint(const Vector2D& point);

		//! Builds Plane2.
		Plane2 Build() const;

		//! Builds shared pointer of Plane2 instance.
		Plane2Ptr MakeShared() const;

	private:
		Vector2D m_normal{ 0, 1 };
		Vector2D m_point{ 0, 0 };
	};
}

#endif