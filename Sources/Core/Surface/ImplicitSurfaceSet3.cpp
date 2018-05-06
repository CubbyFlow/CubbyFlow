/*************************************************************************
> File Name: ImplicitSurfaceSet3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 3-D implicit surface set.
> Created Time: 2017/04/18
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Core/Surface/ImplicitSurfaceSet3.h>
#include <Core/Surface/SurfaceToImplicit3.h>

namespace CubbyFlow
{
	ImplicitSurfaceSet3::ImplicitSurfaceSet3()
	{
		// Do nothing
	}

	ImplicitSurfaceSet3::ImplicitSurfaceSet3(
		const std::vector<ImplicitSurface3Ptr>& surfaces,
		const Transform3& transform,
		bool isNormalFlipped) :
		ImplicitSurface3(transform, isNormalFlipped), m_surfaces(surfaces)
	{
		// Do nothing
	}

	ImplicitSurfaceSet3::ImplicitSurfaceSet3(
		const std::vector<Surface3Ptr>& surfaces,
		const Transform3& transform,
		bool isNormalFlipped) :
		ImplicitSurface3(transform, isNormalFlipped)
	{
		for (const auto& surface : surfaces)
		{
			AddExplicitSurface(surface);
		}
	}

	ImplicitSurfaceSet3::ImplicitSurfaceSet3(const ImplicitSurfaceSet3& other) :
		ImplicitSurface3(other), m_surfaces(other.m_surfaces)
	{
		// Do nothing
	}

	void ImplicitSurfaceSet3::UpdateQueryEngine()
	{
		BuildBVH();
	}

	size_t ImplicitSurfaceSet3::NumberOfSurfaces() const
	{
		return m_surfaces.size();
	}

	const ImplicitSurface3Ptr& ImplicitSurfaceSet3::SurfaceAt(size_t i) const
	{
		return m_surfaces[i];
	}

	void ImplicitSurfaceSet3::AddExplicitSurface(const Surface3Ptr& surface)
	{
		AddSurface(std::make_shared<SurfaceToImplicit3>(surface));
	}

	void ImplicitSurfaceSet3::AddSurface(const ImplicitSurface3Ptr& surface)
	{
		m_surfaces.push_back(surface);
		InvalidateBVH();
	}

	Vector3D ImplicitSurfaceSet3::ClosestPointLocal(const Vector3D& otherPoint) const
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

	double ImplicitSurfaceSet3::ClosestDistanceLocal(const Vector3D& otherPoint) const
	{
		BuildBVH();

		const auto distanceFunc = [](const Surface3Ptr& surface, const Vector3D& pt)
		{
			return surface->ClosestDistance(pt);
		};

		const auto queryResult = m_bvh.GetNearestNeighbor(otherPoint, distanceFunc);
		return queryResult.distance;
	}

	Vector3D ImplicitSurfaceSet3::ClosestNormalLocal(const Vector3D& otherPoint) const
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

	bool ImplicitSurfaceSet3::IntersectsLocal(const Ray3D& ray) const
	{
		BuildBVH();

		const auto testFunc = [](const Surface3Ptr& surface, const Ray3D& ray)
		{
			return surface->Intersects(ray);
		};

		return m_bvh.IsIntersects(ray, testFunc);
	}

	SurfaceRayIntersection3 ImplicitSurfaceSet3::ClosestIntersectionLocal(const Ray3D& ray) const
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

	BoundingBox3D ImplicitSurfaceSet3::BoundingBoxLocal() const
	{
		BuildBVH();

		return m_bvh.GetBoundingBox();
	}

	double ImplicitSurfaceSet3::SignedDistanceLocal(const Vector3D& otherPoint) const
	{
		double sdf = std::numeric_limits<double>::max();

		for (const auto& surface : m_surfaces)
		{
			sdf = std::min(sdf, surface->SignedDistance(otherPoint));
		}

		return sdf;
	}

	void ImplicitSurfaceSet3::InvalidateBVH() const
	{
		m_bvhInvalidated = true;
	}

	void ImplicitSurfaceSet3::BuildBVH() const
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


	ImplicitSurfaceSet3::Builder ImplicitSurfaceSet3::GetBuilder()
	{
		return Builder();
	}

	ImplicitSurfaceSet3::Builder& ImplicitSurfaceSet3::Builder::WithSurfaces(
		const std::vector<ImplicitSurface3Ptr>& surfaces)
	{
		m_surfaces = surfaces;
		return *this;
	}

	ImplicitSurfaceSet3::Builder& ImplicitSurfaceSet3::Builder::WithExplicitSurfaces(
		const std::vector<Surface3Ptr>& surfaces)
	{
		m_surfaces.clear();

		for (const auto& surface : surfaces)
		{
			m_surfaces.push_back(std::make_shared<SurfaceToImplicit3>(surface));
		}

		return *this;
	}

	ImplicitSurfaceSet3 ImplicitSurfaceSet3::Builder::Build() const
	{
		return ImplicitSurfaceSet3(m_surfaces, m_transform, m_isNormalFlipped);
	}

	ImplicitSurfaceSet3Ptr ImplicitSurfaceSet3::Builder::MakeShared() const
	{
		return std::shared_ptr<ImplicitSurfaceSet3>(new ImplicitSurfaceSet3(m_surfaces, m_transform, m_isNormalFlipped),
			[](ImplicitSurfaceSet3* obj)
		{
			delete obj;
		});
	}
}