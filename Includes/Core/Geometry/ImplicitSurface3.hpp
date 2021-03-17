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

#include <Core/Geometry/Surface.hpp>

namespace CubbyFlow
{
//! Abstract base class for 3-D implicit surface.
class ImplicitSurface3 : public Surface3
{
 public:
    //! Default constructor.
    ImplicitSurface3(const Transform3& _transform = Transform3{},
                     bool _isNormalFlipped = false);

    //! Default copy constructor.
    ImplicitSurface3(const ImplicitSurface3&) = default;

    //! Default move constructor.
    ImplicitSurface3(ImplicitSurface3&&) noexcept = default;

    //! Default virtual destructor.
    ~ImplicitSurface3() override = default;

    //! Default copy assignment operator.
    ImplicitSurface3& operator=(const ImplicitSurface3&) = default;

    //! Default move assignment operator.
    ImplicitSurface3& operator=(ImplicitSurface3&&) noexcept = default;

    //! Returns signed distance from the given point \p otherPoint.
    [[nodiscard]] double SignedDistance(const Vector3D& otherPoint) const;

 protected:
    //! Returns signed distance from the given point \p otherPoint in local
    //! space.
    [[nodiscard]] virtual double SignedDistanceLocal(
        const Vector3D& otherPoint) const = 0;

 private:
    [[nodiscard]] double ClosestDistanceLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] bool IsInsideLocal(const Vector3D& otherPoint) const override;
};

//! Shared pointer type for the ImplicitSurface3.
using ImplicitSurface3Ptr = std::shared_ptr<ImplicitSurface3>;
}  // namespace CubbyFlow

#endif