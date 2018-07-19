/*************************************************************************
> File Name: ImplicitTriangleMesh3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: TriangleMesh3 to ImplicitSurface3 converter.
> Created Time: 2017/09/23
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Geometry/ImplicitTriangleMesh3.h>
#include <Geometry/TriangleMeshToSDF.h>

namespace CubbyFlow
{
	ImplicitTriangleMesh3::ImplicitTriangleMesh3(
		const TriangleMesh3Ptr& mesh,
		size_t resolutionX, double margin,
		const Transform3& transform,
		bool isNormalFlipped) :
		ImplicitSurface3(transform, isNormalFlipped), m_mesh(mesh)
	{
		BoundingBox3D box = m_mesh->BoundingBox();
		Vector3D scale(box.GetWidth(), box.GetHeight(), box.GetDepth());
		box.lowerCorner -= margin * scale;
		box.upperCorner += margin * scale;

		size_t resolutionY = static_cast<size_t>(std::ceil(resolutionX * box.GetHeight() / box.GetWidth()));
		size_t resolutionZ = static_cast<size_t>(std::ceil(resolutionX * box.GetDepth() / box.GetWidth()));

		double dx = box.GetWidth() / resolutionX;

		m_grid = std::make_shared<VertexCenteredScalarGrid3>();
		m_grid->Resize(resolutionX, resolutionY, resolutionZ, dx, dx, dx, box.lowerCorner.x, box.lowerCorner.y, box.lowerCorner.z);

		TriangleMeshToSDF(*m_mesh, m_grid.get());

		m_customImplicitSurface = CustomImplicitSurface3::Builder()
			.WithSignedDistanceFunction([&](const Vector3D& pt) -> double { return m_grid->Sample(pt); })
			.WithDomain(m_grid->BoundingBox())
			.WithResolution(dx)
			.MakeShared();
	}

	ImplicitTriangleMesh3::~ImplicitTriangleMesh3()
	{
		// Do nothing
	}

	Vector3D ImplicitTriangleMesh3::ClosestPointLocal(const Vector3D& otherPoint) const
	{
		return m_customImplicitSurface->ClosestPoint(otherPoint);
	}

	double ImplicitTriangleMesh3::ClosestDistanceLocal(const Vector3D& otherPoint) const
	{
		return m_customImplicitSurface->ClosestDistance(otherPoint);
	}

	bool ImplicitTriangleMesh3::IntersectsLocal(const Ray3D& ray) const
	{
		return m_customImplicitSurface->Intersects(ray);
	}

	BoundingBox3D ImplicitTriangleMesh3::BoundingBoxLocal() const
	{
		return m_mesh->BoundingBox();
	}

	Vector3D ImplicitTriangleMesh3::ClosestNormalLocal(const Vector3D& otherPoint) const
	{
		return m_customImplicitSurface->ClosestNormal(otherPoint);
	}

	double ImplicitTriangleMesh3::SignedDistanceLocal(const Vector3D& otherPoint) const
	{
		return m_customImplicitSurface->SignedDistance(otherPoint);
	}

	SurfaceRayIntersection3 ImplicitTriangleMesh3::ClosestIntersectionLocal(const Ray3D& ray) const
	{
		return m_customImplicitSurface->ClosestIntersection(ray);
	}

	ImplicitTriangleMesh3::Builder ImplicitTriangleMesh3::GetBuilder()
	{
		return ImplicitTriangleMesh3::Builder();
	}

	const VertexCenteredScalarGrid3Ptr& ImplicitTriangleMesh3::GetGrid() const
	{
		return m_grid;
	}

	ImplicitTriangleMesh3::Builder& ImplicitTriangleMesh3::Builder::WithTriangleMesh(const TriangleMesh3Ptr& mesh)
	{
		m_mesh = mesh;
		return *this;
	}

	ImplicitTriangleMesh3::Builder& ImplicitTriangleMesh3::Builder::WithResolutionX(size_t resolutionX)
	{
		m_resolutionX = resolutionX;
		return *this;
	}

	ImplicitTriangleMesh3::Builder& ImplicitTriangleMesh3::Builder::WithMargin(double margin)
	{
		m_margin = margin;
		return *this;
	}

	ImplicitTriangleMesh3 ImplicitTriangleMesh3::Builder::Build() const
	{
		return ImplicitTriangleMesh3(m_mesh, m_resolutionX, m_margin, m_transform, m_isNormalFlipped);
	}

	ImplicitTriangleMesh3Ptr ImplicitTriangleMesh3::Builder::MakeShared() const
	{
		return std::shared_ptr<ImplicitTriangleMesh3>(
			new ImplicitTriangleMesh3(m_mesh, m_resolutionX, m_margin, m_transform, m_isNormalFlipped),
			[](ImplicitTriangleMesh3* obj)
		{
			delete obj;
		});
	}
}