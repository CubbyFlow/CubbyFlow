/*************************************************************************
> File Name: Functors-Impl.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Functors for CubbyFlow.
> Created Time: 2017/09/26
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FUNCTORS_IMPL_H
#define CUBBYFLOW_FUNCTORS_IMPL_H

namespace CubbyFlow
{
	template <typename T, typename U>
	constexpr U TypeCast<T, U>::operator()(const T& a) const
	{
		return static_cast<U>(a);
	}

	template <typename T>
	constexpr T RMinus<T>::operator()(const T& a, const T& b) const
	{
		return b - a;
	}

	template <typename T>
	constexpr T RDivides<T>::operator()(const T& a, const T& b) const
	{
		return b / a;
	}
}

#endif