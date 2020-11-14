// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Geometry/RigidBodyCollider3.hpp>

namespace CubbyFlow
{
RigidBodyCollider3::RigidBodyCollider3(const Surface3Ptr& surface)
{
    SetSurface(surface);
}

RigidBodyCollider3::RigidBodyCollider3(const Surface3Ptr& surface,
                                       const Vector3D& _linearVelocity,
                                       const Vector3D& _angularVelocity)
    : linearVelocity(_linearVelocity), angularVelocity(_angularVelocity)
{
    SetSurface(surface);
}

Vector3D RigidBodyCollider3::VelocityAt(const Vector3D& point) const
{
    Vector3D r = point - GetSurface()->transform.GetTranslation();
    return linearVelocity + angularVelocity.Cross(r);
}

RigidBodyCollider3::Builder RigidBodyCollider3::GetBuilder()
{
    return Builder();
}

RigidBodyCollider3::Builder& RigidBodyCollider3::Builder::WithSurface(
    const Surface3Ptr& surface)
{
    m_surface = surface;
    return *this;
}

RigidBodyCollider3::Builder& RigidBodyCollider3::Builder::WithLinearVelocity(
    const Vector3D& _linearVelocity)
{
    m_linearVelocity = _linearVelocity;
    return *this;
}

RigidBodyCollider3::Builder& RigidBodyCollider3::Builder::WithAngularVelocity(
    const Vector3D& _angularVelocity)
{
    m_angularVelocity = _angularVelocity;
    return *this;
}

RigidBodyCollider3 RigidBodyCollider3::Builder::Build() const
{
    return RigidBodyCollider3(m_surface, m_linearVelocity, m_angularVelocity);
}

RigidBodyCollider3Ptr RigidBodyCollider3::Builder::MakeShared() const
{
    return std::shared_ptr<RigidBodyCollider3>(
        new RigidBodyCollider3(m_surface, m_linearVelocity, m_angularVelocity),
        [](RigidBodyCollider3* obj) { delete obj; });
}
}  // namespace CubbyFlow