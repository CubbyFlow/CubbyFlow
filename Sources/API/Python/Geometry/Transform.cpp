// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Geometry/Transform.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Geometry/Transform2.hpp>
#include <Core/Geometry/Transform3.hpp>
#include <Core/Math/Quaternion.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddTransform2(pybind11::module& m)
{
    pybind11::class_<Transform2>(m, "Transform2")
        .def(
            "__init__",
            [](Transform2& instance, pybind11::object translation,
               double orientation) {
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
        .def(
            "__init__",
            [](Transform3& instance, pybind11::object translation,
               pybind11::object orientation) {
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