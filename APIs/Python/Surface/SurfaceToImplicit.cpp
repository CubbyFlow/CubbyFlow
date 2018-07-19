/*************************************************************************
> File Name: SurfaceToImplicit.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: SurfaceToImplicit functions for CubbyFlow Python API.
> Created Time: 2018/01/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Python/Surface/SurfaceToImplicit.h>
#include <Surface/SurfaceToImplicit2.h>
#include <Surface/SurfaceToImplicit3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddSurfaceToImplicit2(pybind11::module& m)
{
	pybind11::class_<SurfaceToImplicit2, SurfaceToImplicit2Ptr, ImplicitSurface2>(m, "SurfaceToImplicit2",
		R"pbdoc(
			2-D implicit surface wrapper for generic Surface2 instance.

			This class represents 2-D implicit surface that converts Surface2 instance
			to an ImplicitSurface2 object. The conversion is made by evaluating closest
			point and normal from a given point for the given (explicit) surface. Thus,
			this conversion won't work for every single surfaces, especially
			TriangleMesh2. To use TriangleMesh2 as an ImplicitSurface2 instance,
			please take a look at ImplicitTriangleMesh2. Use this class only
			for the basic primitives such as Sphere2 or Box2.
		)pbdoc")
	.def(pybind11::init<Surface2Ptr, Transform2, bool>(),
		R"pbdoc(
			Constructs an instance with generic Surface2 instance.
		)pbdoc",
		pybind11::arg("surface"),
		pybind11::arg("transform") = Transform2(),
		pybind11::arg("isNormalFlipped") = false)
	.def_property_readonly("surface", &SurfaceToImplicit2::GetSurface,
		R"pbdoc(
			The raw surface instance.
		)pbdoc");
}

void AddSurfaceToImplicit3(pybind11::module& m)
{
	pybind11::class_<SurfaceToImplicit3, SurfaceToImplicit3Ptr, ImplicitSurface3>(m, "SurfaceToImplicit3",
		R"pbdoc(
			3-D implicit surface wrapper for generic Surface3 instance.

			This class represents 3-D implicit surface that converts Surface3 instance
			to an ImplicitSurface3 object. The conversion is made by evaluating closest
			point and normal from a given point for the given (explicit) surface. Thus,
			this conversion won't work for every single surfaces, especially
			TriangleMesh3. To use TriangleMesh3 as an ImplicitSurface3 instance,
			please take a look at ImplicitTriangleMesh3. Use this class only
			for the basic primitives such as Sphere3 or Box3.
		)pbdoc")
	.def(pybind11::init<Surface3Ptr, Transform3, bool>(),
		R"pbdoc(
			Constructs an instance with generic Surface3 instance.
		)pbdoc",
		pybind11::arg("surface"),
		pybind11::arg("transform") = Transform3(),
		pybind11::arg("isNormalFlipped") = false)
	.def_property_readonly("surface", &SurfaceToImplicit3::GetSurface,
		R"pbdoc(
			The raw surface instance.
		)pbdoc");
}