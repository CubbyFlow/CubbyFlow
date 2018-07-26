/*************************************************************************
> File Name: CustomScalarField.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: CustomScalarField functions for CubbyFlow Python API.
> Created Time: 2018/02/04
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_PYTHON_CUSTOM_SCALAR_FIELD_H
#define CUBBYFLOW_PYTHON_CUSTOM_SCALAR_FIELD_H

#include <pybind11/pybind11.h>

void AddCustomScalarField2(pybind11::module& m);
void AddCustomScalarField3(pybind11::module& m);

#endif