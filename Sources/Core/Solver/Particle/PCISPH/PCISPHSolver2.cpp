// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/PointGenerator/TrianglePointGenerator.hpp>
#include <Core/SPH/SPHStdKernel2.hpp>
#include <Core/Solver/Particle/PCISPH/PCISPHSolver2.hpp>
#include <Core/Utils/Logging.hpp>

namespace CubbyFlow
{
// Heuristically chosen
const double DEFAULT_TIME_STEP_LIMIT_SCALE = 5.0;

PCISPHSolver2::PCISPHSolver2()
{
    SetTimeStepLimitScale(DEFAULT_TIME_STEP_LIMIT_SCALE);
}

PCISPHSolver2::PCISPHSolver2(double targetDensity, double targetSpacing,
                             double relativeKernelRadius)
    : SPHSolver2(targetDensity, targetSpacing, relativeKernelRadius)
{
    SetTimeStepLimitScale(DEFAULT_TIME_STEP_LIMIT_SCALE);
}

PCISPHSolver2::~PCISPHSolver2()
{
    // Do nothing
}

double PCISPHSolver2::GetMaxDensityErrorRatio() const
{
    return m_maxDensityErrorRatio;
}

void PCISPHSolver2::SetMaxDensityErrorRatio(double ratio)
{
    m_maxDensityErrorRatio = std::max(ratio, 0.0);
}

unsigned int PCISPHSolver2::GetMaxNumberOfIterations() const
{
    return m_maxNumberOfIterations;
}

void PCISPHSolver2::SetMaxNumberOfIterations(unsigned int n)
{
    m_maxNumberOfIterations = n;
}

void PCISPHSolver2::AccumulatePressureForce(double timeIntervalInSeconds)
{
    auto particles = GetSPHSystemData();
    const size_t numberOfParticles = particles->GetNumberOfParticles();
    const double delta = ComputeDelta(timeIntervalInSeconds);
    const double targetDensity = particles->GetTargetDensity();
    const double mass = particles->GetMass();

    auto p = particles->GetPressures();
    auto d = particles->GetDensities();
    auto x = particles->GetPositions();
    auto v = particles->GetVelocities();
    auto f = particles->GetForces();

    // Predicted density ds
    Array1<double> ds(numberOfParticles, 0.0);

    SPHStdKernel2 kernel(particles->GetKernelRadius());

    // Initialize buffers
    ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i) {
        p[i] = 0.0;
        m_pressureForces[i] = Vector2D();
        m_densityErrors[i] = 0.0;
        ds[i] = d[i];
    });

    unsigned int maxNumIter = 0;
    double maxDensityError;
    double densityErrorRatio = 0.0;

    for (unsigned int k = 0; k < m_maxNumberOfIterations; ++k)
    {
        // Predict velocity and position
        ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i) {
            m_tempVelocities[i] = v[i] + timeIntervalInSeconds / mass *
                                             (f[i] + m_pressureForces[i]);
            m_tempPositions[i] =
                x[i] + timeIntervalInSeconds * m_tempVelocities[i];
        });

        // Resolve collisions
        ResolveCollision(m_tempPositions, m_tempVelocities);

        // Compute pressure from density error
        ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i) {
            double weightSum = 0.0;
            const auto& neighbors = particles->GetNeighborLists()[i];

            for (size_t j : neighbors)
            {
                double dist = m_tempPositions[j].DistanceTo(m_tempPositions[i]);
                weightSum += kernel(dist);
            }
            weightSum += kernel(0);

            double density = mass * weightSum;
            double densityError = (density - targetDensity);
            double pressure = delta * densityError;

            if (pressure < 0.0)
            {
                pressure *= GetNegativePressureScale();
                densityError *= GetNegativePressureScale();
            }

            p[i] += pressure;
            ds[i] = density;
            m_densityErrors[i] = densityError;
        });

        // Compute pressure gradient force
        m_pressureForces.Set(Vector2D());
        SPHSolver2::AccumulatePressureForce(x, ds.ConstAccessor(), p,
                                            m_pressureForces.Accessor());

        // Compute max density error
        maxDensityError = 0.0;
        for (size_t i = 0; i < numberOfParticles; ++i)
        {
            maxDensityError = AbsMax(maxDensityError, m_densityErrors[i]);
        }

        densityErrorRatio = maxDensityError / targetDensity;
        maxNumIter = k + 1;

        if (std::fabs(densityErrorRatio) < m_maxDensityErrorRatio)
        {
            break;
        }
    }

    CUBBYFLOW_INFO << "Number of PCI iterations: " << maxNumIter;
    CUBBYFLOW_INFO << "Max density error after PCI iteration: "
                   << maxDensityError;

    if (std::fabs(densityErrorRatio) > m_maxDensityErrorRatio)
    {
        CUBBYFLOW_WARN
            << "Max density error ratio is greater than the threshold!";
        CUBBYFLOW_WARN << "Ratio: " << densityErrorRatio
                       << " Threshold: " << m_maxDensityErrorRatio;
    }

    // Accumulate pressure force
    ParallelFor(ZERO_SIZE, numberOfParticles,
                [this, &f](size_t i) { f[i] += m_pressureForces[i]; });
}

