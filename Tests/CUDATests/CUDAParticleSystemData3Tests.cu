// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include "doctest_proxy.hpp"

#include <Core/CUDA/CUDAParticleSystemData3.hpp>
#include <Core/CUDA/CUDAUtils.hpp>

using namespace CubbyFlow;

namespace
{
Vector4F MakeVector4F(float4 f)
{
    return Vector4F{ f.x, f.y, f.z, f.w };
}

struct ForEachCallback
{
    ForEachCallback(int* cnt) : count(cnt)
    {
        // Do nothing
    }

    template <typename Index, typename Float4>
    CUBBYFLOW_CUDA_HOST_DEVICE void operator()(size_t i, Float4 o, Index j,
                                               Float4 pt)
    {
        (void)o, (void)j, (void)pt;
        count[i] += 1;
    }

    int* count;
};

}  // namespace

TEST_CASE("[CUDAParticleSystemData3] - Constructors")
{
    CUDAParticleSystemData3 particleSystem;
    CHECK_EQ(0u, particleSystem.NumberOfParticles());

    CUDAParticleSystemData3 particleSystem2(100);
    CHECK_EQ(100u, particleSystem2.NumberOfParticles());

    size_t a0 = particleSystem2.AddFloatData(2.0f);
    size_t a1 = particleSystem2.AddFloatData(9.0f);
    size_t a2 = particleSystem2.AddVectorData({ 1.0f, -3.0f, 5.0f, 4.0f });
    size_t a3 = particleSystem2.AddIntData(8);

    CUDAParticleSystemData3 particleSystem3(particleSystem2);
    CHECK_EQ(100u, particleSystem3.NumberOfParticles());
    auto as0 = particleSystem3.FloatDataAt(a0);
    for (size_t i = 0; i < 100; ++i)
    {
        CHECK_EQ(2.0f, as0[i]);
    }

    auto as1 = particleSystem3.FloatDataAt(a1);
    for (size_t i = 0; i < 100; ++i)
    {
        CHECK_EQ(9.0f, as1[i]);
    }

    auto as2 = particleSystem3.VectorDataAt(a2);
    for (size_t i = 0; i < 100; ++i)
    {
        float4 f = as2[i];
        CHECK_EQ(1.0f, f.x);
        CHECK_EQ(-3.0f, f.y);
        CHECK_EQ(5.0f, f.z);
        CHECK_EQ(4.0f, f.w);
    }

    auto as3 = particleSystem3.IntDataAt(a3);
    for (size_t i = 0; i < 100; ++i)
    {
        CHECK_EQ(8, as3[i]);
    }
}

TEST_CASE("[CUDAParticleSystemData3] - Resize")
{
    CUDAParticleSystemData3 particleSystem;
    particleSystem.Resize(12);

    CHECK_EQ(12u, particleSystem.NumberOfParticles());
}

TEST_CASE("[CUDAParticleSystemData3] - AddFloatData")
{
    CUDAParticleSystemData3 particleSystem;
    particleSystem.Resize(12);

    size_t a0 = particleSystem.AddFloatData(2.0f);
    size_t a1 = particleSystem.AddFloatData(9.0f);

    CHECK_EQ(12u, particleSystem.NumberOfParticles());
    CHECK_EQ(0u, a0);
    CHECK_EQ(1u, a1);

    auto as0 = particleSystem.FloatDataAt(a0);
    for (size_t i = 0; i < 12; ++i)
    {
        CHECK_EQ(2.0f, as0[i]);
    }

    auto as1 = particleSystem.FloatDataAt(a1);
    for (size_t i = 0; i < 12; ++i)
    {
        CHECK_EQ(9.0f, as1[i]);
    }
}

