// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Solver/Hybrid/APIC/APICSolver3.hpp>

namespace CubbyFlow
{
APICSolver3::APICSolver3() : APICSolver3({ 1, 1, 1 }, { 1, 1, 1 }, { 0, 0, 0 })
{
    // Do nothing
}

APICSolver3::APICSolver3(const Size3& resolution, const Vector3D& GridSpacing,
                         const Vector3D& gridOrigin)
    : PICSolver3(resolution, GridSpacing, gridOrigin)
{
    // Do nothing
}

APICSolver3::~APICSolver3()
{
    // Do nothing
}

void APICSolver3::TransferFromParticlesToGrids()
{
    auto flow = GetGridSystemData()->GetVelocity();
    const auto particles = GetParticleSystemData();
    const auto positions = particles->GetPositions();
    auto velocities = particles->GetVelocities();
    const size_t numberOfParticles = particles->GetNumberOfParticles();
    const auto hh = flow->GridSpacing() / 2.0;
    const auto bbox = flow->BoundingBox();

    // Allocate buffers
    m_cX.Resize(numberOfParticles);
    m_cY.Resize(numberOfParticles);
    m_cZ.Resize(numberOfParticles);

    // Clear velocity to zero
    flow->Fill(Vector3D());

    // Weighted-average velocity
    auto u = flow->GetUAccessor();
    auto v = flow->GetVAccessor();
    auto w = flow->GetWAccessor();
    const auto uPos = flow->GetUPosition();
    const auto vPos = flow->GetVPosition();
    const auto wPos = flow->GetWPosition();
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

        auto uPosClamped = positions[i];
        uPosClamped.y = std::clamp(uPosClamped.y, bbox.lowerCorner.y + hh.y,
                                   bbox.upperCorner.y - hh.y);
        uPosClamped.z = std::clamp(uPosClamped.z, bbox.lowerCorner.z + hh.z,
                                   bbox.upperCorner.z - hh.z);
        uSampler.GetCoordinatesAndWeights(uPosClamped, &indices, &weights);

        for (int j = 0; j < 8; ++j)
        {
            Vector3D gridPos = uPos(indices[j].x, indices[j].y, indices[j].z);
            double apicTerm = m_cX[i].Dot(gridPos - uPosClamped);

            u(indices[j]) += weights[j] * (velocities[i].x + apicTerm);
            uWeight(indices[j]) += weights[j];
            m_uMarkers(indices[j]) = 1;
        }

        auto vPosClamped = positions[i];
        vPosClamped.x = std::clamp(vPosClamped.x, bbox.lowerCorner.x + hh.x,
                                   bbox.upperCorner.x - hh.x);
        vPosClamped.z = std::clamp(vPosClamped.z, bbox.lowerCorner.z + hh.z,
                                   bbox.upperCorner.z - hh.z);
        vSampler.GetCoordinatesAndWeights(vPosClamped, &indices, &weights);

        for (int j = 0; j < 8; ++j)
        {
            Vector3D gridPos = vPos(indices[j].x, indices[j].y, indices[j].z);
            double apicTerm = m_cY[i].Dot(gridPos - vPosClamped);

            v(indices[j]) += weights[j] * (velocities[i].y + apicTerm);
            vWeight(indices[j]) += weights[j];
            m_vMarkers(indices[j]) = 1;
        }

        auto wPosClamped = positions[i];
        wPosClamped.x = std::clamp(wPosClamped.x, bbox.lowerCorner.x + hh.x,
                                   bbox.upperCorner.x - hh.x);
        wPosClamped.y = std::clamp(wPosClamped.y, bbox.lowerCorner.y + hh.y,
                                   bbox.upperCorner.y - hh.y);
        wSampler.GetCoordinatesAndWeights(wPosClamped, &indices, &weights);

        for (int j = 0; j < 8; ++j)
        {
            Vector3D gridPos = wPos(indices[j].x, indices[j].y, indices[j].z);
            double apicTerm = m_cZ[i].Dot(gridPos - wPosClamped);

            w(indices[j]) += weights[j] * (velocities[i].z + apicTerm);
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

void APICSolver3::TransferFromGridsToParticles()
{
    const auto flow = GetGridSystemData()->GetVelocity();
    const auto particles = GetParticleSystemData();
    auto positions = particles->GetPositions();
    auto velocities = particles->GetVelocities();
    const size_t numberOfParticles = particles->GetNumberOfParticles();
    const auto hh = flow->GridSpacing() / 2.0;
    const auto bbox = flow->BoundingBox();

    // Allocate buffers
    m_cX.Resize(numberOfParticles);
    m_cY.Resize(numberOfParticles);
    m_cZ.Resize(numberOfParticles);
    m_cX.Set(Vector3D());
    m_cY.Set(Vector3D());
    m_cZ.Set(Vector3D());

    auto u = flow->GetUAccessor();
    auto v = flow->GetVAccessor();
    auto w = flow->GetWAccessor();

    LinearArraySampler3<double, double> uSampler(u, flow->GridSpacing(),
                                                 flow->GetUOrigin());
    LinearArraySampler3<double, double> vSampler(v, flow->GridSpacing(),
                                                 flow->GetVOrigin());
    LinearArraySampler3<double, double> wSampler(w, flow->GridSpacing(),
                                                 flow->GetWOrigin());

    ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i) {
        velocities[i] = flow->Sample(positions[i]);

        std::array<Point3UI, 8> indices;
        std::array<Vector3D, 8> gradWeights;

        // x
        auto uPosClamped = positions[i];
        uPosClamped.y = std::clamp(uPosClamped.y, bbox.lowerCorner.y + hh.y,
                                   bbox.upperCorner.y - hh.y);
        uPosClamped.z = std::clamp(uPosClamped.z, bbox.lowerCorner.z + hh.z,
                                   bbox.upperCorner.z - hh.z);
        uSampler.GetCoordinatesAndGradientWeights(uPosClamped, &indices,
                                                  &gradWeights);

        for (int j = 0; j < 8; ++j)
        {
            m_cX[i] += gradWeights[j] * u(indices[j]);
        }

        // y
        auto vPosClamped = positions[i];
        vPosClamped.x = std::clamp(vPosClamped.x, bbox.lowerCorner.x + hh.x,
                                   bbox.upperCorner.x - hh.x);
        vPosClamped.z = std::clamp(vPosClamped.z, bbox.lowerCorner.z + hh.z,
                                   bbox.upperCorner.z - hh.z);
        vSampler.GetCoordinatesAndGradientWeights(vPosClamped, &indices,
                                                  &gradWeights);

        for (int j = 0; j < 8; ++j)
        {
            m_cY[i] += gradWeights[j] * v(indices[j]);
        }

        // z
        auto wPosClamped = positions[i];
        wPosClamped.x = std::clamp(wPosClamped.x, bbox.lowerCorner.x + hh.x,
                                   bbox.upperCorner.x - hh.x);
        wPosClamped.y = std::clamp(wPosClamped.y, bbox.lowerCorner.y + hh.y,
                                   bbox.upperCorner.y - hh.y);
        wSampler.GetCoordinatesAndGradientWeights(wPosClamped, &indices,
                                                  &gradWeights);

        for (int j = 0; j < 8; ++j)
        {
            m_cZ[i] += gradWeights[j] * w(indices[j]);
        }
    });
}

APICSolver3::Builder APICSolver3::GetBuilder()
{
    return Builder();
}

APICSolver3 APICSolver3::Builder::Build() const
{
    return APICSolver3(m_resolution, GetGridSpacing(), m_gridOrigin);
}

APICSolver3Ptr APICSolver3::Builder::MakeShared() const
{
    return std::shared_ptr<APICSolver3>(
        new APICSolver3(m_resolution, GetGridSpacing(), m_gridOrigin),
        [](APICSolver3* obj) { delete obj; });
}
}  // namespace CubbyFlow