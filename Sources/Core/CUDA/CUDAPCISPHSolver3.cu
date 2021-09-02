// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/CUDA/CUDAPCISPHSolver3.hpp>
#include <Core/CUDA/CUDASPHKernels3.hpp>

#include <thrust/extrema.h>

using namespace CubbyFlow;

namespace
{
class InitializeBuffersAndComputeForces
{
 public:
    inline InitializeBuffersAndComputeForces(
        float m, float h, float4 gravity, float viscosity,
        const uint32_t* neighborStarts, const uint32_t* neighborEnds,
        const uint32_t* neighborLists, const float4* positions,
        const float4* velocities, float4* smoothedVelocities, float4* forces,
        const float* densities, float* pressures, float4* pressureForces,
        float* densityErrors, float* densitiesPredicted)
        : m_mass(m),
          m_massSquared(m * m),
          m_gravity(gravity),
          m_viscosity(viscosity),
          m_spikyKernel(h),
          m_neighborStarts(neighborStarts),
          m_neighborEnds(neighborEnds),
          m_neighborLists(neighborLists),
          m_positions(positions),
          m_velocities(velocities),
          m_smoothedVelocities(smoothedVelocities),
          m_forces(forces),
          m_densities(densities),
          m_pressures(pressures),
          m_pressureForces(pressureForces),
          m_densityErrors(densityErrors),
          m_densitiesPredicted(densitiesPredicted)
    {
        // Do nothing
    }

    template <typename Index>
    inline CUBBYFLOW_CUDA_DEVICE void operator()(Index i)
    {
        // Initialize buffers
        m_pressures[i] = 0.0f;
        m_pressureForces[i] = make_float4(0, 0, 0, 0);
        m_densityErrors[i] = 0.0f;
        m_densitiesPredicted[i] = m_densities[i];

        // Compute forces
        uint32_t ns = m_neighborStarts[i];
        uint32_t ne = m_neighborEnds[i];
        float4 x_i = m_positions[i];
        float4 v_i = m_velocities[i];
        float d_i = m_densities[i];
        float4 f = m_gravity;
        float w_i = m_mass / d_i * m_spikyKernel(0.0f);
        float weightSum = w_i;
        float4 smoothedVelocity = w_i * v_i;

        for (uint32_t jj = ns; jj < ne; ++jj)
        {
            uint32_t j = m_neighborLists[jj];

            float4 r = m_positions[j] - x_i;
            float dist = Length(r);

            if (dist > 0.0f)
            {
                float4 v_j = m_velocities[j];
                float d_j = m_densities[j];

                // Viscosity force
                f += m_viscosity * m_massSquared * (v_j - v_i) / d_j *
                     m_spikyKernel.SecondDerivative(dist);

                // Pseudo viscosity
                float w_j = m_mass / d_j * m_spikyKernel(dist);
                weightSum += w_j;
                smoothedVelocity += w_j * v_j;
            }
        }

        m_forces[i] = f;

        smoothedVelocity /= weightSum;
        m_smoothedVelocities[i] = smoothedVelocity;
    }

 private:
    float m_mass;
    float m_massSquared;
    float4 m_gravity;
    float m_viscosity;
    CUDASPHSpikyKernel3 m_spikyKernel;
    const uint32_t* m_neighborStarts;
    const uint32_t* m_neighborEnds;
    const uint32_t* m_neighborLists;
    const float4* m_positions;
    const float4* m_velocities;
    float4* m_smoothedVelocities;
    float4* m_forces;
    const float* m_densities;
    float* m_pressures;
    float4* m_pressureForces;
    float* m_densitiesPredicted;
    float* m_densityErrors;
};

#define BND_R 0.0f

class TimeIntegration
{
 public:
    TimeIntegration(float dt, float m, float smoothFactor, float3 lower,
                    float3 upper, float4* positions, float4* velocities,
                    float4* newPositions, float4* newVelocities,
                    float4* smoothedVelocities, float4* forces,
                    float4* pressureForces)
        : m_dt(dt),
          m_mass(m),
          m_smoothFactor(smoothFactor),
          m_lower(lower),
          m_upper(upper),
          m_positions(positions),
          m_velocities(velocities),
          m_newPositions(newPositions),
          m_newVelocities(newVelocities),
          m_smoothedVelocities(smoothedVelocities),
          m_forces(forces),
          m_pressureForces(pressureForces)
    {
        // Do nothing
    }

