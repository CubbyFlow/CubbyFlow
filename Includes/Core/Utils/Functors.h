/*************************************************************************
> File Name: Functors.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Functors for CubbyFlow.
> Created Time: 2017/09/26
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FUNCTORS_H
#define CUBBYFLOW_FUNCTORS_H

namespace CubbyFlow
{
	//! Type casting operator.
	template <typename T, typename U>
	struct TypeCast
	{
		constexpr U operator()(const T& a) const;
	};

	//! Reverse minus operator.
	template <typename T>
	struct RMinus
	{
		constexpr T operator()(const T& a, const T& b) const;
	};

	//! Reverse divides operator.
	template <typename T>
	struct RDivides
	{
		constexpr T operator()(const T& a, const T& b) const;
	};
}

#include <Core/Utils/Functors-Impl.h>

#endif