// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_IMPLICIT_SURFACE_HPP
#define CUBBYFLOW_IMPLICIT_SURFACE_HPP

#include <Core/Geometry/Surface.hpp>

namespace CubbyFlow
{
//! Abstract base class for N-D implicit surface.
template <size_t N>
class ImplicitSurface : public Surface<N>
{
 public:
    using Surface<N>::transform;
    using Surface<N>::isNormalFlipped;

    //! Default constructor.
    ImplicitSurface(const Transform<N>& _transform = Transform<N>{},
                    bool _isNormalFlipped = false);

    //! Copy constructor.
    ImplicitSurface(const ImplicitSurface& other);

    //! Default destructor.
    ~ImplicitSurface() override = default;

    //! Returns signed distance from the given point \p otherPoint.
    double SignedDistance(const Vector<double, N>& otherPoint) const;

 protected:
    //! Returns signed distance from the given point \p otherPoint in local
    //! space.
    virtual double SignedDistanceLocal(
        const Vector<double, N>& otherPoint) const = 0;

 private:
    double ClosestDistanceLocal(
        const Vector<double, N>& otherPoint) const override;
};

//! 2-D ImplicitSurface type.
using ImplicitSurface2 = ImplicitSurface<2>;

//! 3-D ImplicitSurface type.
using ImplicitSurface3 = ImplicitSurface<3>;

//! Shared pointer type for the ImplicitSurface2.
using ImplicitSurface2Ptr = std::shared_ptr<ImplicitSurface2>;

//! Shared pointer type for the ImplicitSurface3.
using ImplicitSurface3Ptr = std::shared_ptr<ImplicitSurface3>;
}  // namespace CubbyFlow

#endif