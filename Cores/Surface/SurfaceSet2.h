/*************************************************************************
> File Name: SurfaceSet2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D surface set.
> Created Time: 2017/04/04
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SURFACE_SET2_H
#define CUBBYFLOW_SURFACE_SET2_H

#include <Geometry/BVH2.h>
#include <Surface/Surface2.h>

#include <vector>

namespace CubbyFlow
{
	//!
	//! \brief 2-D surface set.
	//!
	//! This class represents 2-D surface set which extends Surface2 by overriding
	//! surface-related queries. This is class can hold a collection of other surface
	//! instances.
	//!
	class SurfaceSet2 final : public Surface2
	{
	public:
		class Builder;

		//! Constructs an empty surface set.
		SurfaceSet2();

		//! Constructs with a list of other surfaces.
		explicit SurfaceSet2(
			const std::vector<Surface2Ptr>& others,
			const Transform2& transform = Transform2(),
			bool isNormalFlipped = false);

		//! Copy constructor.
		SurfaceSet2(const SurfaceSet2& other);

		//! Updates internal spatial query engine.
		void UpdateQueryEngine() override;

		//! Returns the number of surfaces.
		size_t NumberOfSurfaces() const;

		//! Returns the i-th surface.
		const Surface2Ptr& SurfaceAt(size_t i) const;

		//! Adds a surface instance.
		void AddSurface(const Surface2Ptr& surface);

		//! Returns builder for SurfaceSet2.
		static Builder GetBuilder();

	private:
		std::vector<Surface2Ptr> m_surfaces;
		mutable BVH2<Surface2Ptr> m_bvh;
		mutable bool m_bvhInvalidated = true;

		// Surface2 implementations
		Vector2D ClosestPointLocal(const Vector2D& otherPoint) const override;
		
		BoundingBox2D BoundingBoxLocal() const override;
		
		double ClosestDistanceLocal(const Vector2D& otherPoint) const override;

		bool IntersectsLocal(const Ray2D& ray) const override;

		Vector2D ClosestNormalLocal(const Vector2D& otherPoint) const override;

		SurfaceRayIntersection2 ClosestIntersectionLocal(const Ray2D& ray) const override;

		void InvalidateBVH() const;
		
		void BuildBVH() const;
	};

	//! Shared pointer for the SurfaceSet2 type.
	using SurfaceSet2Ptr = std::shared_ptr<SurfaceSet2>;

	//!
	//! \brief Front-end to create SurfaceSet2 objects step by step.
	//!
	class SurfaceSet2::Builder final : public SurfaceBuilderBase2<SurfaceSet2>
	{
	public:
		//! Returns builder with other surfaces.
		Builder& WithSurfaces(const std::vector<Surface2Ptr>& others);

		//! Builds SurfaceSet2.
		SurfaceSet2 Build() const;

		//! Builds shared pointer of SurfaceSet2 instance.
		SurfaceSet2Ptr MakeShared() const;

	private:
		std::vector<Surface2Ptr> m_surfaces;
	};
}

#endif