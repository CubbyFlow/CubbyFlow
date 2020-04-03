// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MACROS_HPP
#define CUBBYFLOW_MACROS_HPP

#if defined(_WIN32) || defined(_WIN64)
#define CUBBYFLOW_WINDOWS
#elif defined(__APPLE__)
#define CUBBYFLOW_APPLE
#ifndef CUBBYFLOW_IOS
#define CUBBYFLOW_MACOSX
#endif
#elif defined(linux) || defined(__linux__)
#define CUBBYFLOW_LINUX
#endif

#ifdef CUBBYFLOW_WINDOWS
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
#include <sys/types.h>
#endif

#ifndef UNUSED_VARIABLE
#define UNUSED_VARIABLE(x) ((void)x)
#endif

#endif