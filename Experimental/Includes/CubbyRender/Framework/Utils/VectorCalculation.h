/*************************************************************************
> File Name: VectorCalculation.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Calculation utilities realted to vector. (ex. calculate normal of three vertices).
> Created Time: 2020/04/04
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VECTORCALCULATION_H
#define CUBBYFLOW_VECTORCALCULATION_H

#include <Core/Vector/Vector.h>

namespace CubbyFlow {
namespace CubbyRender {

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
    Vector<T, N> calculateNormal(const Vector<T, N>& v1, const Vector<T, N>& v2, const Vector<T, N>& v3);
}
}

#include <Framework/Utils/VectorCalculation-Impl.h>

#endif