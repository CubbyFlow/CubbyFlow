/*************************************************************************
> File Name: Field3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 3-D fields.
> Created Time: 2017/03/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FIELD3_H
#define CUBBYFLOW_FIELD3_H

#include <memory>

namespace CubbyFlow
{
	//! Abstract base class for 3-D fields.
	class Field3
	{
	public:
		Field3();
		virtual ~Field3();
	};

	using Field3Ptr = std::shared_ptr<Field3>;
}

#endif