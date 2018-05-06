/*************************************************************************
> File Name: Grid.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Grid functions for CubbyFlow Python API.
> Created Time: 2018/02/04
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Grid/Grid.h>
#include <Core/Grid/Grid2.h>
#include <Core/Grid/Grid3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddGrid2(pybind11::module& m)
{
	pybind11::class_<Grid2, Grid2Ptr, Serializable>(static_cast<pybind11::handle>(m), "Grid2",
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
	pybind11::class_<Grid3, Grid3Ptr, Serializable>(static_cast<pybind11::handle>(m), "Grid3",
		R"pbdoc(
			Abstract base class for 3-D cartesian grid structure.

			This class represents 3-D cartesian grid structure. This class is an
			abstract base class and does not store any data. The class only stores the
			shape of the grid. The grid structure is axis-aligned and can have different
			grid spacing per axis.
		)pbdoc");
}