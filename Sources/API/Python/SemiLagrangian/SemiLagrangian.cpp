/*************************************************************************
> File Name: SemiLagrangian.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: SemiLagrangian functions for CubbyFlow Python API.
> Created Time: 2018/02/10
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/SemiLagrangian/SemiLagrangian.h>
#include <Core/SemiLagrangian/SemiLagrangian2.h>
#include <Core/SemiLagrangian/SemiLagrangian3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddSemiLagrangian2(pybind11::module& m)
{
	pybind11::class_<SemiLagrangian2, SemiLagrangian2Ptr, AdvectionSolver2>(static_cast<pybind11::handle>(m), "SemiLagrangian2",
		R"pbdoc(
			Implementation of 2-D semi-Lagrangian advection solver.

			This class implements 2-D semi-Lagrangian advection solver. By default, the
			class implements 1st-order (linear) algorithm for the spatial interpolation.
			For the back-tracing, this class uses 2nd-order mid-point rule with adaptive
			time-stepping (CFL <= 1).
		)pbdoc")
	.def(pybind11::init<>())
	.def("Solve", [](SemiLagrangian2& instance, const Grid2Ptr& input, const VectorField2Ptr& flow, double dt, Grid2Ptr output, const ScalarField2Ptr& boundarySDF)
	{
		auto inputSG = std::dynamic_pointer_cast<ScalarGrid2>(input);
		auto inputCG = std::dynamic_pointer_cast<CollocatedVectorGrid2>(input);
		auto inputFG = std::dynamic_pointer_cast<FaceCenteredGrid2>(input);

		auto outputSG = std::dynamic_pointer_cast<ScalarGrid2>(output);
		auto outputCG = std::dynamic_pointer_cast<CollocatedVectorGrid2>(output);
		auto outputFG = std::dynamic_pointer_cast<FaceCenteredGrid2>(output);

		if (inputSG != nullptr && outputSG != nullptr)
		{
			instance.Advect(*inputSG, *flow, dt, outputSG.get(), *boundarySDF);
		}
		else if (inputCG != nullptr && outputCG != nullptr)
		{
			instance.Advect(*inputCG, *flow, dt, outputCG.get(), *boundarySDF);
		}
		else if (inputFG != nullptr && outputFG != nullptr)
		{
			instance.Advect(*inputFG, *flow, dt, outputFG.get(), *boundarySDF);
		}
		else
		{
			throw std::invalid_argument("Grids input and output must have same type.");
		}
	},
		R"pbdoc(
			Computes semi-Lagrangian for given scalar grid.

			This function computes semi-Lagrangian method to solve advection
			equation for given field `input` and underlying vector field
			`flow` that carries the input field. The solution after solving the
			equation for given time-step `dt` should be stored in field
			`output`. The boundary interface is given by a signed-distance field.
			The field is negative inside the boundary. By default, a constant field
			with max double value () is used, meaning no boundary.

			Parameters
			----------
			- input : Input grid.
			- flow : Vector field that advects the input field.
			- dt : Time-step for the advection.
			- output : Output grid.
			- boundarySDF : Boundary interface defined by signed-distance field.
		)pbdoc",
		pybind11::arg("input"),
		pybind11::arg("flow"),
		pybind11::arg("dt"),
		pybind11::arg("output"),
		pybind11::arg("boundarySDF") = ConstantScalarField2::Builder().WithValue(std::numeric_limits<double>::max()).MakeShared());
}

void AddSemiLagrangian3(pybind11::module& m)
{
	pybind11::class_<SemiLagrangian3, SemiLagrangian3Ptr, AdvectionSolver3>(static_cast<pybind11::handle>(m), "SemiLagrangian3",
		R"pbdoc(
			Implementation of 3-D semi-Lagrangian advection solver.

			This class implements 3-D semi-Lagrangian advection solver. By default, the
			class implements 1st-order (linear) algorithm for the spatial interpolation.
			For the back-tracing, this class uses 3nd-order mid-point rule with adaptive
			time-stepping (CFL <= 1).
		)pbdoc")
	.def(pybind11::init<>())
	.def("Solve", [](SemiLagrangian3& instance, const Grid3Ptr& input, const VectorField3Ptr& flow, double dt, Grid3Ptr output, const ScalarField3Ptr& boundarySDF)
	{
		auto inputSG = std::dynamic_pointer_cast<ScalarGrid3>(input);
		auto inputCG = std::dynamic_pointer_cast<CollocatedVectorGrid3>(input);
		auto inputFG = std::dynamic_pointer_cast<FaceCenteredGrid3>(input);

		auto outputSG = std::dynamic_pointer_cast<ScalarGrid3>(output);
		auto outputCG = std::dynamic_pointer_cast<CollocatedVectorGrid3>(output);
		auto outputFG = std::dynamic_pointer_cast<FaceCenteredGrid3>(output);

		if (inputSG != nullptr && outputSG != nullptr)
		{
			instance.Advect(*inputSG, *flow, dt, outputSG.get(), *boundarySDF);
		}
		else if (inputCG != nullptr && outputCG != nullptr)
		{
			instance.Advect(*inputCG, *flow, dt, outputCG.get(), *boundarySDF);
		}
		else if (inputFG != nullptr && outputFG != nullptr)
		{
			instance.Advect(*inputFG, *flow, dt, outputFG.get(), *boundarySDF);
		}
		else
		{
			throw std::invalid_argument("Grids input and output must have same type.");
		}
	},
		R"pbdoc(
			Computes semi-Lagrangian for given scalar grid.

			This function computes semi-Lagrangian method to solve advection
			equation for given field `input` and underlying vector field
			`flow` that carries the input field. The solution after solving the
			equation for given time-step `dt` should be stored in field
			`output`. The boundary interface is given by a signed-distance field.
			The field is negative inside the boundary. By default, a constant field
			with max double value (kMaxD) is used, meaning no boundary.

			Parameters
			----------
			- input : Input grid.
			- flow : Vector field that advects the input field.
			- dt : Time-step for the advection.
			- output : Output grid.
			- boundarySDF : Boundary interface defined by signed-distance field.
		)pbdoc",
		pybind11::arg("input"),
		pybind11::arg("flow"),
		pybind11::arg("dt"),
		pybind11::arg("output"),
		pybind11::arg("boundarySDF") = ConstantScalarField3::Builder().WithValue(std::numeric_limits<double>::max()).MakeShared());
}