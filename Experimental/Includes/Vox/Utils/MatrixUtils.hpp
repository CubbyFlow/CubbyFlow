/*************************************************************************
> File Name: MatrixUtils.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Calculation utilities realted to matrix. (ex. create perspective matrix).
> Created Time: 2021/01/10
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_MATRIX_UTILS_HPP
#define CUBBYFLOW_VOX_MATRIX_UTILS_HPP

#include <Core/Matrix/Matrix4x4.hpp>
#include <Core/Vector/Vector3.hpp>

namespace Vox {

	//!
	//! \brief Return perspective projection matrix.
	//! \param aspect - (window width) / (window height) 
	//! \param near - camera near value which is frustum culling min z value
	//! \param far - camera far value which is frustum culling max z value
	//! \param fov - field of view, must be degree value, not radian.
	template <typename T>
	[[nodiscard]] CubbyFlow::Matrix4x4<T> Perspective(const T aspect, const T near, const T far, const T fov);

    //!
	//! \brief Return view matrix.
	//! \param origin - camera position
	//! \param dir - camera looking direction vector
	//! \param up - camera up vector
    template <typename T>
	[[nodiscard]] CubbyFlow::Matrix4x4<T> LookAt(const CubbyFlow::Vector3<T>& origin, const CubbyFlow::Vector3<T>& dir, const CubbyFlow::Vector3<T>& up);

}

#include <Vox/Utils/MatrixUtils-Impl.hpp>

#endif