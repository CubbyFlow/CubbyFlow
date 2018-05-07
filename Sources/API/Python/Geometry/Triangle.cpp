/*************************************************************************
> File Name: Triangle.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Triangle functions for CubbyFlow Python API.
> Created Time: 2018/01/28
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Geometry/Triangle.h>
#include <API/Python/Utils/pybind11Utils.h>
#include <Core/Geometry/Triangle3.h>

#include <pybind11/pybind11.h>

#include <array>

using namespace CubbyFlow;

void AddTriangle3(pybind11::module& m)
{
	pybind11::class_<Triangle3, Triangle3Ptr, Surface3>(static_cast<pybind11::handle>(m), "Triangle3",
		R"pbdoc(
			3-D triangle geometry.

			This class represents 3-D triangle geometry which extends Surface3 by
			overriding surface-related queries.
		)pbdoc")
	.def("__init__", [](Triangle3& instance, pybind11::list points, pybind11::list normals, pybind11::list uvs, const Transform3& transform, bool isNormalFlipped)
	{
		if (points.size() != 3)
		{
			throw std::invalid_argument("Wrong size of point list.");
		}
		if (normals.size() != 3)
		{
			throw std::invalid_argument("Wrong size of normal list.");
		}
		if (uvs.size() != 3)
		{
			throw std::invalid_argument("Wrong size of UV list.");
		}

		std::array<Vector3D, 3> points_;
		std::array<Vector3D, 3> normals_;
		std::array<Vector2D, 3> uvs_;

		for (size_t i = 0; i < 3; ++i)
		{
			points_[i] = ObjectToVector3D(points);
			normals_[i] = ObjectToVector3D(normals);
			uvs_[i] = ObjectToVector2D(uvs);
		}

		new (&instance) Triangle3(points_, normals_, uvs_, transform, isNormalFlipped);
	},
		R"pbdoc(
			Constructs a triangle with given `points`, `normals`, and `uvs`.
		)pbdoc",
		pybind11::arg("points"),
		pybind11::arg("normals"),
		pybind11::arg("uvs"),
		pybind11::arg("transform") = Transform3(),
		pybind11::arg("isNormalFlipped") = false)
	.def_readwrite("points", &Triangle3::points)
	.def_readwrite("normals", &Triangle3::normals)
	.def_readwrite("uvs", &Triangle3::uvs);
}