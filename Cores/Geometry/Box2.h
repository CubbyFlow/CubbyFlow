/*************************************************************************
> File Name: Box2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D box geometry.
> Created Time: 2017/06/21
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_BOX2_H
#define CUBBYFLOW_BOX2_H

#include <Surface/Surface2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D box geometry.
	//!
	//! This class represents 2-D box geometry which extends Surface2 by overriding
	//! surface-related queries. This box implementation is an axis-aligned box
	//! that wraps lower-level primitive type, BoundingBox2D.
	//!
	class Box2 final : public Surface2
	{
	public:
		class Builder;

		//! Bounding box of this box.
		BoundingBox2D bound = BoundingBox2D(Vector2D(), Vector2D(1.0, 1.0));

		//! Constructs (0, 0) x (1, 1) box.
		Box2(const Transform2& transform = Transform2(), bool isNormalFlipped = false);

		//! Constructs a box with given \p lowerCorner and \p upperCorner.
		Box2(const Vector2D& lowerCorner, const Vector2D& upperCorner,
			const Transform2& transform = Transform2(), bool isNormalFlipped = false);

		//! Constructs a box with BoundingBox2D instance.
		Box2(const BoundingBox2D& boundingBox, const Transform2& transform = Transform2(), bool isNormalFlipped = false);

		//! Copy constructor.
		Box2(const Box2& other);

		//! Returns builder fox Box2.
		static Builder GetBuilder();

	protected:
		// Surface2 implementations
		Vector2D ClosestPointLocal(const Vector2D& otherPoint) const override;

		bool IntersectsLocal(const Ray2D& ray) const override;

		BoundingBox2D BoundingBoxLocal() const override;

		Vector2D ClosestNormalLocal(const Vector2D& otherPoint) const override;

		SurfaceRayIntersection2 ClosestIntersectionLocal(const Ray2D& ray) const override;
	};

	//! Shared pointer type for the Box2.
	using Box2Ptr = std::shared_ptr<Box2>;
	
	//!
	//! \brief Front-end to create Box2 objects step by step.
	//!
	class Box2::Builder final : public SurfaceBuilderBase2<Box2::Builder>
	{
	public:
		//! Returns builder with lower corner set.
		Builder& WithLowerCorner(const Vector2D& pt);

		//! Returns builder with upper corner set.
		Builder& WithUpperCorner(const Vector2D& pt);

		//! Returns builder with bounding box.
		Builder& WithBoundingBox(const BoundingBox2D& bbox);

		//! Builds Box2.
		Box2 Build() const;

		//! Builds shared pointer of Box2 instance.
		Box2Ptr MakeShared() const;

	private:
		Vector2D m_lowerCorner{ 0, 0 };
		Vector2D m_upperCorner{ 1, 1 };
	};
}

#endif