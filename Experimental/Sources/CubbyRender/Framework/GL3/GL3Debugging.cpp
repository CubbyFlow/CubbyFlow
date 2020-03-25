/*************************************************************************
> File Name: GL3Debugging.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Super simple GL3 Debugger implementation
> Created Time: 2020/02/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifdef CUBBYFLOW_USE_GL

#include <Framework/GL3/GL3Debugging.h>

#include <Core/Utils/Logging.h>
#include <glad/glad.h>
#include <string>

namespace CubbyFlow {
namespace CubbyRender {

	int GLDebugger::CheckError(const char* filename, int line)
	{
		GLenum errorCode;
		std::string error;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
		}

        if (error.empty() == false)
        {
            CUBBYFLOW_ERROR << filename << ":" << line << ": error: " << error;
        }
		else
		{
			CUBBYFLOW_ERROR << filename << ":" << line << " No error occurred here.";
		}

		return errorCode == GL_NO_ERROR ? 0 : 1;
	}
}
}

	

#endif 