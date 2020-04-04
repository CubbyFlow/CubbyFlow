// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Solver/Grid/GridFractionalSinglePhasePressureSolver.hpp>
#include <Core/Solver/Grid/GridFractionalSinglePhasePressureSolver2.hpp>
#include <Core/Solver/Grid/GridFractionalSinglePhasePressureSolver3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddGridFractionalSinglePhasePressureSolver2(pybind11::module& m)
{
    pybind11::class_<GridFractionalSinglePhasePressureSolver2,
                     GridFractionalSinglePhasePressureSolver2Ptr,
                     GridPressureSolver2>(
        static_cast<pybind11::handle>(m),
        "GridFractionalSinglePhasePressureSolver2",
        R"pbdoc(
			2-D fractional single-phase pressure solver.

			This class implements 2-D fractional (or variational) single-phase pressure
			solver. It is called fractional because the solver encodes the boundaries
			to the grid cells like anti-aliased pixels, meaning that a grid cell will
			record the partially overlapping boundary as a fractional number.
			Alternative approach is to represent boundaries like Lego blocks which is
			the case for GridSinglePhasePressureSolver2.
			In addition, this class solves single-phase flow, solving the pressure for
			selected fluid region only and treat other area as an atmosphere region.
			Thus, the pressure outside the fluid will be set to a constant value and
			velocity field won't be altered. This solver also computes the fluid
			boundary in fractional manner, meaning that the solver tries to capture the
			subgrid structures. This class uses ghost fluid method for such calculation.
			\see Batty, Christopher, Florence Bertails, and Robert Bridson.
			"A fast variational framework for accurate solid-fluid coupling."
			ACM Transactions on Graphics (TOG). Vol. 26. No. 2. ACM, 2007.
			\see Enright, Doug, et al. "Using the particle level set method and
			a second order accurate pressure boundary condition for free surface
			flows." ASME/JSME 2002 4th Joint Fluids Summer Engineering Conference.
			American Society of Mechanical Engineers, 2002.
		)pbdoc")
        .def(pybind11::init<>())
        .def_property(
            "linearSystemSolver",
            &GridFractionalSinglePhasePressureSolver2::GetLinearSystemSolver,
            &GridFractionalSinglePhasePressureSolver2::SetLinearSystemSolver,
            R"pbdoc(
			"The linear system solver."
		)pbdoc");
}

void AddGridFractionalSinglePhasePressureSolver3(pybind11::module& m)
{
    pybind11::class_<GridFractionalSinglePhasePressureSolver3,
                     GridFractionalSinglePhasePressureSolver3Ptr,
                     GridPressureSolver3>(
        static_cast<pybind11::handle>(m),
        "GridFractionalSinglePhasePressureSolver3",
        R"pbdoc(
			3-D fractional single-phase pressure solver.

			This class implements 3-D fractional (or variational) single-phase pressure
			solver. It is called fractional because the solver encodes the boundaries
			to the grid cells like anti-aliased pixels, meaning that a grid cell will
			record the partially overlapping boundary as a fractional number.
			Alternative approach is to represent boundaries like Lego blocks which is
			the case for GridSinglePhasePressureSolver2.
			In addition, this class solves single-phase flow, solving the pressure for
			selected fluid region only and treat other area as an atmosphere region.
			Thus, the pressure outside the fluid will be set to a constant value and
			velocity field won't be altered. This solver also computes the fluid
			boundary in fractional manner, meaning that the solver tries to capture the
			subgrid structures. This class uses ghost fluid method for such calculation.
			\see Batty, Christopher, Florence Bertails, and Robert Bridson.
			"A fast variational framework for accurate solid-fluid coupling."
			ACM Transactions on Graphics (TOG). Vol. 26. No. 3. ACM, 2007.
			\see Enright, Doug, et al. "Using the particle level set method and
			a second order accurate pressure boundary condition for free surface
			flows." ASME/JSME 2003 4th Joint Fluids Summer Engineering Conference.
			American Society of Mechanical Engineers, 2003.
		)pbdoc")
        .def(pybind11::init<>())
        .def_property(
            "linearSystemSolver",
            &GridFractionalSinglePhasePressureSolver3::GetLinearSystemSolver,
            &GridFractionalSinglePhasePressureSolver3::SetLinearSystemSolver,
            R"pbdoc(
			"The linear system solver."
		)pbdoc");
}