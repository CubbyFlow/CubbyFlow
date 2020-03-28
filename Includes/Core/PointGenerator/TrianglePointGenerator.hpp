// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_TRIANGLE_POINT_GENERATOR_HPP
#define CUBBYFLOW_TRIANGLE_POINT_GENERATOR_HPP

#include <Core/PointGenerator/PointGenerator2.hpp>

namespace CubbyFlow
{
//!
//! \brief Right triangle point generator.
//!
class TrianglePointGenerator final : public PointGenerator2
{
 public:
    //!
    //! \brief Invokes \p callback function for each right triangle points
    //! inside \p boundingBox.
    //!
    //! This function iterates every right triangle points inside \p boundingBox
    //! where \p spacing is the size of the right triangle structure.
    //!
    void ForEachPoint(
        const BoundingBox2D& boundingBox, double spacing,
        const std::function<bool(const Vector2D&)>& callback) const override;
};

using TrianglePointGeneratorPtr = std::shared_ptr<TrianglePointGenerator>;
}  // namespace CubbyFlow

#endif