TEST_CASE("[CUDAParticleSystemData3] - AddVectorData")
{
    CUDAParticleSystemData3 particleSystem;
    particleSystem.Resize(12);

    size_t a0 = particleSystem.AddVectorData(Vector4F(2.0f, 4.0f, -1.0f, 9.0f));
    size_t a1 = particleSystem.AddVectorData(Vector4F(9.0f, -2.0f, 5.0f, 7.0f));

    CHECK_EQ(12u, particleSystem.NumberOfParticles());
    CHECK_EQ(2u, a0);
    CHECK_EQ(3u, a1);

    auto as0 = particleSystem.VectorDataAt(a0);
    for (size_t i = 0; i < 12; ++i)
    {
        CHECK_EQ(Vector4F(2.0f, 4.0f, -1.0f, 9.0f), MakeVector4F(as0[i]));
    }

    auto as1 = particleSystem.VectorDataAt(a1);
    for (size_t i = 0; i < 12; ++i)
    {
        CHECK_EQ(Vector4F(9.0f, -2.0f, 5.0f, 7.0f), MakeVector4F(as1[i]));
    }
}

TEST_CASE("[CUDAParticleSystemData3] - AddParticles")
{
    CUDAParticleSystemData3 particleSystem;
    particleSystem.Resize(12);

    particleSystem.AddParticles(
        Array1<Vector4F>({ Vector4F(1.0f, 2.0f, 3.0f, 4.0f),
                           Vector4F(4.0f, 5.0f, 6.0f, 7.0f) })
            .View(),
        Array1<Vector4F>({ Vector4F(7.0f, 8.0f, 9.0f, 10.0f),
                           Vector4F(8.0f, 7.0f, 6.0f, 5.0f) })
            .View());

    CHECK_EQ(14u, particleSystem.NumberOfParticles());
    auto p = particleSystem.Positions();
    auto v = particleSystem.Velocities();

    CHECK_EQ(Vector4F(1.0f, 2.0f, 3.0f, 4.0f), MakeVector4F(p[12]));
    CHECK_EQ(Vector4F(4.0f, 5.0f, 6.0f, 7.0f), MakeVector4F(p[13]));
    CHECK_EQ(Vector4F(7.0f, 8.0f, 9.0f, 10.0f), MakeVector4F(v[12]));
    CHECK_EQ(Vector4F(8.0f, 7.0f, 6.0f, 5.0f), MakeVector4F(v[13]));
}

TEST_CASE("[CUDAParticleSystemData3] - BuildNeighborSearcher")
{
    CUDAParticleSystemData3 particleSystem;
    Array1<Vector4F> positions = {
        Vector4F{ 0.1f, 0.0f, 0.4f, 0.0f }, Vector4F{ 0.6f, 0.2f, 0.6f, 0.0f },
        Vector4F{ 1.0f, 0.3f, 0.4f, 0.0f }, Vector4F{ 0.9f, 0.2f, 0.2f, 0.0f },
        Vector4F{ 0.8f, 0.4f, 0.9f, 0.0f }, Vector4F{ 0.1f, 0.6f, 0.2f, 0.0f },
        Vector4F{ 0.8f, 0.0f, 0.5f, 0.0f }, Vector4F{ 0.9f, 0.8f, 0.2f, 0.0f },
        Vector4F{ 0.3f, 0.5f, 0.2f, 0.0f }, Vector4F{ 0.1f, 0.6f, 0.6f, 0.0f },
        Vector4F{ 0.1f, 0.2f, 0.1f, 0.0f }, Vector4F{ 0.2f, 0.0f, 0.0f, 0.0f },
        Vector4F{ 0.2f, 0.6f, 0.1f, 0.0f }, Vector4F{ 0.1f, 0.3f, 0.7f, 0.0f },
        Vector4F{ 0.9f, 0.7f, 0.6f, 0.0f }, Vector4F{ 0.4f, 0.5f, 0.1f, 0.0f },
        Vector4F{ 0.1f, 0.1f, 0.6f, 0.0f }, Vector4F{ 0.7f, 0.8f, 1.0f, 0.0f },
        Vector4F{ 0.6f, 0.9f, 0.4f, 0.0f }, Vector4F{ 0.7f, 0.7f, 0.0f, 0.0f }
    };
    particleSystem.AddParticles(positions.View());

    float radius = 0.4f;
    particleSystem.BuildNeighborSearcher(radius);

    auto searcher = particleSystem.NeighborSearcher();
    Vector4F o{ 0.1f, 0.2f, 0.3f, 0.0f };
    CUDAArray1<float4> searchOrigin(1, ToFloat4(o));
    CUDAArray1<int> count(1, 0);
    searcher->ForEachNearbyPoint(searchOrigin.View(), radius,
                                 ForEachCallback(count.data()));

    int ans = 0;
    for (auto p : positions)
    {
        if ((p - o).Length() <= radius)
        {
            ans++;
        }
    }

    CHECK_EQ(ans, count[0]);
}

