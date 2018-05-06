/*************************************************************************
> File Name: SurfaceToImplicit3.cpp
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: 3-D implicit surface wrapper for generic Surface3 instance.
> Created Time: 2017/04/18
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#include <Core/Geometry/TriangleMesh3.h>
#include <Core/Surface/SurfaceToImplicit3.h>
#include <Core/Utils/Logging.h>

namespace CubbyFlow
{
	SurfaceToImplicit3::SurfaceToImplicit3(
		const Surface3Ptr& surface,
		const Transform3& transform,
		bool isNormalFlipped) :
		ImplicitSurface3(transform, isNormalFlipped), m_surface(surface)
	{
		if (std::dynamic_pointer_cast<TriangleMesh3>(surface) != nullptr)
		{
			CUBBYFLOW_WARN << "Using TriangleMesh3 with SurfaceToImplicit3 can cause "
				<< "undefined behavior. Use ImplicitTriangleMesh3 instead.";
		}
	}

	SurfaceToImplicit3::SurfaceToImplicit3(const SurfaceToImplicit3& other) :
		ImplicitSurface3(other), m_surface(other.m_surface)
	{
		// Do nothing
	}

	Surface3Ptr SurfaceToImplicit3::GetSurface() const
	{
		return m_surface;
	}

	SurfaceToImplicit3::Builder SurfaceToImplicit3::GetBuilder()
	{
		return Builder();
	}

	Vector3D SurfaceToImplicit3::ClosestPointLocal(const Vector3D& otherPoint) const
	{
		return m_surface->ClosestPoint(otherPoint);
	}

	Vector3D SurfaceToImplicit3::ClosestNormalLocal(const Vector3D& otherPoint) const
	{
		return m_surface->ClosestNormal(otherPoint);
	}

	double SurfaceToImplicit3::ClosestDistanceLocal(const Vector3D& otherPoint) const
	{
		return m_surface->ClosestDistance(otherPoint);
	}

	bool SurfaceToImplicit3::IntersectsLocal(const Ray3D& ray) const
	{
		return m_surface->Intersects(ray);
	}

	SurfaceRayIntersection3 SurfaceToImplicit3::ClosestIntersectionLocal(const Ray3D& ray) const
	{
		return m_surface->ClosestIntersection(ray);
	}

	BoundingBox3D SurfaceToImplicit3::BoundingBoxLocal() const
	{
		return m_surface->BoundingBox();
	}

	double SurfaceToImplicit3::SignedDistanceLocal(const Vector3D& otherPoint) const
	{
		Vector3D x = m_surface->ClosestPoint(otherPoint);
		Vector3D n = m_surface->ClosestNormal(otherPoint);

		n = (isNormalFlipped) ? -n : n;

		if (n.Dot(otherPoint - x) < 0.0)
		{
			return -x.DistanceTo(otherPoint);
		}

		return x.DistanceTo(otherPoint);
	}

	SurfaceToImplicit3::Builder& SurfaceToImplicit3::Builder::WithSurface(const Surface3Ptr& surface)
	{
		m_surface = surface;
		return *this;
	}

	SurfaceToImplicit3 SurfaceToImplicit3::Builder::Build() const
	{
		return SurfaceToImplicit3(m_surface, m_transform, m_isNormalFlipped);
	}

	SurfaceToImplicit3Ptr SurfaceToImplicit3::Builder::MakeShared() const
	{
		return std::shared_ptr<SurfaceToImplicit3>(
			new SurfaceToImplicit3(m_surface, m_transform, m_isNormalFlipped),
			[](SurfaceToImplicit3* obj)
		{
			delete obj;
		});
	}
}
