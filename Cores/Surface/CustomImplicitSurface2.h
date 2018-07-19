/*************************************************************************
> File Name: CustomImplicitSurface2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Custom 2-D implicit surface using arbitrary function.
> Created Time: 2017/09/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CUSTOM_IMPLICIT_SURFACE2_H
#define CUBBYFLOW_CUSTOM_IMPLICIT_SURFACE2_H

#include <Surface/ImplicitSurface2.h>

#include <functional>

namespace CubbyFlow
{
	//! Custom 2-D implicit surface using arbitrary function.
	class CustomImplicitSurface2 final : public ImplicitSurface2
	{
	public:
		class Builder;

		//!
		//! Constructs an implicit surface using the given signed-distance function.
		//!
		//! \param func Custom SDF function object.
		//! \param domain Bounding box of the SDF if exists.
		//! \param resolution Finite differencing resolution for derivatives.
		//! \param rayMarchingResolution Ray marching resolution for ray tests.
		//! \param maxNumberOfIterations Number of iterations for closest point search.
		//! \param transform Local-to-world transform.
		//! \param isNormalFlipped True if normal is flipped.
		//!
		CustomImplicitSurface2(
			const std::function<double(const Vector2D&)>& func,
			const BoundingBox2D& domain = BoundingBox2D(),
			double resolution = 1e-3,
			double rayMarchingResolution = 1e-6,
			unsigned int maxNumberOfIterations = 5,
			const Transform2& transform = Transform2(),
			bool isNormalFlipped = false);

		//! Destructor.
		virtual ~CustomImplicitSurface2();

		//! Returns builder for CustomImplicitSurface2.
		static Builder GetBuilder();

	private:
		std::function<double(const Vector2D&)> m_func;
		BoundingBox2D m_domain;
		double m_resolution = 1e-3;
		double m_rayMarchingResolution = 1e-6;
		unsigned int m_maxNumberOfIterations = 5;

		Vector2D ClosestPointLocal(const Vector2D& otherPoint) const override;

		bool IntersectsLocal(const Ray2D& ray) const override;

		BoundingBox2D BoundingBoxLocal() const override;

		Vector2D ClosestNormalLocal(const Vector2D& otherPoint) const override;

		double SignedDistanceLocal(const Vector2D& otherPoint) const override;

		SurfaceRayIntersection2 ClosestIntersectionLocal(const Ray2D& ray) const override;

		Vector2D GradientLocal(const Vector2D& x) const;
	};

	//! Shared pointer type for the CustomImplicitSurface2.
	using CustomImplicitSurface2Ptr = std::shared_ptr<CustomImplicitSurface2>;
	
	//!
	//! \brief Front-end to create CustomImplicitSurface2 objects step by step.
	//!
	class CustomImplicitSurface2::Builder final : public SurfaceBuilderBase2<CustomImplicitSurface2::Builder>
	{
	public:
		//! Returns builder with custom signed-distance function
		Builder& WithSignedDistanceFunction(const std::function<double(const Vector2D&)>& func);

		//! Returns builder with domain.
		Builder& WithDomain(const BoundingBox2D& domain);

		//! Returns builder with finite differencing resolution.
		Builder& WithResolution(double resolution);

		//! Returns builder with ray marching resolution which determines the ray
		//! intersection quality.
		Builder& WithRayMarchingResolution(double rayMarchingResolution);

		//! Returns builder with number of iterations for closest point/normal searches.
		Builder& WithMaxNumberOfIterations(unsigned int numIter);

		//! Builds CustomImplicitSurface2.
		CustomImplicitSurface2 Build() const;

		//! Builds shared pointer of CustomImplicitSurface2 instance.
		CustomImplicitSurface2Ptr MakeShared() const;

	private:
		std::function<double(const Vector2D&)> m_func;
		BoundingBox2D m_domain;
		double m_resolution = 1e-3;
		double m_rayMarchingResolution = 1e-6;
		unsigned int m_maxNumberOfIterations = 5;
	};
}

#endif