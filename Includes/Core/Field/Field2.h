/*************************************************************************
> File Name: Field2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 2-D fields.
> Created Time: 2017/03/13
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_FIELD2_H
#define CUBBYFLOW_FIELD2_H

#include <memory>

namespace CubbyFlow
{
	//! Abstract base class for 2-D fields.
	class Field2
	{
	public:
		Field2();
		virtual ~Field2();
	};

	using Field2Ptr = std::shared_ptr<Field2>;
}

#endif