// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Geometry/ImplicitSurface.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Geometry/ImplicitSurface2.hpp>
#include <Core/Geometry/ImplicitSurface3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddImplicitSurface2(pybind11::module& m)
{
    pybind11::class_<ImplicitSurface2, ImplicitSurface2Ptr, Surface2>(
        static_cast<pybind11::handle>(m), "ImplicitSurface2",
        R"pbdoc(
			Abstract base class for 2-D implicit surface.
		)pbdoc")
        .def(
            "SignedDistance",
            [](const ImplicitSurface2& instance, pybind11::object otherPoint) {
                return instance.SignedDistance(ObjectToVector2D(otherPoint));
            },
            R"pbdoc(
			Returns signed distance from the given point otherPoint.
		)pbdoc",
            pybind11::arg("otherPoint"));
}

void AddImplicitSurface3(pybind11::module& m)
{
    pybind11::class_<ImplicitSurface3, ImplicitSurface3Ptr, Surface3>(
        static_cast<pybind11::handle>(m), "ImplicitSurface3",
        R"pbdoc(
			Abstract base class for 3-D implicit surface.
		)pbdoc")
        .def(
            "SignedDistance",
            [](const ImplicitSurface3& instance, pybind11::object otherPoint) {
                return instance.SignedDistance(ObjectToVector3D(otherPoint));
            },
            R"pbdoc(
			Returns signed distance from the given point otherPoint.
		)pbdoc",
            pybind11::arg("otherPoint"));
}