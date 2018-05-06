/*************************************************************************
> File Name: TriangleMesh3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D triangle mesh geometry.
> Created Time: 2017/04/04
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_TRIANGLE_MESH3_H
#define CUBBYFLOW_TRIANGLE_MESH3_H

#include <Core/Array/Array1.h>
#include <Core/Geometry/BVH3.h>
#include <Core/Geometry/Triangle3.h>
#include <Core/Point/Point3.h>
#include <Core/Surface/Surface3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D triangle mesh geometry.
	//!
	//! This class represents 3-D triangle mesh geometry which extends Surface3 by
	//! overriding surface-related queries. The mesh structure stores point,
	//! normals, and UV coordinates.
	//!
	class TriangleMesh3 final : public Surface3
	{
	public:
		class Builder;

		using Vector2DArray = Array1<Vector2D>;
		using Vector3DArray = Array1<Vector3D>;
		using IndexArray = Array1<Point3UI>;

		using PointArray = Vector3DArray;
		using NormalArray = Vector3DArray;
		using UVArray = Vector2DArray;

		//! Default constructor.
		TriangleMesh3(
			const Transform3& transform = Transform3(),
			bool isNormalFlipped = false);

		//! Constructs mesh with points, normals, uvs, and their indices.
		TriangleMesh3(
			const PointArray& points,
			const NormalArray& normals,
			const UVArray& uvs,
			const IndexArray& pointIndices,
			const IndexArray& normalIndices,
			const IndexArray& uvIndices,
			const Transform3& transform_ = Transform3(),
			bool isNormalFlipped_ = false);

		//! Copy constructor.
		TriangleMesh3(const TriangleMesh3& other);

		//! Updates internal spatial query engine.
		void UpdateQueryEngine() override;

		//! Clears all content.
		void Clear();

		//! Copies the contents from \p other mesh.
		void Set(const TriangleMesh3& other);

		//! Swaps the contents with \p other mesh.
		void Swap(TriangleMesh3& other);

		//! Returns area of this mesh.
		double Area() const;

		//! Returns volume of this mesh.
		double Volume() const;

		//! Returns constant reference to the i-th point.
		const Vector3D& Point(size_t i) const;

		//! Returns reference to the i-th point.
		Vector3D& Point(size_t i);

		//! Returns constant reference to the i-th normal.
		const Vector3D& Normal(size_t i) const;

		//! Returns reference to the i-th normal.
		Vector3D& Normal(size_t i);

		//! Returns constant reference to the i-th UV coordinates.
		const Vector2D& UV(size_t i) const;

		//! Returns reference to the i-th UV coordinates.
		Vector2D& UV(size_t i);

		//! Returns constant reference to the point indices of i-th triangle.
		const Point3UI& PointIndex(size_t i) const;

		//! Returns reference to the point indices of i-th triangle.
		Point3UI& PointIndex(size_t i);

		//! Returns constant reference to the normal indices of i-th triangle.
		const Point3UI& NormalIndex(size_t i) const;

		//! Returns reference to the normal indices of i-th triangle.
		Point3UI& NormalIndex(size_t i);

		//! Returns constant reference to the UV indices of i-th triangle.
		const Point3UI& UVIndex(size_t i) const;

		//! Returns reference to the UV indices of i-th triangle.
		Point3UI& UVIndex(size_t i);

		//! Returns i-th triangle.
		Triangle3 Triangle(size_t i) const;

		//! Returns number of points.
		size_t NumberOfPoints() const;

		//! Returns number of normals.
		size_t NumberOfNormals() const;

		//! Returns number of UV coordinates.
		size_t NumberOfUVs() const;

		//! Returns number of triangles.
		size_t NumberOfTriangles() const;

		//! Returns true if the mesh has normals.
		bool HasNormals() const;

		//! Returns true if the mesh has UV coordinates.
		bool HasUVs() const;

		//! Adds a point.
		void AddPoint(const Vector3D& pt);

		//! Adds a normal.
		void AddNormal(const Vector3D& n);

		//! Adds a UV.
		void AddUV(const Vector2D& t);

		//! Adds a triangle with points.
		void AddPointTriangle(const Point3UI& newPointIndices);

		//! Adds a triangle with point and normal.
		void AddPointNormalTriangle(
			const Point3UI& newPointIndices,
			const Point3UI& newNormalIndices);

		//! Adds a triangle with point, normal, and UV.
		void AddPointUVNormalTriangle(
			const Point3UI& newPointIndices,
			const Point3UI& newUVIndices,
			const Point3UI& newNormalIndices);

		//! Adds a triangle with point and UV.
		void AddPointUVTriangle(
			const Point3UI& newPointIndices,
			const Point3UI& newUVIndices);

		//! Add a triangle.
		void AddTriangle(const Triangle3& tri);

		//! Sets entire normals to the face normals.
		void SetFaceNormal();

		//! Sets angle weighted vertex normal.
		void SetAngleWeightedVertexNormal();

		//! Scales the mesh by given factor.
		void Scale(double factor);

		//! Translates the mesh.
		void Translate(const Vector3D& t);

		//! Rotates the mesh.
		void Rotate(const QuaternionD& q);

		//! Writes the mesh in obj format to the output stream.
		void WriteObj(std::ostream* stream) const;

		//! Writes the mesh in obj format to the file.
		bool WriteObj(const std::string& fileName) const;

		//! Reads the mesh in obj format from the input stream.
		bool ReadObj(std::istream* stream);

		//! Reads the mesh in obj format from the file.
		bool ReadObj(const std::string& fileName);

		//! Copies \p other mesh.
		TriangleMesh3& operator=(const TriangleMesh3& other);

		//! Returns builder fox TriangleMesh3.
		static Builder GetBuilder();

	protected:
		Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

		double ClosestDistanceLocal(const Vector3D& otherPoint) const override;

		bool IntersectsLocal(const Ray3D& ray) const override;

		BoundingBox3D BoundingBoxLocal() const override;

		Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const override;

		SurfaceRayIntersection3 ClosestIntersectionLocal(const Ray3D& ray) const override;

	private:
		PointArray m_points;
		NormalArray m_normals;
		UVArray m_uvs;
		IndexArray m_pointIndices;
		IndexArray m_normalIndices;
		IndexArray m_uvIndices;

		mutable BVH3<size_t> m_bvh;
		mutable bool m_bvhInvalidated = true;

		void InvalidateBVH() const;

		void BuildBVH() const;
	};

	//! Shared pointer for the TriangleMesh3 type.
	using TriangleMesh3Ptr = std::shared_ptr<TriangleMesh3>;

	//!
	//! \brief Front-end to create TriangleMesh3 objects step by step.
	//!
	class TriangleMesh3::Builder final : public SurfaceBuilderBase3<TriangleMesh3::Builder>
	{
	public:
		//! Returns builder with points.
		Builder& WithPoints(const PointArray& points);

		//! Returns builder with normals.
		Builder& WithNormals(const NormalArray& normals);

		//! Returns builder with uvs.
		Builder& WithUVs(const UVArray& uvs);

		//! Returns builder with point indices.
		Builder& WithPointIndices(const IndexArray& pointIndices);

		//! Returns builder with normal indices.
		Builder& WithNormalIndices(const IndexArray& normalIndices);

		//! Returns builder with uv indices.
		Builder& WithUVIndices(const IndexArray& uvIndices);

		//! Builds TriangleMesh3.
		TriangleMesh3 Build() const;

		//! Builds shared pointer of TriangleMesh3 instance.
		TriangleMesh3Ptr MakeShared() const;

	private:
		PointArray m_points;
		NormalArray m_normals;
		UVArray m_uvs;
		IndexArray m_pointIndices;
		IndexArray m_normalIndices;
		IndexArray m_uvIndices;
	};
}

#endif