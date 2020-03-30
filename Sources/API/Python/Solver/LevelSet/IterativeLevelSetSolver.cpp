/*************************************************************************
> File Name: IterativeLevelSetSolver.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: IterativeLevelSetSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Solver/LevelSet/IterativeLevelSetSolver.h>
#include <Core/Solver/LevelSet/IterativeLevelSetSolver2.hpp>
#include <Core/Solver/LevelSet/IterativeLevelSetSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddIterativeLevelSetSolver2(pybind11::module& m)
{
	pybind11::class_<IterativeLevelSetSolver2, IterativeLevelSetSolver2Ptr, LevelSetSolver2>(static_cast<pybind11::handle>(m), "IterativeLevelSetSolver2",
		R"pbdoc(
			Abstract base class for 2-D PDE-based iterative level set solver.

			This class provides infrastructure for 2-D PDE-based iterative level set
			solver. Internally, the class implements upwind-style wave propagation and
			the inheriting classes must provide a way to compute the derivatives for
			given grid points.

			- See Osher, Stanley, and Ronald Fedkiw. Level set methods and dynamic
			implicit surfaces. Vol. 153. Springer Science & Business Media, 2006.
		)pbdoc")
	.def("Reinitialize", [](IterativeLevelSetSolver2& instance, const ScalarGrid2Ptr& inputSDF, double maxDistance, ScalarGrid2Ptr outputSDF)
	{
		instance.Reinitialize(*inputSDF, maxDistance, outputSDF.get());
	},
		R"pbdoc(
			Reinitializes given scalar field to signed-distance field.

			Parameters
			----------
			- inputSDF : Input signed-distance field which can be distorted.
			- maxDistance : Max range of reinitialization.
			- outputSDF : Output signed-distance field.
		)pbdoc",
		pybind11::arg("inputSDF"),
		pybind11::arg("maxDistance"),
		pybind11::arg("outputSDF"))
	.def("Extrapolate", [](IterativeLevelSetSolver2& instance, const Grid2Ptr& input, const ScalarGrid2Ptr& sdf, double maxDistance, Grid2Ptr output)
	{
		auto inputSG = std::dynamic_pointer_cast<ScalarGrid2>(input);
		auto inputCG = std::dynamic_pointer_cast<CollocatedVectorGrid2>(input);
		auto inputFG = std::dynamic_pointer_cast<FaceCenteredGrid2>(input);

		auto outputSG = std::dynamic_pointer_cast<ScalarGrid2>(output);
		auto outputCG = std::dynamic_pointer_cast<CollocatedVectorGrid2>(output);
		auto outputFG = std::dynamic_pointer_cast<FaceCenteredGrid2>(output);

		if (inputSG != nullptr && outputSG != nullptr)
		{
			instance.Extrapolate(*inputSG, *sdf, maxDistance, outputSG.get());
		}
		else if (inputCG != nullptr && outputCG != nullptr)
		{
			instance.Extrapolate(*inputCG, *sdf, maxDistance, outputCG.get());
		}
		else if (inputFG != nullptr && outputFG != nullptr)
		{
			instance.Extrapolate(*inputFG, *sdf, maxDistance, outputFG.get());
		}
		else
		{
			throw std::invalid_argument("Grids input and output must have same type.");
		}
	},
		R"pbdoc(
			Extrapolates given field from negative to positive SDF region.

			Parameters
			----------
			- input : Input field to be extrapolated.
			- sdf : Reference signed-distance field.
			- maxDistance : Max range of extrapolation.
			- output : Output field.
		)pbdoc",
		pybind11::arg("input"),
		pybind11::arg("sdf"),
		pybind11::arg("maxDistance"),
		pybind11::arg("output"))
	.def_property("maxCFL", &IterativeLevelSetSolver2::GetMaxCFL, &IterativeLevelSetSolver2::SetMaxCFL,
		R"pbdoc(
			The maximum CFL limit.
		)pbdoc");
}

void AddIterativeLevelSetSolver3(pybind11::module& m)
{
	pybind11::class_<IterativeLevelSetSolver3, IterativeLevelSetSolver3Ptr, LevelSetSolver3>(static_cast<pybind11::handle>(m), "IterativeLevelSetSolver3",
		R"pbdoc(
			Abstract base class for 3-D PDE-based iterative level set solver.

			This class provides infrastructure for 3-D PDE-based iterative level set
			solver. Internally, the class implements upwind-style wave propagation and
			the inheriting classes must provide a way to compute the derivatives for
			given grid points.

			- See Osher, Stanley, and Ronald Fedkiw. Level set methods and dynamic
			implicit surfaces. Vol. 153. Springer Science & Business Media, 2006.
		)pbdoc")
	.def("Reinitialize", [](IterativeLevelSetSolver3& instance, const ScalarGrid3Ptr& inputSDF, double maxDistance, ScalarGrid3Ptr outputSDF)
	{
		instance.Reinitialize(*inputSDF, maxDistance, outputSDF.get());
	},
		R"pbdoc(
			Reinitializes given scalar field to signed-distance field.

			Parameters
			----------
			- inputSDF : Input signed-distance field which can be distorted.
			- maxDistance : Max range of reinitialization.
			- outputSDF : Output signed-distance field.
		)pbdoc",
		pybind11::arg("inputSDF"),
		pybind11::arg("maxDistance"),
		pybind11::arg("outputSDF"))
	.def("Extrapolate", [](IterativeLevelSetSolver3& instance, const Grid3Ptr& input, const ScalarGrid3Ptr& sdf, double maxDistance, Grid3Ptr output)
	{
		auto inputSG = std::dynamic_pointer_cast<ScalarGrid3>(input);
		auto inputCG = std::dynamic_pointer_cast<CollocatedVectorGrid3>(input);
		auto inputFG = std::dynamic_pointer_cast<FaceCenteredGrid3>(input);

		auto outputSG = std::dynamic_pointer_cast<ScalarGrid3>(output);
		auto outputCG = std::dynamic_pointer_cast<CollocatedVectorGrid3>(output);
		auto outputFG = std::dynamic_pointer_cast<FaceCenteredGrid3>(output);

		if (inputSG != nullptr && outputSG != nullptr)
		{
			instance.Extrapolate(*inputSG, *sdf, maxDistance, outputSG.get());
		}
		else if (inputCG != nullptr && outputCG != nullptr)
		{
			instance.Extrapolate(*inputCG, *sdf, maxDistance, outputCG.get());
		}
		else if (inputFG != nullptr && outputFG != nullptr)
		{
			instance.Extrapolate(*inputFG, *sdf, maxDistance, outputFG.get());
		}
		else
		{
			throw std::invalid_argument("Grids input and output must have same type.");
		}
	},
		R"pbdoc(
			Extrapolates given field from negative to positive SDF region.

			Parameters
			----------
			- input : Input field to be extrapolated.
			- sdf : Reference signed-distance field.
			- maxDistance : Max range of extrapolation.
			- output : Output field.
		)pbdoc",
		pybind11::arg("input"),
		pybind11::arg("sdf"),
		pybind11::arg("maxDistance"),
		pybind11::arg("output"))
	.def_property("maxCFL", &IterativeLevelSetSolver3::GetMaxCFL, &IterativeLevelSetSolver3::SetMaxCFL,
		R"pbdoc(
			The maximum CFL limit.
		)pbdoc");
}