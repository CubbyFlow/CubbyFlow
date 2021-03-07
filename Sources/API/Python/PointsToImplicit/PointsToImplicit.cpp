// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/PointsToImplicit/PointsToImplicit.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/PointsToImplicit/PointsToImplicit2.hpp>
#include <Core/PointsToImplicit/PointsToImplicit3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddPointsToImplicit2(pybind11::module& m)
{
    pybind11::class_<PointsToImplicit2, PointsToImplicit2Ptr>(
        m, "PointsToImplicit2",
        R"pbdoc(
			Abstract base class for 2-D points-to-implicit converters.
		)pbdoc")
        .def(
            "Convert",
            [](const PointsToImplicit2& instance, const pybind11::list& points,
               ScalarGrid2Ptr output) {
                std::vector<Vector2D> points_;
                for (size_t i = 0; i < points.size(); ++i)
                {
                    points_.push_back(ObjectToVector2D(points[i]));
                }

                ConstArrayView1<Vector2D> pointsAcc(points_.data(),
                                                    points_.size());
                instance.Convert(pointsAcc, output.get());
            },
            R"pbdoc(
			Converts the given points to implicit surface scalar field.

			Parameters
			----------
			- points : List of 2D vectors.
			- output : Scalar grid output.
		)pbdoc",
            pybind11::arg("points"), pybind11::arg("output"));
}

void AddPointsToImplicit3(pybind11::module& m)
{
    pybind11::class_<PointsToImplicit3, PointsToImplicit3Ptr>(
        m, "PointsToImplicit3",
        R"pbdoc(
			Abstract base class for 3-D points-to-implicit converters.
		)pbdoc")
        .def(
            "Convert",
            [](const PointsToImplicit3& instance, const pybind11::list& points,
               ScalarGrid3Ptr output) {
                std::vector<Vector3D> points_;
                for (size_t i = 0; i < points.size(); ++i)
                {
                    points_.push_back(ObjectToVector3D(points[i]));
                }

                ConstArrayView1<Vector3D> pointsAcc(points_.data(),
                                                    points_.size());
                instance.Convert(pointsAcc, output.get());
            },
            R"pbdoc(
			Converts the given points to implicit surface scalar field.

			Parameters
			----------
			- points : List of 3D vectors.
			- output : Scalar grid output.
		)pbdoc",
            pybind11::arg("points"), pybind11::arg("output"));
}