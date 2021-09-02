// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/CUDA/CUDASPHKernels2.hpp>
#include <Core/CUDA/CUDAWCSPHSolver2.hpp>

using namespace CubbyFlow;

namespace
{
inline __device__ float ComputePressureFromEOS(float density,
                                               float targetDensity,
                                               float eosScale,
                                               float eosExponent,
                                               float negativePressureScale)
{
    // Equation of state
    // (http://www.ifi.uzh.ch/vmml/publications/pcisph/pcisph.pdf)
    float p = eosScale / eosExponent *
              (powf((density / targetDensity), eosExponent) - 1.0f);

    // Negative pressure scaling
    if (p < 0)
    {
        p *= negativePressureScale;
    }

    return p;
}

__global__ void ComputePressureKernel(float targetDensity, float eosScale,
                                      float eosExponent,
                                      float negativePressureScale,
                                      const float* densities, size_t n,
                                      float* pressures)
{
    size_t i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n)
    {
        pressures[i] =
            ComputePressureFromEOS(densities[i], targetDensity, eosScale,
                                   eosExponent, negativePressureScale);
    }
}

__global__ void ComputeForcesKernel(
    float mass, float2 gravity, float viscosity,
    CUDASPHSpikyKernel2 spikyKernel, const uint32_t* neighborStarts,
    const uint32_t* neighborEnds, const uint32_t* neighborLists,
    const float2* positions, const float2* velocities, const float* densities,
    const float* pressures, size_t n, float2* smoothedVelocities,
    float2* forces)
{
    size_t i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n)
    {
        uint32_t ns = neighborStarts[i];
        uint32_t ne = neighborEnds[i];

        float2 x_i = positions[i];
        float2 v_i = velocities[i];
        float d_i = densities[i];
        float p_i = pressures[i];
        float2 f = gravity;

        float massSquared = mass * mass;

        float w_i = mass / d_i * spikyKernel(0.0f);
        float weightSum = w_i;
        float2 smoothedVelocity = w_i * v_i;

        for (uint32_t jj = ns; jj < ne; ++jj)
        {
            uint32_t j = neighborLists[jj];

            float2 r = positions[j] - x_i;
            float dist = Length(r);

            if (dist > 0.0f)
            {
                float2 dir = r / dist;

                float2 v_j = velocities[j];
                float d_j = densities[j];
                float p_j = pressures[j];

                // Pressure force
                f -= massSquared * (p_i / (d_i * d_i) + p_j / (d_j * d_j)) *
                     spikyKernel.Gradient(dist, dir);

                // Viscosity force
                f += viscosity * massSquared * (v_j - v_i) / d_j *
                     spikyKernel.SecondDerivative(dist);

                // Pseudo viscosity
                float w_j = mass / d_j * spikyKernel(dist);
                weightSum += w_j;
                smoothedVelocity += w_j * v_j;
            }
        }

        forces[i] = f;

        smoothedVelocity /= weightSum;
        smoothedVelocities[i] = smoothedVelocity;
    }
}

#define BND_R 0.0f

__global__ void TimeIntegrationKernel(float dt, float mass, float smoothFactor,
                                      float2 lower, float2 upper,
                                      const float2* smoothedVelocities,
                                      const float2* forces, size_t n,
                                      float2* positions, float2* velocities)
{
    size_t i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n)
    {
        float2 x = positions[i];
        float2 v = velocities[i];
        float2 s = smoothedVelocities[i];
        float2 f = forces[i];

        v = (1.0f - smoothFactor) * v + smoothFactor * s;
        v += dt * f / mass;
        x += dt * v;

        // TODO: Add proper collider support
        if (x.x > upper.x)
        {
            x.x = upper.x;
            v.x *= BND_R;
        }
        if (x.x < lower.x)
        {
            x.x = lower.x;
            v.x *= BND_R;
        }
        if (x.y > upper.y)
        {
            x.y = upper.y;
            v.y *= BND_R;
        }
        if (x.y < lower.y)
        {
            x.y = lower.y;
            v.y *= BND_R;
        }

        positions[i] = x;
        velocities[i] = v;
    }
}
}  // namespace

void CUDAWCSPHSolver2::OnAdvanceTimeStep(double timeStepInSeconds)
{
    auto sph = SPHSystemData();
    size_t n = sph->NumberOfParticles();

    // Build neighbor searcher
    sph->BuildNeighborSearcher();
    sph->BuildNeighborListsAndUpdateDensities();

    // Compute pressure
    auto d = sph->Densities();
    auto p = sph->Pressures();
    const float targetDensity = sph->TargetDensity();
    const float eosScale =
        targetDensity * Square(SpeedOfSound()) / m_eosExponent;

    unsigned int numBlocks, numThreads;
    CUDAComputeGridSize(static_cast<unsigned int>(n), 256, numBlocks,
                        numThreads);

    ComputePressureKernel<<<numBlocks, numThreads>>>(
        targetDensity, eosScale, m_eosExponent, NegativePressureScale(),
        d.data(), n, p.data());

    // Compute pressure/viscosity forces and smoothed velocity
    float mass = sph->Mass();
    float h = sph->KernelRadius();
    auto ns = sph->NeighborStarts();
    auto ne = sph->NeighborEnds();
    auto nl = sph->NeighborLists();
    auto x = sph->Positions();
    auto v = sph->Velocities();
    auto s = SmoothedVelocities();
    auto f = Forces();

    ComputeForcesKernel<<<numBlocks, numThreads>>>(
        mass, ToFloat2(Gravity()), ViscosityCoefficient(),
        CUDASPHSpikyKernel2(h), ns.data(), ne.data(), nl.data(), x.data(),
        v.data(), d.data(), p.data(), n, s.data(), f.data());

    // Time-integration
    float dt = static_cast<float>(timeStepInSeconds);
    float factor = dt * PseudoViscosityCoefficient();
    factor = Clamp(factor, 0.0f, 1.0f);
    auto lower = ToFloat2(Container().lowerCorner);
    auto upper = ToFloat2(Container().upperCorner);

    TimeIntegrationKernel<<<numBlocks, numThreads>>>(dt, mass, factor, lower,
                                                     upper, s.data(), f.data(),
                                                     n, x.data(), v.data());
}