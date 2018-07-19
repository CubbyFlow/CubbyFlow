/*************************************************************************
> File Name: SPHPointsToImplicit.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: SPHPointsToImplicit functions for CubbyFlow Python API.
> Created Time: 2018/02/10
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Python/PointsToImplicit/SPHPointsToImplicit.h>
#include <PointsToImplicit/SPHPointsToImplicit2.h>
#include <PointsToImplicit/SPHPointsToImplicit3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddSPHPointsToImplicit2(pybind11::module& m)
{
	pybind11::class_<SPHPointsToImplicit2, PointsToImplicit2, SPHPointsToImplicit2Ptr>(m, "SPHPointsToImplicit2",
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
	pybind11::class_<SPHPointsToImplicit3, PointsToImplicit3, SPHPointsToImplicit3Ptr>(m, "SPHPointsToImplicit3",
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