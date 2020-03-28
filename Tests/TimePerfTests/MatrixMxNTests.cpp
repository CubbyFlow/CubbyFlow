#include "benchmark/benchmark.h"

#include <Core/Matrix/MatrixMxN.hpp>
#include <Core/Vector/VectorN.h>

#include <random>

using CubbyFlow::VectorND;

class MatrixMxN : public ::benchmark::Fixture
{
protected:
    CubbyFlow::MatrixMxND mat;
    VectorND x;
    VectorND y;

    void SetUp(const ::benchmark::State& state)
    {
        std::mt19937 rng{ 0 };
        std::uniform_real_distribution<> d(0.0, 1.0);

        const auto n = static_cast<size_t>(state.range(0));

        mat.Resize(n, n);
        x.Resize(n);
        y.Resize(n);
        mat.ForEachIndex([&](size_t i, size_t j)
        {
            mat(i, j) = d(rng);
        });
        x.ForEachIndex([&](size_t i)
        {
            x[i] = d(rng);
            y[i] = d(rng);
        });
    }
};

BENCHMARK_DEFINE_F(MatrixMxN, MVM)(benchmark::State& state)
{
    while (state.KeepRunning())
    {
        y = mat * x;
    }
}

BENCHMARK_REGISTER_F(MatrixMxN, MVM)->Arg(1 << 8)->Arg(1 << 10)->Arg(1 << 12);