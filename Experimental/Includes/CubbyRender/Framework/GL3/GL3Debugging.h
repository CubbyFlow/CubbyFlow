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

    #ifdef _DEBUG
	int CheckError(const char* filename, int line);
	//! locate this macro where you want to check whether if error exists or not.
	#define CHECKERROR() GLDebugger::CheckError(__FILE__, __LINE__)
	#else
	#define CHECKERROR()
	#endif
}
}

#endif

#endif 