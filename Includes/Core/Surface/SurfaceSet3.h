/*************************************************************************
> File Name: SurfaceSet3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D surface set.
> Created Time: 2017/04/04
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SURFACE_SET3_H
#define CUBBYFLOW_SURFACE_SET3_H

#include <Core/Geometry/BVH3.h>
#include <Core/Surface/Surface3.h>

#include <vector>

namespace CubbyFlow
{
	//!
	//! \brief 3-D surface set.
	//!
	//! This class represents 3-D surface set which extends Surface3 by overriding
	//! surface-related queries. This is class can hold a collection of other surface
	//! instances.
	//!
	class SurfaceSet3 final : public Surface3
	{
	public:
		class Builder;

		//! Constructs an empty surface set.
		SurfaceSet3();

		//! Constructs with a list of other surfaces.
		explicit SurfaceSet3(
			const std::vector<Surface3Ptr>& others,
			const Transform3& transform = Transform3(),
			bool isNormalFlipped = false);

		//! Copy constructor.
		SurfaceSet3(const SurfaceSet3& other);

		//! Updates internal spatial query engine.
		void UpdateQueryEngine() override;

		//! Returns the number of surfaces.
		size_t NumberOfSurfaces() const;

		//! Returns the i-th surface.
		const Surface3Ptr& SurfaceAt(size_t i) const;

		//! Adds a surface instance.
		void AddSurface(const Surface3Ptr& surface);

		//! Returns builder for SurfaceSet3.
		static Builder GetBuilder();

	private:
		std::vector<Surface3Ptr> m_surfaces;
		mutable BVH3<Surface3Ptr> m_bvh;
		mutable bool m_bvhInvalidated = true;

		// Surface3 implementations
		Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

		BoundingBox3D BoundingBoxLocal() const override;

		double ClosestDistanceLocal(const Vector3D& otherPoint) const override;

		bool IntersectsLocal(const Ray3D& ray) const override;

		Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const override;

		SurfaceRayIntersection3 ClosestIntersectionLocal(const Ray3D& ray) const override;

		void InvalidateBVH() const;

		void BuildBVH() const;
	};

	//! Shared pointer for the SurfaceSet3 type.
	using SurfaceSet3Ptr = std::shared_ptr<SurfaceSet3>;

	//!
	//! \brief Front-end to create SurfaceSet3 objects step by step.
	//!
	class SurfaceSet3::Builder final : public SurfaceBuilderBase3<SurfaceSet3>
	{
	public:
		//! Returns builder with other surfaces.
		Builder& WithSurfaces(const std::vector<Surface3Ptr>& others);

		//! Builds SurfaceSet3.
		SurfaceSet3 Build() const;

		//! Builds shared pointer of SurfaceSet3 instance.
		SurfaceSet3Ptr MakeShared() const;

	private:
		std::vector<Surface3Ptr> m_surfaces;
	};
}

#endif