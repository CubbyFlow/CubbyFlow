// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Grid/VectorGrid.hpp>
#include <Core/Grid/VectorGrid2.hpp>
#include <Core/Grid/VectorGrid3.hpp>

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddVectorGrid2(pybind11::module& m)
{
    pybind11::class_<VectorGrid2, VectorGrid2Ptr, VectorField2, Grid2>(
        static_cast<pybind11::handle>(m), "VectorGrid2",
        R"pbdoc(
			Abstract base class for 2-D vector grid structure.
		)pbdoc");
}

void AddVectorGrid3(pybind11::module& m)
{
    pybind11::class_<VectorGrid3, VectorGrid3Ptr, VectorField3, Grid3>(
        static_cast<pybind11::handle>(m), "VectorGrid3",
        R"pbdoc(
			Abstract base class for 3-D vector grid structure.
		)pbdoc");
}