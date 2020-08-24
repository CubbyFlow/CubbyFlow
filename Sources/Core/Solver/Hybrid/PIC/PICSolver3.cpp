// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Array/ArrayUtils.hpp>
#include <Core/Grid/CellCenteredScalarGrid3.hpp>
#include <Core/Solver/Hybrid/PIC/PICSolver3.hpp>
#include <Core/Utils/Logging.hpp>
#include <Core/Utils/Timer.hpp>

namespace CubbyFlow
{
PICSolver3::PICSolver3() : PICSolver3({ 1, 1, 1 }, { 1, 1, 1 }, { 0, 0, 0 })
{
    // Do nothing
}

PICSolver3::PICSolver3(const Size3& resolution, const Vector3D& gridSpacing,
                       const Vector3D& gridOrigin)
    : GridFluidSolver3(resolution, gridSpacing, gridOrigin)
{
    auto grids = GetGridSystemData();
    m_signedDistanceFieldID = grids->AddScalarData(
        std::make_shared<CellCenteredScalarGrid3::Builder>(),
        std::numeric_limits<double>::max());
    m_particles = std::make_shared<ParticleSystemData3>();
}

PICSolver3::~PICSolver3()
{
    // Do nothing
}

ScalarGrid3Ptr PICSolver3::GetSignedDistanceField() const
{
    return GetGridSystemData()->GetScalarDataAt(m_signedDistanceFieldID);
}

const ParticleSystemData3Ptr& PICSolver3::GetParticleSystemData() const
{
    return m_particles;
}

const ParticleEmitter3Ptr& PICSolver3::GetParticleEmitter() const
{
    return m_particleEmitter;
}

void PICSolver3::SetParticleEmitter(const ParticleEmitter3Ptr& newEmitter)
{
    m_particleEmitter = newEmitter;
    newEmitter->SetTarget(m_particles);
}

void PICSolver3::OnInitialize()
{
    GridFluidSolver3::OnInitialize();

    Timer timer;
    UpdateParticleEmitter(0.0);
    CUBBYFLOW_INFO << "Update particle emitter took "
                   << timer.DurationInSeconds() << " seconds";
}

void PICSolver3::OnBeginAdvanceTimeStep(double timeIntervalInSeconds)
{
    CUBBYFLOW_INFO << "Number of PIC-type particles: "
                   << m_particles->GetNumberOfParticles();

    Timer timer;
    UpdateParticleEmitter(timeIntervalInSeconds);
    CUBBYFLOW_INFO << "Update particle emitter took "
                   << timer.DurationInSeconds() << " seconds";

    CUBBYFLOW_INFO << "Number of PIC-type particles: "
                   << m_particles->GetNumberOfParticles();

    timer.Reset();
    TransferFromParticlesToGrids();
    CUBBYFLOW_INFO << "TransferFromParticlesToGrids took "
                   << timer.DurationInSeconds() << " seconds";

    timer.Reset();
    BuildSignedDistanceField();
    CUBBYFLOW_INFO << "BuildSignedDistanceField took "
                   << timer.DurationInSeconds() << " seconds";

    timer.Reset();
    ExtrapolateVelocityToAir();
    CUBBYFLOW_INFO << "ExtrapolateVelocityToAir took "
                   << timer.DurationInSeconds() << " seconds";

    ApplyBoundaryCondition();
}

void PICSolver3::ComputeAdvection(double timeIntervalInSeconds)
{
    Timer timer;
    ExtrapolateVelocityToAir();
    CUBBYFLOW_INFO << "ExtrapolateVelocityToAir took "
                   << timer.DurationInSeconds() << " seconds";

    ApplyBoundaryCondition();

    timer.Reset();
    TransferFromGridsToParticles();
    CUBBYFLOW_INFO << "TransferFromGridsToParticles took "
                   << timer.DurationInSeconds() << " seconds";

    timer.Reset();
    MoveParticles(timeIntervalInSeconds);
    CUBBYFLOW_INFO << "MoveParticles took " << timer.DurationInSeconds()
                   << " seconds";
}

ScalarField3Ptr PICSolver3::GetFluidSDF() const
{
    return GetSignedDistanceField();
}

void PICSolver3::TransferFromParticlesToGrids()
{
    auto flow = GetGridSystemData()->GetVelocity();
    auto positions = m_particles->GetPositions();
    auto velocities = m_particles->GetVelocities();
    size_t numberOfParticles = m_particles->GetNumberOfParticles();

    // Clear velocity to zero
    flow->Fill(Vector3D());

    // Weighted-average velocity
    auto u = flow->GetUAccessor();
    auto v = flow->GetVAccessor();
    auto w = flow->GetWAccessor();
    Array3<double> uWeight(u.size());
    Array3<double> vWeight(v.size());
    Array3<double> wWeight(w.size());
    m_uMarkers.Resize(u.size());
    m_vMarkers.Resize(v.size());
    m_wMarkers.Resize(w.size());
    m_uMarkers.Set(0);
    m_vMarkers.Set(0);
    m_wMarkers.Set(0);
    LinearArraySampler3<double, double> uSampler(
        flow->GetUConstAccessor(), flow->GridSpacing(), flow->GetUOrigin());
    LinearArraySampler3<double, double> vSampler(
        flow->GetVConstAccessor(), flow->GridSpacing(), flow->GetVOrigin());
    LinearArraySampler3<double, double> wSampler(
        flow->GetWConstAccessor(), flow->GridSpacing(), flow->GetWOrigin());
    for (size_t i = 0; i < numberOfParticles; ++i)
    {
        std::array<Point3UI, 8> indices;
        std::array<double, 8> weights;

        uSampler.GetCoordinatesAndWeights(positions[i], &indices, &weights);
        for (int j = 0; j < 8; ++j)
        {
            u(indices[j]) += velocities[i].x * weights[j];
            uWeight(indices[j]) += weights[j];
            m_uMarkers(indices[j]) = 1;
        }

        vSampler.GetCoordinatesAndWeights(positions[i], &indices, &weights);
        for (int j = 0; j < 8; ++j)
        {
            v(indices[j]) += velocities[i].y * weights[j];
            vWeight(indices[j]) += weights[j];
            m_vMarkers(indices[j]) = 1;
        }

        wSampler.GetCoordinatesAndWeights(positions[i], &indices, &weights);
        for (int j = 0; j < 8; ++j)
        {
            w(indices[j]) += velocities[i].z * weights[j];
            wWeight(indices[j]) += weights[j];
            m_wMarkers(indices[j]) = 1;
        }
    }

    uWeight.ParallelForEachIndex([&](size_t i, size_t j, size_t k) {
        if (uWeight(i, j, k) > 0.0)
        {
            u(i, j, k) /= uWeight(i, j, k);
        }
    });
    vWeight.ParallelForEachIndex([&](size_t i, size_t j, size_t k) {
        if (vWeight(i, j, k) > 0.0)
        {
            v(i, j, k) /= vWeight(i, j, k);
        }
    });
    wWeight.ParallelForEachIndex([&](size_t i, size_t j, size_t k) {
        if (wWeight(i, j, k) > 0.0)
        {
            w(i, j, k) /= wWeight(i, j, k);
        }
    });
}

void PICSolver3::TransferFromGridsToParticles()
{
    auto flow = GetGridSystemData()->GetVelocity();
    auto positions = m_particles->GetPositions();
    auto velocities = m_particles->GetVelocities();
    size_t numberOfParticles = m_particles->GetNumberOfParticles();

    ParallelFor(ZERO_SIZE, numberOfParticles,
                [&](size_t i) { velocities[i] = flow->Sample(positions[i]); });
}

void PICSolver3::MoveParticles(double timeIntervalInSeconds)
{
    auto flow = GetGridSystemData()->GetVelocity();
    auto positions = m_particles->GetPositions();
    auto velocities = m_particles->GetVelocities();
    size_t numberOfParticles = m_particles->GetNumberOfParticles();
    int domainBoundaryFlag = GetClosedDomainBoundaryFlag();
    BoundingBox3D boundingBox = flow->BoundingBox();

    ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i) {
        Vector3D pt0 = positions[i];
        Vector3D pt1 = pt0;
        Vector3D vel = velocities[i];

        // Adaptive time-stepping
        unsigned int numSubSteps =
            static_cast<unsigned int>(std::max(GetMaxCFL(), 1.0));
        double dt = timeIntervalInSeconds / numSubSteps;
        for (unsigned int t = 0; t < numSubSteps; ++t)
        {
            Vector3D vel0 = flow->Sample(pt0);

            // Mid-point rule
            Vector3D midPt = pt0 + 0.5 * dt * vel0;
            Vector3D midVel = flow->Sample(midPt);
            pt1 = pt0 + dt * midVel;

            pt0 = pt1;
        }

        if ((domainBoundaryFlag & DIRECTION_LEFT) &&
            pt1.x <= boundingBox.lowerCorner.x)
        {
            pt1.x = boundingBox.lowerCorner.x;
            vel.x = 0.0;
        }
        if ((domainBoundaryFlag & DIRECTION_RIGHT) &&
            pt1.x >= boundingBox.upperCorner.x)
        {
            pt1.x = boundingBox.upperCorner.x;
            vel.x = 0.0;
        }
        if ((domainBoundaryFlag & DIRECTION_DOWN) &&
            pt1.y <= boundingBox.lowerCorner.y)
        {
            pt1.y = boundingBox.lowerCorner.y;
            vel.y = 0.0;
        }
        if ((domainBoundaryFlag & DIRECTION_UP) &&
            pt1.y >= boundingBox.upperCorner.y)
        {
            pt1.y = boundingBox.upperCorner.y;
            vel.y = 0.0;
        }
        if ((domainBoundaryFlag & DIRECTION_BACK) &&
            pt1.z <= boundingBox.lowerCorner.z)
        {
            pt1.z = boundingBox.lowerCorner.z;
            vel.z = 0.0;
        }
        if ((domainBoundaryFlag & DIRECTION_FRONT) &&
            pt1.z >= boundingBox.upperCorner.z)
        {
            pt1.z = boundingBox.upperCorner.z;
            vel.z = 0.0;
        }

        positions[i] = pt1;
        velocities[i] = vel;
    });

    Collider3Ptr col = GetCollider();
    if (col != nullptr)
    {
        ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i) {
            col->ResolveCollision(0.0, 0.0, &positions[i], &velocities[i]);
        });
    }
}

