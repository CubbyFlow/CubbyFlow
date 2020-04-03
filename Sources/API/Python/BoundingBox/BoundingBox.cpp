// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/BoundingBox/BoundingBox.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/BoundingBox/BoundingBox2.hpp>
#include <Core/BoundingBox/BoundingBox3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddBoundingBox2F(pybind11::module& m)
{
    pybind11::class_<BoundingBoxRayIntersection2F>(
        static_cast<pybind11::handle>(m), "BoundingBoxRayIntersection2F",
        R"pbdoc(
			2-D box-ray intersection result (32-bit float).
		)pbdoc")
        .def_readwrite("isIntersecting",
                       &BoundingBoxRayIntersection2F::isIntersecting,
                       R"pbdoc(
			True if the box and ray intersects.
		)pbdoc")
        .def_readwrite("near", &BoundingBoxRayIntersection2F::near,
                       R"pbdoc(
			Distance to the first intersection point.
		)pbdoc")
        .def_readwrite("far", &BoundingBoxRayIntersection2F::far,
                       R"pbdoc(
			Distance to the second (and the last) intersection point.
		)pbdoc");

    pybind11::class_<BoundingBox2F>(static_cast<pybind11::handle>(m),
                                    "BoundingBox2F",
                                    R"pbdoc(
			2-D axis-aligned bounding box class (32-bit float).
		)pbdoc")
        .def(
            "__init__",
            [](BoundingBox2F& instance, pybind11::args args) {
                if (args.size() == 2)
                {
                    new (&instance) BoundingBox2F(ObjectToVector2F(args[0]),
                                                  ObjectToVector2F(args[1]));
                }
                else if (args.size() == 0)
                {
                    new (&instance) BoundingBox2F();
                }
            },
            R"pbdoc(
			Constructs BoundingBox2F

			This method constructs 2-D bounding box with lower and upper
			corners with 32-bit precision.

			Parameters
			----------
			- `*args` : Lower and upper corner of the bounding box. If empty,
						empty box will be created. Must be size of 0 or 2.
		)pbdoc")
        .def_readwrite("lowerCorner", &BoundingBox2F::lowerCorner,
                       R"pbdoc(
			Lower corner of the bounding box.
		)pbdoc")
        .def_readwrite("upperCorner", &BoundingBox2F::upperCorner,
                       R"pbdoc(
			Upper corner of the bounding box.
		)pbdoc")
        .def_property_readonly("width", &BoundingBox2F::GetWidth,
                               R"pbdoc(
			Width of the box.
		)pbdoc")
        .def_property_readonly("height", &BoundingBox2F::GetHeight,
                               R"pbdoc(
			Height of the box.
		)pbdoc")
        .def("Length", &BoundingBox2F::Length,
             R"pbdoc(
			Returns length of the box in given axis.

			Parameters
			----------
			- axis : 0 or 1.
		)pbdoc",
             pybind11::arg("axis"))
        .def("Overlaps", &BoundingBox2F::Overlaps,
             R"pbdoc(
			Returns true of this box and other box overlaps.

			Parameters
			----------
			- other : Other bounding box to test with.
		)pbdoc",
             pybind11::arg("other"))
        .def(
            "Contains",
            [](const BoundingBox2F& instance, pybind11::object point) {
                return instance.Contains(ObjectToVector2F(point));
            },
            R"pbdoc(
			Returns true if the input vector is inside of this box.

			Parameters
			----------
			- point : Point to test.
		)pbdoc",
            pybind11::arg("point"))
        .def("Intersects", &BoundingBox2F::Intersects,
             R"pbdoc(
			Returns true if the input ray is intersecting with this box.

			Parameters
			----------
			- ray : Ray to test.
		)pbdoc",
             pybind11::arg("ray"))
        .def("ClosestIntersection", &BoundingBox2F::ClosestIntersection,
             R"pbdoc(
			Returns closest intersection for given ray.

			Returns intersection.isIntersecting = true if the input ray is
			intersecting with this box. If intersects, intersection.near is
			assigned with distant to the closest intersecting point, and
			intersection.far with furthest.

			Parameters
			----------
			- ray : Ray to test.
		)pbdoc",
             pybind11::arg("ray"))
        .def("MidPoint", &BoundingBox2F::MidPoint,
             R"pbdoc(
			Returns the mid-point of this box.
		)pbdoc")
        .def("DiagonalLength", &BoundingBox2F::DiagonalLength,
             R"pbdoc(
			Returns diagonal length of this box.
		)pbdoc")
        .def("DiagonalLengthSquared", &BoundingBox2F::DiagonalLengthSquared,
             R"pbdoc(
			Returns squared diagonal length of this box.
		)pbdoc")
        .def("Reset", &BoundingBox2F::Reset,
             R"pbdoc(
			Resets this box to initial state (min = infinite, max = -infinite).
		)pbdoc")
        .def(
            "Merge",
            [](BoundingBox2F& instance, pybind11::object other) {
                if (pybind11::isinstance<pybind11::tuple>(
                        static_cast<pybind11::handle>(other)) ||
                    pybind11::isinstance<Vector2F>(
                        static_cast<pybind11::handle>(other)))
                {
                    instance.Merge(ObjectToVector2F(other));
                }
                else if (pybind11::isinstance<BoundingBox2F>(
                             static_cast<pybind11::handle>(other)))
                {
                    instance.Merge(other.cast<BoundingBox2F>());
                }
                else
                {
                    throw std::invalid_argument(
                        "Incompatible argument type for merge.");
                }
            },
            R"pbdoc(
			Merges this and other point or box.

			Parameters
			----------
			- other : Other point or bounding box to test with.
		)pbdoc",
            pybind11::arg("other"))
        .def("Expand", &BoundingBox2F::Expand,
             R"pbdoc(
			Expands this box by given delta to all direction.

			If the width of the box was x, expand(y) will result a box with
			x + y + y width.

			Parameters
			----------
			- delta : Amount to expand.
		)pbdoc",
             pybind11::arg("delta"))
        .def("Corner", &BoundingBox2F::Corner,
             R"pbdoc(
			Returns corner position. Index starts from x-first order.

			Parameters
			----------
			- idx : Index of the corner.
		)pbdoc",
             pybind11::arg("idx"))
        .def("Clamp", &BoundingBox2F::Clamp,
             R"pbdoc(
			Returns the clamped point.

			Parameters
			----------
			- point : Point to clamp.
		)pbdoc",
             pybind11::arg("point"))
        .def("IsEmpty", &BoundingBox2F::IsEmpty,
             R"pbdoc(
			Returns true if the box is empty.
		)pbdoc");
}

