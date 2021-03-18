// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_COLLIDER_SET2_HPP
#define CUBBYFLOW_COLLIDER_SET2_HPP

#include <Core/Geometry/Collider.hpp>

#include <vector>

namespace CubbyFlow
{
//! Collection of 2-D colliders
class ColliderSet2 final : public Collider2
{
 public:
    class Builder;

    //! Default constructor.
    ColliderSet2();

    //! Constructs with other colliders.
    explicit ColliderSet2(const std::vector<Collider2Ptr>& others);

    //! Returns the velocity of the collider at given \p point.
    [[nodiscard]] Vector2D VelocityAt(const Vector2D& point) const override;

    //! Adds a collider to the set.
    void AddCollider(const Collider2Ptr& collider);

    //! Returns number of colliders.
    [[nodiscard]] size_t NumberOfColliders() const;

    //! Returns collider at index \p i.
    [[nodiscard]] Collider2Ptr Collider(size_t i) const;

    //! Returns builder fox ColliderSet2.
    [[nodiscard]] static Builder GetBuilder();

 private:
    std::vector<Collider2Ptr> m_colliders;
};

//! Shared pointer for the ColliderSet2 type.
typedef std::shared_ptr<ColliderSet2> ColliderSet2Ptr;

//!
//! \brief Front-end to create ColliderSet2 objects step by step.
//!
class ColliderSet2::Builder final
{
 public:
    //! Returns builder with other colliders.
    [[nodiscard]] Builder& WithColliders(
        const std::vector<Collider2Ptr>& others);

    //! Builds ColliderSet2.
    [[nodiscard]] ColliderSet2 Build() const;

    //! Builds shared pointer of ColliderSet2 instance.
    [[nodiscard]] ColliderSet2Ptr MakeShared() const;

 private:
    std::vector<Collider2Ptr> m_colliders;
};
}  // namespace CubbyFlow

#endif