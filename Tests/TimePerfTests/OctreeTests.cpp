#include "benchmark/benchmark.h"

#include <Core/BoundingBox/BoundingBox3.hpp>
#include <Core/Geometry/Octree.hpp>
#include <Core/Geometry/Triangle3.hpp>
#include <Core/Geometry/TriangleMesh3.hpp>
#include <Core/Ray/Ray3.hpp>
#include <Core/Vector/Vector3.hpp>

#include <fstream>
#include <random>

using CubbyFlow::TriangleMesh3;
using CubbyFlow::Triangle3;
using CubbyFlow::BoundingBox3D;
using CubbyFlow::Vector3D;
using CubbyFlow::Ray3D;

class Octree : public ::benchmark::Fixture
{
public:
	std::mt19937 rng{ 0 };
	std::uniform_real_distribution<> dist{ 0.0, 1.0 };
	TriangleMesh3 triMesh;
	CubbyFlow::Octree<Triangle3> queryEngine;

	void SetUp(const ::benchmark::State&) 
	{
		std::ifstream file(RESOURCES_DIR "bunny.obj");

		if (file)
		{
			triMesh.ReadObj(&file);
			file.close();
		}

		std::vector<Triangle3> triangles;
		BoundingBox3D bound;
		for (size_t i = 0; i < triMesh.NumberOfTriangles(); ++i)
		{
			auto tri = triMesh.Triangle(i);
			triangles.push_back(tri);
			bound.Merge(tri.BoundingBox());
		}

		const auto triBoxTestFunc = [](const Triangle3& tri, const BoundingBox3D& box)
		{
			// TODO: Implement actual intersecting test
			return tri.BoundingBox().Overlaps(box);
		};

		queryEngine.Build(triangles, bound, triBoxTestFunc, 6);
	}

	Vector3D MakeVec()
	{
		return Vector3D(dist(rng), dist(rng), dist(rng));
	}

	static double DistanceFunc(const Triangle3& tri, const Vector3D& pt)
	{
		return tri.ClosestDistance(pt);
	}

	static bool IntersectsFunc(const Triangle3& tri, const Ray3D& ray)
	{
		return tri.Intersects(ray);
	}
};

BENCHMARK_DEFINE_F(Octree, Nearest)(benchmark::State& state)
{
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(queryEngine.GetNearestNeighbor(MakeVec(), DistanceFunc));
	}
}

BENCHMARK_REGISTER_F(Octree, Nearest);

BENCHMARK_DEFINE_F(Octree, RayIntersects)(benchmark::State& state)
{
	while (state.KeepRunning())
	{
		benchmark::DoNotOptimize(queryEngine.IsIntersects(Ray3D(MakeVec(), MakeVec().Normalized()), IntersectsFunc));
	}
}

BENCHMARK_REGISTER_F(Octree, RayIntersects);