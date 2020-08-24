// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Utils/Constants.hpp>
#include <Core/Utils/Constants.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddConstants(pybind11::module& m)
{
    m.attr("DIRECTION_NONE") = pybind11::int_(DIRECTION_NONE);
    m.attr("DIRECTION_LEFT") = pybind11::int_(DIRECTION_LEFT);
    m.attr("DIRECTION_RIGHT") = pybind11::int_(DIRECTION_RIGHT);
    m.attr("DIRECTION_DOWN") = pybind11::int_(DIRECTION_DOWN);
    m.attr("DIRECTION_UP") = pybind11::int_(DIRECTION_UP);
    m.attr("DIRECTION_BACK") = pybind11::int_(DIRECTION_BACK);
    m.attr("DIRECTION_FRONT") = pybind11::int_(DIRECTION_FRONT);
    m.attr("DIRECTION_ALL") = pybind11::int_(DIRECTION_ALL);
}