void PICSolver3::ExtrapolateVelocityToAir() const
{
    auto vel = GetGridSystemData()->GetVelocity();
    auto u = vel->GetUAccessor();
    auto v = vel->GetVAccessor();
    auto w = vel->GetWAccessor();

    unsigned int depth = static_cast<unsigned int>(std::ceil(GetMaxCFL()));
    ExtrapolateToRegion(vel->GetUConstAccessor(), m_uMarkers, depth, u);
    ExtrapolateToRegion(vel->GetVConstAccessor(), m_vMarkers, depth, v);
    ExtrapolateToRegion(vel->GetWConstAccessor(), m_wMarkers, depth, w);
}

void PICSolver3::BuildSignedDistanceField()
{
    auto sdf = GetSignedDistanceField();
    auto sdfPos = sdf->GetDataPosition();
    double maxH = std::max(
        { sdf->GridSpacing().x, sdf->GridSpacing().y, sdf->GridSpacing().z });
    double radius = 1.2 * maxH / std::sqrt(2.0);
    double sdfBandRadius = 2.0 * radius;

    m_particles->BuildNeighborSearcher(2 * radius);
    auto searcher = m_particles->GetNeighborSearcher();
    sdf->ParallelForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        Vector3D pt = sdfPos(i, j, k);
        double minDist = sdfBandRadius;

        searcher->ForEachNearbyPoint(
            pt, sdfBandRadius, [&](size_t, const Vector3D& x) {
                minDist = std::min(minDist, pt.DistanceTo(x));
            });
        (*sdf)(i, j, k) = minDist - radius;
    });

    ExtrapolateIntoCollider(sdf.get());
}

void PICSolver3::UpdateParticleEmitter(double timeIntervalInSeconds) const
{
    if (m_particleEmitter != nullptr)
    {
        m_particleEmitter->Update(GetCurrentTimeInSeconds(),
                                  timeIntervalInSeconds);
    }
}

PICSolver3::Builder PICSolver3::GetBuilder()
{
    return Builder();
}

PICSolver3 PICSolver3::Builder::Build() const
{
    return PICSolver3(m_resolution, GetGridSpacing(), m_gridOrigin);
}

PICSolver3Ptr PICSolver3::Builder::MakeShared() const
{
    return std::shared_ptr<PICSolver3>(
        new PICSolver3(m_resolution, GetGridSpacing(), m_gridOrigin),
        [](PICSolver3* obj) { delete obj; });
}
}  // namespace CubbyFlow