void AddBoundingBox2D(pybind11::module& m)
{
    pybind11::class_<BoundingBoxRayIntersection2D>(
        static_cast<pybind11::handle>(m), "BoundingBoxRayIntersection2D",
        R"pbdoc(
			2-D box-ray intersection result (64-bit float).
		)pbdoc")
        .def_readwrite("isIntersecting",
                       &BoundingBoxRayIntersection2D::isIntersecting,
                       R"pbdoc(
			True if the box and ray intersects.
		)pbdoc")
        .def_readwrite("near", &BoundingBoxRayIntersection2D::near,
                       R"pbdoc(
			Distance to the first intersection point.
		)pbdoc")
        .def_readwrite("far", &BoundingBoxRayIntersection2D::far,
                       R"pbdoc(
			Distance to the second (and the last) intersection point.
		)pbdoc");

    pybind11::class_<BoundingBox2D>(static_cast<pybind11::handle>(m),
                                    "BoundingBox2D",
                                    R"pbdoc(
			2-D axis-aligned bounding box class (64-bit float).
		)pbdoc")
        .def(
            "__init__",
            [](BoundingBox2D& instance, pybind11::args args) {
                if (args.size() == 2)
                {
                    new (&instance) BoundingBox2D(ObjectToVector2D(args[0]),
                                                  ObjectToVector2D(args[1]));
                }
                else if (args.size() == 0)
                {
                    new (&instance) BoundingBox2D();
                }
            },
            R"pbdoc(
			Constructs BoundingBox2D

			This method constructs 2-D bounding box with lower and upper
			corners with 64-bit precision.

			Parameters
			----------
			- `*args` : Lower and upper corner of the bounding box. If empty,
						empty box will be created. Must be size of 0 or 2.
		)pbdoc")
        .def_readwrite("lowerCorner", &BoundingBox2D::lowerCorner,
                       R"pbdoc(
			Lower corner of the bounding box.
		)pbdoc")
        .def_readwrite("upperCorner", &BoundingBox2D::upperCorner,
                       R"pbdoc(
			Upper corner of the bounding box.
		)pbdoc")
        .def_property_readonly("width", &BoundingBox2D::GetWidth,
                               R"pbdoc(
			Width of the box.
		)pbdoc")
        .def_property_readonly("height", &BoundingBox2D::GetHeight,
                               R"pbdoc(
			Height of the box.
		)pbdoc")
        .def("Length", &BoundingBox2D::Length,
             R"pbdoc(
			Returns length of the box in given axis.

			Parameters
			----------
			- axis : 0 or 1.
		)pbdoc",
             pybind11::arg("axis"))
        .def("Overlaps", &BoundingBox2D::Overlaps,
             R"pbdoc(
			Returns true of this box and other box overlaps.

			Parameters
			----------
			- other : Other bounding box to test with.
		)pbdoc",
             pybind11::arg("other"))
        .def(
            "Contains",
            [](const BoundingBox2D& instance, pybind11::object point) {
                return instance.Contains(ObjectToVector2D(point));
            },
            R"pbdoc(
			Returns true if the input vector is inside of this box.

			Parameters
			----------
			- point : Point to test.
		)pbdoc",
            pybind11::arg("point"))
        .def("Intersects", &BoundingBox2D::Intersects,
             R"pbdoc(
			Returns true if the input ray is intersecting with this box.

			Parameters
			----------
			- ray : Ray to test.
		)pbdoc",
             pybind11::arg("ray"))
        .def("ClosestIntersection", &BoundingBox2D::ClosestIntersection,
             R"pbdoc(
			Returns closest intersection for given ray.

			Returns intersection.isIntersecting = true if the input ray is
			intersecting with this box. If intersects, intersection.near is
			assigned with distant to the closest intersecting point, and
			intersection.far with furthest.

			Parameters
			----------
			- ray : Ray to test.
		)pbdoc",
             pybind11::arg("ray"))
        .def("MidPoint", &BoundingBox2D::MidPoint,
             R"pbdoc(
			Returns the mid-point of this box.
		)pbdoc")
        .def("DiagonalLength", &BoundingBox2D::DiagonalLength,
             R"pbdoc(
			Returns diagonal length of this box.
		)pbdoc")
        .def("DiagonalLengthSquared", &BoundingBox2D::DiagonalLengthSquared,
             R"pbdoc(
			Returns squared diagonal length of this box.
		)pbdoc")
        .def("Reset", &BoundingBox2D::Reset,
             R"pbdoc(
			Resets this box to initial state (min = infinite, max = -infinite).
		)pbdoc")
        .def(
            "Merge",
            [](BoundingBox2D& instance, pybind11::object other) {
                if (pybind11::isinstance<pybind11::tuple>(
                        static_cast<pybind11::handle>(other)) ||
                    pybind11::isinstance<Vector2D>(
                        static_cast<pybind11::handle>(other)))
                {
                    instance.Merge(ObjectToVector2D(other));
                }
                else if (pybind11::isinstance<BoundingBox2D>(
                             static_cast<pybind11::handle>(other)))
                {
                    instance.Merge(other.cast<BoundingBox2D>());
                }
                else
                {
                    throw std::invalid_argument(
                        "Incompatible argument type for merge.");
                }
            },
            R"pbdoc(
			Merges this and other point or box.

			Parameters
			----------
			- other : Other point or bounding box to test with.
		)pbdoc",
            pybind11::arg("other"))
        .def("Expand", &BoundingBox2D::Expand,
             R"pbdoc(
			Expands this box by given delta to all direction.

			If the width of the box was x, expand(y) will result a box with
			x + y + y width.

			Parameters
			----------
			- delta : Amount to expand.
		)pbdoc",
             pybind11::arg("delta"))
        .def("Corner", &BoundingBox2D::Corner,
             R"pbdoc(
			Returns corner position. Index starts from x-first order.

			Parameters
			----------
			- idx : Index of the corner.
		)pbdoc",
             pybind11::arg("idx"))
        .def("Clamp", &BoundingBox2D::Clamp,
             R"pbdoc(
			Returns the clamped point.

			Parameters
			----------
			- point : Point to clamp.
		)pbdoc",
             pybind11::arg("point"))
        .def("IsEmpty", &BoundingBox2D::IsEmpty,
             R"pbdoc(
			Returns true if the box is empty.
		)pbdoc");
}

