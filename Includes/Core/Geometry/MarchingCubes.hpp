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

#include <Core/Array/ArrayView.hpp>
#include <Core/Geometry/TriangleMesh3.hpp>
#include <Core/Matrix/Matrix.hpp>

namespace CubbyFlow
{
//!
//! \brief      Computes marching cubes and extract triangle mesh from grid.
//!
//! This function computes the marching cube algorithm to extract triangle mesh
//! from the scalar grid field. The triangle mesh will be the iso-surface, and
//! the iso value can be specified. For the boundaries (the walls), it can be
//! specified whether to close or open with \p bndClose (default: close all).
//! Another boundary flag \p bndConnectivity can be used for specifying
//! topological connectivity of the boundary meshes (default: disconnect all).
//!
//! \param[in]  grid            The grid.
//! \param[in]  gridSize        The grid size.
//! \param[in]  origin          The origin.
//! \param[out] mesh            The output triangle mesh.
//! \param[in]  isoValue        The iso-surface value.
//! \param[in]  bndClose        The boundary open flag.
//! \param[in]  bndConnectivity The boundary connectivity flag.
//!
void MarchingCubes(const ConstArrayView3<double>& grid,
                   const Vector3D& gridSize, const Vector3D& origin,
                   TriangleMesh3* mesh, double isoValue = 0,
                   int bndClose = DIRECTION_ALL,
                   int bndConnectivity = DIRECTION_NONE);
}  // namespace CubbyFlow

#endif