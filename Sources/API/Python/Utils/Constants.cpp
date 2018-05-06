/*************************************************************************
> File Name: Constants.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Constants functions for CubbyFlow Python API.
> Created Time: 2018/02/01
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Utils/Constants.h>
#include <Core/Utils/Constants.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddConstants(pybind11::module& m)
{
	m.attr("DIRECTION_NONE") = pybind11::int_(DIRECTION_NONE);
	m.attr("DIRECTION_LEFT") = pybind11::int_(DIRECTION_LEFT);
	m.attr("DIRECTION_RIGHT") = pybind11::int_(DIRECTION_RIGHT);
	m.attr("DIRECTION_DOWN") = pybind11::int_(DIRECTION_DOWN);
	m.attr("DIRECTION_UP") = pybind11::int_(DIRECTION_UP);
	m.attr("DIRECTION_BACK") = pybind11::int_(DIRECTION_BACK);
	m.attr("DIRECTION_FRONT") = pybind11::int_(DIRECTION_FRONT);
	m.attr("DIRECTION_ALL") = pybind11::int_(DIRECTION_ALL);
}