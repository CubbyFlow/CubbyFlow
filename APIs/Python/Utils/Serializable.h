/*************************************************************************
> File Name: Serializable.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Serializable functions for CubbyFlow Python API.
> Created Time: 2018/02/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_SERIALIZABLE_H
#define CUBBYFLOW_PYTHON_SERIALIZABLE_H

#include <pybind11/pybind11.h>

void AddSerializable(pybind11::module& m);

#endif