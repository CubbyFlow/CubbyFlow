/*************************************************************************
> File Name: PDE.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Partial differential equation functions for CubbyFlow.
> Created Time: 2017/08/31
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PDE_H
#define CUBBYFLOW_PDE_H

#include <array>

namespace CubbyFlow
{
	//!
	//! \brief 1st order upwind differencing.
	//! d0[1] is the origin.
	//!
	//! \code
	//!   |------|------|
	//! d0[0]  d0[1]  d0[2]
	//! \endcode
	//!
	//! Returns two solutions for each side.
	//!
	template <typename T>
	std::array<T, 2> Upwind1(T* d0, T dx);

	//!
	//! \brief 1st order upwind differencing.
	//! d0[1] is the origin.
	//!
	//! \code
	//!   |------|------|
	//! d0[0]  d0[1]  d0[2]
	//! \endcode
	//!
	template <typename T>
	T Upwind1(T* d0, T dx, bool isDirectionPositive);

	//!
	//! \brief 2nd order central differencing.
	//! d0[1] is the origin.
	//!
	//! \code
	//!   |------|------|
	//! d0[0]  d0[1]  d0[2]
	//! \endcode
	//!
	template <typename T>
	T CD2(T* d0, T dx);

	//!
	//! \brief 3rd order ENO.
	//! d0[3] is the origin.
	//!
	//! \code
	//!   |------|------|------|------|------|------|
	//! d0[0]  d0[1]  d0[2]  d0[3]  d0[4]  d0[5]  d0[6]
	//! \endcode
	//!
	//! Returns two solutions for each side.
	//!
	template <typename T>
	std::array<T, 2> ENO3(T* d0, T dx);

	//!
	//! \brief 3rd order ENO.
	//! d0[3] is the origin.
	//!
	//! \code
	//!   |------|------|------|------|------|------|
	//! d0[0]  d0[1]  d0[2]  d0[3]  d0[4]  d0[5]  d0[6]
	//! \endcode
	//!
	//!
	template <typename T>
	T ENO3(T* d0, T dx, bool isDirectionPositive);

	//!
	//! \brief 5th order WENO.
	//! d0[3] is the origin.
	//!
	//! \code
	//!   |------|------|------|------|------|------|
	//! d0[0]  d0[1]  d0[2]  d0[3]  d0[4]  d0[5]  d0[6]
	//! \endcode
	//!
	//! Returns two solutions for each side.
	//!
	//!
	template <typename T>
	std::array<T, 2> WENO5(T* v, T h, T eps = 1.0e-8);

	//!
	//! \brief 5th order WENO.
	//! d0[3] is the origin.
	//!
	//! \code
	//!   |------|------|------|------|------|------|
	//! d0[0]  d0[1]  d0[2]  d0[3]  d0[4]  d0[5]  d0[6]
	//! \endcode
	//!
	template <typename T>
	T WENO5(T* v, T h, bool is_velocity_positive, T eps = 1.0e-8);
}

#include <Math/PDE-Impl.h>

#endif