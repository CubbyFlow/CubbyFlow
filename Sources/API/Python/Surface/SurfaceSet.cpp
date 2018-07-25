/*************************************************************************
> File Name: SurfaceSet.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: SurfaceSet functions for CubbyFlow Python API.
> Created Time: 2018/01/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Surface/SurfaceSet.h>
#include <Core/Surface/SurfaceSet2.h>
#include <Core/Surface/SurfaceSet3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddSurfaceSet2(pybind11::module& m)
{
	pybind11::class_<SurfaceSet2, SurfaceSet2Ptr, Surface2>(m, "SurfaceSet2",
		R"pbdoc(
			2-D surface set.

			This class represents 2-D surface set which extends Surface2 by overriding
			surface-related queries. This is class can hold a collection of other
			surface instances.
		)pbdoc")
	.def("__init__", [](SurfaceSet2& instance, pybind11::list others, const Transform2& transform, bool isNormalFlipped)
	{
		std::vector<Surface2Ptr> others_;

		for (size_t i = 0; i < others.size(); ++i)
		{
			others_.push_back(others[i].cast<Surface2Ptr>());
		}

		new (&instance) SurfaceSet2(others_, transform, isNormalFlipped);
	},
		R"pbdoc(
			Constructs with a list of other surfaces.
		)pbdoc",
		pybind11::arg("others") = pybind11::list(),
		pybind11::arg("transform") = Transform2(),
		pybind11::arg("isNormalFlipped") = false)
	.def("NumberOfSurfaces", &SurfaceSet2::NumberOfSurfaces,
		R"pbdoc(
			Returns the number of surfaces.
		)pbdoc")
	.def("SurfaceAt", &SurfaceSet2::SurfaceAt,
		R"pbdoc(
			Returns the `i`-th surface.
		)pbdoc",
		pybind11::arg("i"))
	.def("AddSurface", &SurfaceSet2::AddSurface,
		R"pbdoc(
			Adds a surface instance.
		)pbdoc",
		pybind11::arg("surface"));
}

void AddSurfaceSet3(pybind11::module& m)
{
	pybind11::class_<SurfaceSet3, SurfaceSet3Ptr, Surface3>(m, "SurfaceSet3",
		R"pbdoc(
			3-D surface set.

			This class represents 3-D surface set which extends Surface3 by overriding
			surface-related queries. This is class can hold a collection of other
			surface instances.
		)pbdoc")
	.def("__init__", [](SurfaceSet3& instance, pybind11::list others, const Transform3& transform, bool isNormalFlipped)
	{
		std::vector<Surface3Ptr> others_;

		for (size_t i = 0; i < others.size(); ++i)
		{
			others_.push_back(others[i].cast<Surface3Ptr>());
		}

		new (&instance) SurfaceSet3(others_, transform, isNormalFlipped);
	},
		R"pbdoc(
			Constructs with a list of other surfaces.
		)pbdoc",
		pybind11::arg("others") = pybind11::list(),
		pybind11::arg("transform") = Transform3(),
		pybind11::arg("isNormalFlipped") = false)
	.def("NumberOfSurfaces", &SurfaceSet3::NumberOfSurfaces,
		R"pbdoc(
			Returns the number of surfaces.
		)pbdoc")
	.def("SurfaceAt", &SurfaceSet3::SurfaceAt,
		R"pbdoc(
			Returns the `i`-th surface.
		)pbdoc",
		pybind11::arg("i"))
	.def("AddSurface", &SurfaceSet3::AddSurface,
		R"pbdoc(
			Adds a surface instance.
		)pbdoc",
		pybind11::arg("surface"));
}