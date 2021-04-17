// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Field/CustomVectorField.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Field/CustomVectorField.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddCustomVectorField2(pybind11::module& m)
{
    pybind11::class_<CustomVectorField2, CustomVectorField2Ptr, VectorField2>(
        static_cast<pybind11::handle>(m), "CustomVectorField2",
        R"pbdoc(
			2-D vector field with custom field function.
		)pbdoc")
        .def(
            "__init__",
            [](CustomVectorField2& instance, pybind11::function func,
               pybind11::function divFunc, pybind11::function curlFunc) {
                if (!divFunc)
                {
                    new (&instance) CustomVectorField2(
                        [func](const Vector2D& x) -> Vector2D {
                            return func(x).cast<Vector2D>();
                        });
                    return;
                }

                if (!curlFunc)
                {
                    new (&instance) CustomVectorField2(
                        [func](const Vector2D& x) -> Vector2D {
                            return func(x).cast<Vector2D>();
                        },
                        [divFunc](const Vector2D& x) -> double {
                            return divFunc(x).cast<double>();
                        });
                    return;
                }
                new (&instance) CustomVectorField2(
                    [func](const Vector2D& x) -> Vector2D {
                        return func(x).cast<Vector2D>();
                    },
                    [divFunc](const Vector2D& x) -> double {
                        return divFunc(x).cast<double>();
                    },
                    [curlFunc](const Vector2D& x) -> double {
                        return curlFunc(x).cast<double>();
                    });
            },
            R"pbdoc(
			Constructs a field with given field, gradient, and Laplacian function.
		)pbdoc",
            pybind11::arg("func"), pybind11::arg("divFunc") = nullptr,
            pybind11::arg("curlFunc") = nullptr)
        .def(
            "Sample",
            [](const CustomVectorField2& instance, pybind11::object obj) {
                return instance.Sample(ObjectToVector2D(obj));
            },
            R"pbdoc(
			Returns sampled value at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Divergence",
            [](const CustomVectorField2& instance, pybind11::object obj) {
                return instance.Divergence(ObjectToVector2D(obj));
            },
            R"pbdoc(
			Returns divergence at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Curl",
            [](const CustomVectorField2& instance, pybind11::object obj) {
                return instance.Curl(ObjectToVector2D(obj));
            },
            R"pbdoc(Returns curl at given position `x`.)pbdoc",
            pybind11::arg("x"))
        .def(
            "Sampler",
            [](const CustomVectorField2& instance) {
                return instance.Sampler();
            },
            R"pbdoc(
			Returns the sampler function.

			This function returns the data sampler function object. The sampling
			function is linear.
		)pbdoc");
}

void AddCustomVectorField3(pybind11::module& m)
{
    pybind11::class_<CustomVectorField3, CustomVectorField3Ptr, VectorField3>(
        static_cast<pybind11::handle>(m), "CustomVectorField3",
        R"pbdoc(
			3-D vector field with custom field function.
		)pbdoc")
        .def(
            "__init__",
            [](CustomVectorField3& instance, pybind11::function func,
               pybind11::function divFunc, pybind11::function curlFunc) {
                if (!divFunc)
                {
                    new (&instance) CustomVectorField3(
                        [func](const Vector3D& x) -> Vector3D {
                            return func(x).cast<Vector3D>();
                        });
                    return;
                }

                if (!curlFunc)
                {
                    new (&instance) CustomVectorField3(
                        [func](const Vector3D& x) -> Vector3D {
                            return func(x).cast<Vector3D>();
                        },
                        [divFunc](const Vector3D& x) -> double {
                            return divFunc(x).cast<double>();
                        });
                    return;
                }
                new (&instance) CustomVectorField3(
                    [func](const Vector3D& x) -> Vector3D {
                        return func(x).cast<Vector3D>();
                    },
                    [divFunc](const Vector3D& x) -> double {
                        return divFunc(x).cast<double>();
                    },
                    [curlFunc](const Vector3D& x) -> Vector3D {
                        return curlFunc(x).cast<Vector3D>();
                    });
            },
            R"pbdoc(
			Constructs a field with given field, gradient, and Laplacian function.
		)pbdoc",
            pybind11::arg("func"), pybind11::arg("divFunc") = nullptr,
            pybind11::arg("curlFunc") = nullptr)
        .def(
            "Sample",
            [](const CustomVectorField3& instance, pybind11::object obj) {
                return instance.Sample(ObjectToVector3D(obj));
            },
            R"pbdoc(
			Returns sampled value at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Divergence",
            [](const CustomVectorField3& instance, pybind11::object obj) {
                return instance.Divergence(ObjectToVector3D(obj));
            },
            R"pbdoc(
			Returns divergence at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Curl",
            [](const CustomVectorField3& instance, pybind11::object obj) {
                return instance.Curl(ObjectToVector3D(obj));
            },
            R"pbdoc(
			Returns curl at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Sampler",
            [](const CustomVectorField3& instance) {
                return instance.Sampler();
            },
            R"pbdoc(
			Returns the sampler function.

			This function returns the data sampler function object. The sampling
			function is linear.
		)pbdoc");
}