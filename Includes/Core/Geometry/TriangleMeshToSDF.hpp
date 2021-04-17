// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_TRIANGLE_MESH_TO_SDF_HPP
#define CUBBYFLOW_TRIANGLE_MESH_TO_SDF_HPP

#include <Core/Geometry/TriangleMesh3.hpp>
#include <Core/Grid/ScalarGrid.hpp>

namespace CubbyFlow
{
//! \brief Generates signed-distance field out of given triangle mesh.
//! This function generates signed-distance field from a triangle mesh. The sign
//! is determined by TriangleMesh3::IsInside (negative means inside).
//!
//! \param[in]      mesh	The mesh.
//! \param[in,out]  sdf     The output signed-distance field.
//!
void TriangleMeshToSDF(const TriangleMesh3& mesh, ScalarGrid3* sdf);
}  // namespace CubbyFlow

#endif