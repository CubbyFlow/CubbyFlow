// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Point/Point.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Point/Point2.hpp>
#include <Core/Point/Point3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddPoint2UI(pybind11::module& m)
{
    pybind11::class_<Point2UI>(static_cast<pybind11::handle>(m), "Point2UI")
        .def(
            "__init__",
            [](Point2UI& instance, size_t x, size_t y) {
                new (&instance) Point2UI(x, y);
            },
            R"pbdoc(
			Constructs Point2UI

			This method constructs 2D point with x and y.
		)pbdoc",
            pybind11::arg("x") = 0, pybind11::arg("y") = 0)
        .def_readwrite("x", &Point2UI::x)
        .def_readwrite("y", &Point2UI::y)
        .def("__eq__", [](const Point2UI& instance, pybind11::object obj) {
            Point2UI other = ObjectToPoint2UI(obj);
            return instance == other;
        });
}

void AddPoint3UI(pybind11::module& m)
{
    pybind11::class_<Point3UI>(static_cast<pybind11::handle>(m), "Point3UI")
        .def(
            "__init__",
            [](Point3UI& instance, size_t x, size_t y, size_t z) {
                new (&instance) Point3UI(x, y, z);
            },
            R"pbdoc(
			Constructs Point3UI

			This method constructs 3D point with x, y, and z.
		)pbdoc",
            pybind11::arg("x") = 0, pybind11::arg("y") = 0,
            pybind11::arg("z") = 0)
        .def_readwrite("x", &Point3UI::x)
        .def_readwrite("y", &Point3UI::y)
        .def_readwrite("z", &Point3UI::z)
        .def("__eq__", [](const Point3UI& instance, pybind11::object obj) {
            Point3UI other = ObjectToPoint3UI(obj);
            return instance == other;
        });
}