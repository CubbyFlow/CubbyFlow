/*************************************************************************
> File Name: Ray.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Class for ray.
> Created Time: 2017/03/21
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_RAY_H
#define CUBBYFLOW_RAY_H

#include <cstdlib>
#include <type_traits>

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