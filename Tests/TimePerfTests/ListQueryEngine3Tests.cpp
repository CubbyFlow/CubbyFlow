#include "benchmark/benchmark.h"

#include <Core/BoundingBox/BoundingBox3.hpp>
#include <Core/Geometry/Triangle3.hpp>
#include <Core/Geometry/TriangleMesh3.hpp>
#include <Core/QueryEngine/ListQueryEngine3.hpp>
#include <Core/Ray/Ray3.h>
#include <Core/Vector/Vector3.h>

#include <fstream>
#include <random>

using CubbyFlow::TriangleMesh3;
using CubbyFlow::Triangle3;
using CubbyFlow::BoundingBox3D;
using CubbyFlow::Vector3D;
using CubbyFlow::Ray3D;

class ListQueryEngine3 : public ::benchmark::Fixture
{
public:
    std::mt19937 rng{ 0 };
    std::uniform_real_distribution<> dist{ 0.0, 1.0 };
    TriangleMesh3 triMesh;
    CubbyFlow::ListQueryEngine3<Triangle3> queryEngine;

    void SetUp(const ::benchmark::State&)
    {
        std::ifstream file(RESOURCES_DIR "bunny.obj");

        if (file)
        {
            triMesh.ReadObj(&file);
            file.close();
        }

        for (size_t i = 0; i < triMesh.NumberOfTriangles(); ++i)
        {
            auto tri = triMesh.Triangle(i);
            queryEngine.Add(tri);
        }
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

BENCHMARK_DEFINE_F(ListQueryEngine3, Nearest)(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(queryEngine.GetNearestNeighbor(MakeVec(), DistanceFunc));
    }
}

BENCHMARK_REGISTER_F(ListQueryEngine3, Nearest);

BENCHMARK_DEFINE_F(ListQueryEngine3, RayIntersects)(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(queryEngine.IsIntersects(Ray3D(MakeVec(), MakeVec().Normalized()), IntersectsFunc));
    }
}

BENCHMARK_REGISTER_F(ListQueryEngine3, RayIntersects);