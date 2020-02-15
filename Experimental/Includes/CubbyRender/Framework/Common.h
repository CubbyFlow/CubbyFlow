/*************************************************************************
> File Name: Common.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for Renderer object.
> Created Time: 2020/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

//! Jet public headers
#include <Core/Utils/Constants.h>
#include <Core/Utils/Macros.h>
#include <Core/Utils/Logging.h>

//! OpenGL Stuffs
#ifdef CUBBYRENDER_USE_GL
#include <GL/gl3w.h>
#endif