// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_COLLIDER_HPP
#define CUBBYFLOW_COLLIDER_HPP

#include <Core/Geometry/Surface.hpp>

#include <functional>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for generic collider object.
//!
//! This class contains basic interfaces for colliders. Most of the
//! functionalities are implemented within this class, except the member
//! function Collider::VelocityAt. This class also let the subclasses to
//! provide a Surface instance to define collider surface using
//! Collider::SetSurface function.
//!
template <size_t N>
class Collider
{
 public:
    //!
    //! \brief Callback function type for update calls.
    //!
    //! This type of callback function will take the collider pointer, current
    //! time, and time interval in seconds.
    //!
    using OnBeginUpdateCallback =
        std::function<void(Collider*, double, double)>;

    //! Default constructor.
    Collider() = default;

    //! Default virtual destructor.
    virtual ~Collider() = default;

    //! Default copy constructor.
    Collider(const Collider& other) = default;

    //! Default move constructor.
    Collider(Collider&& other) noexcept = default;

    //! Default copy assignment operator.
    Collider& operator=(const Collider& other) = default;

    //! Default move assignment operator.
    Collider& operator=(Collider&& other) noexcept = default;

    //! Returns the velocity of the collider at given \p point.
    [[nodiscard]] virtual Vector<double, N> VelocityAt(
        const Vector<double, N>& point) const = 0;

    //!
    //! Resolves collision for given point.
    //!
    //! \param radius Radius of the colliding point.
    //! \param restitutionCoefficient Defines the restitution effect.
    //! \param position Input and output position of the point.
    //! \param velocity Input and output velocity of the point.
    //!
    void ResolveCollision(double radius, double restitutionCoefficient,
                          Vector<double, N>* position,
                          Vector<double, N>* velocity);

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
    [[nodiscard]] const std::shared_ptr<Surface<N>>& GetSurface() const;

    //! Updates the collider state.
    void Update(double currentTimeInSeconds, double timeIntervalInSeconds);

    //!
    //! \brief      Sets the callback function to be called when
    //!             Collider::update function is invoked.
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
        Vector<double, N> point;
        Vector<double, N> normal;
        Vector<double, N> velocity;
    };

    //! Assigns the surface instance from the subclass.
    void SetSurface(const std::shared_ptr<Surface<N>>& newSurface);

    //! Outputs closest point's information.
    void GetClosestPoint(const std::shared_ptr<Surface<N>>& surface,
                         const Vector<double, N>& queryPoint,
                         ColliderQueryResult* result) const;

    //! Returns true if given point is in the opposite side of the surface.
    [[nodiscard]] bool IsPenetrating(const ColliderQueryResult& colliderPoint,
                                     const Vector<double, N>& position,
                                     double radius);

 private:
    std::shared_ptr<Surface<N>> m_surface;
    double m_frictionCoefficient = 0.0;
    OnBeginUpdateCallback m_onUpdateCallback;
};

//! 2-D collider type.
using Collider2 = Collider<2>;

//! 3-D collider type.
using Collider3 = Collider<3>;

//! Shared pointer type for the Collider2.
using Collider2Ptr = std::shared_ptr<Collider2>;

//! Shared pointer type for the Collider3.
using Collider3Ptr = std::shared_ptr<Collider3>;
}  // namespace CubbyFlow

#endif