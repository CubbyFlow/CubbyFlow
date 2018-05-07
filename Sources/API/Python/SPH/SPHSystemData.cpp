/*************************************************************************
> File Name: SPHSystemData.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: SPHSystemData functions for CubbyFlow Python API.
> Created Time: 2018/02/08
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/SPH/SPHSystemData.h>
#include <Core/SPH/SPHSystemData2.h>
#include <Core/SPH/SPHSystemData3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddSPHSystemData2(pybind11::module& m)
{
	pybind11::class_<SPHSystemData2, SPHSystemData2Ptr, ParticleSystemData2>(m, "SPHSystemData2",
		R"pbdoc(
			2-D SPH particle system data.

			This class extends ParticleSystemData2 to specialize the data model for SPH.
			It includes density and pressure array as a default particle attribute, and
			it also contains SPH utilities such as interpolation operator.
		)pbdoc")
	.def(pybind11::init<size_t>(),
		R"pbdoc(
			Constructs SPH system data with given number of particles.
		)pbdoc",
		pybind11::arg("numberOfParticles") = 0)
	.def_property_readonly("densities", [](SPHSystemData2& instance)
	{
		return instance.GetDensities();
	},
		R"pbdoc(
			The density array accessor.
		)pbdoc")
	.def_property_readonly("pressures", [](SPHSystemData2& instance)
	{
		return instance.GetPressures();
	},
		R"pbdoc(
			The pressure array accessor.
		)pbdoc")
	.def("UpdateDensities", &SPHSystemData2::UpdateDensities,
		R"pbdoc(
			Updates the density array with the latest particle positions.
		)pbdoc")
	.def_property("targetDensity", &SPHSystemData2::GetTargetDensity, &SPHSystemData2::SetTargetDensity,
		R"pbdoc(
			The target density of this particle system.
		)pbdoc")
	.def_property("targetSpacing", &SPHSystemData2::GetTargetSpacing, &SPHSystemData2::SetTargetSpacing,
		R"pbdoc(
			The target particle spacing in meters.

			Once this property has changed, hash grid and density should be
			updated using UpdateHashGrid() and UpdateDensities()).
		)pbdoc")
	.def_property("relativeKernelRadius", &SPHSystemData2::GetRelativeKernelRadius, &SPHSystemData2::SetRelativeKernelRadius,
		R"pbdoc(
			The relative kernel radius.

			The relative kernel radius compared to the target particle
			spacing (i.e. kernel radius / target spacing).
			Once this property has changed, hash grid and density should
			be updated using UpdateHashGrid() and UpdateDensities()).
		)pbdoc")
	.def_property("kernelRadius", &SPHSystemData2::GetKernelRadius, &SPHSystemData2::SetKernelRadius,
		R"pbdoc(
			The kernel radius in meters unit.

			Sets the absolute kernel radius compared to the target particle
			spacing (i.e. relative kernel radius * target spacing).
			Once this function is called, hash grid and density should
			be updated using UpdateHashGrid() and UpdateDensities()).
		)pbdoc")
	.def("BuildNeighborSearcher", &SPHSystemData2::BuildNeighborSearcher,
		R"pbdoc(
			Builds neighbor searcher with kernel radius.
		)pbdoc")
	.def("BuildNeighborLists", &SPHSystemData2::BuildNeighborLists,
		R"pbdoc(
			Builds neighbor lists with kernel radius.
		)pbdoc")
	.def("Set", &SPHSystemData2::Set,
		R"pbdoc(
			Copies from other SPH system data.
		)pbdoc");
}

void AddSPHSystemData3(pybind11::module& m)
{
	pybind11::class_<SPHSystemData3, SPHSystemData3Ptr, ParticleSystemData3>(m, "SPHSystemData3",
		R"pbdoc(
			3-D SPH particle system data.

			This class extends ParticleSystemData3 to specialize the data model for SPH.
			It includes density and pressure array as a default particle attribute, and
			it also contains SPH utilities such as interpolation operator.
		)pbdoc")
	.def(pybind11::init<size_t>(),
		R"pbdoc(
			Constructs SPH system data with given number of particles.
		)pbdoc",
		pybind11::arg("numberOfParticles") = 0)
	.def_property_readonly("densities", [](SPHSystemData3& instance)
	{
		return instance.GetDensities();
	},
		R"pbdoc(
			The density array accessor.
		)pbdoc")
	.def_property_readonly("pressures", [](SPHSystemData3& instance)
	{
		return instance.GetPressures();
	},
		R"pbdoc(
			The pressure array accessor.
		)pbdoc")
	.def("UpdateDensities", &SPHSystemData3::UpdateDensities,
		R"pbdoc(
			Updates the density array with the latest particle positions.
		)pbdoc")
	.def_property("targetDensity", &SPHSystemData3::GetTargetDensity, &SPHSystemData3::SetTargetDensity,
		R"pbdoc(
			The target density of this particle system.
		)pbdoc")
	.def_property("targetSpacing", &SPHSystemData3::GetTargetSpacing, &SPHSystemData3::SetTargetSpacing,
		R"pbdoc(
			 The target particle spacing in meters.

			 Once this property has changed, hash grid and density should be
			 updated using UpdateHashGrid() and UpdateDensities()).
		)pbdoc")
	.def_property("relativeKernelRadius", &SPHSystemData3::GetRelativeKernelRadius, &SPHSystemData3::SetRelativeKernelRadius,
		R"pbdoc(
			The relative kernel radius.

			The relative kernel radius compared to the target particle
			spacing (i.e. kernel radius / target spacing).
			Once this property has changed, hash grid and density should
			be updated using UpdateHashGrid() and UpdateDensities()).
		)pbdoc")
	.def_property("kernelRadius", &SPHSystemData3::GetKernelRadius, &SPHSystemData3::SetKernelRadius,
		R"pbdoc(
			The kernel radius in meters unit.

			Sets the absolute kernel radius compared to the target particle
			spacing (i.e. relative kernel radius * target spacing).
			Once this function is called, hash grid and density should
			be updated using UpdateHashGrid() and UpdateDensities()).
		)pbdoc")
	.def("BuildNeighborSearcher", &SPHSystemData3::BuildNeighborSearcher,
		R"pbdoc(
			Builds neighbor searcher with kernel radius.
		)pbdoc")
	.def("BuildNeighborLists", &SPHSystemData3::BuildNeighborLists,
		R"pbdoc(
			Builds neighbor lists with kernel radius.
		)pbdoc")
	.def("Set", &SPHSystemData3::Set,
		R"pbdoc(
			Copies from other SPH system data.
		)pbdoc");
}