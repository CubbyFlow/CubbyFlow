/*************************************************************************
> File Name: MarchingCubes.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Computes marching cubes and extract triangle mesh from grid.
> Created Time: 2017/07/09
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_MARCHING_CUBES_H
#define CUBBYFLOW_MARCHING_CUBES_H

#include <Array/ArrayAccessor3.h>
#include <Geometry/TriangleMesh3.h>
#include <Vector/Vector3.h>

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
	void MarchingCubes(
		const ConstArrayAccessor3<double>& grid,
		const Vector3D& gridSize,
		const Vector3D& origin,
		TriangleMesh3* mesh,
		double isoValue = 0,
		int bndFlag = DIRECTION_ALL);
}

#endif