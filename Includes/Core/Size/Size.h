/*************************************************************************
> File Name: Size.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
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