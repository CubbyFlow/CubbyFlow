// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_IMPLICIT_SURFACE3_HPP
#define CUBBYFLOW_IMPLICIT_SURFACE3_HPP

#include <Core/Surface/Surface3.hpp>

namespace CubbyFlow
{
//! Abstract base class for 3-D implicit surface.
class ImplicitSurface3 : public Surface3
{
 public:
    //! Default constructor.
    ImplicitSurface3(const Transform3& transform = Transform3(),
                     bool isNormalFlipped = false);

    //! Copy constructor.
    ImplicitSurface3(const ImplicitSurface3& other);

    //! Default copy assignment operator.
    ImplicitSurface3& operator=(const ImplicitSurface3& other) = default;

    //! Default destructor.
    virtual ~ImplicitSurface3();

    //! Returns signed distance from the given point \p otherPoint.
    double SignedDistance(const Vector3D& otherPoint) const;

 protected:
    //! Returns signed distance from the given point \p otherPoint in local
    //! space.
    virtual double SignedDistanceLocal(const Vector3D& otherPoint) const = 0;

 private:
    double ClosestDistanceLocal(const Vector3D& otherPoint) const override;

    bool IsInsideLocal(const Vector3D& otherPoint) const override;
};

//! Shared pointer type for the ImplicitSurface3.
using ImplicitSurface3Ptr = std::shared_ptr<ImplicitSurface3>;
}  // namespace CubbyFlow

#endif