/*************************************************************************
> File Name: ImplicitSurface.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: ImplicitSurface functions for CubbyFlow Python API.
> Created Time: 2018/02/07
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Surface/ImplicitSurface.h>
#include <API/Python/Utils/pybind11Utils.h>
#include <Core/Surface/ImplicitSurface2.h>
#include <Core/Surface/ImplicitSurface3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddImplicitSurface2(pybind11::module& m)
{
	pybind11::class_<ImplicitSurface2, ImplicitSurface2Ptr, Surface2>(static_cast<pybind11::handle>(m), "ImplicitSurface2",
		R"pbdoc(
			Abstract base class for 2-D implicit surface.
		)pbdoc")
	.def("SignedDistance", [](const ImplicitSurface2& instance, pybind11::object otherPoint)
	{
		return instance.SignedDistance(ObjectToVector2D(otherPoint));
	},
		R"pbdoc(
			Returns signed distance from the given point otherPoint.
		)pbdoc",
		pybind11::arg("otherPoint"));
}

void AddImplicitSurface3(pybind11::module& m)
{
	pybind11::class_<ImplicitSurface3, ImplicitSurface3Ptr, Surface3>(static_cast<pybind11::handle>(m), "ImplicitSurface3",
		R"pbdoc(
			Abstract base class for 3-D implicit surface.
		)pbdoc")
	.def("SignedDistance", [](const ImplicitSurface3& instance, pybind11::object otherPoint)
	{
		return instance.SignedDistance(ObjectToVector3D(otherPoint));
	},
		R"pbdoc(
			Returns signed distance from the given point otherPoint.
		)pbdoc",
		pybind11::arg("otherPoint"));
}