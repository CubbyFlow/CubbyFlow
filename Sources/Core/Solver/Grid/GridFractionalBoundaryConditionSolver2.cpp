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
#include <Core/Geometry/ImplicitSurface2.hpp>
#include <Core/Geometry/SurfaceToImplicit2.hpp>
#include <Core/Solver/Grid/GridFractionalBoundaryConditionSolver2.hpp>
#include <Core/Utils/LevelSetUtils.hpp>
#include <Core/Utils/PhysicsHelpers.hpp>

namespace CubbyFlow
{
void GridFractionalBoundaryConditionSolver2::ConstrainVelocity(
    FaceCenteredGrid2* velocity, unsigned int extrapolationDepth)
{
    const Size2 size = velocity->Resolution();

    if (m_colliderSDF == nullptr || m_colliderSDF->Resolution() != size)
    {
        UpdateCollider(GetCollider(), size, velocity->GridSpacing(),
                       velocity->Origin());
    }

    ArrayAccessor2<double> u = velocity->GetUAccessor();
    ArrayAccessor2<double> v = velocity->GetVAccessor();
    auto uPos = velocity->GetUPosition();
    auto vPos = velocity->GetVPosition();

    Array2<double> uTemp{ u.size() };
    Array2<double> vTemp{ v.size() };
    Array2<char> uMarker{ u.size(), 1 };
    Array2<char> vMarker{ v.size(), 1 };

    Vector2D h = velocity->GridSpacing();

    // Assign collider's velocity first and initialize markers
    velocity->ParallelForEachUIndex([&](size_t i, size_t j) {
        const Vector2D pt = uPos(i, j);
        const double phi0 =
            m_colliderSDF->Sample(pt - Vector2D{ 0.5 * h.x, 0.0 });
        const double phi1 =
            m_colliderSDF->Sample(pt + Vector2D{ 0.5 * h.x, 0.0 });
        double frac = FractionInsideSDF(phi0, phi1);
        frac = 1.0 - std::clamp(frac, 0.0, 1.0);

        if (frac > 0.0)
        {
            uMarker(i, j) = 1;
        }
        else
        {
            const Vector2D colliderVel = GetCollider()->VelocityAt(pt);
            u(i, j) = colliderVel.x;
            uMarker(i, j) = 0;
        }
    });

    velocity->ParallelForEachVIndex([&](size_t i, size_t j) {
        const Vector2D pt = vPos(i, j);
        const double phi0 =
            m_colliderSDF->Sample(pt - Vector2D{ 0.0, 0.5 * h.y });
        const double phi1 =
            m_colliderSDF->Sample(pt + Vector2D{ 0.0, 0.5 * h.y });
        double frac = FractionInsideSDF(phi0, phi1);
        frac = 1.0 - std::clamp(frac, 0.0, 1.0);

        if (frac > 0.0)
        {
            vMarker(i, j) = 1;
        }
        else
        {
            const Vector2D colliderVel = GetCollider()->VelocityAt(pt);
            v(i, j) = colliderVel.y;
            vMarker(i, j) = 0;
        }
    });

    // Free-slip: Extrapolate fluid velocity into the collider
    ExtrapolateToRegion(velocity->GetUConstAccessor(), uMarker,
                        extrapolationDepth, u);
    ExtrapolateToRegion(velocity->GetVConstAccessor(), vMarker,
                        extrapolationDepth, v);

    // No-flux: project the extrapolated velocity to the collider's surface
    // normal
    velocity->ParallelForEachUIndex([&](size_t i, size_t j) {
        const Vector2D pt = uPos(i, j);

        if (IsInsideSDF(m_colliderSDF->Sample(pt)))
        {
            const Vector2D colliderVel = GetCollider()->VelocityAt(pt);
            const Vector2D vel = velocity->Sample(pt);
            const Vector2D g = m_colliderSDF->Gradient(pt);

            if (g.LengthSquared() > 0.0)
            {
                const Vector2D n = g.Normalized();
                const Vector2D velr = vel - colliderVel;
                const Vector2D velt = ProjectAndApplyFriction(
                    velr, n, GetCollider()->GetFrictionCoefficient());
                const Vector2D velp = velt + colliderVel;

                uTemp(i, j) = velp.x;
            }
            else
            {
                uTemp(i, j) = colliderVel.x;
            }
        }
        else
        {
            uTemp(i, j) = u(i, j);
        }
    });

    velocity->ParallelForEachVIndex([&](size_t i, size_t j) {
        const Vector2D pt = vPos(i, j);

        if (IsInsideSDF(m_colliderSDF->Sample(pt)))
        {
            const Vector2D colliderVel = GetCollider()->VelocityAt(pt);
            const Vector2D vel = velocity->Sample(pt);
            const Vector2D g = m_colliderSDF->Gradient(pt);

            if (g.LengthSquared() > 0.0)
            {
                const Vector2D n = g.Normalized();
                const Vector2D velr = vel - colliderVel;
                const Vector2D velt = ProjectAndApplyFriction(
                    velr, n, GetCollider()->GetFrictionCoefficient());
                const Vector2D velp = velt + colliderVel;

                vTemp(i, j) = velp.y;
            }
            else
            {
                vTemp(i, j) = colliderVel.y;
            }
        }
        else
        {
            vTemp(i, j) = v(i, j);
        }
    });

    // Transfer results
    u.ParallelForEachIndex([&](size_t i, size_t j) { u(i, j) = uTemp(i, j); });
    v.ParallelForEachIndex([&](size_t i, size_t j) { v(i, j) = vTemp(i, j); });

    // No-flux: Project velocity on the domain boundary if closed
    if (GetClosedDomainBoundaryFlag() & DIRECTION_LEFT)
    {
        for (size_t j = 0; j < u.size().y; ++j)
        {
            u(0, j) = 0;
        }
    }
    if (GetClosedDomainBoundaryFlag() & DIRECTION_RIGHT)
    {
        for (size_t j = 0; j < u.size().y; ++j)
        {
            u(u.size().x - 1, j) = 0;
        }
    }
    if (GetClosedDomainBoundaryFlag() & DIRECTION_DOWN)
    {
        for (size_t i = 0; i < v.size().x; ++i)
        {
            v(i, 0) = 0;
        }
    }
    if (GetClosedDomainBoundaryFlag() & DIRECTION_UP)
    {
        for (size_t i = 0; i < v.size().x; ++i)
        {
            v(i, v.size().y - 1) = 0;
        }
    }
}

ScalarField2Ptr GridFractionalBoundaryConditionSolver2::GetColliderSDF() const
{
    return m_colliderSDF;
}

VectorField2Ptr
GridFractionalBoundaryConditionSolver2::GetColliderVelocityField() const
{
    return m_colliderVel;
}

void GridFractionalBoundaryConditionSolver2::OnColliderUpdated(
    const Size2& gridSize, const Vector2D& gridSpacing,
    const Vector2D& gridOrigin)
{
    if (m_colliderSDF == nullptr)
    {
        m_colliderSDF = std::make_shared<CellCenteredScalarGrid2>();
    }

    m_colliderSDF->Resize(gridSize, gridSpacing, gridOrigin);

    if (GetCollider() != nullptr)
    {
        Surface2Ptr surface = GetCollider()->GetSurface();
        ImplicitSurface2Ptr implicitSurface =
            std::dynamic_pointer_cast<ImplicitSurface2>(surface);
        if (implicitSurface == nullptr)
        {
            implicitSurface = std::make_shared<SurfaceToImplicit2>(surface);
        }

        m_colliderSDF->Fill([&](const Vector2D& pt) {
            return implicitSurface->SignedDistance(pt);
        });

        m_colliderVel = CustomVectorField2::Builder{}
                            .WithFunction([&](const Vector2D& x) {
                                return GetCollider()->VelocityAt(x);
                            })
                            .WithDerivativeResolution(gridSpacing.x)
                            .MakeShared();
    }
    else
    {
        m_colliderSDF->Fill(std::numeric_limits<double>::max());

        m_colliderVel =
            CustomVectorField2::Builder{}
                .WithFunction([](const Vector2D&) { return Vector2D{}; })
                .WithDerivativeResolution(gridSpacing.x)
                .MakeShared();
    }
}
}  // namespace CubbyFlow
