// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Geometry/Plane.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Geometry/Plane2.hpp>
#include <Core/Geometry/Plane3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddPlane2(pybind11::module& m)
{
    pybind11::class_<Plane2, Plane2Ptr, Surface2>(
        static_cast<pybind11::handle>(m), "Plane2",
        R"pbdoc(
			2-D plane geometry.

			This class represents 2-D plane geometry which extends Surface2 by
			overriding surface-related queries.
		)pbdoc")
        .def(
            "__init__",
            [](Plane2& instance, pybind11::object normal,
               pybind11::object point, const Transform2& transform,
               bool isNormalFlipped) {
                Vector2D normal_ = ObjectToVector2D(normal);
                Vector2D point_ = ObjectToVector2D(point);

                new (&instance)
                    Plane2(normal_, point_, transform, isNormalFlipped);
            },
            R"pbdoc(
				Constructs a plane that cross `point` with surface `normal`.
			)pbdoc",
            pybind11::arg("normal"), pybind11::arg("point"),
            pybind11::arg("transform") = Transform2(),
            pybind11::arg("isNormalFlipped") = false)
        .def_readwrite("normal", &Plane2::normal)
        .def_readwrite("point", &Plane2::point);
}

void AddPlane3(pybind11::module& m)
{
    pybind11::class_<Plane3, Plane3Ptr, Surface3>(
        static_cast<pybind11::handle>(m), "Plane3",
        R"pbdoc(
			3-D plane geometry.

			This class represents 3-D plane geometry which extends Surface3 by
			overriding surface-related queries.
		)pbdoc")
        .def(
            "__init__",
            [](Plane3& instance, pybind11::object normal,
               pybind11::object point, const Transform3& transform,
               bool isNormalFlipped) {
                Vector3D normal_ = ObjectToVector3D(normal);
                Vector3D point_ = ObjectToVector3D(point);

                new (&instance)
                    Plane3(normal_, point_, transform, isNormalFlipped);
            },
            R"pbdoc(
			Constructs a plane that cross `point` with surface `normal`.
		)pbdoc",
            pybind11::arg("normal"), pybind11::arg("point"),
            pybind11::arg("transform") = Transform3(),
            pybind11::arg("isNormalFlipped") = false)
        .def_readwrite("normal", &Plane3::normal)
        .def_readwrite("point", &Plane3::point);
}