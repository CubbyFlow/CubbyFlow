/*************************************************************************
> File Name: ConstantVectorField.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: ConstantVectorField functions for CubbyFlow Python API.
> Created Time: 2018/02/04
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Field/ConstantVectorField.h>
#include <API/Python/Utils/pybind11Utils.h>
#include <Core/Field/ConstantVectorField2.h>
#include <Core/Field/ConstantVectorField3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddConstantVectorField2(pybind11::module& m)
{
	pybind11::class_<ConstantVectorField2, ConstantVectorField2Ptr, VectorField2>(static_cast<pybind11::handle>(m), "ConstantVectorField2",
		R"pbdoc(
			2-D constant vector field.
		)pbdoc")
	.def(pybind11::init<Vector2D>(),
		R"pbdoc(
			Constructs a constant vector field with given `value`.
		)pbdoc",
		pybind11::arg("value"))
	.def("Sample", [](const ConstantVectorField2& instance, pybind11::object obj)
	{
		return instance.Sample(ObjectToVector2D(obj));
	},
		R"pbdoc(
			Returns sampled value at given position `x`.
		)pbdoc",
		pybind11::arg("x"))
	.def("Sampler", [](const ConstantVectorField2& instance)
	{
		return instance.Sampler();
	},
		R"pbdoc(
			Returns the sampler function.

			This function returns the data sampler function object. The sampling
			function is linear.
		)pbdoc");
}

void AddConstantVectorField3(pybind11::module& m)
{
	pybind11::class_<ConstantVectorField3, ConstantVectorField3Ptr, VectorField3>(static_cast<pybind11::handle>(m), "ConstantVectorField3",
		R"pbdoc(
			3-D constant vector field.
		)pbdoc")
	.def(pybind11::init<Vector3D>(),
		R"pbdoc(
			Constructs a constant vector field with given `value`.
		)pbdoc",
		pybind11::arg("value"))
	.def("Sample", [](const ConstantVectorField3& instance, pybind11::object obj)
	{
		return instance.Sample(ObjectToVector3D(obj));
	},
		R"pbdoc(
			Returns sampled value at given position `x`.
		)pbdoc",
		pybind11::arg("x"))
	.def("Sampler", [](const ConstantVectorField3& instance)
	{
		return instance.Sampler();
	},
		R"pbdoc(
			Returns the sampler function.

			This function returns the data sampler function object. The sampling
			function is linear.
		)pbdoc");
}