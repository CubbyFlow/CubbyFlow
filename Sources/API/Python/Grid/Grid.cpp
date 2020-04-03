// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Grid/Grid.hpp>
#include <Core/Grid/Grid2.hpp>
#include <Core/Grid/Grid3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddGrid2(pybind11::module& m)
{
    pybind11::class_<Grid2, Grid2Ptr, Serializable>(
        static_cast<pybind11::handle>(m), "Grid2",
        R"pbdoc(
			Abstract base class for 2-D cartesian grid structure.

			This class represents 2-D cartesian grid structure. This class is an
			abstract base class and does not store any data. The class only stores the
			shape of the grid. The grid structure is axis-aligned and can have different
			grid spacing per axis.
		)pbdoc");
}

void AddGrid3(pybind11::module& m)
{
    pybind11::class_<Grid3, Grid3Ptr, Serializable>(
        static_cast<pybind11::handle>(m), "Grid3",
        R"pbdoc(
			Abstract base class for 3-D cartesian grid structure.

			This class represents 3-D cartesian grid structure. This class is an
			abstract base class and does not store any data. The class only stores the
			shape of the grid. The grid structure is axis-aligned and can have different
			grid spacing per axis.
		)pbdoc");
}