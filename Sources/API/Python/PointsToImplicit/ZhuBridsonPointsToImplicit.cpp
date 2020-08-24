// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/PointsToImplicit/ZhuBridsonPointsToImplicit.hpp>
#include <Core/PointsToImplicit/ZhuBridsonPointsToImplicit2.hpp>
#include <Core/PointsToImplicit/ZhuBridsonPointsToImplicit3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddZhuBridsonPointsToImplicit2(pybind11::module& m)
{
    pybind11::class_<ZhuBridsonPointsToImplicit2, PointsToImplicit2,
                     ZhuBridsonPointsToImplicit2Ptr>(
        m, "ZhuBridsonPointsToImplicit2",
        R"pbdoc(
			2-D points-to-implicit converter based on Zhu and Bridson's method.

			\see Zhu, Yongning, and Robert Bridson. "Animating sand as a fluid."
			ACM Transactions on Graphics (TOG). Vol. 24. No. 3. ACM, 2005.
		)pbdoc")
        .def(pybind11::init<double, double, bool>(),
             R"pbdoc(
			Constructs the converter with given kernel radius and cut-off threshold.

			Parameters
			----------
			- kernelRadius : Smoothing kernel radius.
			- cutOffThreshold : Iso-contour value.
			- isOutputSDF : True if the output should be signed-distance field.
		)pbdoc",
             pybind11::arg("kernelRadius") = 1.0,
             pybind11::arg("cutOffThreshold") = 0.25,
             pybind11::arg("isOutputSDF") = true);
}

void AddZhuBridsonPointsToImplicit3(pybind11::module& m)
{
    pybind11::class_<ZhuBridsonPointsToImplicit3, PointsToImplicit3,
                     ZhuBridsonPointsToImplicit3Ptr>(
        m, "ZhuBridsonPointsToImplicit3",
        R"pbdoc(
			3-D points-to-implicit converter based on Zhu and Bridson's method.

			\see Zhu, Yongning, and Robert Bridson. "Animating sand as a fluid."
			ACM Transactions on Graphics (TOG). Vol. 24. No. 3. ACM, 2005.
		)pbdoc")
        .def(pybind11::init<double, double, bool>(),
             R"pbdoc(
			Constructs the converter with given kernel radius and cut-off threshold.

			Parameters
			----------
			- kernelRadius : Smoothing kernel radius.
			- cutOffThreshold : Iso-contour value.
			- isOutputSDF : True if the output should be signed-distance field.
		)pbdoc",
             pybind11::arg("kernelRadius") = 1.0,
             pybind11::arg("cutOffThreshold") = 0.25,
             pybind11::arg("isOutputSDF") = true);
}