void AddBoundingBox3F(pybind11::module& m)
{
    pybind11::class_<BoundingBoxRayIntersection3F>(
        static_cast<pybind11::handle>(m), "BoundingBoxRayIntersection3F",
        R"pbdoc(
			3-D box-ray intersection result (32-bit float).
		)pbdoc")
        .def_readwrite("isIntersecting",
                       &BoundingBoxRayIntersection3F::isIntersecting,
                       R"pbdoc(
			True if the box and ray intersects.
		)pbdoc")
        .def_readwrite("near", &BoundingBoxRayIntersection3F::near,
                       R"pbdoc(
			Distance to the first intersection point.
		)pbdoc")
        .def_readwrite("far", &BoundingBoxRayIntersection3F::far,
                       R"pbdoc(
			Distance to the second (and the last) intersection point.
		)pbdoc");

    pybind11::class_<BoundingBox3F>(static_cast<pybind11::handle>(m),
                                    "BoundingBox3F",
                                    R"pbdoc(
			3-D axis-aligned bounding box class (32-bit float).
		)pbdoc")
        .def(
            "__init__",
            [](BoundingBox3F& instance, pybind11::args args) {
                if (args.size() == 2)
                {
                    new (&instance) BoundingBox3F(ObjectToVector3F(args[0]),
                                                  ObjectToVector3F(args[1]));
                }
                else if (args.size() == 0)
                {
                    new (&instance) BoundingBox3F();
                }
            },
            R"pbdoc(
			Constructs BoundingBox3F

			This method constructs 3-D bounding box with lower and upper
			corners with 32-bit precision.

			Parameters
			----------
			- `*args` : Lower and upper corner of the bounding box. If empty,
						empty box will be created. Must be size of 0 or 2.
		)pbdoc")
        .def_readwrite("lowerCorner", &BoundingBox3F::lowerCorner,
                       R"pbdoc(
			Lower corner of the bounding box.
		)pbdoc")
        .def_readwrite("upperCorner", &BoundingBox3F::upperCorner,
                       R"pbdoc(
			Upper corner of the bounding box.
		)pbdoc")
        .def_property_readonly("width", &BoundingBox3F::GetWidth,
                               R"pbdoc(
			Width of the box.
		)pbdoc")
        .def_property_readonly("height", &BoundingBox3F::GetHeight,
                               R"pbdoc(
			Height of the box.
		)pbdoc")
        .def_property_readonly("depth", &BoundingBox3F::GetDepth,
                               R"pbdoc(
			Depth of the box.
		)pbdoc")
        .def("Length", &BoundingBox3F::Length,
             R"pbdoc(
			Returns length of the box in given axis.

			Parameters
			----------
			- axis : 0, 1, or 2.
		)pbdoc",
             pybind11::arg("axis"))
        .def("Overlaps", &BoundingBox3F::Overlaps,
             R"pbdoc(
			Returns true of this box and other box overlaps.

			Parameters
			----------
			- other : Other bounding box to test with.
		)pbdoc",
             pybind11::arg("other"))
        .def(
            "Contains",
            [](const BoundingBox3F& instance, pybind11::object point) {
                return instance.Contains(ObjectToVector3F(point));
            },
            R"pbdoc(
			Returns true if the input vector is inside of this box.

			Parameters
			----------
			- point : Point to test.
		)pbdoc",
            pybind11::arg("point"))
        .def("Intersects", &BoundingBox3F::Intersects,
             R"pbdoc(
			Returns true if the input ray is intersecting with this box.

			Parameters
			----------
			- ray : Ray to test.
		)pbdoc",
             pybind11::arg("ray"))
        .def("ClosestIntersection", &BoundingBox3F::ClosestIntersection,
             R"pbdoc(
			Returns closest intersection for given ray.

			Returns intersection.isIntersecting = true if the input ray is
			intersecting with this box. If intersects, intersection.near is
			assigned with distant to the closest intersecting point, and
			intersection.far with furthest.

			Parameters
			----------
			- ray : Ray to test.
		)pbdoc",
             pybind11::arg("ray"))
        .def("MidPoint", &BoundingBox3F::MidPoint,
             R"pbdoc(
			Returns the mid-point of this box.
		)pbdoc")
        .def("DiagonalLength", &BoundingBox3F::DiagonalLength,
             R"pbdoc(
			Returns diagonal length of this box.
		)pbdoc")
        .def("DiagonalLengthSquared", &BoundingBox3F::DiagonalLengthSquared,
             R"pbdoc(
			Returns squared diagonal length of this box.
		)pbdoc")
        .def("Reset", &BoundingBox3F::Reset,
             R"pbdoc(
			Resets this box to initial state (min = infinite, max = -infinite).
		)pbdoc")
        .def(
            "Merge",
            [](BoundingBox3F& instance, pybind11::object other) {
                if (pybind11::isinstance<pybind11::tuple>(
                        static_cast<pybind11::handle>(other)) ||
                    pybind11::isinstance<Vector3F>(
                        static_cast<pybind11::handle>(other)))
                {
                    instance.Merge(ObjectToVector3F(other));
                }
                else if (pybind11::isinstance<BoundingBox3F>(
                             static_cast<pybind11::handle>(other)))
                {
                    instance.Merge(other.cast<BoundingBox3F>());
                }
                else
                {
                    throw std::invalid_argument(
                        "Incompatible argument type for merge.");
                }
            },
            R"pbdoc(
			Merges this and other point or box.

			Parameters
			----------
			- other : Other point or bounding box to test with.
		)pbdoc",
            pybind11::arg("other"))
        .def("Expand", &BoundingBox3F::Expand,
             R"pbdoc(
			Expands this box by given delta to all direction.

			If the width of the box was x, expand(y) will result a box with
			x + y + y width.

			Parameters
			----------
			- delta : Amount to expand.
		)pbdoc",
             pybind11::arg("delta"))
        .def("Corner", &BoundingBox3F::Corner,
             R"pbdoc(
			Returns corner position. Index starts from x-first order.

			Parameters
			----------
			- idx : Index of the corner.
		)pbdoc",
             pybind11::arg("idx"))
        .def("Clamp", &BoundingBox3F::Clamp,
             R"pbdoc(
			Returns the clamped point.

			Parameters
			----------
			- point : Point to clamp.
		)pbdoc",
             pybind11::arg("point"))
        .def("IsEmpty", &BoundingBox3F::IsEmpty,
             R"pbdoc(
			Returns true if the box is empty.
		)pbdoc");
}