    template <typename Index>
    inline CUBBYFLOW_CUDA_HOST_DEVICE void operator()(Index i)
    {
        float4 x = m_positions[i];
        float4 v = m_velocities[i];
        float4 s = m_smoothedVelocities[i];
        float4 f = m_forces[i];
        float4 pf = m_pressureForces[i];

        v = (1.0f - m_smoothFactor) * v + m_smoothFactor * s;
        v += m_dt * (f + pf) / m_mass;
        x += m_dt * v;

        // TODO: Add proper collider support
        if (x.x > m_upper.x)
        {
            x.x = m_upper.x;
            v.x *= BND_R;
        }
        if (x.x < m_lower.x)
        {
            x.x = m_lower.x;
            v.x *= BND_R;
        }
        if (x.y > m_upper.y)
        {
            x.y = m_upper.y;
            v.y *= BND_R;
        }
        if (x.y < m_lower.y)
        {
            x.y = m_lower.y;
            v.y *= BND_R;
        }

        m_newPositions[i] = x;
        m_newVelocities[i] = v;
    }

 private:
    float m_dt;
    float m_mass;
    float m_smoothFactor;
    float3 m_lower;
    float3 m_upper;
    float4* m_positions;
    float4* m_velocities;
    float4* m_newPositions;
    float4* m_newVelocities;
    float4* m_smoothedVelocities;
    float4* m_forces;
    float4* m_pressureForces;
};

class ComputeDensityError
{
 public:
    inline ComputeDensityError(float m, float h, float targetDensity,
                               float delta, float negativePressureScale,
                               const uint32_t* neighborStarts,
                               const uint32_t* neighborEnds,
                               const uint32_t* neighborLists,
                               const float4* positions, float* pressures,
                               float* densityErrors, float* densitiesPredicted)
        : m_mass(m),
          m_targetDensity(targetDensity),
          m_delta(delta),
          m_negativePressureScale(negativePressureScale),
          m_neighborStarts(neighborStarts),
          m_neighborEnds(neighborEnds),
          m_neighborLists(neighborLists),
          m_positions(positions),
          m_pressures(pressures),
          m_densityErrors(densityErrors),
          m_densitiesPredicted(densitiesPredicted),
          m_stdKernel(h)
    {
        // Do nothing
    }

    template <typename Index>
    inline CUBBYFLOW_CUDA_DEVICE void operator()(Index i)
    {
        uint32_t ns = m_neighborStarts[i];
        uint32_t ne = m_neighborEnds[i];
        float4 x_i = m_positions[i];
        float kernelSum = m_stdKernel(0.f);

        for (uint32_t jj = ns; jj < ne; ++jj)
        {
            uint32_t j = m_neighborLists[jj];

            float4 r = m_positions[j] - x_i;
            float dist = Length(r);

            if (dist > 0.0f)
            {
                kernelSum += m_stdKernel(dist);
            }
        }

        float density = m_mass * kernelSum;
        float densityError = (density - m_targetDensity);
        float pressure = m_delta * densityError;

        if (pressure < 0.0f)
        {
            pressure *= m_negativePressureScale;
            densityError *= m_negativePressureScale;
        }

        m_pressures[i] += pressure;
        m_densitiesPredicted[i] = density;
        m_densityErrors[i] = densityError;
    }

 private:
    float m_mass;
    float m_targetDensity;
    float m_delta;
    float m_negativePressureScale;
    const uint32_t* m_neighborStarts;
    const uint32_t* m_neighborEnds;
    const uint32_t* m_neighborLists;
    const float4* m_positions;
    float* m_pressures;
    float* m_densitiesPredicted;
    float* m_densityErrors;
    CUDASPHStdKernel3 m_stdKernel;
};

class ComputePressureForces
{
 public:
    inline ComputePressureForces(float m, float h,
                                 const uint32_t* neighborStarts,
                                 const uint32_t* neighborEnds,
                                 const uint32_t* neighborLists,
                                 const float4* positions,
                                 float4* pressureForces, const float* densities,
                                 const float* pressures)
        : m_mass(m),
          m_massSquared(m * m),
          m_spikyKernel(h),
          m_neighborStarts(neighborStarts),
          m_neighborEnds(neighborEnds),
          m_neighborLists(neighborLists),
          m_positions(positions),
          m_pressureForces(pressureForces),
          m_densities(densities),
          m_pressures(pressures)
    {
        // Do nothing
    }

