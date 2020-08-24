/*************************************************************************
> File Name: VectorUtils.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Calculation utilities realted to vector. (ex. calculate normal of three vertices).
> Created Time: 2020/04/04
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_VECTOR_UTILS_HPP
#define CUBBYFLOW_VOX_VECTOR_UTILS_HPP

#include <Core/Vector/Vector.hpp>

namespace Vox {

	//!
	//! \brief Return given combination of the vertices can be triangle or not.
	//!
	//! Simply compute two slopes of the line v1~v2 and v2~v3.
	//! If computed two slopes are equal, it means v1, v2 and v3 are on the same line.
	//! ,e.g it cant be a triangle.
	//! Otherwise they can be a triangle.
	//! \param v1  a vertex of the triangle
	//! \param v2  a vertex of the triangle
	//! \param v3  a vertex of the triangle
	//!
    template <typename T, size_t N>
	bool CheckTriangle(const CubbyFlow::Vector<T, N>& v1, const CubbyFlow::Vector<T, N>& v2, const CubbyFlow::Vector<T, N>& v3);

    //!
	//! \brief Return Normal Vector of the given three vectors.
	//!
	//! This Util function calculate two edge from the three given vertices.
    //! Calculate cross product of the two edge and return.
	//! 
    //! **Three given vertices must be able to construct the triangle.**
    //!
	//! \code{.cpp}
	//! Vector3F v1, v2, v3;
    //! //! insert data to v1, v2, v3 ...
    //! Vector3F normal = calculateNormal(v1, v2, v3);
	//! \endcode
	//!
	//! \param v1  a vertex of the triangle
	//! \param v2  a vertex of the triangle
	//! \param v3  a vertex of the triangle
	//!
    template <typename T, size_t N>
    CubbyFlow::Vector<T, N> CalculateNormal(const CubbyFlow::Vector<T, N>& v1, const CubbyFlow::Vector<T, N>& v2, const CubbyFlow::Vector<T, N>& v3);
}

#include <Vox/VectorUtils-Impl.hpp>

#endif