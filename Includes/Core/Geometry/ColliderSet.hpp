// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_COLLIDER_SET_HPP
#define CUBBYFLOW_COLLIDER_SET_HPP

#include <Core/Array/Array.hpp>
#include <Core/Geometry/Collider.hpp>
#include <Core/Geometry/SurfaceSet.hpp>

#include <memory>

namespace CubbyFlow
{
//! Collection of N-D colliders
template <size_t N>
class ColliderSet final : public Collider<N>
{
 public:
    class Builder;

    using Collider<N>::GetSurface;
    using Collider<N>::SetSurface;

    //! Default constructor.
    ColliderSet();

    //! Constructs with other colliders.
    explicit ColliderSet(
        const ConstArrayView1<std::shared_ptr<Collider<N>>>& others);

    //! Returns the velocity of the collider at given \p point.
    [[nodiscard]] Vector<double, N> VelocityAt(
        const Vector<double, N>& point) const override;

    //! Adds a collider to the set.
    void AddCollider(const std::shared_ptr<Collider<N>>& collider);

    //! Returns number of colliders.
    [[nodiscard]] size_t NumberOfColliders() const;

    //! Returns collider at index \p i.
    [[nodiscard]] std::shared_ptr<Collider<N>> GetCollider(size_t i) const;

    //! Returns builder for ColliderSet.
    static Builder GetBuilder();

 private:
    Array1<std::shared_ptr<Collider<N>>> m_colliders;
};

//! 2-D ColliderSet type.
using ColliderSet2 = ColliderSet<2>;

//! 3-D ColliderSet type.
using ColliderSet3 = ColliderSet<3>;

//! Shared pointer for the ColliderSet2 type.
using ColliderSet2Ptr = std::shared_ptr<ColliderSet2>;

//! Shared pointer for the ColliderSet3 type.
using ColliderSet3Ptr = std::shared_ptr<ColliderSet3>;

//!
//! \brief Front-end to create ColliderSet objects step by step.
//!
template <size_t N>
class ColliderSet<N>::Builder final
{
 public:
    //! Returns builder with other colliders.
    Builder& WithColliders(
        const ConstArrayView1<std::shared_ptr<Collider<N>>>& others);

    //! Builds ColliderSet.
    ColliderSet Build() const;

    //! Builds shared pointer of ColliderSet instance.
    std::shared_ptr<ColliderSet<N>> MakeShared() const;

 private:
    Array1<std::shared_ptr<Collider<N>>> m_colliders;
};
}  // namespace CubbyFlow

#endif