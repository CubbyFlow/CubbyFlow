// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Utils/PhysicsHelpers.h>
#include <Core/Array/ArrayUtils.hpp>
#include <Core/LevelSet/LevelSetUtils.hpp>
#include <Core/Solver/Grid/GridFractionalBoundaryConditionSolver3.hpp>
#include <Core/Surface/ImplicitSurface3.hpp>
#include <Core/Surface/SurfaceToImplicit3.hpp>

namespace CubbyFlow
{
GridFractionalBoundaryConditionSolver3::GridFractionalBoundaryConditionSolver3()
{
    // Do nothing
}

GridFractionalBoundaryConditionSolver3::
    ~GridFractionalBoundaryConditionSolver3()
{
    // Do nothing
}

void GridFractionalBoundaryConditionSolver3::ConstrainVelocity(
    FaceCenteredGrid3* velocity, unsigned int extrapolationDepth)
{
    Size3 size = velocity->Resolution();

    if (m_colliderSDF == nullptr || m_colliderSDF->Resolution() != size)
    {
        UpdateCollider(GetCollider(), size, velocity->GridSpacing(),
                       velocity->Origin());
    }

    auto u = velocity->GetUAccessor();
    auto v = velocity->GetVAccessor();
    auto w = velocity->GetWAccessor();
    auto uPos = velocity->GetUPosition();
    auto vPos = velocity->GetVPosition();
    auto wPos = velocity->GetWPosition();

    Array3<double> uTemp(u.size());
    Array3<double> vTemp(v.size());
    Array3<double> wTemp(w.size());
    Array3<char> uMarker(u.size(), 1);
    Array3<char> vMarker(v.size(), 1);
    Array3<char> wMarker(w.size(), 1);

    Vector3D h = velocity->GridSpacing();

    // Assign collider's velocity first and initialize markers
    velocity->ParallelForEachUIndex([&](size_t i, size_t j, size_t k) {
        Vector3D pt = uPos(i, j, k);
        double phi0 = m_colliderSDF->Sample(pt - Vector3D(0.5 * h.x, 0.0, 0.0));
        double phi1 = m_colliderSDF->Sample(pt + Vector3D(0.5 * h.x, 0.0, 0.0));
        double frac = FractionInsideSDF(phi0, phi1);
        frac = 1.0 - std::clamp(frac, 0.0, 1.0);

        if (frac > 0.0)
        {
            uMarker(i, j, k) = 1;
        }
        else
        {
            Vector3D colliderVel = GetCollider()->VelocityAt(pt);
            u(i, j, k) = colliderVel.x;
            uMarker(i, j, k) = 0;
        }
    });

    velocity->ParallelForEachVIndex([&](size_t i, size_t j, size_t k) {
        Vector3D pt = vPos(i, j, k);
        double phi0 = m_colliderSDF->Sample(pt - Vector3D(0.0, 0.5 * h.y, 0.0));
        double phi1 = m_colliderSDF->Sample(pt + Vector3D(0.0, 0.5 * h.y, 0.0));
        double frac = FractionInsideSDF(phi0, phi1);
        frac = 1.0 - std::clamp(frac, 0.0, 1.0);

        if (frac > 0.0)
        {
            vMarker(i, j, k) = 1;
        }
        else
        {
            Vector3D colliderVel = GetCollider()->VelocityAt(pt);
            v(i, j, k) = colliderVel.y;
            vMarker(i, j, k) = 0;
        }
    });

    velocity->ParallelForEachWIndex([&](size_t i, size_t j, size_t k) {
        Vector3D pt = wPos(i, j, k);
        double phi0 = m_colliderSDF->Sample(pt - Vector3D(0.0, 0.0, 0.5 * h.z));
        double phi1 = m_colliderSDF->Sample(pt + Vector3D(0.0, 0.0, 0.5 * h.z));
        double frac = FractionInsideSDF(phi0, phi1);
        frac = 1.0 - std::clamp(frac, 0.0, 1.0);

        if (frac > 0.0)
        {
            wMarker(i, j, k) = 1;
        }
        else
        {
            Vector3D colliderVel = GetCollider()->VelocityAt(pt);
            w(i, j, k) = colliderVel.z;
            wMarker(i, j, k) = 0;
        }
    });

    // Free-slip: Extrapolate fluid velocity into the collider
    ExtrapolateToRegion(velocity->GetUConstAccessor(), uMarker,
                        extrapolationDepth, u);
    ExtrapolateToRegion(velocity->GetVConstAccessor(), vMarker,
                        extrapolationDepth, v);
    ExtrapolateToRegion(velocity->GetWConstAccessor(), wMarker,
                        extrapolationDepth, w);

    // No-flux: project the extrapolated velocity to the collider's surface
    // normal
    velocity->ParallelForEachUIndex([&](size_t i, size_t j, size_t k) {
        Vector3D pt = uPos(i, j, k);

        if (IsInsideSDF(m_colliderSDF->Sample(pt)))
        {
            Vector3D colliderVel = GetCollider()->VelocityAt(pt);
            Vector3D vel = velocity->Sample(pt);
            Vector3D g = m_colliderSDF->Gradient(pt);

            if (g.LengthSquared() > 0.0)
            {
                Vector3D n = g.Normalized();
                Vector3D velr = vel - colliderVel;
                Vector3D velt = ProjectAndApplyFriction(
                    velr, n, GetCollider()->GetFrictionCoefficient());
                Vector3D velp = velt + colliderVel;

                uTemp(i, j, k) = velp.x;
            }
            else
            {
                uTemp(i, j, k) = colliderVel.x;
            }
        }
        else
        {
            uTemp(i, j, k) = u(i, j, k);
        }
    });

    velocity->ParallelForEachVIndex([&](size_t i, size_t j, size_t k) {
        Vector3D pt = vPos(i, j, k);

        if (IsInsideSDF(m_colliderSDF->Sample(pt)))
        {
            Vector3D colliderVel = GetCollider()->VelocityAt(pt);
            Vector3D vel = velocity->Sample(pt);
            Vector3D g = m_colliderSDF->Gradient(pt);

            if (g.LengthSquared() > 0.0)
            {
                Vector3D n = g.Normalized();
                Vector3D velr = vel - colliderVel;
                Vector3D velt = ProjectAndApplyFriction(
                    velr, n, GetCollider()->GetFrictionCoefficient());
                Vector3D velp = velt + colliderVel;

                vTemp(i, j, k) = velp.y;
            }
            else
            {
                vTemp(i, j, k) = colliderVel.y;
            }
        }
        else
        {
            vTemp(i, j, k) = v(i, j, k);
        }
    });

    velocity->ParallelForEachWIndex([&](size_t i, size_t j, size_t k) {
        Vector3D pt = wPos(i, j, k);

        if (IsInsideSDF(m_colliderSDF->Sample(pt)))
        {
            Vector3D colliderVel = GetCollider()->VelocityAt(pt);
            Vector3D vel = velocity->Sample(pt);
            Vector3D g = m_colliderSDF->Gradient(pt);

            if (g.LengthSquared() > 0.0)
            {
                Vector3D n = g.Normalized();
                Vector3D velr = vel - colliderVel;
                Vector3D velt = ProjectAndApplyFriction(
                    velr, n, GetCollider()->GetFrictionCoefficient());
                Vector3D velp = velt + colliderVel;

                wTemp(i, j, k) = velp.z;
            }
            else
            {
                wTemp(i, j, k) = colliderVel.z;
            }
        }
        else
        {
            wTemp(i, j, k) = w(i, j, k);
        }
    });

    // Transfer results
    u.ParallelForEachIndex(
        [&](size_t i, size_t j, size_t k) { u(i, j, k) = uTemp(i, j, k); });
    v.ParallelForEachIndex(
        [&](size_t i, size_t j, size_t k) { v(i, j, k) = vTemp(i, j, k); });
    w.ParallelForEachIndex(
        [&](size_t i, size_t j, size_t k) { w(i, j, k) = wTemp(i, j, k); });

    // No-flux: Project velocity on the domain boundary if closed
    if (GetClosedDomainBoundaryFlag() & DIRECTION_LEFT)
    {
        for (size_t k = 0; k < u.size().z; ++k)
        {
            for (size_t j = 0; j < u.size().y; ++j)
            {
                u(0, j, k) = 0;
            }
        }
    }
    if (GetClosedDomainBoundaryFlag() & DIRECTION_RIGHT)
    {
        for (size_t k = 0; k < u.size().z; ++k)
        {
            for (size_t j = 0; j < u.size().y; ++j)
            {
                u(u.size().x - 1, j, k) = 0;
            }
        }
    }
    if (GetClosedDomainBoundaryFlag() & DIRECTION_DOWN)
    {
        for (size_t k = 0; k < v.size().z; ++k)
        {
            for (size_t i = 0; i < v.size().x; ++i)
            {
                v(i, 0, k) = 0;
            }
        }
    }
    if (GetClosedDomainBoundaryFlag() & DIRECTION_UP)
    {
        for (size_t k = 0; k < v.size().z; ++k)
        {
            for (size_t i = 0; i < v.size().x; ++i)
            {
                v(i, v.size().y - 1, k) = 0;
            }
        }
    }
    if (GetClosedDomainBoundaryFlag() & DIRECTION_BACK)
    {
        for (size_t j = 0; j < w.size().y; ++j)
        {
            for (size_t i = 0; i < w.size().x; ++i)
            {
                w(i, j, 0) = 0;
            }
        }
    }
    if (GetClosedDomainBoundaryFlag() & DIRECTION_FRONT)
    {
        for (size_t j = 0; j < w.size().y; ++j)
        {
            for (size_t i = 0; i < w.size().x; ++i)
            {
                w(i, j, w.size().z - 1) = 0;
            }
        }
    }
}

ScalarField3Ptr GridFractionalBoundaryConditionSolver3::GetColliderSDF() const
{
    return m_colliderSDF;
}

VectorField3Ptr
GridFractionalBoundaryConditionSolver3::GetColliderVelocityField() const
{
    return m_colliderVel;
}

void GridFractionalBoundaryConditionSolver3::OnColliderUpdated(
    const Size3& gridSize, const Vector3D& gridSpacing,
    const Vector3D& gridOrigin)
{
    if (m_colliderSDF == nullptr)
    {
        m_colliderSDF = std::make_shared<CellCenteredScalarGrid3>();
    }

    m_colliderSDF->Resize(gridSize, gridSpacing, gridOrigin);

    if (GetCollider() != nullptr)
    {
        Surface3Ptr surface = GetCollider()->GetSurface();
        ImplicitSurface3Ptr implicitSurface =
            std::dynamic_pointer_cast<ImplicitSurface3>(surface);
        if (implicitSurface == nullptr)
        {
            implicitSurface = std::make_shared<SurfaceToImplicit3>(surface);
        }

        m_colliderSDF->Fill([&](const Vector3D& pt) {
            return implicitSurface->SignedDistance(pt);
        });

        m_colliderVel = CustomVectorField3::Builder()
                            .WithFunction([&](const Vector3D& x) {
                                return GetCollider()->VelocityAt(x);
                            })
                            .WithDerivativeResolution(gridSpacing.x)
                            .MakeShared();
    }
    else
    {
        m_colliderSDF->Fill(std::numeric_limits<double>::max());

        m_colliderVel =
            CustomVectorField3::Builder()
                .WithFunction([](const Vector3D&) { return Vector3D(); })
                .WithDerivativeResolution(gridSpacing.x)
                .MakeShared();
    }
}
}  // namespace CubbyFlow