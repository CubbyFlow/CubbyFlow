#include "benchmark/benchmark.h"

#include <Core/Geometry/TriangleMesh3.h>
#include <Core/Vector/Vector3.h>

#include <fstream>
#include <random>

using CubbyFlow::Vector3D;

class TriangleMesh3 : public ::benchmark::Fixture
{
protected:
    std::mt19937 rng{ 0 };
    std::uniform_real_distribution<> dist{ 0.0, 1.0 };
    CubbyFlow::TriangleMesh3 triMesh;

    void SetUp(const ::benchmark::State&)
    {
        std::ifstream file(RESOURCES_DIR "bunny.obj");

        if (file)
        {
            triMesh.ReadObj(&file);
            file.close();
        }
    }

    Vector3D MakeVec()
    {
        return Vector3D(dist(rng), dist(rng), dist(rng));
    }
};

BENCHMARK_DEFINE_F(TriangleMesh3, ClosestPoint)(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        benchmark::DoNotOptimize(triMesh.ClosestPoint(MakeVec()));
    }
}

BENCHMARK_REGISTER_F(TriangleMesh3, ClosestPoint);