TEST_CASE("[CUDAParticleSystemData3] - BuildNeighborLists")
{
    CUDAParticleSystemData3 particleSystem;
    Array1<Vector4F> positions = {
        Vector4F{ 0.1f, 0.0f, 0.4f, 0.0f }, Vector4F{ 0.6f, 0.2f, 0.6f, 0.0f },
        Vector4F{ 1.0f, 0.3f, 0.4f, 0.0f }, Vector4F{ 0.9f, 0.2f, 0.2f, 0.0f },
        Vector4F{ 0.8f, 0.4f, 0.9f, 0.0f }, Vector4F{ 0.1f, 0.6f, 0.2f, 0.0f },
        Vector4F{ 0.8f, 0.0f, 0.5f, 0.0f }, Vector4F{ 0.9f, 0.8f, 0.2f, 0.0f },
        Vector4F{ 0.3f, 0.5f, 0.2f, 0.0f }, Vector4F{ 0.1f, 0.6f, 0.6f, 0.0f },
        Vector4F{ 0.1f, 0.2f, 0.1f, 0.0f }, Vector4F{ 0.2f, 0.0f, 0.0f, 0.0f },
        Vector4F{ 0.2f, 0.6f, 0.1f, 0.0f }, Vector4F{ 0.1f, 0.3f, 0.7f, 0.0f },
        Vector4F{ 0.9f, 0.7f, 0.6f, 0.0f }, Vector4F{ 0.4f, 0.5f, 0.1f, 0.0f },
        Vector4F{ 0.1f, 0.1f, 0.6f, 0.0f }, Vector4F{ 0.7f, 0.8f, 1.0f, 0.0f },
        Vector4F{ 0.6f, 0.9f, 0.4f, 0.0f }, Vector4F{ 0.7f, 0.7f, 0.0f, 0.0f }
    };
    particleSystem.AddParticles(positions.View());

    float radius = 0.4f;
    particleSystem.BuildNeighborSearcher(radius);
    particleSystem.BuildNeighborLists(radius);

    Array1<size_t> ansNeighborStarts(positions.Length());
    Array1<size_t> ansNeighborEnds(positions.Length());

    for (size_t i = 0; i < positions.Length(); ++i)
    {
        size_t cnt = 0;

        for (size_t j = 0; j < positions.Length(); ++j)
        {
            if (i != j && (positions[i] - positions[j]).Length() <= radius)
            {
                ++cnt;
            }
        }

        ansNeighborStarts[i] = cnt;
    }

    ansNeighborEnds[0] = ansNeighborStarts[0];
    for (size_t i = 1; i < ansNeighborStarts.Length(); ++i)
    {
        ansNeighborEnds[i] = ansNeighborEnds[i - 1] + ansNeighborStarts[i];
    }
    for (size_t i = 0; i < ansNeighborStarts.Length(); ++i)
    {
        ansNeighborStarts[i] = ansNeighborEnds[i] - ansNeighborStarts[i];
    }

    auto cuNeighborStarts = particleSystem.NeighborStarts();
    auto cuNeighborEnds = particleSystem.NeighborEnds();

    for (size_t i = 0; i < ansNeighborStarts.Length(); ++i)
    {
        CHECK_EQ(ansNeighborStarts[i], cuNeighborStarts[i]);
        CHECK_EQ(ansNeighborEnds[i], cuNeighborEnds[i]);
    }

    auto cuNeighborLists = particleSystem.NeighborLists();
    for (size_t i = 0; i < ansNeighborStarts.Length(); ++i)
    {
        size_t start = ansNeighborStarts[i];
        size_t end = ansNeighborEnds[i];
        for (size_t jj = start; jj < end; ++jj)
        {
            size_t j = cuNeighborLists[jj];
            CHECK_LE((positions[i] - positions[j]).Length(), radius);
        }
    }
}