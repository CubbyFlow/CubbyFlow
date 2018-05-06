/*************************************************************************
> File Name: SurfaceSet3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 3-D surface set.
> Created Time: 2017/04/14
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Core/Surface/SurfaceSet3.h>

namespace CubbyFlow
{
	SurfaceSet3::SurfaceSet3()
	{
		// Do nothing
	}

	SurfaceSet3::SurfaceSet3(
		const std::vector<Surface3Ptr>& others,
		const Transform3& transform,
		bool isNormalFlipped) :
		Surface3(transform, isNormalFlipped), m_surfaces(others)
	{
		InvalidateBVH();
	}

	SurfaceSet3::SurfaceSet3(const SurfaceSet3& other) :
		Surface3(other), m_surfaces(other.m_surfaces)
	{
		InvalidateBVH();
	}

	void SurfaceSet3::UpdateQueryEngine()
	{
		BuildBVH();
	}

	size_t SurfaceSet3::NumberOfSurfaces() const
	{
		return m_surfaces.size();
	}

	const Surface3Ptr& SurfaceSet3::SurfaceAt(size_t i) const
	{
		return m_surfaces[i];
	}

	void SurfaceSet3::AddSurface(const Surface3Ptr& surface)
	{
		m_surfaces.push_back(surface);
		InvalidateBVH();
	}

	Vector3D SurfaceSet3::ClosestPointLocal(const Vector3D& otherPoint) const
	{
		BuildBVH();

		const auto distanceFunc = [](const Surface3Ptr& surface, const Vector3D& pt)
		{
			return surface->ClosestDistance(pt);
		};

		const auto queryResult = m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);
		if (queryResult.item != nullptr)
		{
			return (*queryResult.item)->ClosestPoint(otherPoint);
		}

		return Vector3D{ std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
	}

	Vector3D SurfaceSet3::ClosestNormalLocal(const Vector3D& otherPoint) const
	{
		BuildBVH();

		const auto distanceFunc = [](const Surface3Ptr& surface, const Vector3D& pt)
		{
			return surface->ClosestDistance(pt);
		};

		const auto queryResult = m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);
		if (queryResult.item != nullptr)
		{
			return (*queryResult.item)->ClosestNormal(otherPoint);
		}

		return Vector3D{ 1.0, 0.0, 0.0 };
	}

	double SurfaceSet3::ClosestDistanceLocal(const Vector3D& otherPoint) const
	{
		BuildBVH();

		const auto distanceFunc = [](const Surface3Ptr& surface, const Vector3D& pt)
		{
			return surface->ClosestDistance(pt);
		};

		const auto queryResult = m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);
		return queryResult.distance;
	}

	bool SurfaceSet3::IntersectsLocal(const Ray3D& ray) const
	{
		BuildBVH();

		const auto testFunc = [](const Surface3Ptr& surface, const Ray3D& ray)
		{
			return surface->Intersects(ray);
		};

		return m_bvh.IsIntersects(ray, testFunc);
	}

	SurfaceRayIntersection3 SurfaceSet3::ClosestIntersectionLocal(const Ray3D& ray) const
	{
		BuildBVH();

		const auto testFunc = [](const Surface3Ptr& surface, const Ray3D& ray)
		{
			SurfaceRayIntersection3 result = surface->ClosestIntersection(ray);
			return result.distance;
		};

		const auto queryResult = m_bvh.GetClosestIntersection(ray, testFunc);
		
		SurfaceRayIntersection3 result;
		result.distance = queryResult.distance;
		result.isIntersecting = queryResult.item != nullptr;
		
		if (queryResult.item != nullptr)
		{
			result.point = ray.PointAt(queryResult.distance);
			result.normal = (*queryResult.item)->ClosestNormal(result.point);
		}
		
		return result;
	}

	BoundingBox3D SurfaceSet3::BoundingBoxLocal() const
	{
		BuildBVH();

		return m_bvh.GetBoundingBox();
	}

	void SurfaceSet3::InvalidateBVH() const
	{
		m_bvhInvalidated = true;
	}

	void SurfaceSet3::BuildBVH() const
	{
		if (m_bvhInvalidated)
		{
			std::vector<BoundingBox3D> bounds(m_surfaces.size());
			for (size_t i = 0; i < m_surfaces.size(); ++i)
			{
				bounds[i] = m_surfaces[i]->BoundingBox();
			}

			m_bvh.Build(m_surfaces, bounds);
			m_bvhInvalidated = false;
		}
	}

	SurfaceSet3::Builder SurfaceSet3::GetBuilder()
	{
		return Builder();
	}

	SurfaceSet3::Builder& SurfaceSet3::Builder::WithSurfaces(const std::vector<Surface3Ptr>& others)
	{
		m_surfaces = others;
		return *this;
	}

	SurfaceSet3 SurfaceSet3::Builder::Build() const
	{
		return SurfaceSet3(m_surfaces, m_transform, m_isNormalFlipped);
	}

	SurfaceSet3Ptr SurfaceSet3::Builder::MakeShared() const
	{
		return std::shared_ptr<SurfaceSet3>(new SurfaceSet3(m_surfaces, m_transform, m_isNormalFlipped),
			[](SurfaceSet3* obj)
		{
			delete obj;
		});
	}
}