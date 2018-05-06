/*************************************************************************
> File Name: VectorGrid.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: VectorGrid functions for CubbyFlow Python API.
> Created Time: 2018/02/04
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Grid/VectorGrid.h>
#include <Core/Grid/VectorGrid2.h>
#include <Core/Grid/VectorGrid3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddVectorGrid2(pybind11::module& m)
{
	pybind11::class_<VectorGrid2, VectorGrid2Ptr, VectorField2, Grid2>(static_cast<pybind11::handle>(m), "VectorGrid2",
		R"pbdoc(
			Abstract base class for 2-D vector grid structure.
		)pbdoc");
}

void AddVectorGrid3(pybind11::module& m)
{
	pybind11::class_<VectorGrid3, VectorGrid3Ptr, VectorField3, Grid3>(static_cast<pybind11::handle>(m), "VectorGrid3",
		R"pbdoc(
			Abstract base class for 3-D vector grid structure.
		)pbdoc");
}