#include "pch.h"
#include "UnitTestsUtils.h"

#include <Core/Geometry/Triangle3.hpp>

using namespace CubbyFlow;

TEST(Triangle3, Constructors)
{
	Triangle3 tri1;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			EXPECT_DOUBLE_EQ(0.0, tri1.points[i][j]);
			EXPECT_DOUBLE_EQ(0.0, tri1.normals[i][j]);
		}
		for (int j = 0; j < 2; ++j)
		{
			EXPECT_DOUBLE_EQ(0.0, tri1.uvs[i][j]);
		}
	}

	std::array<Vector3D, 3> points = { { Vector3D(1, 2, 3), Vector3D(4, 5, 6), Vector3D(7, 8, 9) } };
	std::array<Vector3D, 3> normals = { { Vector3D(1, 0, 0), Vector3D(0, 1, 0), Vector3D(0, 0, 1) } };
	std::array<Vector2D, 3> uvs = { { Vector2D(1, 0), Vector2D(0, 1), Vector2D(0.5, 0.5) } };

	Triangle3 tri2(points, normals, uvs);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			EXPECT_DOUBLE_EQ(points[i][j], tri2.points[i][j]);
			EXPECT_DOUBLE_EQ(normals[i][j], tri2.normals[i][j]);
		}
		for (int j = 0; j < 2; ++j)
		{
			EXPECT_DOUBLE_EQ(uvs[i][j], tri2.uvs[i][j]);
		}
	}

	Triangle3 tri3(tri2);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			EXPECT_DOUBLE_EQ(points[i][j], tri3.points[i][j]);
			EXPECT_DOUBLE_EQ(normals[i][j], tri3.normals[i][j]);
		}
		for (int j = 0; j < 2; ++j)
		{
			EXPECT_DOUBLE_EQ(uvs[i][j], tri3.uvs[i][j]);
		}
	}
}

TEST(Triangle3, BasicGetters)
{
	Triangle3 tri;
	tri.points = { { Vector3D(0, 0, -1), Vector3D(1, 0, -1), Vector3D(0, 1, -1) } };

	EXPECT_DOUBLE_EQ(0.5, tri.Area());

	double b0, b1, b2;
	tri.GetBarycentricCoords(Vector3D(0.5, 0.5, -1), &b0, &b1, &b2);
	EXPECT_DOUBLE_EQ(0.0, b0);
	EXPECT_DOUBLE_EQ(0.5, b1);
	EXPECT_DOUBLE_EQ(0.5, b2);

	Vector3D n = tri.FaceNormal();
	EXPECT_VECTOR3_EQ(Vector3D(0, 0, 1), n);
}

TEST(Triangle3, SurfaceGetters)
{
	Triangle3 tri;
	tri.points = { { Vector3D(0, 0, -1), Vector3D(1, 0, -1), Vector3D(0, 1, -1) } };
	tri.normals = { { Vector3D(1, 0, 0), Vector3D(0, 1, 0), Vector3D(0, 0, 1) } };
	
	Vector3D cp1 = tri.ClosestPoint({ 0.4, 0.4, 3.0 });
	EXPECT_VECTOR3_EQ(Vector3D(0.4, 0.4, -1), cp1);

	Vector3D cp2 = tri.ClosestPoint({ -3.0, -3.0, 0.0 });
	EXPECT_VECTOR3_EQ(Vector3D(0, 0, -1), cp2);

	Vector3D cn1 = tri.ClosestNormal({ 0.4, 0.4, 3.0 });
	EXPECT_VECTOR3_EQ(Vector3D(1, 2, 2).Normalized(), cn1);
	
	Vector3D cn2 = tri.ClosestNormal({ -3.0, -3.0, 0.0 });
	EXPECT_VECTOR3_EQ(Vector3D(1, 0, 0), cn2);

	bool ints1 = tri.Intersects(Ray3D({ 0.4, 0.4, -5.0 }, { 0, 0, 1 }));
	EXPECT_TRUE(ints1);

	bool ints2 = tri.Intersects(Ray3D({ -1, 2, 3 }, { 0, 0, -1 }));
	EXPECT_FALSE(ints2);

	bool ints3 = tri.Intersects(Ray3D({ 1, 1, 0 }, { 0, 0, -1 }));
	EXPECT_FALSE(ints3);

	auto cints1 = tri.ClosestIntersection(Ray3D({ 0.4, 0.4, -5.0 }, { 0, 0, 1 }));
	EXPECT_TRUE(cints1.isIntersecting);
	EXPECT_VECTOR3_EQ(Vector3D(0.4, 0.4, -1), cints1.point);
	EXPECT_DOUBLE_EQ(4.0, cints1.distance);
	EXPECT_VECTOR3_EQ(Vector3D(1, 2, 2).Normalized(), cints1.normal);

}

TEST(Triangle3, Builder)
{
	std::array<Vector3D, 3> points = { { Vector3D(1, 2, 3), Vector3D(4, 5, 6), Vector3D(7, 8, 9) } };
	std::array<Vector3D, 3> normals = { { Vector3D(1, 0, 0), Vector3D(0, 1, 0), Vector3D(0, 0, 1) } };
	std::array<Vector2D, 3> uvs = { { Vector2D(1, 0), Vector2D(0, 1), Vector2D(0.5, 0.5) } };

	Triangle3 tri = Triangle3::GetBuilder()
		.WithPoints(points)
		.WithNormals(normals)
		.WithUVs(uvs)
		.Build();

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			EXPECT_DOUBLE_EQ(points[i][j], tri.points[i][j]);
			EXPECT_DOUBLE_EQ(normals[i][j], tri.normals[i][j]);
		}
		for (int j = 0; j < 2; ++j)
		{
			EXPECT_DOUBLE_EQ(uvs[i][j], tri.uvs[i][j]);
		}
	}
}