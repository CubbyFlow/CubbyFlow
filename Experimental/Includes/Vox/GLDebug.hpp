/*************************************************************************
> File Name: GLDebug.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> References: OpenGL-Insights Sample chapter
> Purpose: Debug utility functions for GL_ARB_debug_output extension.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_GL_DEBUG_HPP
#define CUBBYFLOW_VOX_GL_DEBUG_HPP

#ifdef _WIN32
#include <windows.h>
#include <DbgHelp.h>
#pragma comment(lib,"Dbghelp")

#define STDCALL __stdcall
#else
#define STDCALL
#endif

#ifdef _LINUX
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <stdio.h>
#include <string.h>
#include <execinfo.h>
#include <stdlib.h>
#include <cxxabi.h>

extern const char *__progname;

#endif

#include<Vox/GLTypes.hpp>
#include<string>

namespace Vox {
    namespace Detail {
        /// Helper functions to print human-readable message
    	std::string GetStringForSource(GLenum source);
    	std::string GetStringForType(GLenum type);
    	std::string GetStringForSeverity(GLenum type);

    	/// The function to print the call stack
    #if (defined _WIN32 || defined _LINUX)
    	void PrintStack();
    #endif
    #ifdef _LINUX
    	int GetFileAndLine (unw_word_t addr, char *file, size_t flen, int *line);
    #endif
    }
};

#endif