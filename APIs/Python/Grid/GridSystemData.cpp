/*************************************************************************
> File Name: GridSystemData.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GridSystemData functions for CubbyFlow Python API.
> Created Time: 2018/02/08
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Python/Grid/GridSystemData.h>
#include <Grid/GridSystemData2.h>
#include <Grid/GridSystemData3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddGridSystemData2(pybind11::module& m)
{
	pybind11::class_<GridSystemData2, GridSystemData2Ptr, Serializable>(m, "GridSystemData2",
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
	pybind11::class_<GridSystemData3, GridSystemData3Ptr, Serializable>(m, "GridSystemData3",
		R"pbdoc(
			3-D grid system data.

			This class is the key data structure for storing grid system data. To
			represent a grid system for fluid simulation, velocity field is defined as a
			face-centered (MAC) grid by default. It can also have additional scalar or
			vector attributes by adding extra data layer.
		)pbdoc")
	.def(pybind11::init<>());
}