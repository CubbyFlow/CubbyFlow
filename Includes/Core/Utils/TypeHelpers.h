/*************************************************************************
> File Name: TypeHelpers.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Type helpers for CubbyFlow.
> Created Time: 2017/03/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_TYPE_HELPERS_H
#define CUBBYFLOW_TYPE_HELPERS_H

namespace CubbyFlow
{
	//! Returns the type of the value itself.
	template <typename T>
	struct ScalarType
	{
		typedef T value;
	};
}

#endif