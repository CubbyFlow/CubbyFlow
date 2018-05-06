/*************************************************************************
> File Name: Logging.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Logging functions for CubbyFlow Python API.
> Created Time: 2018/01/23
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_LOGGER_H
#define CUBBYFLOW_PYTHON_LOGGER_H

#include <pybind11/pybind11.h>

void AddLogging(pybind11::module& m);

#endif