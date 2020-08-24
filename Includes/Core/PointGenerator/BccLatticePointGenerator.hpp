// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_BCC_LATTICE_POINT_GENERATOR_HPP
#define CUBBYFLOW_BCC_LATTICE_POINT_GENERATOR_HPP

#include <Core/PointGenerator/PointGenerator3.hpp>

namespace CubbyFlow
{
//!
//! \brief Body-centered lattice points generator.
//!
//! \see http://en.wikipedia.org/wiki/Cubic_crystal_system
//!      http://mathworld.wolfram.com/CubicClosePacking.html
//!
class BccLatticePointGenerator final : public PointGenerator3
{
 public:
    //!
    //! \brief Invokes \p callback function for each BCC-lattice points inside
    //! \p boundingBox.
    //!
    //! This function iterates every BCC-lattice points inside \p boundingBox
    //! where \p spacing is the size of the unit cell of BCC structure.
    //!
    void ForEachPoint(
        const BoundingBox3D& boundingBox, double spacing,
        const std::function<bool(const Vector3D&)>& callback) const override;
};

//! Shared pointer type for the BccLatticePointGenerator.
using BccLatticePointGeneratorPtr = std::shared_ptr<BccLatticePointGenerator>;
}  // namespace CubbyFlow

#endif