/*************************************************************************
> File Name: SurfaceToImplicit3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D implicit surface wrapper for generic Surface3 instance.
> Created Time: 2017/04/18
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_SURFACE_TO_IMPLICIT3_H
#define CUBBYFLOW_SURFACE_TO_IMPLICIT3_H

#include <Surface/ImplicitSurface3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D implicit surface wrapper for generic Surface3 instance.
	//! 
	//! This class represents 3-D implicit surface that converts Surface3 instance.
	//! to an ImplicitSurface3 object. The conversion is made by evaluating closest
	//! point and normal from a given point for the given (explicit) surface. Thus,
	//! this conversion won't work for every single surfaces, especially
	//! TriangleMesh3. To use TriangleMesh3 as an ImplicitSurface3 instance,
	//! please take a look at ImplicitTriangleMesh3. Use this class only
	//! for the basic primitives such as Sphere3 or Box3.
	//!
	class SurfaceToImplicit3 final : public ImplicitSurface3
	{
	public:
		class Builder;

		//! Constructs an instance with generic Surface3 instance.
		SurfaceToImplicit3(
			const Surface3Ptr& surface,
			const Transform3& transform = Transform3(),
			bool isNormalFlipped = false);

		//! Copy constructor.
		SurfaceToImplicit3(const SurfaceToImplicit3& other);

		//! Returns the raw surface instance.
		Surface3Ptr GetSurface() const;

		//! Returns builder for SurfaceToImplicit3.
		static Builder GetBuilder();

	protected:
		Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

		double ClosestDistanceLocal(const Vector3D& otherPoint) const override;

		bool IntersectsLocal(const Ray3D& ray) const override;

		BoundingBox3D BoundingBoxLocal() const override;

		Vector3D ClosestNormalLocal(const Vector3D& ohterPoint) const override;

		double SignedDistanceLocal(const Vector3D& otherPoint) const override;

		SurfaceRayIntersection3 ClosestIntersectionLocal(const Ray3D& ray) const override;

	private:
		Surface3Ptr m_surface;
	};

	//! Shared pointer for the SurfaceToImplicit3
	using SurfaceToImplicit3Ptr = std::shared_ptr<SurfaceToImplicit3>;

	//!
	//! \brief Front-end to create SurfaceToImplicit3 objects step by step.
	//!
	class SurfaceToImplicit3::Builder final : public SurfaceBuilderBase3<SurfaceToImplicit3::Builder>
	{
	public:
		//! Returns builder with surface.
		Builder& WithSurface(const Surface3Ptr& surface);

		//! Builds SurfaceToImplicit3.
		SurfaceToImplicit3 Build() const;

		//! Builds shared pointer of SurfaceToImplicit3 instance.
		SurfaceToImplicit3Ptr MakeShared() const;
	private:
		Surface3Ptr m_surface;
	};
}

#endif