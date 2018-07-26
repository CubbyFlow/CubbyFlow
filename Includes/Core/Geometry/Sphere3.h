/*************************************************************************
> File Name: Sphere3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D sphere geometry.
> Created Time: 2017/04/04
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SPHERE3_H
#define CUBBYFLOW_SPHERE3_H

#include <Core/Surface/Surface3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D sphere geometry.
	//!
	//! This class represents 3-D sphere geometry which extends Surface3 by
	//! overriding surface-related queries.
	//!
	class Sphere3 final : public Surface3
	{
	public:
		class Builder;

		//! Center of the sphere.
		Vector3D center;

		//! Radius of the sphere.
		double radius = 1.0;

		//! Constructs a sphere with center at (0, 0, 0) and radius of 1.
		Sphere3(
			const Transform3& transform = Transform3(),
			bool isNormalFlipped = false);

		//! Constructs a sphere with \p center and \p radius.
		Sphere3(
			const Vector3D& center,
			double radius,
			const Transform3& transform = Transform3(),
			bool isNormalFlipped = false);

		//! Copy constructor.
		Sphere3(const Sphere3& other);

		//! Returns builder fox Sphere3.
		static Builder GetBuilder();

	private:
		Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

		double ClosestDistanceLocal(const Vector3D& otherPoint) const override;

		bool IntersectsLocal(const Ray3D& ray) const override;

		BoundingBox3D BoundingBoxLocal() const override;

		Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const override;

		SurfaceRayIntersection3 ClosestIntersectionLocal(const Ray3D& ray) const override;
	};

	//! Shared pointer for the Sphere3 type.
	using Sphere3Ptr = std::shared_ptr<Sphere3>;

	//!
	//! \brief Front-end to create Sphere3 objects step by step.
	//!
	class Sphere3::Builder final : public SurfaceBuilderBase3<Sphere3::Builder>
	{
	public:
		//! Returns builder with sphere center.
		Builder& WithCenter(const Vector3D& center);

		//! Returns builder with sphere radius.
		Builder& WithRadius(double radius);

		//! Builds Sphere3.
		Sphere3 Build() const;

		//! Builds shared pointer of Sphere3 instance.
		Sphere3Ptr MakeShared() const;

	private:
		Vector3D m_center = { 0, 0, 0 };
		double m_radius = 0.0;
	};
}

#endif