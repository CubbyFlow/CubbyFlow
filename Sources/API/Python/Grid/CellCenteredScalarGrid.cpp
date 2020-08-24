// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Grid/CellCenteredScalarGrid.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Grid/CellCenteredScalarGrid2.hpp>
#include <Core/Grid/CellCenteredScalarGrid3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddCellCenteredScalarGrid2(pybind11::module& m)
{
    pybind11::class_<CellCenteredScalarGrid2, CellCenteredScalarGrid2Ptr,
                     ScalarGrid2>(static_cast<pybind11::handle>(m),
                                  "CellCenteredScalarGrid2",
                                  R"pbdoc(
			2-D Cell-centered scalar grid structure.

			This class represents 2-D cell-centered scalar grid which extends
			ScalarGrid2. As its name suggests, the class defines the data point at the
			center of a grid cell. Thus, the dimension of data points are equal to the
			dimension of the cells.
		)pbdoc")
        .def(
            "__init__",
            [](CellCenteredScalarGrid2& instance, pybind11::args args,
               pybind11::kwargs kwargs) {
                Size2 resolution{ 1, 1 };
                Vector2D gridSpacing{ 1, 1 };
                Vector2D gridOrigin{ 0, 0 };

                ParseGridResizeParams(args, kwargs, resolution, gridSpacing,
                                      gridOrigin);
                new (&instance) CellCenteredScalarGrid2(resolution, gridSpacing,
                                                        gridOrigin);
            },
            R"pbdoc(
			Constructs grid.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point at the grid.
				- domainSizeX : Domain size in x-direction.
			)pbdoc")
        .def_property_readonly("dataSize",
                               &CellCenteredScalarGrid2::GetDataSize,
                               R"pbdoc(
			Returns the actual data point size.
		)pbdoc")
        .def_property_readonly("dataOrigin",
                               &CellCenteredScalarGrid2::GetDataOrigin,
                               R"pbdoc(
			Returns data position for the grid point at (0, 0).

			Note that this is different from origin() since origin() returns
			the lower corner point of the bounding box.
		)pbdoc")
        .def("Set", &CellCenteredScalarGrid2::Set,
             R"pbdoc(
			Sets the contents with the given `other` grid.

			Parameters
			----------
			- other : Other grid to copy from.
		)pbdoc",
             pybind11::arg("other"));
}

void AddCellCenteredScalarGrid3(pybind11::module& m)
{
    pybind11::class_<CellCenteredScalarGrid3, CellCenteredScalarGrid3Ptr,
                     ScalarGrid3>(static_cast<pybind11::handle>(m),
                                  "CellCenteredScalarGrid3",
                                  R"pbdoc(
			3-D Cell-centered scalar grid structure.

			This class represents 3-D cell-centered scalar grid which extends
			ScalarGrid3. As its name suggests, the class defines the data point at the
			center of a grid cell. Thus, the dimension of data points are equal to the
			dimension of the cells.
		)pbdoc")
        .def(
            "__init__",
            [](CellCenteredScalarGrid3& instance, pybind11::args args,
               pybind11::kwargs kwargs) {
                Size3 resolution{ 1, 1, 1 };
                Vector3D gridSpacing{ 1, 1, 1 };
                Vector3D gridOrigin{ 0, 0, 0 };

                ParseGridResizeParams(args, kwargs, resolution, gridSpacing,
                                      gridOrigin);
                new (&instance) CellCenteredScalarGrid3(resolution, gridSpacing,
                                                        gridOrigin);
            },
            R"pbdoc(
			Constructs grid.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point at the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc")
        .def_property_readonly("dataSize",
                               &CellCenteredScalarGrid3::GetDataSize,
                               R"pbdoc(
			Returns the actual data point size.
		)pbdoc")
        .def_property_readonly("dataOrigin",
                               &CellCenteredScalarGrid3::GetDataOrigin,
                               R"pbdoc(
			Returns data position for the grid point at (0, 0, 0).

			Note that this is different from origin() since origin() returns
			the lower corner point of the bounding box.
		)pbdoc")
        .def("Set", &CellCenteredScalarGrid3::Set,
             R"pbdoc(
			Sets the contents with the given `other` grid.

			Parameters
			----------
			- other : Other grid to copy from.
		)pbdoc",
             pybind11::arg("other"));
}