/*************************************************************************
> File Name: PCISPHSolver.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: PCISPHSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/18
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Python/Solver/Particle/PCISPH/PCISPHSolver.h>
#include <Solver/Particle/PCISPH/PCISPHSolver2.h>
#include <Solver/Particle/PCISPH/PCISPHSolver3.h>
#include <Utils/Constants.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddPCISPHSolver2(pybind11::module& m)
{
	pybind11::class_<PCISPHSolver2, PCISPHSolver2Ptr, SPHSolver2>(static_cast<pybind11::handle>(m), "PCISPHSolver2",
		R"pbdoc(
			2-D PCISPH solver.

			This class implements 2-D predictive-corrective SPH solver. The main
			pressure solver is based on Solenthaler and Pajarola's 2009 SIGGRAPH paper.
			- See Solenthaler and Pajarola, Predictive-corrective incompressible SPH,
			ACM transactions on graphics (TOG). Vol. 28. No. 3. ACM, 2009.
		)pbdoc")
	.def(pybind11::init<double, double, double>(),
		R"pbdoc(
			Constructs a solver with target density, spacing, and relative kernel
			radius.
		)pbdoc",
		pybind11::arg("targetDensity") = WATER_DENSITY,
		pybind11::arg("targetSpacing") = 0.1,
		pybind11::arg("relativeKernelRadius") = 1.8)
	.def_property("maxDensityErrorRatio", &PCISPHSolver2::GetMaxDensityErrorRatio, &PCISPHSolver2::SetMaxDensityErrorRatio,
		R"pbdoc(
			The max allowed density error ratio.

			This property sets the max allowed density error ratio during the PCISPH
			iteration. Default is 0.01 (1%). The input value should be positive.
		)pbdoc")
	.def_property("maxNumberOfIterations", &PCISPHSolver2::GetMaxNumberOfIterations, &PCISPHSolver2::SetMaxNumberOfIterations,
		R"pbdoc(
			The max number of PCISPH iterations.

			This property sets the max number of PCISPH iterations. Default is 5.
		)pbdoc");
}

void AddPCISPHSolver3(pybind11::module& m)
{
	pybind11::class_<PCISPHSolver3, PCISPHSolver3Ptr, SPHSolver3>(static_cast<pybind11::handle>(m), "PCISPHSolver3",
		R"pbdoc(
			3-D PCISPH solver.

			This class implements 3-D predictive-corrective SPH solver. The main
			pressure solver is based on Solenthaler and Pajarola's 2009 SIGGRAPH paper.
			- See Solenthaler and Pajarola, Predictive-corrective incompressible SPH,
			ACM transactions on graphics (TOG). Vol. 28. No. 3. ACM, 2009.
		)pbdoc")
	.def(pybind11::init<double, double, double>(),
		R"pbdoc(
			Constructs a solver with target density, spacing, and relative kernel
			radius.
		)pbdoc",
		pybind11::arg("targetDensity") = WATER_DENSITY,
		pybind11::arg("targetSpacing") = 0.1,
		pybind11::arg("relativeKernelRadius") = 1.8)
	.def_property("maxDensityErrorRatio", &PCISPHSolver3::GetMaxDensityErrorRatio, &PCISPHSolver3::SetMaxDensityErrorRatio,
		R"pbdoc(
			The max allowed density error ratio.

			This property sets the max allowed density error ratio during the PCISPH
			iteration. Default is 0.01 (1%). The input value should be positive.
		)pbdoc")
	.def_property("maxNumberOfIterations", &PCISPHSolver3::GetMaxNumberOfIterations, &PCISPHSolver3::SetMaxNumberOfIterations,
		R"pbdoc(
			The max number of PCISPH iterations.

			This property sets the max number of PCISPH iterations. Default is 5.
		)pbdoc");
}