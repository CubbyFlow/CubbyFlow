/*************************************************************************
> File Name: CppUtils-Impl.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: C++ utility methods for CubbyFlow.
> Created Time: 2017/09/26
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_CPP_UTILS_IMPL_H
#define CUBBYFLOW_CPP_UTILS_IMPL_H

#include <algorithm>

namespace CubbyFlow
{
	// Source code from:
	// http://en.cppreference.com/w/cpp/algorithm/lower_bound
	template <class ForwardIter, class T, class Compare>
	ForwardIter BinaryFind(ForwardIter first, ForwardIter last, const T& value, Compare comp)
	{
		// Note: Both type T and the type after ForwardIt is dereferenced
		// must be implicitly convertible to both Type1 and Type2, used in Compare.
		// This is stricter than lower_bound requirement (see above)
		first = std::lower_bound(first, last, value, comp);
		return first != last && !comp(value, *first) ? first : last;
	}
}

#endif