// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Field/ScalarField.hpp>
#include <Core/Field/ScalarField2.hpp>
#include <Core/Field/ScalarField3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddScalarField2(pybind11::module& m)
{
    pybind11::class_<ScalarField2, ScalarField2Ptr, Field2>(
        static_cast<pybind11::handle>(m), "ScalarField2",
        R"pbdoc(
			Abstract base class for 2-D scalar field.
		)pbdoc");
}

void AddScalarField3(pybind11::module& m)
{
    pybind11::class_<ScalarField3, ScalarField3Ptr, Field3>(
        static_cast<pybind11::handle>(m), "ScalarField3",
        R"pbdoc(
			Abstract base class for 3-D scalar field.
		)pbdoc");
}