/*************************************************************************
> File Name: CppUtils.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: C++ utility methods for CubbyFlow.
> Created Time: 2017/09/26
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CPP_UTILS_H
#define CUBBYFLOW_CPP_UTILS_H

#include <algorithm>

namespace CubbyFlow
{
	template <class ForwardIter, class T, class Compare = std::less<T>>
	ForwardIter BinaryFind(ForwardIter first, ForwardIter last, const T& value, Compare comp = {});
}

#include <Core/Utils/CppUtils-Impl.h>

#endif