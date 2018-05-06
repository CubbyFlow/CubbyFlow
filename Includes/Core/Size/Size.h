/*************************************************************************
> File Name: Size.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: N-D size type.
> Created Time: 2017/02/04
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SIZE_H
#define CUBBYFLOW_SIZE_H

#include <Core/Point/Point.h>

namespace CubbyFlow
{
	template <size_t N>
	using Size = Point<size_t, N>;
}

#endif