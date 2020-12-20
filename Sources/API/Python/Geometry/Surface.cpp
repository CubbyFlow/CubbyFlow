// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Geometry/Surface.hpp>
#include <Core/Geometry/Surface2.hpp>
#include <Core/Geometry/Surface3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddSurface2(pybind11::module& m)
{
    pybind11::class_<Surface2, Surface2Ptr>(m, "Surface2")
        .def_readwrite("transform", &Surface2::transform)
        .def_readwrite("isNormalFlipped", &Surface2::isNormalFlipped);
}

void AddSurface3(pybind11::module& m)
{
    pybind11::class_<Surface3, Surface3Ptr>(m, "Surface3")
        .def_readwrite("transform", &Surface3::transform)
        .def_readwrite("isNormalFlipped", &Surface3::isNormalFlipped);
}