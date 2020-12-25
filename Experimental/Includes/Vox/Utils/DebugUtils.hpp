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

#include <Core/Utils/Macros.hpp>
#include <Vox/Utils/GLTypes.hpp>
#include <iostream>
#include <string>
#include <cassert>

#if defined(CUBBYFLOW_LINUX)
    extern const char *__progname;
#endif

namespace Vox {
    //! The function collection for printing the call stack
    class StackTrace
    {
    public:
        #if (defined(CUBBYFLOW_WINDOWS) || defined(CUBBYFLOW_LINUX))
    	static void PrintStack();
        #else
        static void PrintStack() {};
        #endif
    };

    //! The function collection related to opengl.
    class GLDebug
    {
    public:
        //! Debug logging for opengl context with GL_ARB_debug_output extension.
        static void DebugLog(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const GLvoid* userParam);
    };
};

/**
 * brief simple assertion macro for vox application.
 * param expression 
 * param loc - 
 * param msg -
 */
#define VoxAssert(expression, loc, msg)\
{\
    if (!(expression))\
    {\
        std::cerr << "[" << loc << "] " << msg << std::endl;\
        Vox::StackTrace::PrintStack(); \
        std::abort();\
    }\
}

/**
 * brief simple assertion macro for vox application.
 * param expression 
 * param loc - 
 * param msg -
 * param callback - callback function when assertion failed.
 */
#define VoxAssertFailCallback(expression, loc, msg, callback)\
{\
    if (!(expression))\
    {\
        std::cerr << "[" << loc << "] " << msg << std::endl;\
        callback();\
        Vox::StackTrace::PrintStack(); \
        std::abort();\
    }\
}

#endif