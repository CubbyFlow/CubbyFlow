/*************************************************************************
> File Name: Macros.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Macro lists for CubbyFlow.
> Created Time: 2017/01/31
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_MACROS_H
#define CUBBYFLOW_MACROS_H

#if defined(_WIN32) || defined(_WIN64)
#   define CUBBYFLOW_WINDOWS
#elif defined(__APPLE__)
#   define CUBBYFLOW_APPLE
#   ifndef CUBBYFLOW_IOS
#       define CUBBYFLOW_MACOSX
#   endif
#elif defined(linux) || defined(__linux__)
#   define CUBBYFLOW_LINUX
#endif

#ifdef CUBBYFLOW_WINDOWS
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
#include <sys/types.h>
#endif

#ifndef UNUSED_VARIABLE
#	define UNUSED_VARIABLE(x) ((void)x)
#endif

#endif