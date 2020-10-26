// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_COLLIDER_SET3_HPP
#define CUBBYFLOW_COLLIDER_SET3_HPP

#include <Core/Collider/Collider3.hpp>

#include <vector>

namespace CubbyFlow
{
//! Collection of 3-D colliders
class ColliderSet3 final : public Collider3
{
 public:
    class Builder;

    //! Default constructor.
    ColliderSet3();

    //! Constructs with other colliders.
    explicit ColliderSet3(const std::vector<Collider3Ptr>& others);

    //! Returns the velocity of the collider at given \p point.
    [[nodiscard]] Vector3D VelocityAt(const Vector3D& point) const override;

    //! Adds a collider to the set.
    void AddCollider(const Collider3Ptr& collider);

    //! Returns number of colliders.
    [[nodiscard]] size_t NumberOfColliders() const;

    //! Returns collider at index \p i.
    [[nodiscard]] Collider3Ptr Collider(size_t i) const;

    //! Returns builder fox ColliderSet3.
    [[nodiscard]] static Builder GetBuilder();

 private:
    std::vector<Collider3Ptr> m_colliders;
};

//! Shared pointer for the ColliderSet3 type.
using ColliderSet3Ptr = std::shared_ptr<ColliderSet3>;

//!
//! \brief Front-end to create ColliderSet3 objects step by step.
//!
class ColliderSet3::Builder final
{
 public:
    //! Returns builder with other colliders.
    [[nodiscard]] Builder& WithColliders(
        const std::vector<Collider3Ptr>& others);

    //! Builds ColliderSet3.
    [[nodiscard]] ColliderSet3 Build() const;

    //! Builds shared pointer of ColliderSet3 instance.
    [[nodiscard]] ColliderSet3Ptr MakeShared() const;

 private:
    std::vector<Collider3Ptr> m_colliders;
};
}  // namespace CubbyFlow

#endif