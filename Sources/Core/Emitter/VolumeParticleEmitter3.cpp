// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Emitter/VolumeParticleEmitter3.hpp>
#include <Core/Geometry/SurfaceToImplicit3.hpp>
#include <Core/PointGenerator/BccLatticePointGenerator.hpp>
#include <Core/Searcher/PointHashGridSearcher3.hpp>
#include <Core/Utils/Logging.hpp>
#include <Core/Utils/Samplers.hpp>

namespace CubbyFlow
{
static const size_t DEFAULT_HASH_GRID_RESOLUTION = 64;

VolumeParticleEmitter3::VolumeParticleEmitter3(
    ImplicitSurface3Ptr implicitSurface, const BoundingBox3D& bounds,
    double spacing, const Vector3D& initialVel, size_t maxNumberOfParticles,
    double jitter, bool isOneShot, bool allowOverlapping, uint32_t seed)
    : m_rng(seed),
      m_implicitSurface(std::move(implicitSurface)),
      m_bounds(bounds),
      m_spacing(spacing),
      m_initialVel(initialVel),
      m_maxNumberOfParticles(maxNumberOfParticles),
      m_jitter(jitter),
      m_isOneShot(isOneShot),
      m_allowOverlapping(allowOverlapping)
{
    m_pointsGen = std::make_shared<BccLatticePointGenerator>();
}

void VolumeParticleEmitter3::OnUpdate(double currentTimeInSeconds,
                                      double timeIntervalInSeconds)
{
    UNUSED_VARIABLE(currentTimeInSeconds);
    UNUSED_VARIABLE(timeIntervalInSeconds);

    auto particles = GetTarget();

    if (particles == nullptr)
    {
        return;
    }

    if (m_numberOfEmittedParticles > 0 && m_isOneShot)
    {
        return;
    }

    Array1<Vector3D> newPositions;
    Array1<Vector3D> newVelocities;

    Emit(particles, &newPositions, &newVelocities);

    particles->AddParticles(newPositions, newVelocities);
}

void VolumeParticleEmitter3::Emit(const ParticleSystemData3Ptr& particles,
                                  Array1<Vector3D>* newPositions,
                                  Array1<Vector3D>* newVelocities)
{
    if (!m_implicitSurface)
    {
        return;
    }

    m_implicitSurface->UpdateQueryEngine();

    // Reserving more space for jittering
    const double j = GetJitter();
    const double maxJitterDist = 0.5 * j * m_spacing;

    if (m_allowOverlapping || m_isOneShot)
    {
        m_pointsGen->ForEachPoint(
            m_bounds, m_spacing, [&](const Vector3D& point) {
                const Vector3D randomDir =
                    UniformSampleSphere(Random(), Random());
                const Vector3D offset = maxJitterDist * randomDir;
                const Vector3D candidate = point + offset;

                if (m_implicitSurface->SignedDistance(candidate) <= 0.0)
                {
                    if (m_numberOfEmittedParticles < m_maxNumberOfParticles)
                    {
                        newPositions->Append(candidate);
                        ++m_numberOfEmittedParticles;
                    }
                    else
                    {
                        return false;
                    }
                }

                return true;
            });
    }
    else
    {
        // Use serial hash grid searcher for continuous update.
        PointHashGridSearcher3 neighborSearcher(
            Vector3UZ(DEFAULT_HASH_GRID_RESOLUTION,
                      DEFAULT_HASH_GRID_RESOLUTION,
                      DEFAULT_HASH_GRID_RESOLUTION),
            2.0 * m_spacing);

        if (!m_allowOverlapping)
        {
            neighborSearcher.Build(particles->Positions());
        }

        m_pointsGen->ForEachPoint(
            m_bounds, m_spacing, [&](const Vector3D& point) {
                const Vector3D randomDir =
                    UniformSampleSphere(Random(), Random());
                const Vector3D offset = maxJitterDist * randomDir;
                const Vector3D candidate = point + offset;

                if (m_implicitSurface->IsInside(candidate) &&
                    (!m_allowOverlapping &&
                     !neighborSearcher.HasNearbyPoint(candidate, m_spacing)))
                {
                    if (m_numberOfEmittedParticles < m_maxNumberOfParticles)
                    {
                        newPositions->Append(candidate);
                        neighborSearcher.Add(candidate);
                        ++m_numberOfEmittedParticles;
                    }
                    else
                    {
                        return false;
                    }
                }

                return true;
            });
    }

    newVelocities->Resize(newPositions->Size());
    newVelocities->Fill(m_initialVel);
}

void VolumeParticleEmitter3::SetPointGenerator(
    const PointGenerator3Ptr& newPointsGen)
{
    m_pointsGen = newPointsGen;
}

const ImplicitSurface3Ptr& VolumeParticleEmitter3::GetSurface() const
{
    return m_implicitSurface;
}

void VolumeParticleEmitter3::SetSurface(const ImplicitSurface3Ptr& newSurface)
{
    m_implicitSurface = newSurface;
}

const BoundingBox3D& VolumeParticleEmitter3::GetMaxRegion() const
{
    return m_bounds;
}

void VolumeParticleEmitter3::SetMaxRegion(const BoundingBox3D& newMaxRegion)
{
    m_bounds = newMaxRegion;
}

double VolumeParticleEmitter3::GetJitter() const
{
    return m_jitter;
}

void VolumeParticleEmitter3::SetJitter(double newJitter)
{
    m_jitter = std::clamp(newJitter, 0.0, 1.0);
}

bool VolumeParticleEmitter3::GetIsOneShot() const
{
    return m_isOneShot;
}

void VolumeParticleEmitter3::SetIsOneShot(bool newValue)
{
    m_isOneShot = newValue;
}

bool VolumeParticleEmitter3::GetAllowOverlapping() const
{
    return m_allowOverlapping;
}

void VolumeParticleEmitter3::SetAllowOverlapping(bool newValue)
{
    m_allowOverlapping = newValue;
}

size_t VolumeParticleEmitter3::GetMaxNumberOfParticles() const
{
    return m_maxNumberOfParticles;
}

void VolumeParticleEmitter3::SetMaxNumberOfParticles(
    size_t newMaxNumberOfParticles)
{
    m_maxNumberOfParticles = newMaxNumberOfParticles;
}

double VolumeParticleEmitter3::GetSpacing() const
{
    return m_spacing;
}

void VolumeParticleEmitter3::SetSpacing(double newSpacing)
{
    m_spacing = newSpacing;
}

Vector3D VolumeParticleEmitter3::GetInitialVelocity() const
{
    return m_initialVel;
}

void VolumeParticleEmitter3::SetInitialVelocity(const Vector3D& newInitialVel)
{
    m_initialVel = newInitialVel;
}

double VolumeParticleEmitter3::Random()
{
    std::uniform_real_distribution<> d{ 0.0, 1.0 };
    return d(m_rng);
}

VolumeParticleEmitter3::Builder VolumeParticleEmitter3::GetBuilder()
{
    return Builder();
}

VolumeParticleEmitter3::Builder&
VolumeParticleEmitter3::Builder::WithImplicitSurface(
    const ImplicitSurface3Ptr& implicitSurface)
{
    m_implicitSurface = implicitSurface;

    if (!m_isBoundSet)
    {
        m_bounds = m_implicitSurface->BoundingBox();
    }

    return *this;
}

VolumeParticleEmitter3::Builder& VolumeParticleEmitter3::Builder::WithSurface(
    const Surface3Ptr& surface)
{
    m_implicitSurface = std::make_shared<SurfaceToImplicit3>(surface);

    if (!m_isBoundSet)
    {
        m_bounds = surface->BoundingBox();
    }

    return *this;
}

VolumeParticleEmitter3::Builder& VolumeParticleEmitter3::Builder::WithMaxRegion(
    const BoundingBox3D& bounds)
{
    m_bounds = bounds;
    m_isBoundSet = true;
    return *this;
}

VolumeParticleEmitter3::Builder& VolumeParticleEmitter3::Builder::WithSpacing(
    double spacing)
{
    m_spacing = spacing;
    return *this;
}

VolumeParticleEmitter3::Builder&
VolumeParticleEmitter3::Builder::WithInitialVelocity(const Vector3D& initialVel)
{
    m_initialVel = initialVel;
    return *this;
}

VolumeParticleEmitter3::Builder&
VolumeParticleEmitter3::Builder::WithMaxNumberOfParticles(
    size_t maxNumberOfParticles)
{
    m_maxNumberOfParticles = maxNumberOfParticles;
    return *this;
}

VolumeParticleEmitter3::Builder& VolumeParticleEmitter3::Builder::WithJitter(
    double jitter)
{
    m_jitter = jitter;
    return *this;
}

VolumeParticleEmitter3::Builder& VolumeParticleEmitter3::Builder::WithIsOneShot(
    bool isOneShot)
{
    m_isOneShot = isOneShot;
    return *this;
}

VolumeParticleEmitter3::Builder&
VolumeParticleEmitter3::Builder::WithAllowOverlapping(bool allowOverlapping)
{
    m_allowOverlapping = allowOverlapping;
    return *this;
}

VolumeParticleEmitter3::Builder&
VolumeParticleEmitter3::Builder::WithRandomSeed(uint32_t seed)
{
    m_seed = seed;
    return *this;
}

VolumeParticleEmitter3 VolumeParticleEmitter3::Builder::Build() const
{
    return VolumeParticleEmitter3(m_implicitSurface, m_bounds, m_spacing,
                                  m_initialVel, m_maxNumberOfParticles,
                                  m_jitter, m_isOneShot, m_allowOverlapping,
                                  m_seed);
}

VolumeParticleEmitter3Ptr VolumeParticleEmitter3::Builder::MakeShared() const
{
    return std::shared_ptr<VolumeParticleEmitter3>(
        new VolumeParticleEmitter3(m_implicitSurface, m_bounds, m_spacing,
                                   m_initialVel, m_maxNumberOfParticles,
                                   m_jitter, m_isOneShot, m_allowOverlapping),
        [](VolumeParticleEmitter3* obj) { delete obj; });
}
}  // namespace CubbyFlow