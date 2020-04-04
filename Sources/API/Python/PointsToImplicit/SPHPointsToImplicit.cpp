// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/PointsToImplicit/SPHPointsToImplicit.hpp>
#include <Core/PointsToImplicit/SPHPointsToImplicit2.hpp>
#include <Core/PointsToImplicit/SPHPointsToImplicit3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddSPHPointsToImplicit2(pybind11::module& m)
{
    pybind11::class_<SPHPointsToImplicit2, PointsToImplicit2,
                     SPHPointsToImplicit2Ptr>(m, "SPHPointsToImplicit2",
                                              R"pbdoc(
			2-D points-to-implicit converter based on standard SPH kernel.

			\see M\"uller, Matthias, David Charypar, and Markus Gross.
			"Particle-based fluid simulation for interactive applications."
			Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
			animation. Eurographics Association, 2003.
		)pbdoc")
        .def(pybind11::init<double, double, bool>(),
             R"pbdoc(
			Constructs the converter with given kernel radius and cut-off density.

			Parameters
			----------
			- kernelRadius : SPH kernel radius.
			- cutOffDensity : Iso-contour value.
			- isOutputSDF : True if the output should be signed-distance field.
		)pbdoc",
             pybind11::arg("kernelRadius") = 1.0,
             pybind11::arg("cutOffDensity") = 0.5,
             pybind11::arg("isOutputSDF") = true);
}

void AddSPHPointsToImplicit3(pybind11::module& m)
{
    pybind11::class_<SPHPointsToImplicit3, PointsToImplicit3,
                     SPHPointsToImplicit3Ptr>(m, "SPHPointsToImplicit3",
                                              R"pbdoc(
			3-D points-to-implicit converter based on standard SPH kernel.

			\see M\"uller, Matthias, David Charypar, and Markus Gross.
			"Particle-based fluid simulation for interactive applications."
			Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
			animation. Eurographics Association, 2003.
		)pbdoc")
        .def(pybind11::init<double, double, bool>(),
             R"pbdoc(
			Constructs the converter with given kernel radius and cut-off density.

			Parameters
			----------
			- kernelRadius : SPH kernel radius.
			- cutOffDensity : Iso-contour value.
			- isOutputSdf : True if the output should be signed-distance field.
		)pbdoc",
             pybind11::arg("kernelRadius") = 1.0,
             pybind11::arg("cutOffDensity") = 0.5,
             pybind11::arg("isOutputSDF") = true);
}