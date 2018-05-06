/*************************************************************************
> File Name: ParticleSystemSolver.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: ParticleSystemSolver functions for CubbyFlow Python API.
> Created Time: 2018/02/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Solver/Particle/ParticleSystemSolver.h>
#include <Core/Solver/Particle/ParticleSystemSolver2.h>
#include <Core/Solver/Particle/ParticleSystemSolver3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddParticleSystemSolver2(pybind11::module& m)
{
	pybind11::class_<ParticleSystemSolver2, ParticleSystemSolver2Ptr>(m, "ParticleSystemSolver2",
		R"pbdoc(
			Basic 2-D particle system solver.

			This class implements basic particle system solver. It includes gravity,
			air drag, and collision. But it does not compute particle-to-particle
			interaction. Thus, this solver is suitable for performing simple spray-like
			simulations with low computational cost. This class can be further extend
			to add more sophisticated simulations, such as SPH, to handle
			particle-to-particle intersection.
		)pbdoc")
	.def(pybind11::init<double, double>(),
		R"pbdoc(
			Constructs a solver with particle parameters.

			Parameters
			----------
			- radius : Radius of a particle in meters (default 1e-2).
			- mass : Mass of a particle in kg (default 1e-2).
		)pbdoc",
		pybind11::arg("radius") = 1e-2, 
		pybind11::arg("mass") = 1e-2)
	.def_property("dragCoefficient", &ParticleSystemSolver2::GetDragCoefficient, &ParticleSystemSolver2::SetDragCoefficient,
		R"pbdoc(
			The drag coefficient.

			The drag coefficient controls the amount of air-drag. The coefficient
			should be a positive number and 0 means no drag force.
		)pbdoc")
	.def_property("restitutionCoefficient", &ParticleSystemSolver2::GetRestitutionCoefficient, &ParticleSystemSolver2::SetRestitutionCoefficient,
		R"pbdoc(
			The restitution coefficient.

			The restitution coefficient controls the bouncy-ness of a particle when
			it hits a collider surface. The range of the coefficient should be 0 to
			1 -- 0 means no bounce back and 1 means perfect reflection.
		)pbdoc")
	.def_property("gravity", &ParticleSystemSolver2::GetGravity, &ParticleSystemSolver2::SetGravity,
		R"pbdoc(
			The gravity.
		)pbdoc")
	.def_property_readonly("particleSystemData", &ParticleSystemSolver2::GetParticleSystemData,
		R"pbdoc(
			The the particle system data.

			This property returns the particle system data. The data is created when
			this solver is constructed and also owned by the solver.
		)pbdoc")
	.def_property("collider", &ParticleSystemSolver2::GetCollider, &ParticleSystemSolver2::SetCollider,
		R"pbdoc(
			The collider.
		)pbdoc")
	.def_property("emitter", &ParticleSystemSolver2::GetEmitter, &ParticleSystemSolver2::SetEmitter,
		R"pbdoc(
			The emitter.
		)pbdoc")
	.def_property("wind", &ParticleSystemSolver2::GetWind, &ParticleSystemSolver2::SetWind,
		R"pbdoc(
			The wind.

			Wind can be applied to the particle system by setting a vector field to
			the solver.
		)pbdoc");
}

void AddParticleSystemSolver3(pybind11::module& m)
{
	pybind11::class_<ParticleSystemSolver3, ParticleSystemSolver3Ptr>(m, "ParticleSystemSolver3",
		R"pbdoc(
			Basic 3-D particle system solver.

			This class implements basic particle system solver. It includes gravity,
			air drag, and collision. But it does not compute particle-to-particle
			interaction. Thus, this solver is suitable for performing simple spray-like
			simulations with low computational cost. This class can be further extend
			to add more sophisticated simulations, such as SPH, to handle
			particle-to-particle intersection.
		)pbdoc")
	.def(pybind11::init<double, double>(),
		R"pbdoc(
			Constructs a solver with particle parameters.

			Parameters
			----------
			- radius : Radius of a particle in meters (default 1e-3).
			- mass : Mass of a particle in kg (default 1e-3).
		)pbdoc",
		pybind11::arg("radius") = 1e-3,
		pybind11::arg("mass") = 1e-3)
	.def_property("dragCoefficient", &ParticleSystemSolver3::GetDragCoefficient, &ParticleSystemSolver3::SetDragCoefficient,
		R"pbdoc(
			The drag coefficient.

			The drag coefficient controls the amount of air-drag. The coefficient
			should be a positive number and 0 means no drag force.
		)pbdoc")
	.def_property("restitutionCoefficient", &ParticleSystemSolver3::GetRestitutionCoefficient, &ParticleSystemSolver3::SetRestitutionCoefficient,
		R"pbdoc(
			The restitution coefficient.

			The restitution coefficient controls the bouncy-ness of a particle when
			it hits a collider surface. The range of the coefficient should be 0 to
			1 -- 0 means no bounce back and 1 means perfect reflection.
		)pbdoc")
	.def_property("gravity", &ParticleSystemSolver3::GetGravity, &ParticleSystemSolver3::SetGravity,
		R"pbdoc(
			The gravity.
		)pbdoc")
	.def_property_readonly("particleSystemData", &ParticleSystemSolver3::GetParticleSystemData,
		R"pbdoc(
			The the particle system data.

			This property returns the particle system data. The data is created when
			this solver is constructed and also owned by the solver.
		)pbdoc")
	.def_property("collider", &ParticleSystemSolver3::GetCollider, &ParticleSystemSolver3::SetCollider,
		R"pbdoc(
			The collider.
		)pbdoc")
	.def_property("emitter", &ParticleSystemSolver3::GetEmitter, &ParticleSystemSolver3::SetEmitter,
		R"pbdoc(
			The emitter.
		)pbdoc")
	.def_property("wind", &ParticleSystemSolver3::GetWind, &ParticleSystemSolver3::SetWind,
		R"pbdoc(
			The wind.

			Wind can be applied to the particle system by setting a vector field to
			the solver.
		)pbdoc");
}