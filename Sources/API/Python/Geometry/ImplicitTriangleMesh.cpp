// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Geometry/ImplicitTriangleMesh.hpp>
#include <Core/Geometry/ImplicitTriangleMesh3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddImplicitTriangleMesh3(pybind11::module& m)
{
    pybind11::class_<ImplicitTriangleMesh3, ImplicitTriangleMesh3Ptr,
                     ImplicitSurface3>(m, "ImplicitTriangleMesh3",
                                       R"pbdoc(
         TriangleMesh3 to ImplicitSurface3 converter.

         This class builds signed-distance field for given TriangleMesh3 instance so
         that it can be used as an ImplicitSurface3 instance. The mesh is discretize
         into a regular grid and the signed-distance is measured at each grid point.
         Thus, there is a sampling error and its magnitude depends on the grid
         resolution.
         )pbdoc")
        // CTOR
        .def(pybind11::init<TriangleMesh3Ptr, size_t, double, Transform3,
                            bool>(),
             R"pbdoc(
             Constructs an ImplicitSurface3 with mesh and other grid parameters.
             )pbdoc",
             pybind11::arg("mesh"), pybind11::arg("resolutionX") = 32,
             pybind11::arg("margin") = 0.2,
             pybind11::arg("transform") = Transform3(),
             pybind11::arg("isNormalFlipped") = false)
        .def_property_readonly("grid", &ImplicitTriangleMesh3::GetGrid,
                               R"pbdoc(The grid data.)pbdoc");
}