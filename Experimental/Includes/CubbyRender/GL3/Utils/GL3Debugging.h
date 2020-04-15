/*************************************************************************
> File Name: GL3Debugging.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Super simple GL3 Debugger implementation
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_GL3DEBUGGING_H
#define CUBBYFLOW_GL3DEBUGGING_H

#ifdef CUBBYFLOW_USE_GL

namespace CubbyFlow {
namespace CubbyRender {

	class GLDebugger
	{
	public:
		static int CheckError(const char* filename, int line);
	};

	//! locate this macro where you want to check whether if error exists or not.
	#define CUBBYFLOW_CHECK_GLERROR() GLDebugger::CheckError(__FILE__, __LINE__)
}
}

#endif
#endif 