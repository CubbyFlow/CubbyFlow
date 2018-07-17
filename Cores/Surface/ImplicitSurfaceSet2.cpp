/*************************************************************************
> File Name: ImplicitSurfaceSet2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D implicit surface set.
> Created Time: 2017/04/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Surface/ImplicitSurfaceSet2.h>
#include <Surface/SurfaceToImplicit2.h>

namespace CubbyFlow
{
	ImplicitSurfaceSet2::ImplicitSurfaceSet2()
	{
		// Do nothing
	}

	ImplicitSurfaceSet2::ImplicitSurfaceSet2(
		const std::vector<ImplicitSurface2Ptr>& surfaces,
		const Transform2& transform,
		bool isNormalFlipped) :
		ImplicitSurface2(transform, isNormalFlipped), m_surfaces(surfaces)
	{
		// Do nothing
	}

	ImplicitSurfaceSet2::ImplicitSurfaceSet2(
		const std::vector<Surface2Ptr>& surfaces,
		const Transform2& transform,
		bool isNormalFlipped) :
		ImplicitSurface2(transform, isNormalFlipped)
	{
		for (const auto& surface : surfaces)
		{
			AddExplicitSurface(surface);
		}
	}

	ImplicitSurfaceSet2::ImplicitSurfaceSet2(const ImplicitSurfaceSet2& other) :
		ImplicitSurface2(other), m_surfaces(other.m_surfaces)
	{
		// Do nothing
	}

	void ImplicitSurfaceSet2::UpdateQueryEngine()
	{
		BuildBVH();
	}

	size_t ImplicitSurfaceSet2::NumberOfSurfaces() const
	{
		return m_surfaces.size();
	}

	const ImplicitSurface2Ptr& ImplicitSurfaceSet2::SurfaceAt(size_t i) const
	{
		return m_surfaces[i];
	}

	void ImplicitSurfaceSet2::AddExplicitSurface(const Surface2Ptr& surface)
	{
		AddSurface(std::make_shared<SurfaceToImplicit2>(surface));
	}

	void ImplicitSurfaceSet2::AddSurface(const ImplicitSurface2Ptr& surface)
	{
		m_surfaces.push_back(surface);
		InvalidateBVH();
	}

	Vector2D ImplicitSurfaceSet2::ClosestPointLocal(const Vector2D& otherPoint) const
	{
		BuildBVH();

		const auto distanceFunc = [](const Surface2Ptr& surface, const Vector2D& pt)
		{
			return surface->ClosestDistance(pt);
		};

		const auto queryResult = m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);
		if (queryResult.item != nullptr)
		{
			return (*queryResult.item)->ClosestPoint(otherPoint);
		}

		return Vector2D{ std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
	}

	double ImplicitSurfaceSet2::ClosestDistanceLocal(const Vector2D& otherPoint) const
	{
		BuildBVH();

		const auto distanceFunc = [](const Surface2Ptr& surface, const Vector2D& pt)
		{
			return surface->ClosestDistance(pt);
		};

		const auto queryResult = m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);
		return queryResult.distance;
	}

	Vector2D ImplicitSurfaceSet2::ClosestNormalLocal(const Vector2D& otherPoint) const
	{
		BuildBVH();

		const auto distanceFunc = [](const Surface2Ptr& surface, const Vector2D& pt)
		{
			return surface->ClosestDistance(pt);
		};

		const auto queryResult = m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);
		if (queryResult.item != nullptr)
		{
			return (*queryResult.item)->ClosestNormal(otherPoint);
		}

		return Vector2D{ 1.0, 0.0 };
	}

	bool ImplicitSurfaceSet2::IntersectsLocal(const Ray2D& ray) const
	{
		BuildBVH();

		const auto testFunc = [](const Surface2Ptr& surface, const Ray2D& ray)
		{
			return surface->Intersects(ray);
		};

		return m_bvh.IsIntersects(ray, testFunc);
	}

	SurfaceRayIntersection2 ImplicitSurfaceSet2::ClosestIntersectionLocal(const Ray2D& ray) const
	{
		BuildBVH();

		const auto testFunc = [](const Surface2Ptr& surface, const Ray2D& ray)
		{
			SurfaceRayIntersection2 result = surface->ClosestIntersection(ray);
			return result.distance;
		};

		const auto queryResult = m_bvh.GetClosestIntersection(ray, testFunc);

		SurfaceRayIntersection2 result;
		result.distance = queryResult.distance;
		result.isIntersecting = queryResult.item != nullptr;

		if (queryResult.item != nullptr)
		{
			result.point = ray.PointAt(queryResult.distance);
			result.normal = (*queryResult.item)->ClosestNormal(result.point);
		}

		return result;
	}

	BoundingBox2D ImplicitSurfaceSet2::BoundingBoxLocal() const
	{
		BuildBVH();

		return m_bvh.GetBoundingBox();
	}

	double ImplicitSurfaceSet2::SignedDistanceLocal(const Vector2D& otherPoint) const
	{
		double sdf = std::numeric_limits<double>::max();
		
		for (const auto& surface : m_surfaces)
		{
			sdf = std::min(sdf, surface->SignedDistance(otherPoint));
		}

		return sdf;
	}

	void ImplicitSurfaceSet2::InvalidateBVH() const
	{
		m_bvhInvalidated = true;
	}

	void ImplicitSurfaceSet2::BuildBVH() const
	{
		if (m_bvhInvalidated)
		{
			std::vector<BoundingBox2D> bounds(m_surfaces.size());

			for (size_t i = 0; i < m_surfaces.size(); ++i)
			{
				bounds[i] = m_surfaces[i]->BoundingBox();
			}

			m_bvh.Build(m_surfaces, bounds);
			m_bvhInvalidated = false;
		}
	}

	ImplicitSurfaceSet2::Builder ImplicitSurfaceSet2::GetBuilder()
	{
		return Builder();
	}
	
	ImplicitSurfaceSet2::Builder& ImplicitSurfaceSet2::Builder::WithSurfaces(
		const std::vector<ImplicitSurface2Ptr>& surfaces)
	{
		m_surfaces = surfaces;
		return *this;
	}

	ImplicitSurfaceSet2::Builder& ImplicitSurfaceSet2::Builder::WithExplicitSurfaces(
			const std::vector<Surface2Ptr>& surfaces)
	{
		m_surfaces.clear();

		for (const auto& surface : surfaces)
		{
			m_surfaces.push_back(std::make_shared<SurfaceToImplicit2>(surface));
		}

		return *this;
	}

	ImplicitSurfaceSet2 ImplicitSurfaceSet2::Builder::Build() const
	{
		return ImplicitSurfaceSet2(m_surfaces, m_transform, m_isNormalFlipped);
	}

	ImplicitSurfaceSet2Ptr ImplicitSurfaceSet2::Builder::MakeShared() const
	{
		return std::shared_ptr<ImplicitSurfaceSet2>(new ImplicitSurfaceSet2(m_surfaces, m_transform, m_isNormalFlipped),
			[](ImplicitSurfaceSet2* obj)
		{
			delete obj;
		});
	}
}