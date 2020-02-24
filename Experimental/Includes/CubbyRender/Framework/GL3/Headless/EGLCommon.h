/*************************************************************************
> File Name: EGLCommon.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: include frequently used EGL types and declare macro
> Created Time: 2020/02/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifndef CUBBYFLOW_EGLCOMMON_H
#define CUBBYFLOW_EGLCOMMON_H

#ifdef CUBBYFLOW_USE_GL

#include <Core/Utils/Logging.h>
#include <cassert>
#include <GL/gl3w.h>
#include <EGL/egl.h>

#define EGL_ASSERT(result, common_msg) \
{\
    if ((result) == EGL_TRUE)\
    {\
        CUBBYFLOW_INFO << (common_msg) << " Success";\
    }\
    else\
    {\
        CUBBYFLOW_ERROR << (common_msg) << " Failed";\
        assert(false);\
    }\
}

#define CUBBYFLOW_GL_MAJOR_VERSION 4
#define CUBBYFLOW_GL_MINOR_VERSION 3


using GLchar = char;
using GLenum = unsigned int;
using GLboolean = unsigned char;
using GLbitfield = unsigned int;
using GLbyte = signed char;
using GLshort = short;
using GLint = int;
using GLsizei = int;
using GLubyte = unsigned char;
using GLushort = unsigned short;
using GLuint = unsigned int;
using GLhalf = unsigned short;
using GLfloat = float;
using GLclampf = float;
using GLdouble = double;
using GLclampd = double;
using GLvoid = void;

#endif 

#endif 
