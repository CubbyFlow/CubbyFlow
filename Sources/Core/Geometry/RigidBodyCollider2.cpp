// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Geometry/RigidBodyCollider2.hpp>

namespace CubbyFlow
{
RigidBodyCollider2::RigidBodyCollider2(const Surface2Ptr& surface)
{
    SetSurface(surface);
}

RigidBodyCollider2::RigidBodyCollider2(const Surface2Ptr& surface,
                                       const Vector2D& _linearVelocity,
                                       double _angularVelocity)
    : linearVelocity(_linearVelocity), angularVelocity(_angularVelocity)
{
    SetSurface(surface);
}

Vector2D RigidBodyCollider2::VelocityAt(const Vector2D& point) const
{
    const Vector2D r = point - GetSurface()->transform.GetTranslation();
    return linearVelocity + angularVelocity * Vector2D(-r.y, r.x);
}

RigidBodyCollider2::Builder RigidBodyCollider2::GetBuilder()
{
    return Builder();
}

RigidBodyCollider2::Builder& RigidBodyCollider2::Builder::WithSurface(
    const Surface2Ptr& surface)
{
    m_surface = surface;
    return *this;
}

RigidBodyCollider2::Builder& RigidBodyCollider2::Builder::WithLinearVelocity(
    const Vector2D& _linearVelocity)
{
    m_linearVelocity = _linearVelocity;
    return *this;
}

RigidBodyCollider2::Builder& RigidBodyCollider2::Builder::WithAngularVelocity(
    double _angularVelocity)
{
    m_angularVelocity = _angularVelocity;
    return *this;
}

RigidBodyCollider2 RigidBodyCollider2::Builder::Build() const
{
    return RigidBodyCollider2(m_surface, m_linearVelocity, m_angularVelocity);
}

RigidBodyCollider2Ptr RigidBodyCollider2::Builder::MakeShared() const
{
    return std::shared_ptr<RigidBodyCollider2>(
        new RigidBodyCollider2(m_surface, m_linearVelocity, m_angularVelocity),
        [](RigidBodyCollider2* obj) { delete obj; });
}
}  // namespace CubbyFlow