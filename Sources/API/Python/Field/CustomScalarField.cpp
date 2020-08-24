// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Field/CustomScalarField.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Field/CustomScalarField2.hpp>
#include <Core/Field/CustomScalarField3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddCustomScalarField2(pybind11::module& m)
{
    pybind11::class_<CustomScalarField2, CustomScalarField2Ptr, ScalarField2>(
        static_cast<pybind11::handle>(m), "CustomScalarField2",
        R"pbdoc(
			2-D scalar field with custom field function.
		)pbdoc")
        .def(
            "__init__",
            [](CustomScalarField2& instance, pybind11::function func,
               pybind11::function gradFunc, pybind11::function lapFunc) {
                if (!gradFunc)
                {
                    new (&instance)
                        CustomScalarField2([func](const Vector2D& x) -> double {
                            return func(x).cast<double>();
                        });
                    return;
                }

                if (!lapFunc)
                {
                    new (&instance) CustomScalarField2(
                        [func](const Vector2D& x) -> double {
                            return func(x).cast<double>();
                        },
                        [gradFunc](const Vector2D& x) -> Vector2D {
                            return gradFunc(x).cast<Vector2D>();
                        });
                    return;
                }

                new (&instance) CustomScalarField2(
                    [func](const Vector2D& x) -> double {
                        return func(x).cast<double>();
                    },
                    [gradFunc](const Vector2D& x) -> Vector2D {
                        return gradFunc(x).cast<Vector2D>();
                    },
                    [lapFunc](const Vector2D& x) -> double {
                        return lapFunc(x).cast<double>();
                    });
            },
            R"pbdoc(
			Constructs a field with given field, gradient, and Laplacian function.
		)pbdoc",
            pybind11::arg("func"), pybind11::arg("gradFunc") = nullptr,
            pybind11::arg("lapFunc") = nullptr)
        .def(
            "Sample",
            [](const CustomScalarField2& instance, pybind11::object obj) {
                return instance.Sample(ObjectToVector2D(obj));
            },
            R"pbdoc(
			Returns sampled value at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Gradient",
            [](const CustomScalarField2& instance, pybind11::object obj) {
                return instance.Gradient(ObjectToVector2D(obj));
            },
            R"pbdoc(
			Returns gradient at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Laplacian",
            [](const CustomScalarField2& instance, pybind11::object obj) {
                return instance.Laplacian(ObjectToVector2D(obj));
            },
            R"pbdoc(
			Returns Laplacian at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Sampler",
            [](const CustomScalarField2& instance) {
                return instance.Sampler();
            },
            R"pbdoc(
			Returns the sampler function.

			This function returns the data sampler function object. The sampling
			function is linear.
		)pbdoc");
}

void AddCustomScalarField3(pybind11::module& m)
{
    pybind11::class_<CustomScalarField3, CustomScalarField3Ptr, ScalarField3>(
        static_cast<pybind11::handle>(m), "CustomScalarField3",
        R"pbdoc(
			3-D scalar field with custom field function.
		)pbdoc")
        .def(
            "__init__",
            [](CustomScalarField3& instance, pybind11::function func,
               pybind11::function gradFunc, pybind11::function lapFunc) {
                if (!gradFunc)
                {
                    new (&instance)
                        CustomScalarField3([func](const Vector3D& x) -> double {
                            return func(x).cast<double>();
                        });
                    return;
                }

                if (!lapFunc)
                {
                    new (&instance) CustomScalarField3(
                        [func](const Vector3D& x) -> double {
                            return func(x).cast<double>();
                        },
                        [gradFunc](const Vector3D& x) -> Vector3D {
                            return gradFunc(x).cast<Vector3D>();
                        });
                    return;
                }

                new (&instance) CustomScalarField3(
                    [func](const Vector3D& x) -> double {
                        return func(x).cast<double>();
                    },
                    [gradFunc](const Vector3D& x) -> Vector3D {
                        return gradFunc(x).cast<Vector3D>();
                    },
                    [lapFunc](const Vector3D& x) -> double {
                        return lapFunc(x).cast<double>();
                    });
            },
            R"pbdoc(
			Constructs a field with given field, gradient, and Laplacian function.
		)pbdoc",
            pybind11::arg("func"), pybind11::arg("gradFunc") = nullptr,
            pybind11::arg("lapFunc") = nullptr)
        .def(
            "Sample",
            [](const CustomScalarField3& instance, pybind11::object obj) {
                return instance.Sample(ObjectToVector3D(obj));
            },
            R"pbdoc(
			Returns sampled value at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Gradient",
            [](const CustomScalarField3& instance, pybind11::object obj) {
                return instance.Gradient(ObjectToVector3D(obj));
            },
            R"pbdoc(
			Returns gradient at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Laplacian",
            [](const CustomScalarField3& instance, pybind11::object obj) {
                return instance.Laplacian(ObjectToVector3D(obj));
            },
            R"pbdoc(
			Returns Laplacian at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Sampler",
            [](const CustomScalarField3& instance) {
                return instance.Sampler();
            },
            R"pbdoc(
			Returns the sampler function.

			This function returns the data sampler function object. The sampling
			function is linear.
		)pbdoc");
}