    template <typename Index>
    inline CUBBYFLOW_CUDA_HOST_DEVICE void operator()(Index i)
    {
        uint32_t ns = m_neighborStarts[i];
        uint32_t ne = m_neighborEnds[i];

        float4 x_i = m_positions[i];
        float d_i = m_densities[i];
        float p_i = m_pressures[i];

        float4 f = make_float4(0, 0, 0, 0);

        for (uint32_t jj = ns; jj < ne; ++jj)
        {
            uint32_t j = m_neighborLists[jj];

            float4 r = m_positions[j] - x_i;
            float dist = Length(r);

            if (dist > 0.0f)
            {
                float4 dir = r / dist;

                float d_j = m_densities[j];
                float p_j = m_pressures[j];

                // Pressure force
                f -= m_massSquared * (p_i / (d_i * d_i) + p_j / (d_j * d_j)) *
                     m_spikyKernel.Gradient(dist, dir);
            }
        }

        m_pressureForces[i] = f;
    }

 private:
    float m_mass;
    float m_massSquared;
    CUDASPHSpikyKernel3 m_spikyKernel;
    const uint32_t* m_neighborStarts;
    const uint32_t* m_neighborEnds;
    const uint32_t* m_neighborLists;
    const float4* m_positions;
    float4* m_pressureForces;
    const float* m_densities;
    const float* m_pressures;
};
}  // namespace

void CUDAPCISPHSolver3::OnAdvanceTimeStep(double timeStepInSeconds)
{
    auto sph = SPHSystemData();

    // Build neighbor searcher
    sph->BuildNeighborSearcher();
    sph->BuildNeighborListsAndUpdateDensities();

    auto d = sph->Densities();
    auto p = sph->Pressures();
    const float targetDensity = sph->TargetDensity();

    size_t n = sph->NumberOfParticles();
    float mass = sph->Mass();
    float h = sph->KernelRadius();
    auto ns = sph->NeighborStarts();
    auto ne = sph->NeighborEnds();
    auto nl = sph->NeighborLists();
    auto x = sph->Positions();
    auto v = sph->Velocities();
    auto s = SmoothedVelocities();
    auto f = Forces();

    float dt = static_cast<float>(timeStepInSeconds);
    float factor = dt * PseudoViscosityCoefficient();
    factor = Clamp(factor, 0.0f, 1.0f);

    auto xs = TempPositions();
    auto vs = TempVelocities();
    auto pf = PressureForces();
    auto ds = TempDensities();
    auto de = DensityErrors();

    float delta = ComputeDelta(dt);

    // Initialize buffers and compute non-pressure forces
    thrust::for_each(
        thrust::counting_iterator<size_t>(0),
        thrust::counting_iterator<size_t>(n),
        InitializeBuffersAndComputeForces(
            mass, h, ToFloat4(Gravity(), 0.0f), ViscosityCoefficient(),
            ns.data(), ne.data(), nl.data(), x.data(), v.data(), s.data(),
            f.data(), d.data(), p.data(), pf.data(), de.data(), ds.data()));

    // Prediction-correction
    auto lower = ToFloat3(Container().lowerCorner);
    auto upper = ToFloat3(Container().upperCorner);

    for (unsigned int k = 0; k < m_maxNumberOfIterations; ++k)
    {
        // Predict velocity / position and resolve collisions
        thrust::for_each(thrust::counting_iterator<size_t>(0),
                         thrust::counting_iterator<size_t>(n),
                         TimeIntegration(dt, mass, 0.0f, lower, upper, x.data(),
                                         v.data(), xs.data(), vs.data(),
                                         s.data(), f.data(), pf.data()));

        // Compute pressure from density error
        thrust::for_each(thrust::counting_iterator<size_t>(0),
                         thrust::counting_iterator<size_t>(n),
                         ComputeDensityError(mass, h, targetDensity, delta,
                                             NegativePressureScale(), ns.data(),
                                             ne.data(), nl.data(), xs.data(),
                                             p.data(), de.data(), ds.data()));

        // Compute pressure gradient force
        thrust::for_each(
            thrust::counting_iterator<size_t>(0),
            thrust::counting_iterator<size_t>(n),
            ComputePressureForces(mass, h, ns.data(), ne.data(), nl.data(),
                                  x.data(), pf.data(), ds.data(), p.data()));
    }

    // Accumulate pressure force and time-integrate
    thrust::for_each(
        thrust::counting_iterator<size_t>(0),
        thrust::counting_iterator<size_t>(n),
        TimeIntegration(dt, mass, factor, lower, upper, x.data(), v.data(),
                        x.data(), v.data(), s.data(), f.data(), pf.data()));
}
