// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_COLLIDER2_HPP
#define CUBBYFLOW_COLLIDER2_HPP

#include <Core/Geometry/Surface2.hpp>

#include <functional>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for generic collider object.
//!
//! This class contains basic interfaces for colliders. Most of the
//! functionalities are implemented within this class, except the member
//! function Collider2::VelocityAt. This class also let the subclasses to
//! provide a Surface2 instance to define collider surface using
//! Collider2::SetSurface function.
//!
class Collider2
{
 public:
    //!
    //! \brief Callback function type for update calls.
    //!
    //! This type of callback function will take the collider pointer, current
    //! time, and time interval in seconds.
    //!
    using OnBeginUpdateCallback =
        std::function<void(Collider2*, double, double)>;

    //! Default constructor.
    Collider2() = default;

    //! Default copy constructor.
    Collider2(const Collider2&) = default;

    //! Default move constructor.
    Collider2(Collider2&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~Collider2() = default;

    //! Default copy assignment operator.
    Collider2& operator=(const Collider2&) = default;

    //! Default move assignment operator.
    Collider2& operator=(Collider2&&) noexcept = default;

    //! Returns the velocity of the collider at given \p point.
    [[nodiscard]] virtual Vector2D VelocityAt(const Vector2D& point) const = 0;

    //!
    //! Resolves collision for given point.
    //!
    //! \param radius Radius of the colliding point.
    //! \param restitutionCoefficient Defines the restitution effect.
    //! \param position Input and output position of the point.
    //! \param velocity Input and output velocity of the point.
    //!
    void ResolveCollision(double radius, double restitutionCoefficient,
                          Vector2D* position, Vector2D* velocity) const;

    //! Returns friction coefficient.
    [[nodiscard]] double GetFrictionCoefficient() const;

    //!
    //! \brief Sets the friction coefficient.
    //!
    //! This function assigns the friction coefficient to the collider. Any
    //! negative inputs will be clamped to zero.
    //!
    void SetFrictionCoefficient(double newFrictionCoefficient);

    //! Returns the surface instance.
    [[nodiscard]] const Surface2Ptr& GetSurface() const;

    //! Updates the collider state.
    void Update(double currentTimeInSeconds, double timeIntervalInSeconds);

    //!
    //! \brief      Sets the callback function to be called when
    //!             Collider2::update function is invoked.
    //!
    //! The callback function takes current simulation time in seconds unit. Use
    //! this callback to track any motion or state changes related to this
    //! collider.
    //!
    //! \param[in]  callback The callback function.
    //!
    void SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback);

 protected:
    //! Internal query result structure.
    struct ColliderQueryResult final
    {
        double distance = 0.0;
        Vector2D point;
        Vector2D normal;
        Vector2D velocity;
    };

    //! Assigns the surface instance from the subclass.
    void SetSurface(const Surface2Ptr& newSurface);

    //! Outputs closest point's information.
    void GetClosestPoint(const Surface2Ptr& surface, const Vector2D& queryPoint,
                         ColliderQueryResult* result) const;

    //! Returns true if given point is in the opposite side of the surface.
    [[nodiscard]] bool IsPenetrating(const ColliderQueryResult& colliderPoint,
                                     const Vector2D& position,
                                     double radius) const;

 private:
    Surface2Ptr m_surface;
    double m_frictionCoefficient = 0.0;
    OnBeginUpdateCallback m_onUpdateCallback;
};

//! Shared pointer type for the Collider2.
using Collider2Ptr = std::shared_ptr<Collider2>;
}  // namespace CubbyFlow

#endif