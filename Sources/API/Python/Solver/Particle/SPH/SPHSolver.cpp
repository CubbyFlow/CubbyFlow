/*************************************************************************
> File Name: SPHSolver.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: SPHSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/18
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Solver/Particle/SPH/SPHSolver.h>
#include <Core/Solver/Particle/SPH/SPHSolver2.h>
#include <Core/Solver/Particle/SPH/SPHSolver3.h>
#include <Core/Utils/Constants.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddSPHSolver2(pybind11::module& m)
{
	pybind11::class_<SPHSolver2, SPHSolver2Ptr, ParticleSystemSolver2>(static_cast<pybind11::handle>(m), "SPHSolver2",
		R"pbdoc(
			2-D SPH solver.

			This class implements 2-D SPH solver. The main pressure solver is based on
			equation-of-state (EOS).
			- See M{\"u}ller et al., Particle-based fluid simulation for interactive
			applications, SCA 2003.
			- See M. Becker and M. Teschner, Weakly compressible SPH for free surface
			flows, SCA 2007.
			- See Adams and Wicke, Meshless approximation methods and applications in
			physics based modeling and animation, Eurographics tutorials 2009.
		)pbdoc")
	.def(pybind11::init<double, double, double>(),
		R"pbdoc(
			Constructs a solver with target density, spacing, and relative kernel
			radius.
		)pbdoc",
		pybind11::arg("targetDensity") = WATER_DENSITY,
		pybind11::arg("targetSpacing") = 0.1,
		pybind11::arg("relativeKernelRadius") = 1.8)
	.def_property("eosExponent", &SPHSolver2::GetEosExponent, &SPHSolver2::SetEosExponent,
		R"pbdoc(
			The exponent part of the equation-of-state.

			This function sets the exponent part of the equation-of-state.
			The value must be greater than 1.0, and smaller inputs will be clamped.
			Default is 7.
		)pbdoc")
	.def_property("negativePressureScale", &SPHSolver2::GetNegativePressureScale, &SPHSolver2::SetNegativePressureScale,
		R"pbdoc(
			The negative pressure scale.

			This function sets the negative pressure scale. By setting the number
			between 0 and 1, the solver will scale the effect of negative pressure
			which can prevent the clumping of the particles near the surface. Input
			value outside 0 and 1 will be clamped within the range. Default is 0.
		)pbdoc")
	.def_property("viscosityCoefficient", &SPHSolver2::GetViscosityCoefficient, &SPHSolver2::SetViscosityCoefficient,
		R"pbdoc(
			The viscosity coefficient.
		)pbdoc")
	.def_property("pseudoViscosityCoefficient", &SPHSolver2::GetPseudoViscosityCoefficient, &SPHSolver2::SetPseudoViscosityCoefficient,
		R"pbdoc(
			The pseudo viscosity coefficient.

			This function sets the pseudo viscosity coefficient which applies
			additional pseudo-physical damping to the system. Default is 10.
		)pbdoc")
	.def_property("speedOfSound", &SPHSolver2::GetSpeedOfSound, &SPHSolver2::SetSpeedOfSound,
		R"pbdoc(
			The speed of sound.

			This function sets the speed of sound which affects the stiffness of the
			EOS and the time-step size. Higher value will make EOS stiffer and the
			time-step smaller. The input value must be higher than 0.0.
		)pbdoc")
	.def_property("timeStepLimitScale", &SPHSolver2::GetTimeStepLimitScale, &SPHSolver2::SetTimeStepLimitScale,
		R"pbdoc(
			Multiplier that scales the max allowed time-step.

			This function returns the multiplier that scales the max allowed
			time-step. When the scale is 1.0, the time-step is bounded by the speed
			of sound and max acceleration.
		)pbdoc")
	.def_property_readonly("sphSystemData", &SPHSolver2::GetSPHSystemData,
		R"pbdoc(
			The SPH system data.
		)pbdoc");
}

void AddSPHSolver3(pybind11::module& m)
{
	pybind11::class_<SPHSolver3, SPHSolver3Ptr, ParticleSystemSolver3>(static_cast<pybind11::handle>(m), "SPHSolver3",
		R"pbdoc(
			3-D SPH solver.

			This class implements 3-D SPH solver. The main pressure solver is based on
			equation-of-state (EOS).
			- See M{\"u}ller et al., Particle-based fluid simulation for interactive
			applications, SCA 2003.
			- See M. Becker and M. Teschner, Weakly compressible SPH for free surface
			flows, SCA 2007.
			- See Adams and Wicke, Meshless approximation methods and applications in
			physics based modeling and animation, Eurographics tutorials 2009.
		)pbdoc")
	.def(pybind11::init<double, double, double>(),
		R"pbdoc(
			Constructs a solver with target density, spacing, and relative kernel
			radius.
		)pbdoc",
		pybind11::arg("targetDensity") = WATER_DENSITY,
		pybind11::arg("targetSpacing") = 0.1,
		pybind11::arg("relativeKernelRadius") = 1.8)
	.def_property("eosExponent", &SPHSolver3::GetEosExponent, &SPHSolver3::SetEosExponent,
		R"pbdoc(
			The exponent part of the equation-of-state.

			This function sets the exponent part of the equation-of-state.
			The value must be greater than 1.0, and smaller inputs will be clamped.
			Default is 7.
		)pbdoc")
	.def_property("negativePressureScale", &SPHSolver3::GetNegativePressureScale, &SPHSolver3::SetNegativePressureScale,
		R"pbdoc(
			The negative pressure scale.

			This function sets the negative pressure scale. By setting the number
			between 0 and 1, the solver will scale the effect of negative pressure
			which can prevent the clumping of the particles near the surface. Input
			value outside 0 and 1 will be clamped within the range. Default is 0.
		)pbdoc")
	.def_property("viscosityCoefficient", &SPHSolver3::GetViscosityCoefficient, &SPHSolver3::SetViscosityCoefficient,
		R"pbdoc(
			The viscosity coefficient.
		)pbdoc")
	.def_property("pseudoViscosityCoefficient", &SPHSolver3::GetPseudoViscosityCoefficient, &SPHSolver3::SetPseudoViscosityCoefficient,
		R"pbdoc(
			The pseudo viscosity coefficient.

			This function sets the pseudo viscosity coefficient which applies
			additional pseudo-physical damping to the system. Default is 10.
		)pbdoc")
	.def_property("speedOfSound", &SPHSolver3::GetSpeedOfSound, &SPHSolver3::SetSpeedOfSound,
		R"pbdoc(
			The speed of sound.

			This function sets the speed of sound which affects the stiffness of the
			EOS and the time-step size. Higher value will make EOS stiffer and the
			time-step smaller. The input value must be higher than 0.0.
		)pbdoc")
	.def_property("timeStepLimitScale", &SPHSolver3::GetTimeStepLimitScale, &SPHSolver3::SetTimeStepLimitScale,
		R"pbdoc(
			Multiplier that scales the max allowed time-step.

			This function returns the multiplier that scales the max allowed
			time-step. When the scale is 1.0, the time-step is bounded by the speed
			of sound and max acceleration.
		)pbdoc")
	.def_property_readonly("sphSystemData", &SPHSolver3::GetSPHSystemData,
		R"pbdoc(
			The SPH system data.
		)pbdoc");
}