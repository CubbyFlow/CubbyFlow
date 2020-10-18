// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_POINT_GENERATOR2_HPP
#define CUBBYFLOW_GRID_POINT_GENERATOR2_HPP

#include <Core/PointGenerator/PointGenerator2.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D regular-grid point generator.
//!
class GridPointGenerator2 final : public PointGenerator2
{
 public:
    //!
    //! \brief Invokes \p callback function for each regular grid points inside
    //! \p boundingBox.
    //!
    //! This function iterates every regular grid points inside \p boundingBox
    //! where \p spacing is the size of the unit cell of regular grid structure.
    //!
    void ForEachPoint(
        const BoundingBox2D& boundingBox, double spacing,
        const std::function<bool(const Vector2D&)>& callback) const override;
};

//! Shared pointer type for the GridPointGenerator2.
using GridPointGenerator2Ptr = std::shared_ptr<GridPointGenerator2>;
}  // namespace CubbyFlow

#endif