// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_IMPLICIT_SURFACE2_HPP
#define CUBBYFLOW_IMPLICIT_SURFACE2_HPP

#include <Core/Surface/Surface2.hpp>

namespace CubbyFlow
{
//! Abstract base class for 2-D implicit surface.
class ImplicitSurface2 : public Surface2
{
 public:
    //! Default constructor.
    ImplicitSurface2(const Transform2& transform = Transform2(),
                     bool isNormalFlipped = false);

    //! Copy constructor.
    ImplicitSurface2(const ImplicitSurface2& other);

    //! Default destructor.
    virtual ~ImplicitSurface2();

    //! Returns signed distance from the given point \p otherPoint.
    double SignedDistance(const Vector2D& otherPoint) const;

 protected:
    //! Returns signed distance from the given point \p otherPoint in local
    //! space.
    virtual double SignedDistanceLocal(const Vector2D& otherPoint) const = 0;

 private:
    double ClosestDistanceLocal(const Vector2D& otherPoint) const override;

    bool IsInsideLocal(const Vector2D& otherPoint) const override;
};

//! Shared pointer type for the ImplicitSurface2.
using ImplicitSurface2Ptr = std::shared_ptr<ImplicitSurface2>;
}  // namespace CubbyFlow

#endif