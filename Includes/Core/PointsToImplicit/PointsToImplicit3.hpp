// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINTS_TO_IMPLICIT3_HPP
#define CUBBYFLOW_POINTS_TO_IMPLICIT3_HPP

#include <Core/Array/ArrayView.hpp>
#include <Core/Grid/ScalarGrid.hpp>

namespace CubbyFlow
{
//! Abstract base class for 3-D points-to-implicit converters.
class PointsToImplicit3
{
 public:
    //! Default constructor.
    PointsToImplicit3() = default;

    //! Default copy constructor.
    PointsToImplicit3(const PointsToImplicit3&) = default;

    //! Default move constructor.
    PointsToImplicit3(PointsToImplicit3&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~PointsToImplicit3() = default;

    //! Default copy assignment operator.
    PointsToImplicit3& operator=(const PointsToImplicit3&) = default;

    //! Default move assignment operator.
    PointsToImplicit3& operator=(PointsToImplicit3&&) noexcept = default;

    //! Converts the given points to implicit surface scalar field.
    virtual void Convert(const ConstArrayView1<Vector3D>& points,
                         ScalarGrid3* output) const = 0;
};

//! Shared pointer for the PointsToImplicit3 type.
using PointsToImplicit3Ptr = std::shared_ptr<PointsToImplicit3>;
}  // namespace CubbyFlow

#endif