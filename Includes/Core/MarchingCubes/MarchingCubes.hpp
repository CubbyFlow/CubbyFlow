// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MARCHING_CUBES_HPP
#define CUBBYFLOW_MARCHING_CUBES_HPP

#include <Core/Vector/Vector3.h>
#include <Core/Array/ArrayAccessor3.hpp>
#include <Core/Geometry/TriangleMesh3.hpp>

namespace CubbyFlow
{
//!
//! \brief      Computes marching cubes and extract triangle mesh from grid.
//!
//! This function computes the marching cube algorithm to extract triangle mesh
//! from the scalar grid field. The triangle mesh will be the iso-surface, and
//! the iso-value can be specified. For the boundaries (or the walls), it can be
//! specified whether to close or open.
//!
//! \param[in]  grid     The grid.
//! \param[in]  gridSize The grid size.
//! \param[in]  origin   The origin.
//! \param      mesh     The output triangle mesh.
//! \param[in]  isoValue The iso-surface value.
//! \param[in]  bndFlag  The boundary direction flag.
//!
void MarchingCubes(const ConstArrayAccessor3<double>& grid,
                   const Vector3D& gridSize, const Vector3D& origin,
                   TriangleMesh3* mesh, double isoValue = 0,
                   int bndFlag = DIRECTION_ALL);
}  // namespace CubbyFlow

#endif