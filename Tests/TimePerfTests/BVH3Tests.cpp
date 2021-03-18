#include "benchmark/benchmark.h"

#include <Core/Geometry/BVH3.hpp>
#include <Core/Geometry/Triangle3.hpp>
#include <Core/Geometry/TriangleMesh3.hpp>

#include <fstream>
#include <random>

using CubbyFlow::BoundingBox3D;
using CubbyFlow::Ray3D;
using CubbyFlow::Triangle3;
using CubbyFlow::TriangleMesh3;
using CubbyFlow::Vector3D;

class BVH3 : public ::benchmark::Fixture
{
 public:
    std::mt19937 rng{ 0 };
    std::uniform_real_distribution<> dist{ 0.0, 1.0 };
    TriangleMesh3 triMesh;
    CubbyFlow::BVH3<Triangle3> queryEngine;

    void SetUp(const ::benchmark::State&)
    {
        std::ifstream file(RESOURCES_DIR "bunny.obj");

        if (file)
        {
            [[maybe_unused]] bool isLoaded = triMesh.ReadObj(&file);
            file.close();
        }

        std::vector<Triangle3> triangles;
        std::vector<BoundingBox3D> bounds;
        for (size_t i = 0; i < triMesh.NumberOfTriangles(); ++i)
        {
            auto tri = triMesh.Triangle(i);
            triangles.push_back(tri);
            bounds.push_back(tri.GetBoundingBox());
        }

        queryEngine.Build(triangles, bounds);
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

BENCHMARK_DEFINE_F(BVH3, Nearest)(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(queryEngine.Nearest(MakeVec(), DistanceFunc));
    }
}

BENCHMARK_REGISTER_F(BVH3, Nearest);

BENCHMARK_DEFINE_F(BVH3, RayIntersects)(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(queryEngine.Intersects(
            Ray3D(MakeVec(), MakeVec().Normalized()), IntersectsFunc));
    }
}

BENCHMARK_REGISTER_F(BVH3, RayIntersects);