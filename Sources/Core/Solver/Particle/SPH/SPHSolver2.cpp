// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Particle/SPH/SPHStdKernel2.hpp>
#include <Core/Solver/Particle/SPH/SPHSolver2.hpp>
#include <Core/Utils/Logging.hpp>
#include <Core/Utils/PhysicsHelpers.hpp>
#include <Core/Utils/Timer.hpp>

namespace CubbyFlow
{
static double TIME_STEP_LIMIT_BY_SPEED_FACTOR = 0.4;
static double TIME_STEP_LIMIT_BY_FORCE_FACTOR = 0.25;

SPHSolver2::SPHSolver2()
{
    SetParticleSystemData(std::make_shared<SPHSystemData2>());
    SetIsUsingFixedSubTimeSteps(false);
}

SPHSolver2::SPHSolver2(double targetDensity, double targetSpacing,
                       double relativeKernelRadius)
{
    auto sphParticles = std::make_shared<SPHSystemData2>();
    SetParticleSystemData(sphParticles);
    sphParticles->SetTargetDensity(targetDensity);
    sphParticles->SetTargetSpacing(targetSpacing);
    sphParticles->SetRelativeKernelRadius(relativeKernelRadius);
    SetIsUsingFixedSubTimeSteps(false);
}

double SPHSolver2::GetEosExponent() const
{
    return m_eosExponent;
}

void SPHSolver2::SetEosExponent(double newEosExponent)
{
    m_eosExponent = std::max(newEosExponent, 1.0);
}

double SPHSolver2::GetNegativePressureScale() const
{
    return m_negativePressureScale;
}

void SPHSolver2::SetNegativePressureScale(double newNegativePressureScale)
{
    m_negativePressureScale = std::clamp(newNegativePressureScale, 0.0, 1.0);
}

double SPHSolver2::GetViscosityCoefficient() const
{
    return m_viscosityCoefficient;
}

void SPHSolver2::SetViscosityCoefficient(double newViscosityCoefficient)
{
    m_viscosityCoefficient = std::max(newViscosityCoefficient, 0.0);
}

double SPHSolver2::GetPseudoViscosityCoefficient() const
{
    return m_pseudoViscosityCoefficient;
}

void SPHSolver2::SetPseudoViscosityCoefficient(
    double newPseudoViscosityCoefficient)
{
    m_pseudoViscosityCoefficient = std::max(newPseudoViscosityCoefficient, 0.0);
}

double SPHSolver2::GetSpeedOfSound() const
{
    return m_speedOfSound;
}

void SPHSolver2::SetSpeedOfSound(double newSpeedOfSound)
{
    m_speedOfSound =
        std::max(newSpeedOfSound, std::numeric_limits<double>::epsilon());
}

double SPHSolver2::GetTimeStepLimitScale() const
{
    return m_timeStepLimitScale;
}

void SPHSolver2::SetTimeStepLimitScale(double newScale)
{
    m_timeStepLimitScale = std::max(newScale, 0.0);
}

SPHSystemData2Ptr SPHSolver2::GetSPHSystemData() const
{
    return std::dynamic_pointer_cast<SPHSystemData2>(GetParticleSystemData());
}

unsigned int SPHSolver2::GetNumberOfSubTimeSteps(
    double timeIntervalInSeconds) const
{
    SPHSystemData2Ptr particles = GetSPHSystemData();
    const size_t numberOfParticles = particles->GetNumberOfParticles();
    ArrayAccessor1<Vector2D> f = particles->GetForces();

    const double kernelRadius = particles->GetKernelRadius();
    const double mass = particles->GetMass();

    double maxForceMagnitude = 0.0;

    for (size_t i = 0; i < numberOfParticles; ++i)
    {
        maxForceMagnitude = std::max(maxForceMagnitude, f[i].Length());
    }

    const double timeStepLimitBySpeed =
        TIME_STEP_LIMIT_BY_SPEED_FACTOR * kernelRadius / m_speedOfSound;
    const double timeStepLimitByForce =
        TIME_STEP_LIMIT_BY_FORCE_FACTOR *
        std::sqrt(kernelRadius * mass / maxForceMagnitude);

    const double desiredTimeStep =
        m_timeStepLimitScale *
        std::min(timeStepLimitBySpeed, timeStepLimitByForce);

    return static_cast<unsigned int>(
        std::ceil(timeIntervalInSeconds / desiredTimeStep));
}

void SPHSolver2::AccumulateForces(double timeStepInSeconds)
{
    AccumulateNonPressureForces(timeStepInSeconds);
    AccumulatePressureForce(timeStepInSeconds);
}

void SPHSolver2::OnBeginAdvanceTimeStep(double timeStepInSeconds)
{
    UNUSED_VARIABLE(timeStepInSeconds);

    SPHSystemData2Ptr particles = GetSPHSystemData();

    const Timer timer;
    particles->BuildNeighborSearcher();
    particles->BuildNeighborLists();
    particles->UpdateDensities();

    CUBBYFLOW_INFO << "Building neighbor lists and updating densities took "
                   << timer.DurationInSeconds() << " seconds";
}

void SPHSolver2::OnEndAdvanceTimeStep(double timeStepInSeconds)
{
    ComputePseudoViscosity(timeStepInSeconds);

    SPHSystemData2Ptr particles = GetSPHSystemData();
    const size_t numberOfParticles = particles->GetNumberOfParticles();
    ArrayAccessor1<double> densities = particles->GetDensities();

    double maxDensity = 0.0;
    for (size_t i = 0; i < numberOfParticles; ++i)
    {
        maxDensity = std::max(maxDensity, densities[i]);
    }

    CUBBYFLOW_INFO << "Max density: " << maxDensity << " "
                   << "Max density / target density ratio: "
                   << maxDensity / particles->GetTargetDensity();
}

void SPHSolver2::AccumulateNonPressureForces(double timeStepInSeconds)
{
    ParticleSystemSolver2::AccumulateForces(timeStepInSeconds);
    AccumulateViscosityForce();
}

void SPHSolver2::AccumulatePressureForce(double timeStepInSeconds)
{
    UNUSED_VARIABLE(timeStepInSeconds);

    SPHSystemData2Ptr particles = GetSPHSystemData();
    const ArrayAccessor1<Vector2D> x = particles->GetPositions();
    const ArrayAccessor1<double> d = particles->GetDensities();
    const ArrayAccessor1<double> p = particles->GetPressures();
    const ArrayAccessor1<Vector2D> f = particles->GetForces();

    ComputePressure();
    AccumulatePressureForce(x, d, p, f);
}

void SPHSolver2::ComputePressure()
{
    SPHSystemData2Ptr particles = GetSPHSystemData();
    const size_t numberOfParticles = particles->GetNumberOfParticles();
    ArrayAccessor1<double> d = particles->GetDensities();
    ArrayAccessor1<double> p = particles->GetPressures();

    // See Murnaghan-Tait equation of state from
    // https://en.wikipedia.org/wiki/Tait_equation
    const double targetDensity = particles->GetTargetDensity();
    const double eosScale = targetDensity * Square(m_speedOfSound);

    ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i) {
        p[i] = ComputePressureFromEos(d[i], targetDensity, eosScale,
                                      GetEosExponent(),
                                      GetNegativePressureScale());
    });
}

