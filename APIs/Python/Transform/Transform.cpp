/*************************************************************************
> File Name: Transform.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Transform functions for CubbyFlow Python API.
> Created Time: 2018/01/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Python/Transform/Transform.h>
#include <Python/Utils/pybind11Utils.h>
#include <Transform/Transform2.h>
#include <Transform/Transform3.h>
#include <Math/Quaternion.h>
#include <Vector/Vector2.h>
#include <Vector/Vector3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddTransform2(pybind11::module& m)
{
	pybind11::class_<Transform2>(m, "Transform2")
	.def("__init__", [](Transform2& instance, pybind11::object translation, double orientation)
	{
		Vector2D translation_ = ObjectToVector2D(translation);
		
		new (&instance) Transform2(translation_, orientation);
	},
		R"pbdoc(
			Constructs Transform2

			This method constructs 2D transform with translation and
			orientation.
		)pbdoc",
		pybind11::arg("translation") = Vector2D(0, 0),
		pybind11::arg("orientation") = 0.0);
}

void AddTransform3(pybind11::module& m)
{
	pybind11::class_<Transform3>(m, "Transform3")
	.def("__init__", [](Transform3& instance, pybind11::object translation, pybind11::object orientation)
	{
		Vector3D translation_ = ObjectToVector3D(translation);
		QuaternionD orientation_ = ObjectToQuaternionD(orientation);
		
		new (&instance) Transform3(translation_, orientation_);
	},
		R"pbdoc(
			Constructs Transform3

			This method constructs 3D transform with translation and
			orientation.
		)pbdoc",
		pybind11::arg("translation") = Vector3D(0, 0, 0),
		pybind11::arg("orientation") = QuaternionD());
}