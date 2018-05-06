/*************************************************************************
> File Name: Ray.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Class for ray.
> Created Time: 2017/03/21
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_RAY_H
#define CUBBYFLOW_RAY_H

#include <limits>

namespace CubbyFlow
{
	//!
	//! \brief      Class for ray.
	//!
	//! \tparam     T     The value type.
	//! \tparam     N     The dimension.
	//!
	template <typename T, size_t N>
	class Ray
	{
		static_assert(N != 2 && N != 3, "Not implemented.");
		static_assert(std::is_floating_point<T>::value, "Ray only can be instantiated with floating point types");
	};
}

#endif