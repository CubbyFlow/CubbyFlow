// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/PointsToImplicit/AnisotropicPointsToImplicit.hpp>
#include <Core/PointsToImplicit/AnisotropicPointsToImplicit2.hpp>
#include <Core/PointsToImplicit/AnisotropicPointsToImplicit3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddAnisotropicPointsToImplicit2(pybind11::module& m)
{
    pybind11::class_<AnisotropicPointsToImplicit2, PointsToImplicit2,
                     AnisotropicPointsToImplicit2Ptr>(
        m, "AnisotropicPointsToImplicit2",
        R"pbdoc(
			2-D points-to-implicit converter using Anisotropic kernels.

			\see Yu, Jihun, and Greg Turk. "Reconstructing surfaces of particle-based
			fluids using anisotropic kernels." ACM Transactions on Graphics (TOG)
			32.1 (2013): 5.
		)pbdoc")
        .def(pybind11::init<double, double, double, size_t, bool>(),
             R"pbdoc(
			Constructs the converter with given kernel radius and cut-off density.

			Parameters
			----------
			- kernelRadius : Smoothing kernel radius.
			- cutOffDensity : Iso-contour value.
			- positionSmoothingFactor : Position smoothing factor.
			- minNumNeighbors : Minimum number of neighbors to enable anisotropic kernel.
			- isOutputSDF : True if the output should be signed-distance field.
		)pbdoc",
             pybind11::arg("kernelRadius") = 1.0,
             pybind11::arg("cutOffDensity") = 0.5,
             pybind11::arg("positionSmoothingFactor") = 0.5,
             pybind11::arg("minNumNeighbors") = 8,
             pybind11::arg("isOutputSDF") = true);
}

void AddAnisotropicPointsToImplicit3(pybind11::module& m)
{
    pybind11::class_<AnisotropicPointsToImplicit3, PointsToImplicit3,
                     AnisotropicPointsToImplicit3Ptr>(
        m, "AnisotropicPointsToImplicit3",
        R"pbdoc(
			3-D points-to-implicit converter using Anisotropic kernels.

			\see Yu, Jihun, and Greg Turk. "Reconstructing surfaces of particle-based
			fluids using anisotropic kernels." ACM Transactions on Graphics (TOG)
			32.1 (2013): 5.
		)pbdoc")
        .def(pybind11::init<double, double, double, size_t, bool>(),
             R"pbdoc(
			Constructs the converter with given kernel radius and cut-off density.

			Parameters
			----------
			- kernelRadius : Smoothing kernel radius.
			- cutOffDensity : Iso-contour value.
			- positionSmoothingFactor : Position smoothing factor.
			- minNumNeighbors : Minimum number of neighbors to enable anisotropic kernel.
			- isOutputSDF : True if the output should be signed-distance field.
		)pbdoc",
             pybind11::arg("kernelRadius") = 1.0,
             pybind11::arg("cutOffDensity") = 0.5,
             pybind11::arg("positionSmoothingFactor") = 0.5,
             pybind11::arg("minNumNeighbors") = 25,
             pybind11::arg("isOutputSDF") = true);
}