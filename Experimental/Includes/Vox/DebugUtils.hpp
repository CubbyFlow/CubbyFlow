/*************************************************************************
> File Name: DebugUtils.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> References: OpenGL-Insights Sample chapter
> Purpose: Debug utility function collection
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_DEBUG_UTILS_HPP
#define CUBBYFLOW_VOX_DEBUG_UTILS_HPP

#include <Core/Utils/Macros.h>
#include <Vox/GLTypes.hpp>
#include <string>
#include <cassert>

#if defined(CUBBYFLOW_LINUX)
	extern const char *__progname;
#endif

namespace Vox {
    //! The function to print the call stack
    class StackTrace
    {
    public:
        #if (defined(CUBBYFLOW_WINDOWS) || defined(CUBBYFLOW_LINUX))
    	static void PrintStack();
        #else
        static void PrintStack() {};
        #endif
    };
    
    #define VoxAssert(expression, loc, msg) \
    {\
        if (!(expression))\
        {\
            fprintf(stderr, "[%s] %s\n", loc, msg);\
            StackTrace::PrintStack(); \
            std::abort();\
        }\
    }
};

#endif