void AddBoundingBox3D(pybind11::module& m)
{
    pybind11::class_<BoundingBoxRayIntersection3D>(
        static_cast<pybind11::handle>(m), "BoundingBoxRayIntersection3D",
        R"pbdoc(
			3-D box-ray intersection result (64-bit float).
		)pbdoc")
        .def_readwrite("isIntersecting",
                       &BoundingBoxRayIntersection3D::isIntersecting,
                       R"pbdoc(
			True if the box and ray intersects.
		)pbdoc")
        .def_readwrite("near", &BoundingBoxRayIntersection3D::near,
                       R"pbdoc(
			Distance to the first intersection point.
		)pbdoc")
        .def_readwrite("far", &BoundingBoxRayIntersection3D::far,
                       R"pbdoc(
			Distance to the second (and the last) intersection point.
		)pbdoc");

    pybind11::class_<BoundingBox3D>(static_cast<pybind11::handle>(m),
                                    "BoundingBox3D",
                                    R"pbdoc(
			3-D axis-aligned bounding box class (64-bit float).
		)pbdoc")
        .def(
            "__init__",
            [](BoundingBox3D& instance, pybind11::args args) {
                if (args.size() == 2)
                {
                    new (&instance) BoundingBox3D(ObjectToVector3D(args[0]),
                                                  ObjectToVector3D(args[1]));
                }
                else if (args.size() == 0)
                {
                    new (&instance) BoundingBox3D();
                }
            },
            R"pbdoc(
			Constructs BoundingBox3D

			This method constructs 3-D bounding box with lower and upper
			corners with 64-bit precision.

			Parameters
			----------
			- `*args` : Lower and upper corner of the bounding box. If empty,
						empty box will be created. Must be size of 0 or 2.
		)pbdoc")
        .def_readwrite("lowerCorner", &BoundingBox3D::lowerCorner,
                       R"pbdoc(
			Lower corner of the bounding box.
		)pbdoc")
        .def_readwrite("upperCorner", &BoundingBox3D::upperCorner,
                       R"pbdoc(
			Upper corner of the bounding box.
		)pbdoc")
        .def_property_readonly("width", &BoundingBox3D::GetWidth,
                               R"pbdoc(
			Width of the box.
		)pbdoc")
        .def_property_readonly("height", &BoundingBox3D::GetHeight,
                               R"pbdoc(
			Height of the box.
		)pbdoc")
        .def_property_readonly("depth", &BoundingBox3D::GetDepth,
                               R"pbdoc(
			Depth of the box.
		)pbdoc")
        .def("Length", &BoundingBox3D::Length,
             R"pbdoc(
			Returns length of the box in given axis.

			Parameters
			----------
			- axis : 0, 1, or 2.
		)pbdoc",
             pybind11::arg("axis"))
        .def("Overlaps", &BoundingBox3D::Overlaps,
             R"pbdoc(
			Returns true of this box and other box overlaps.

			Parameters
			----------
			- other : Other bounding box to test with.
		)pbdoc",
             pybind11::arg("other"))
        .def(
            "Contains",
            [](const BoundingBox3D& instance, pybind11::object point) {
                return instance.Contains(ObjectToVector3D(point));
            },
            R"pbdoc(
			Returns true if the input vector is inside of this box.

			Parameters
			----------
			- point : Point to test.
		)pbdoc",
            pybind11::arg("point"))
        .def("Intersects", &BoundingBox3D::Intersects,
             R"pbdoc(
			Returns true if the input ray is intersecting with this box.

			Parameters
			----------
			- ray : Ray to test.
		)pbdoc",
             pybind11::arg("ray"))
        .def("ClosestIntersection", &BoundingBox3D::ClosestIntersection,
             R"pbdoc(
			Returns closest intersection for given ray.

			Returns intersection.isIntersecting = true if the input ray is
			intersecting with this box. If intersects, intersection.near is
			assigned with distant to the closest intersecting point, and
			intersection.far with furthest.

			Parameters
			----------
			- ray : Ray to test.
		)pbdoc",
             pybind11::arg("ray"))
        .def("MidPoint", &BoundingBox3D::MidPoint,
             R"pbdoc(
			Returns the mid-point of this box.
		)pbdoc")
        .def("DiagonalLength", &BoundingBox3D::DiagonalLength,
             R"pbdoc(
			Returns diagonal length of this box.
		)pbdoc")
        .def("DiagonalLengthSquared", &BoundingBox3D::DiagonalLengthSquared,
             R"pbdoc(
			Returns squared diagonal length of this box.
		)pbdoc")
        .def("Reset", &BoundingBox3D::Reset,
             R"pbdoc(
			Resets this box to initial state (min = infinite, max = -infinite).
		)pbdoc")
        .def(
            "Merge",
            [](BoundingBox3D& instance, pybind11::object other) {
                if (pybind11::isinstance<pybind11::tuple>(
                        static_cast<pybind11::handle>(other)) ||
                    pybind11::isinstance<Vector3D>(
                        static_cast<pybind11::handle>(other)))
                {
                    instance.Merge(ObjectToVector3D(other));
                }
                else if (pybind11::isinstance<BoundingBox3D>(
                             static_cast<pybind11::handle>(other)))
                {
                    instance.Merge(other.cast<BoundingBox3D>());
                }
                else
                {
                    throw std::invalid_argument(
                        "Incompatible argument type for merge.");
                }
            },
            R"pbdoc(
			Merges this and other point or box.

			Parameters
			----------
			- other : Other point or bounding box to test with.
		)pbdoc",
            pybind11::arg("other"))
        .def("Expand", &BoundingBox3D::Expand,
             R"pbdoc(
			Expands this box by given delta to all direction.

			If the width of the box was x, expand(y) will result a box with
			x + y + y width.

			Parameters
			----------
			- delta : Amount to expand.
		)pbdoc",
             pybind11::arg("delta"))
        .def("Corner", &BoundingBox3D::Corner,
             R"pbdoc(
			Returns corner position. Index starts from x-first order.

			Parameters
			----------
			- idx : Index of the corner.
		)pbdoc",
             pybind11::arg("idx"))
        .def("Clamp", &BoundingBox3D::Clamp,
             R"pbdoc(
			Returns the clamped point.

			Parameters
			----------
			- point : Point to clamp.
		)pbdoc",
             pybind11::arg("point"))
        .def("IsEmpty", &BoundingBox3D::IsEmpty,
             R"pbdoc(
			Returns true if the box is empty.
		)pbdoc");
}