void SPHSolver2::AccumulatePressureForce(
    const ConstArrayAccessor1<Vector2D>& positions,
    const ConstArrayAccessor1<double>& densities,
    const ConstArrayAccessor1<double>& pressures,
    ArrayAccessor1<Vector2D> pressureForces)
{
    SPHSystemData2Ptr particles = GetSPHSystemData();
    const size_t numberOfParticles = particles->GetNumberOfParticles();

    const double massSquared = Square(particles->GetMass());
    const SPHSpikyKernel2 kernel{ particles->GetKernelRadius() };

    ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i) {
        const std::vector<size_t>& neighbors = particles->GetNeighborLists()[i];
        for (size_t j : neighbors)
        {
            const double dist = positions[i].DistanceTo(positions[j]);
            if (dist > 0.0)
            {
                Vector2D dir = (positions[j] - positions[i]) / dist;
                pressureForces[i] -=
                    massSquared *
                    (pressures[i] / (densities[i] * densities[i]) +
                     pressures[j] / (densities[j] * densities[j])) *
                    kernel.Gradient(dist, dir);
            }
        }
    });
}

void SPHSolver2::AccumulateViscosityForce()
{
    SPHSystemData2Ptr particles = GetSPHSystemData();
    const size_t numberOfParticles = particles->GetNumberOfParticles();
    ArrayAccessor1<Vector2D> x = particles->GetPositions();
    ArrayAccessor1<Vector2D> v = particles->GetVelocities();
    ArrayAccessor1<double> d = particles->GetDensities();
    ArrayAccessor1<Vector2D> f = particles->GetForces();

    const double massSquared = Square(particles->GetMass());
    const SPHSpikyKernel2 kernel{ particles->GetKernelRadius() };

    ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i) {
        const auto& neighbors = particles->GetNeighborLists()[i];
        for (size_t j : neighbors)
        {
            const double dist = x[i].DistanceTo(x[j]);

            f[i] += GetViscosityCoefficient() * massSquared * (v[j] - v[i]) /
                    d[j] * kernel.SecondDerivative(dist);
        }
    });
}

void SPHSolver2::ComputePseudoViscosity(double timeStepInSeconds)
{
    SPHSystemData2Ptr particles = GetSPHSystemData();
    const size_t numberOfParticles = particles->GetNumberOfParticles();
    ArrayAccessor1<Vector2D> x = particles->GetPositions();
    ArrayAccessor1<Vector2D> v = particles->GetVelocities();
    ArrayAccessor1<double> d = particles->GetDensities();

    const double mass = particles->GetMass();
    const SPHSpikyKernel2 kernel{ particles->GetKernelRadius() };

    Array1<Vector2D> smoothedVelocities{ numberOfParticles };

    ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i) {
        double weightSum = 0.0;
        Vector2D smoothedVelocity;

        const auto& neighbors = particles->GetNeighborLists()[i];
        for (size_t j : neighbors)
        {
            const double dist = x[i].DistanceTo(x[j]);
            const double wj = mass / d[j] * kernel(dist);
            weightSum += wj;
            smoothedVelocity += wj * v[j];
        }

        const double wi = mass / d[i];
        weightSum += wi;
        smoothedVelocity += wi * v[i];

        if (weightSum > 0.0)
        {
            smoothedVelocity /= weightSum;
        }

        smoothedVelocities[i] = smoothedVelocity;
    });

    double factor = timeStepInSeconds * m_pseudoViscosityCoefficient;
    factor = std::clamp(factor, 0.0, 1.0);

    ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i) {
        v[i] = Lerp(v[i], smoothedVelocities[i], factor);
    });
}

SPHSolver2::Builder SPHSolver2::GetBuilder()
{
    return Builder{};
}

SPHSolver2 SPHSolver2::Builder::Build() const
{
    return SPHSolver2{ m_targetDensity, m_targetSpacing,
                       m_relativeKernelRadius };
}

SPHSolver2Ptr SPHSolver2::Builder::MakeShared() const
{
    return std::shared_ptr<SPHSolver2>(
        new SPHSolver2{ m_targetDensity, m_targetSpacing,
                        m_relativeKernelRadius },
        [](SPHSolver2* obj) { delete obj; });
}
}  // namespace CubbyFlow