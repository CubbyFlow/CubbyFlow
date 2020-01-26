/*************************************************************************
> File Name: FDMUtils.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Finite Difference Method util functions for CubbyFlow.
> Created Time: 2017/07/06
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FDM_UTILS_H
#define CUBBYFLOW_FDM_UTILS_H

#include <Core/Array/ArrayAccessor2.hpp>
#include <Core/Array/ArrayAccessor3.hpp>
#include <Core/Vector/Vector2.h>
#include <Core/Vector/Vector3.h>

namespace CubbyFlow
{
	//! \brief Returns 2-D gradient vector from given 2-D scalar grid-like array
	//!        \p data, \p gridSpacing, and array index (\p i, \p j).
	Vector2D Gradient2(const ConstArrayAccessor2<double>& data,
		const Vector2D& gridSpacing, size_t i, size_t j);

	//! \brief Returns 2-D gradient vectors from given 2-D vector grid-like array
	//!        \p data, \p gridSpacing, and array index (\p i, \p j).
	std::array<Vector2D, 2> Gradient2(const ConstArrayAccessor2<Vector2D>& data,
		const Vector2D& gridSpacing, size_t i, size_t j);

	//! \brief Returns 3-D gradient vector from given 3-D scalar grid-like array
	//!        \p data, \p gridSpacing, and array index (\p i, \p j, \p k).
	Vector3D Gradient3(const ConstArrayAccessor3<double>& data,
		const Vector3D& gridSpacing, size_t i, size_t j, size_t k);

	//! \brief Returns 3-D gradient vectors from given 3-D vector grid-like array
	//!        \p data, \p gridSpacing, and array index (\p i, \p j, \p k).
	std::array<Vector3D, 3> Gradient3(const ConstArrayAccessor3<Vector3D>& data,
		const Vector3D& gridSpacing, size_t i, size_t j, size_t k);

	//! \brief Returns Laplacian value from given 2-D scalar grid-like array
	//!        \p data, \p gridSpacing, and array index (\p i, \p j).
	double Laplacian2(const ConstArrayAccessor2<double>& data,
		const Vector2D& gridSpacing, size_t i, size_t j);

	//! \brief Returns 2-D Laplacian vectors from given 2-D vector grid-like array
	//!        \p data, \p gridSpacing, and array index (\p i, \p j).
	Vector2D Laplacian2(const ConstArrayAccessor2<Vector2D>& data,
		const Vector2D& gridSpacing, size_t i, size_t j);

	//! \brief Returns Laplacian value from given 3-D scalar grid-like array
	//!        \p data, \p gridSpacing, and array index (\p i, \p j, \p k).
	double Laplacian3(const ConstArrayAccessor3<double>& data,
		const Vector3D& gridSpacing, size_t i, size_t j, size_t k);

	//! \brief Returns 3-D Laplacian vectors from given 3-D vector grid-like array
	//!        \p data, \p gridSpacing, and array index (\p i, \p j, \p k).
	Vector3D Laplacian3(const ConstArrayAccessor3<Vector3D>& data,
		const Vector3D& gridSpacing, size_t i, size_t j, size_t k);
}

#endif