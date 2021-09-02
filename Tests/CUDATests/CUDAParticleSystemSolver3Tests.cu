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

#include <Core/CUDA/CUDAParticleSystemSolver3.hpp>

using namespace CubbyFlow;

TEST_CASE("[CUDAParticleSystemSolver3 - PerfectBounce]")
{
    CUDAParticleSystemSolver3 solver;
    solver.SetDragCoefficient(0.0);
    solver.SetRestitutionCoefficient(1.0);

    auto particles = solver.ParticleSystemData();
    particles->AddParticle({ 0.0f, 3.0f, 0.0f, 0.0f },
                           { 1.0f, 0.0f, 0.0f, 0.0f });

    Array1<float> x(1000);
    Array1<float> y(1000);

    Frame frame;
    frame.timeIntervalInSeconds = 1.0 / 300.0;
    for (; frame.index < 1000; frame.Advance())
    {
        solver.Update(frame);

        float4 pos = particles->Positions()[0];
        x[frame.index] = pos.x;
        y[frame.index] = pos.y;
    }
}