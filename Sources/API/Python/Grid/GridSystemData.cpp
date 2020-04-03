// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Grid/GridSystemData.hpp>
#include <Core/Grid/GridSystemData2.hpp>
#include <Core/Grid/GridSystemData3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddGridSystemData2(pybind11::module& m)
{
    pybind11::class_<GridSystemData2, GridSystemData2Ptr, Serializable>(
        m, "GridSystemData2",
        R"pbdoc(
			2-D grid system data.

			This class is the key data structure for storing grid system data. To
			represent a grid system for fluid simulation, velocity field is defined as a
			face-centered (MAC) grid by default. It can also have additional scalar or
			vector attributes by adding extra data layer.
		)pbdoc")
        .def(pybind11::init<>());
}

void AddGridSystemData3(pybind11::module& m)
{
    pybind11::class_<GridSystemData3, GridSystemData3Ptr, Serializable>(
        m, "GridSystemData3",
        R"pbdoc(
			3-D grid system data.

			This class is the key data structure for storing grid system data. To
			represent a grid system for fluid simulation, velocity field is defined as a
			face-centered (MAC) grid by default. It can also have additional scalar or
			vector attributes by adding extra data layer.
		)pbdoc")
        .def(pybind11::init<>());
}