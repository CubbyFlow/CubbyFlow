// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINTS_TO_IMPLICIT2_HPP
#define CUBBYFLOW_POINTS_TO_IMPLICIT2_HPP

#include <Core/Array/ArrayView.hpp>
#include <Core/Grid/ScalarGrid.hpp>

namespace CubbyFlow
{
//! Abstract base class for 2-D points-to-implicit converters.
class PointsToImplicit2
{
 public:
    //! Default constructor.
    PointsToImplicit2() = default;

    //! Default copy constructor.
    PointsToImplicit2(const PointsToImplicit2&) = default;

    //! Default move constructor.
    PointsToImplicit2(PointsToImplicit2&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~PointsToImplicit2() = default;

    //! Default copy assignment operator.
    PointsToImplicit2& operator=(const PointsToImplicit2&) = default;

    //! Default move assignment operator.
    PointsToImplicit2& operator=(PointsToImplicit2&&) noexcept = default;

    //! Converts the given points to implicit surface scalar field.
    virtual void Convert(const ConstArrayView1<Vector2D>& points,
                         ScalarGrid2* output) const = 0;
};

//! Shared pointer for the PointsToImplicit2 type.
using PointsToImplicit2Ptr = std::shared_ptr<PointsToImplicit2>;
}  // namespace CubbyFlow

#endif