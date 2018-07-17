/*************************************************************************
> File Name: APICSolver2.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D Affine Particle-in-Cell (APIC) implementation.
> Created Time: 2017/10/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Solver/Hybrid/APIC/APICSolver2.h>

namespace CubbyFlow
{
    APICSolver2::APICSolver2() : APICSolver2({ 1, 1 }, { 1, 1 }, { 0, 0 })
    {
        // Do nothing
    }

    APICSolver2::APICSolver2(
        const Size2& resolution,
        const Vector2D& gridSpacing,
        const Vector2D& gridOrigin) :
        PICSolver2(resolution, gridSpacing, gridOrigin)
    {
        // Do nothing
    }

    APICSolver2::~APICSolver2()
    {
        // Do nothing
    }

    void APICSolver2::TransferFromParticlesToGrids()
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

        // Clear velocity to zero
        flow->Fill(Vector2D());

        // Weighted-average velocity
        auto u = flow->GetUAccessor();
        auto v = flow->GetVAccessor();
        const auto uPos = flow->GetUPosition();
        const auto vPos = flow->GetVPosition();
        Array2<double> uWeight(u.size());
        Array2<double> vWeight(v.size());
        m_uMarkers.Resize(u.size());
        m_vMarkers.Resize(v.size());
        m_uMarkers.Set(0);
        m_vMarkers.Set(0);

        LinearArraySampler2<double, double> uSampler(
            flow->GetUConstAccessor(),
            flow->GridSpacing(),
            flow->GetUOrigin());
        LinearArraySampler2<double, double> vSampler(
            flow->GetVConstAccessor(),
            flow->GridSpacing(),
            flow->GetVOrigin());

        for (size_t i = 0; i < numberOfParticles; ++i)
        {
            std::array<Point2UI, 4> indices;
            std::array<double, 4> weights;

            auto uPosClamped = positions[i];
            uPosClamped.y = std::clamp(uPosClamped.y, bbox.lowerCorner.y + hh.y, bbox.upperCorner.y - hh.y);
            uSampler.GetCoordinatesAndWeights(uPosClamped, &indices, &weights);

            for (int j = 0; j < 4; ++j)
            {
                Vector2D gridPos = uPos(indices[j].x, indices[j].y);
                double apicTerm = m_cX[i].Dot(gridPos - uPosClamped);

                u(indices[j]) += weights[j] * (velocities[i].x + apicTerm);
                uWeight(indices[j]) += weights[j];
                m_uMarkers(indices[j]) = 1;
            }

            auto vPosClamped = positions[i];
            vPosClamped.x = std::clamp(vPosClamped.x, bbox.lowerCorner.x + hh.x, bbox.upperCorner.x - hh.x);
            vSampler.GetCoordinatesAndWeights(vPosClamped, &indices, &weights);

            for (int j = 0; j < 4; ++j)
            {
                Vector2D gridPos = vPos(indices[j].x, indices[j].y);
                double apicTerm = m_cY[i].Dot(gridPos - vPosClamped);

                v(indices[j]) += weights[j] * (velocities[i].y + apicTerm);
                vWeight(indices[j]) += weights[j];
                m_vMarkers(indices[j]) = 1;
            }
        }

        uWeight.ParallelForEachIndex([&](size_t i, size_t j)
        {
            if (uWeight(i, j) > 0.0)
            {
                u(i, j) /= uWeight(i, j);
            }
        });
        vWeight.ParallelForEachIndex([&](size_t i, size_t j)
        {
            if (vWeight(i, j) > 0.0)
            {
                v(i, j) /= vWeight(i, j);
            }
        });
    }

    void APICSolver2::TransferFromGridsToParticles()
    {
        const auto flow = GetGridSystemData()->GetVelocity();
        auto particles = GetParticleSystemData();
        auto positions = particles->GetPositions();
        auto velocities = particles->GetVelocities();
        const size_t numberOfParticles = particles->GetNumberOfParticles();
        const auto hh = flow->GridSpacing() / 2.0;
        const auto bbox = flow->BoundingBox();

        // Allocate buffers
        m_cX.Resize(numberOfParticles);
        m_cY.Resize(numberOfParticles);
        m_cX.Set(Vector2D());
        m_cY.Set(Vector2D());

        auto u = flow->GetUAccessor();
        auto v = flow->GetVAccessor();

        LinearArraySampler2<double, double> uSampler(u, flow->GridSpacing(), flow->GetUOrigin());
        LinearArraySampler2<double, double> vSampler(v, flow->GridSpacing(), flow->GetVOrigin());

        ParallelFor(ZERO_SIZE, numberOfParticles, [&](size_t i)
        {
            velocities[i] = flow->Sample(positions[i]);

            std::array<Point2UI, 4> indices;
            std::array<Vector2D, 4> gradWeights;

            // x
            auto uPosClamped = positions[i];
            uPosClamped.y = std::clamp(uPosClamped.y, bbox.lowerCorner.y + hh.y, bbox.upperCorner.y - hh.y);
            uSampler.GetCoordinatesAndGradientWeights(uPosClamped, &indices, &gradWeights);

            for (int j = 0; j < 4; ++j)
            {
                m_cX[i] += gradWeights[j] * u(indices[j]);
            }

            // y
            auto vPosClamped = positions[i];
            vPosClamped.x = std::clamp(vPosClamped.x, bbox.lowerCorner.x + hh.x, bbox.upperCorner.x - hh.x);
            vSampler.GetCoordinatesAndGradientWeights(vPosClamped, &indices, &gradWeights);

            for (int j = 0; j < 4; ++j)
            {
                m_cY[i] += gradWeights[j] * v(indices[j]);
            }
        });
    }

    APICSolver2::Builder APICSolver2::GetBuilder()
    {
        return Builder();
    }

    APICSolver2 APICSolver2::Builder::Build() const
    {
        return APICSolver2(m_resolution, GetGridSpacing(), m_gridOrigin);
    }

    APICSolver2Ptr APICSolver2::Builder::MakeShared() const
    {
        return std::shared_ptr<APICSolver2>(new APICSolver2(m_resolution, GetGridSpacing(), m_gridOrigin),
            [](APICSolver2* obj)
        {
            delete obj;
        });
    }
}