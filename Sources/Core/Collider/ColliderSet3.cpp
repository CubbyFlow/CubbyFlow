// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Collider/ColliderSet3.hpp>
#include <Core/Surface/SurfaceSet3.hpp>

namespace CubbyFlow
{
ColliderSet3::ColliderSet3() : ColliderSet3(std::vector<Collider3Ptr>())
{
    // Do nothing
}

ColliderSet3::ColliderSet3(const std::vector<Collider3Ptr>& others)
{
    SetSurface(std::make_shared<SurfaceSet3>());
    for (const auto& collider : others)
    {
        AddCollider(collider);
    }
}

Vector3D ColliderSet3::VelocityAt(const Vector3D& point) const
{
    size_t closestCollider = std::numeric_limits<size_t>::max();
    double closestDist = std::numeric_limits<double>::max();

    for (size_t i = 0; i < m_colliders.size(); ++i)
    {
        const double dist = m_colliders[i]->GetSurface()->ClosestDistance(point);

        if (dist < closestDist)
        {
            closestDist = dist;
            closestCollider = i;
        }
    }

    if (closestCollider != std::numeric_limits<size_t>::max())
    {
        return m_colliders[closestCollider]->VelocityAt(point);
    }

    return Vector3D();
}

void ColliderSet3::AddCollider(const Collider3Ptr& collider)
{
    auto surfaceSet = std::dynamic_pointer_cast<SurfaceSet3>(GetSurface());
    m_colliders.push_back(collider);
    surfaceSet->AddSurface(collider->GetSurface());
}

size_t ColliderSet3::NumberOfColliders() const
{
    return m_colliders.size();
}

Collider3Ptr ColliderSet3::Collider(size_t i) const
{
    return m_colliders[i];
}

ColliderSet3::Builder ColliderSet3::GetBuilder()
{
    return Builder();
}

ColliderSet3::Builder& ColliderSet3::Builder::WithColliders(
    const std::vector<Collider3Ptr>& others)
{
    m_colliders = others;
    return *this;
}

ColliderSet3 ColliderSet3::Builder::Build() const
{
    return ColliderSet3(m_colliders);
}

ColliderSet3Ptr ColliderSet3::Builder::MakeShared() const
{
    return std::shared_ptr<ColliderSet3>(new ColliderSet3(m_colliders),
                                         [](ColliderSet3* obj) { delete obj; });
}
}  // namespace CubbyFlow