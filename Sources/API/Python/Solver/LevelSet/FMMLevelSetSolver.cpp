/*************************************************************************
> File Name: FMMLevelSetSolver.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: FMMLevelSetSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Solver/LevelSet/FMMLevelSetSolver.h>
#include <Core/Solver/LevelSet/FMMLevelSetSolver2.h>
#include <Core/Solver/LevelSet/FMMLevelSetSolver3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddFMMLevelSetSolver2(pybind11::module& m)
{
	pybind11::class_<FMMLevelSetSolver2, FMMLevelSetSolver2Ptr, LevelSetSolver2>(m, "FMMLevelSetSolver2",
		R"pbdoc(
			2-D fast marching method (FMM) implementation.

			This class implements 2-D FMM. First-order upwind-style differencing is used
			to solve the PDE.

			- See https://math.berkeley.edu/~sethian/2006/Explanations/fast_marching_explain.html
			- See Sethian, James A. "A fast marching level set method for monotonically
			advancing fronts." Proceedings of the National Academy of Sciences 93.4
			(1996): 1591-1595.
		)pbdoc")
	.def("Reinitialize", [](FMMLevelSetSolver2& instance, const ScalarGrid2Ptr& inputSDF, double maxDistance, ScalarGrid2Ptr outputSDF)
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
	.def("Extrapolate", [](FMMLevelSetSolver2& instance, const Grid2Ptr& input, const ScalarGrid2Ptr& sdf, double maxDistance, Grid2Ptr output)
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
		pybind11::arg("output"));
}

void AddFMMLevelSetSolver3(pybind11::module& m)
{
	pybind11::class_<FMMLevelSetSolver3, FMMLevelSetSolver3Ptr, LevelSetSolver3>(m, "FMMLevelSetSolver3",
		R"pbdoc(
			3-D fast marching method (FMM) implementation.

			This class implements 3-D FMM. First-order upwind-style differencing is used
			to solve the PDE.

			- See https://math.berkeley.edu/~sethian/2006/Explanations/fast_marching_explain.html
			- See Sethian, James A. "A fast marching level set method for monotonically
			advancing fronts." Proceedings of the National Academy of Sciences 93.4
			(1996): 1591-1595.
		)pbdoc")
	.def("Reinitialize", [](FMMLevelSetSolver3& instance, const ScalarGrid3Ptr& inputSDF, double maxDistance, ScalarGrid3Ptr outputSDF)
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
	.def("Extrapolate", [](FMMLevelSetSolver3& instance, const Grid3Ptr& input, const ScalarGrid3Ptr& sdf, double maxDistance, Grid3Ptr output)
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
		pybind11::arg("output"));
}