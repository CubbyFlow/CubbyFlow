/*************************************************************************
> File Name: Triangle3.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D triangle geometry.
> Created Time: 2017/04/14
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Geometry/Triangle3.h>

namespace CubbyFlow
{
	inline Vector3D ClosestPointOnLine(const Vector3D& v0, const Vector3D& v1, const Vector3D& pt)
	{
		const double lenSquared = (v1 - v0).LengthSquared();
		if (lenSquared < std::numeric_limits<double>::epsilon())
		{
			return v0;
		}

		const double t = (pt - v0).Dot(v1 - v0) / lenSquared;
		if (t < 0.0)
		{
			return v0;
		}
		if (t > 1.0)
		{
			return v1;
		}

		return v0 + t * (v1 - v0);
	}

	inline Vector3D ClosestNormalOnLine(const Vector3D& v0, const Vector3D& v1, const Vector3D& n0, const Vector3D& n1, const Vector3D& pt)
	{
		const double lenSquared = (v1 - v0).LengthSquared();
		if (lenSquared < std::numeric_limits<double>::epsilon())
		{
			return n0;
		}

		const double t = (pt - v0).Dot(v1 - v0) / lenSquared;
		if (t < 0.0)
		{
			return n0;
		}
		if (t > 1.0)
		{
			return n1;
		}

		return (n0 + t * (n1 - n0)).Normalized();
	}

	Triangle3::Triangle3(const Transform3& transform_, bool isNormalFlipped_) :
	Surface3(transform_, isNormalFlipped_)
	{
		// Do nothing
	}

	Triangle3::Triangle3(
		const std::array<Vector3D, 3>& newPoints,
		const std::array<Vector3D, 3>& newNormals,
		const std::array<Vector2D, 3>& newUvs,
		const Transform3& transform_,
		bool isNormalFlipped_) :
		Surface3(transform_, isNormalFlipped_),
		points(newPoints), normals(newNormals), uvs(newUvs)
	{
		// Do nothing
	}

	Triangle3::Triangle3(const Triangle3& other) :
		Surface3(other), points(other.points), normals(other.normals), uvs(other.uvs)
	{
		// Do nothing
	}

	Vector3D Triangle3::ClosestPointLocal(const Vector3D& otherPoint) const
	{
		Vector3D n = FaceNormal();
		double nd = n.Dot(n);
		double d = n.Dot(points[0]);
		double t = (d - n.Dot(otherPoint)) / nd;

		Vector3D q = t * n + otherPoint;

		Vector3D q01 = (points[1] - points[0]).Cross(q - points[0]);
		if (n.Dot(q01) < 0)
		{
			return ClosestPointOnLine(points[0], points[1], q);
		}

		Vector3D q12 = (points[2] - points[1]).Cross(q - points[1]);
		if (n.Dot(q12) < 0)
		{
			return ClosestPointOnLine(points[1], points[2], q);
		}

		Vector3D q02 = (points[0] - points[2]).Cross(q - points[2]);
		if (n.Dot(q02) < 0)
		{
			return ClosestPointOnLine(points[0], points[2], q);
		}

		double a = Area();
		double b0 = 0.5 * q12.Length() / a;
		double b1 = 0.5 * q02.Length() / a;
		double b2 = 0.5 * q01.Length() / a;

		return b0 * points[0] + b1 * points[1] + b2 * points[2];
	}

	Vector3D Triangle3::ClosestNormalLocal(const Vector3D& otherPoint) const
	{
		Vector3D n = FaceNormal();
		double nd = n.Dot(n);
		double d = n.Dot(points[0]);
		double t = (d - n.Dot(otherPoint)) / nd;

		Vector3D q = t * n + otherPoint;

		Vector3D q01 = (points[1] - points[0]).Cross(q - points[0]);
		if (n.Dot(q01) < 0)
		{
			return ClosestNormalOnLine(points[0], points[1], normals[0], normals[1], q);
		}

		Vector3D q12 = (points[2] - points[1]).Cross(q - points[1]);
		if (n.Dot(q12) < 0)
		{
			return ClosestNormalOnLine(points[1], points[2], normals[1], normals[2], q);
		}

		Vector3D q02 = (points[0] - points[2]).Cross(q - points[2]);
		if (n.Dot(q02) < 0)
		{
			return ClosestNormalOnLine(points[0], points[2], normals[0], normals[2], q);
		}

		double a = Area();
		double b0 = 0.5 * q12.Length() / a;
		double b1 = 0.5 * q02.Length() / a;
		double b2 = 0.5 * q01.Length() / a;

		return (b0 * normals[0] + b1 * normals[1] + b2 * normals[2]).Normalized();
	}

	bool Triangle3::IntersectsLocal(const Ray3D& ray) const
	{
		Vector3D n = FaceNormal();
		double nd = n.Dot(ray.direction);

		if (nd < std::numeric_limits<double>::epsilon())
		{
			return false;
		}

		double d = n.Dot(points[0]);
		double t = (d - n.Dot(ray.origin)) / nd;

		if (t < 0.0)
		{
			return false;
		}

		Vector3D q = ray.PointAt(t);

		Vector3D q01 = (points[1] - points[0]).Cross(q - points[0]);
		if (n.Dot(q01) <= 0.0)
		{
			return false;
		}

		Vector3D q12 = (points[2] - points[1]).Cross(q - points[1]);
		if (n.Dot(q12) <= 0.0)
		{
			return false;
		}

		Vector3D q02 = (points[0] - points[2]).Cross(q - points[2]);
		if (n.Dot(q02) <= 0.0)
		{
			return false;
		}

		return true;
	}

	SurfaceRayIntersection3 Triangle3::ClosestIntersectionLocal(const Ray3D& ray) const
	{
		SurfaceRayIntersection3 intersection;
		Vector3D n = FaceNormal();
		double nd = n.Dot(ray.direction);

		if (nd < std::numeric_limits<double>::epsilon())
		{
			intersection.isIntersecting = false;
			return intersection;
		}

		double d = n.Dot(points[0]);
		double t = (d - n.Dot(ray.origin)) / nd;

		if (t < 0.0)
		{
			intersection.isIntersecting = false;
			return intersection;
		}

		Vector3D q = ray.PointAt(t);

		Vector3D q01 = (points[1] - points[0]).Cross(q - points[0]);
		if (n.Dot(q01) <= 0.0)
		{
			intersection.isIntersecting = false;
			return intersection;
		}

		Vector3D q12 = (points[2] - points[1]).Cross(q - points[1]);
		if (n.Dot(q12) <= 0.0)
		{
			intersection.isIntersecting = false;
			return intersection;
		}

		Vector3D q02 = (points[0] - points[2]).Cross(q - points[2]);
		if (n.Dot(q02) <= 0.0)
		{
			intersection.isIntersecting = false;
			return intersection;
		}

		double a = Area();
		double b0 = 0.5 * q12.Length() / a;
		double b1 = 0.5 * q02.Length() / a;
		double b2 = 0.5 * q01.Length() / a;

		Vector3D normal = b0 * normals[0] + b1 * normals[1] + b2 * normals[2];

		intersection.isIntersecting = true;
		intersection.distance = t;
		intersection.point = q;
		intersection.normal = normal.Normalized();

		return intersection;
	}

	BoundingBox3D Triangle3::BoundingBoxLocal() const
	{
		BoundingBox3D box(points[0], points[1]);
		box.Merge(points[2]);
		return box;
	}

	double Triangle3::Area() const
	{
		return 0.5 * (points[1] - points[0]).Cross(points[2] - points[0]).Length();
	}

	void Triangle3::GetBarycentricCoords(const Vector3D& pt, double* b0, double* b1, double* b2) const
	{
		Vector3D q01 = (points[1] - points[0]).Cross(pt - points[0]);
		Vector3D q12 = (points[2] - points[1]).Cross(pt - points[1]);
		Vector3D q02 = (points[0] - points[2]).Cross(pt - points[2]);

		double a = Area();
		*b0 = 0.5 * q12.Length() / a;
		*b1 = 0.5 * q02.Length() / a;
		*b2 = 0.5 * q01.Length() / a;
	}

	Vector3D Triangle3::FaceNormal() const
	{
		Vector3D ret = (points[1] - points[0]).Cross(points[2] - points[0]);
		return ret.Normalized();
	}

	void Triangle3::SetNormalsToFaceNormal()
	{
		normals[0] = normals[1] = normals[2] = FaceNormal();
	}

	Triangle3::Builder Triangle3::GetBuilder()
	{
		return Builder();
	}

	Triangle3::Builder& Triangle3::Builder::WithPoints(const std::array<Vector3D, 3>& points)
	{
		m_points = points;
		return *this;
	}

	Triangle3::Builder& Triangle3::Builder::WithNormals(const std::array<Vector3D, 3>& normals)
	{
		m_normals = normals;
		return *this;
	}

	Triangle3::Builder& Triangle3::Builder::WithUVs(const std::array<Vector2D, 3>& uvs)
	{
		m_uvs = uvs;
		return *this;
	}

	Triangle3 Triangle3::Builder::Build() const
	{
		return Triangle3(m_points, m_normals, m_uvs, m_transform, m_isNormalFlipped);
	}

	Triangle3Ptr Triangle3::Builder::MakeShared() const
	{
		return std::shared_ptr<Triangle3>(new Triangle3(m_points, m_normals, m_uvs, m_transform, m_isNormalFlipped),
			[](Triangle3* obj)
		{
			delete obj;
		});
	}
}