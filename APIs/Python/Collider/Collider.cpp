/*************************************************************************
> File Name: Collider.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Collider functions for CubbyFlow Python API.
> Created Time: 2018/02/10
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Python/Collider/Collider.h>
#include <Collider/Collider2.h>
#include <Collider/Collider3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddCollider2(pybind11::module& m)
{
	pybind11::class_<Collider2, Collider2Ptr>(m, "Collider2",
		R"pbdoc(
			Abstract base class for generic 2-D collider object.

			This class contains basic interfaces for colliders. Most of the
			functionalities are implemented within this class, except the member
			function Collider2::VelocityAt. This class also let the subclasses to
			provide a Surface2 instance to define collider surface using
			Collider2::SetSurface function.
		)pbdoc")
	.def_property("frictionCoefficient", &Collider2::GetFrictionCoefficient, &Collider2::SetFrictionCoefficient,
		R"pbdoc(
			The friction coefficient.

			This property specifies the friction coefficient to the collider. Any
			negative inputs will be clamped to zero.
		)pbdoc")
	.def_property_readonly("surface", &Collider2::GetSurface,
		R"pbdoc(
			The surface instance.
		)pbdoc");
}

void AddCollider3(pybind11::module& m)
{
	pybind11::class_<Collider3, Collider3Ptr>(m, "Collider3",
		R"pbdoc(
			Abstract base class for generic 3-D collider object.

			This class contains basic interfaces for colliders. Most of the
			functionalities are implemented within this class, except the member
			function Collider3::VelocityAt. This class also let the subclasses to
			provide a Surface3 instance to define collider surface using
			Collider3::SetSurface function.
		)pbdoc")
	.def_property("frictionCoefficient", &Collider3::GetFrictionCoefficient, &Collider3::SetFrictionCoefficient,
		R"pbdoc(
			The friction coefficient.

			This property specifies the friction coefficient to the collider. Any
			negative inputs will be clamped to zero.
		)pbdoc")
	.def_property_readonly("surface", &Collider3::GetSurface,
		R"pbdoc(
			The surface instance.
		)pbdoc");
}