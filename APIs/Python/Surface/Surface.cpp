/*************************************************************************
> File Name: Surface.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Surface functions for CubbyFlow Python API.
> Created Time: 2018/01/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Python/Surface/Surface.h>
#include <Surface/Surface2.h>
#include <Surface/Surface3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddSurface2(pybind11::module& m)
{
	pybind11::class_<Surface2, Surface2Ptr>(m, "Surface2")
	.def_readwrite("transform", &Surface2::transform)
	.def_readwrite("isNormalFlipped", &Surface2::isNormalFlipped);
}

void AddSurface3(pybind11::module& m)
{
	pybind11::class_<Surface3, Surface3Ptr>(m, "Surface3")
	.def_readwrite("transform", &Surface3::transform)
	.def_readwrite("isNormalFlipped", &Surface3::isNormalFlipped);
}