// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Field/ConstantScalarField.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Field/ConstantScalarField2.hpp>
#include <Core/Field/ConstantScalarField3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddConstantScalarField2(pybind11::module& m)
{
    pybind11::class_<ConstantScalarField2, ConstantScalarField2Ptr,
                     ScalarField2>(static_cast<pybind11::handle>(m),
                                   "ConstantScalarField2",
                                   R"pbdoc(
			2-D constant scalar field.
		)pbdoc")
        .def(pybind11::init<double>(),
             R"pbdoc(
			Constructs a constant scalar field with given `value`.
		)pbdoc",
             pybind11::arg("value"))
        .def(
            "Sample",
            [](const ConstantScalarField2& instance, pybind11::object obj) {
                return instance.Sample(ObjectToVector2D(obj));
            },
            R"pbdoc(
			Returns sampled value at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Sampler",
            [](const ConstantScalarField2& instance) {
                return instance.Sampler();
            },
            R"pbdoc(
			Returns the sampler function.

			This function returns the data sampler function object. The sampling
			function is linear.
		)pbdoc");
}

void AddConstantScalarField3(pybind11::module& m)
{
    pybind11::class_<ConstantScalarField3, ConstantScalarField3Ptr,
                     ScalarField3>(static_cast<pybind11::handle>(m),
                                   "ConstantScalarField3",
                                   R"pbdoc(
			3-D constant scalar field.
		)pbdoc")
        .def(pybind11::init<double>(),
             R"pbdoc(
			Constructs a constant scalar field with given `value`.
		)pbdoc",
             pybind11::arg("value"))
        .def(
            "Sample",
            [](const ConstantScalarField3& instance, pybind11::object obj) {
                return instance.Sample(ObjectToVector3D(obj));
            },
            R"pbdoc(
			Returns sampled value at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Sampler",
            [](const ConstantScalarField3& instance) {
                return instance.Sampler();
            },
            R"pbdoc(
			Returns the sampler function.

			This function returns the data sampler function object. The sampling
			function is linear.
		)pbdoc");
}