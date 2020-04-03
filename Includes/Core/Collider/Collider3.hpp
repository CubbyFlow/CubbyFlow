// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_COLLIDER3_HPP
#define CUBBYFLOW_COLLIDER3_HPP

#include <Core/Surface/Surface3.hpp>

#include <functional>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for generic collider object.
//!
//! This class contains basic interfaces for colliders. Most of the
//! functionalities are implemented within this class, except the member
//! function Collider3::VelocityAt. This class also let the subclasses to
//! provide a Surface3 instance to define collider surface using
//! Collider3::SetSurface function.
//!
class Collider3
{
 public:
    //!
    //! \brief Callback function type for update calls.
    //!
    //! This type of callback function will take the collider pointer, current
    //! time, and time interval in seconds.
    //!
    using OnBeginUpdateCallback =
        std::function<void(Collider3*, double, double)>;

    //! Default constructor.
    Collider3();

    //! Default destructor.
    virtual ~Collider3();

    //! Returns the velocity of the collider at given \p point.
    virtual Vector3D VelocityAt(const Vector3D& point) const = 0;

    //!
    //! Resolves collision for given point.
    //!
    //! \param radius Radius of the colliding point.
    //! \param restitutionCoefficient Defines the restitution effect.
    //! \param position Input and output position of the point.
    //! \param velocity Input and output velocity of the point.
    //!
    void ResolveCollision(double radius, double restitutionCoefficient,
                          Vector3D* position, Vector3D* velocity);

    //! Returns friction coefficient.
    double GetFrictionCoefficient() const;

    //!
    //! \brief Sets the friction coefficient.
    //!
    //! This function assigns the friction coefficient to the collider. Any
    //! negative inputs will be clamped to zero.
    //!
    void SetFrictionCoefficient(double newFrictionCoeffient);

    //! Returns the surface instance.
    const Surface3Ptr& GetSurface() const;

    //! Updates the collider state.
    void Update(double currentTimeInSeconds, double timeIntervalInSeconds);

    //!
    //! \brief      Sets the callback function to be called when
    //!             Collider3::update function is invoked.
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
        double distance;
        Vector3D point;
        Vector3D normal;
        Vector3D velocity;
    };

    //! Assigns the surface instance from the subclass.
    void SetSurface(const Surface3Ptr& newSurface);

    //! Outputs closest point's information.
    void GetClosestPoint(const Surface3Ptr& surface, const Vector3D& queryPoint,
                         ColliderQueryResult* result) const;

    //! Returns true if given point is in the opposite side of the surface.
    bool IsPenetrating(const ColliderQueryResult& colliderPoint,
                       const Vector3D& position, double radius);

 private:
    Surface3Ptr m_surface;
    double m_frictionCoeffient = 0.0;
    OnBeginUpdateCallback m_onUpdateCallback;
};

//! Shared pointer type for the Collider3.
using Collider3Ptr = std::shared_ptr<Collider3>;
}  // namespace CubbyFlow

#endif