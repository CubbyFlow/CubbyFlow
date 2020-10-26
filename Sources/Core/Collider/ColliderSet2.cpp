// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Collider/ColliderSet2.hpp>
#include <Core/Surface/SurfaceSet2.hpp>

namespace CubbyFlow
{
ColliderSet2::ColliderSet2() : ColliderSet2(std::vector<Collider2Ptr>())
{
    // Do nothing
}

ColliderSet2::ColliderSet2(const std::vector<Collider2Ptr>& others)
{
    SetSurface(std::make_shared<SurfaceSet2>());
    for (const auto& collider : others)
    {
        AddCollider(collider);
    }
}

Vector2D ColliderSet2::VelocityAt(const Vector2D& point) const
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

    return Vector2D();
}

void ColliderSet2::AddCollider(const Collider2Ptr& collider)
{
    auto surfaceSet = std::dynamic_pointer_cast<SurfaceSet2>(GetSurface());
    m_colliders.push_back(collider);
    surfaceSet->AddSurface(collider->GetSurface());
}

size_t ColliderSet2::NumberOfColliders() const
{
    return m_colliders.size();
}

Collider2Ptr ColliderSet2::Collider(size_t i) const
{
    return m_colliders[i];
}

ColliderSet2::Builder ColliderSet2::GetBuilder()
{
    return Builder();
}

ColliderSet2::Builder& ColliderSet2::Builder::WithColliders(
    const std::vector<Collider2Ptr>& others)
{
    m_colliders = others;
    return *this;
}

ColliderSet2 ColliderSet2::Builder::Build() const
{
    return ColliderSet2(m_colliders);
}

ColliderSet2Ptr ColliderSet2::Builder::MakeShared() const
{
    return std::shared_ptr<ColliderSet2>(new ColliderSet2(m_colliders),
                                         [](ColliderSet2* obj) { delete obj; });
}
}  // namespace CubbyFlow