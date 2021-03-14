// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT_GENERATOR2_HPP
#define CUBBYFLOW_POINT_GENERATOR2_HPP

#include <Core/Array/Array.hpp>
#include <Core/Geometry/BoundingBox.hpp>

#include <functional>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for 2-D point generator.
//!
//! This class provides interface for 2-D point generator. For given bounding
//! box and point spacing, the inherited classes generates points with specified
//! pattern.
//!
class PointGenerator2
{
 public:
    //! Default constructor.
    PointGenerator2() = default;

    //! Default copy constructor.
    PointGenerator2(const PointGenerator2&) = default;

    //! Default move constructor.
    PointGenerator2(PointGenerator2&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~PointGenerator2() = default;

    //! Default copy assignment operator.
    PointGenerator2& operator=(const PointGenerator2&) = default;

    //! Default move assignment operator.
    PointGenerator2& operator=(PointGenerator2&&) noexcept = default;

    //! Generates points to output array \p points inside given \p boundingBox
    //! with target point \p spacing.
    void Generate(const BoundingBox2D& boundingBox, double spacing,
                  Array1<Vector2D>* points) const;

    //!
    //! \brief Iterates every point within the bounding box with specified
    //! point pattern and invokes the callback function.
    //!
    //! This function iterates every point within the bounding box and invokes
    //! the callback function. The position of the point is specified by the
    //! actual implementation. The suggested spacing between the points are
    //! given by \p spacing. The input parameter of the callback function is
    //! the position of the point and the return value tells whether the
    //! iteration should stop or not.
    //!
    virtual void ForEachPoint(
        const BoundingBox2D& boundingBox, double spacing,
        const std::function<bool(const Vector2D&)>& callback) const = 0;
};

//! Shared pointer for the PointGenerator2 type.
using PointGenerator2Ptr = std::shared_ptr<PointGenerator2>;
}  // namespace CubbyFlow

#endif