void PCISPHSolver2::OnBeginAdvanceTimeStep(double timeStepInSeconds)
{
    SPHSolver2::OnBeginAdvanceTimeStep(timeStepInSeconds);

    // Allocate temp buffers
    size_t numberOfParticles = GetParticleSystemData()->GetNumberOfParticles();
    m_tempPositions.Resize(numberOfParticles);
    m_tempVelocities.Resize(numberOfParticles);
    m_pressureForces.Resize(numberOfParticles);
    m_densityErrors.Resize(numberOfParticles);
}

double PCISPHSolver2::ComputeDelta(double timeStepInSeconds) const
{
    auto particles = GetSPHSystemData();
    const double kernelRadius = particles->GetKernelRadius();

    Array1<Vector2D> points;
    TrianglePointGenerator pointsGenerator;
    Vector2D origin;
    BoundingBox2D sampleBound(origin, origin);
    sampleBound.Expand(1.5 * kernelRadius);

    pointsGenerator.Generate(sampleBound, particles->GetTargetSpacing(),
                             &points);

    SPHSpikyKernel2 kernel(kernelRadius);

    double denom = 0;
    Vector2D denom1;
    double denom2 = 0;

    for (size_t i = 0; i < points.size(); ++i)
    {
        const Vector2D& point = points[i];
        double distanceSquared = point.LengthSquared();

        if (distanceSquared < kernelRadius * kernelRadius)
        {
            double distance = std::sqrt(distanceSquared);
            Vector2D direction =
                (distance > 0.0) ? point / distance : Vector2D();

            // grad(Wij)
            Vector2D gradWij = kernel.Gradient(distance, direction);
            denom1 += gradWij;
            denom2 += gradWij.Dot(gradWij);
        }
    }

    denom += -denom1.Dot(denom1) - denom2;

    return (std::fabs(denom) > 0.0)
               ? -1 / (ComputeBeta(timeStepInSeconds) * denom)
               : 0;
}

double PCISPHSolver2::ComputeBeta(double timeStepInSeconds) const
{
    auto particles = GetSPHSystemData();
    return 2.0 * Square(particles->GetMass() * timeStepInSeconds /
                        particles->GetTargetDensity());
}

PCISPHSolver2::Builder PCISPHSolver2::GetBuilder()
{
    return Builder();
}

PCISPHSolver2 PCISPHSolver2::Builder::Build() const
{
    return PCISPHSolver2(m_targetDensity, m_targetSpacing,
                         m_relativeKernelRadius);
}

PCISPHSolver2Ptr PCISPHSolver2::Builder::MakeShared() const
{
    return std::shared_ptr<PCISPHSolver2>(
        new PCISPHSolver2(m_targetDensity, m_targetSpacing,
                          m_relativeKernelRadius),
        [](PCISPHSolver2* obj) { delete obj; });
}
}  // namespace CubbyFlow