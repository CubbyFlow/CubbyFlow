/*************************************************************************
> File Name: ImplicitSurfaceSet2.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 2-D implicit surface set.
> Created Time: 2017/04/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_IMPLICIT_SURFACE_SET2_H
#define CUBBYFLOW_IMPLICIT_SURFACE_SET2_H

#include <Core/Geometry/BVH2.h>
#include <Core/Surface/ImplicitSurface2.h>

#include <vector>

namespace CubbyFlow
{
	//!
	//! \brief 2-D implicit surface set.
	//!
	//! This class represents 2-D implicit surface set which extends
	//! ImplicitSurface2 by overriding implicit surface-related queries. This is
	//! class can hold a collection of other implicit surface instances.
	//!
	class ImplicitSurfaceSet2 final : public ImplicitSurface2
	{
	public:
		class Builder;

		//! Constructs an empty implicit surface set.
		ImplicitSurfaceSet2();

		//! Constructs an implicit surface set using list of other surfaces.
		ImplicitSurfaceSet2(
			const std::vector<ImplicitSurface2Ptr>& surfaces,
			const Transform2& transform = Transform2(),
			bool isNormalFlipped = false);

		//! Constructs an implicit surface set using list of other surfaces.
		ImplicitSurfaceSet2(
			const std::vector<Surface2Ptr>& surfaces,
			const Transform2& transform = Transform2(),
			bool isNormalFlipped = false);

		//! Copy constructor.
		ImplicitSurfaceSet2(const ImplicitSurfaceSet2& other);

		//! Updates internal spatial query engine.
		void UpdateQueryEngine() override;

		//! Returns the number of implicit surfaces.
		size_t NumberOfSurfaces() const;

		//! Returns the i-th implicit surface.
		const ImplicitSurface2Ptr& SurfaceAt(size_t i) const;

		//! Adds an explicit surface instance.
		void AddExplicitSurface(const Surface2Ptr& surface);

		//! Adds an implicit surface instance.
		void AddSurface(const ImplicitSurface2Ptr& surface);

		//! Returns builder fox ImplicitSurfaceSet2.
		static Builder GetBuilder();

	private:
		std::vector<ImplicitSurface2Ptr> m_surfaces;
		mutable BVH2<ImplicitSurface2Ptr> m_bvh;
		mutable bool m_bvhInvalidated = true;

		// Surface2 implementations
		Vector2D ClosestPointLocal(const Vector2D& otherPoint) const override;

		BoundingBox2D BoundingBoxLocal() const override;

		double ClosestDistanceLocal(const Vector2D& otherPoint) const override;

		bool IntersectsLocal(const Ray2D& ray) const override;

		Vector2D ClosestNormalLocal(const Vector2D& otherPoint) const override;

		SurfaceRayIntersection2 ClosestIntersectionLocal(const Ray2D& ray) const override;

		// ImplicitSurface2 implementations
		double SignedDistanceLocal(const Vector2D& otherPoint) const override;

		void InvalidateBVH() const;

		void BuildBVH() const;
	};

	//! Shared pointer type for the ImplicitSurfaceSet2.
	using ImplicitSurfaceSet2Ptr = std::shared_ptr<ImplicitSurfaceSet2>;
	
	//!
	//! \brief Front-end to create ImplicitSurfaceSet2 objects step by step.
	//!
	class ImplicitSurfaceSet2::Builder final : public SurfaceBuilderBase2<ImplicitSurfaceSet2::Builder>
	{
	public:
		//! Returns builder with surfaces.
		Builder& WithSurfaces(const std::vector<ImplicitSurface2Ptr>& surfaces);

		//! Returns builder with explicit surfaces.
		Builder& WithExplicitSurfaces(const std::vector<Surface2Ptr>& surfaces);

		//! Builds ImplicitSurfaceSet2.
		ImplicitSurfaceSet2 Build() const;

		//! Builds shared pointer of ImplicitSurfaceSet2 instance.
		ImplicitSurfaceSet2Ptr MakeShared() const;

	private:
		bool m_isNormalFlipped = false;
		std::vector<ImplicitSurface2Ptr> m_surfaces;
	};
}

#endif