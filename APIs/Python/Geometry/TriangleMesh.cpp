/*************************************************************************
> File Name: TriangleMesh.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: TriangleMesh functions for CubbyFlow Python API.
> Created Time: 2018/01/28
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <Python/Geometry/TriangleMesh.h>
#include <Python/Utils/pybind11Utils.h>
#include <Geometry/TriangleMesh3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddTriangleMesh3(pybind11::module& m)
{
    pybind11::class_<TriangleMesh3, TriangleMesh3Ptr, Surface3>(
        static_cast<pybind11::handle>(m), "TriangleMesh3",
        R"pbdoc(
			3-D triangle mesh geometry.

			This class represents 3-D triangle mesh geometry which extends Surface3 by
			overriding surface-related queries. The mesh structure stores point,
			normals, and UV coordinates.
		)pbdoc")
        .def("__init__",
             [](TriangleMesh3& instance, pybind11::list points,
                pybind11::list normals, pybind11::list uvs,
                pybind11::list pointIndices, pybind11::list normalIndices,
                pybind11::list uvIndices, const Transform3& transform,
                bool isNormalFlipped) {
                 TriangleMesh3::PointArray points_;
                 TriangleMesh3::NormalArray normals_;
                 TriangleMesh3::UVArray uvs_;
                 TriangleMesh3::IndexArray pointIndices_;
                 TriangleMesh3::IndexArray normalIndices_;
                 TriangleMesh3::IndexArray uvIndices_;

                 points_.Resize(points.size());
                 for (size_t i = 0; i < points.size(); ++i)
                 {
                     points_[i] = ObjectToVector3D(points[i]);
                 }

                 normals_.Resize(normals.size());
                 for (size_t i = 0; i < normals.size(); ++i)
                 {
                     normals_[i] = ObjectToVector3D(normals[i]);
                 }

                 uvs_.Resize(uvs.size());
                 for (size_t i = 0; i < uvs.size(); ++i)
                 {
                     uvs_[i] = ObjectToVector2D(uvs[i]);
                 }

                 pointIndices_.Resize(pointIndices.size());
                 for (size_t i = 0; i < pointIndices.size(); ++i)
                 {
                     pointIndices_[i] = ObjectToPoint3UI(pointIndices[i]);
                 }

                 normalIndices_.Resize(normalIndices.size());
                 for (size_t i = 0; i < normalIndices.size(); ++i)
                 {
                     normalIndices_[i] = ObjectToPoint3UI(normalIndices[i]);
                 }

                 uvIndices_.Resize(uvIndices.size());
                 for (size_t i = 0; i < uvIndices.size(); ++i)
                 {
                     uvIndices_[i] = ObjectToPoint3UI(uvIndices[i]);
                 }

                 new (&instance) TriangleMesh3(
                     points_, normals_, uvs_, pointIndices_, normalIndices_,
                     uvIndices_, transform, isNormalFlipped);
             },
             R"pbdoc(
			Constructs mesh with points, normals, uvs, and their indices.
		)pbdoc",
             pybind11::arg("points") = pybind11::list(),
             pybind11::arg("normals") = pybind11::list(),
             pybind11::arg("uvs") = pybind11::list(),
             pybind11::arg("pointIndices") = pybind11::list(),
             pybind11::arg("normalIndices") = pybind11::list(),
             pybind11::arg("uvIndices") = pybind11::list(),
             pybind11::arg("transform") = Transform3(),
             pybind11::arg("isNormalFlipped") = false)
        .def("Clear", &TriangleMesh3::Clear,
             R"pbdoc(
			Clears all content.
		)pbdoc")
        .def("Set", &TriangleMesh3::Set,
             R"pbdoc(
			Copies the contents from `other` mesh.
		)pbdoc",
             pybind11::arg("other"))
        .def("Swap",
             [](TriangleMesh3& instance, const TriangleMesh3Ptr& other) {
                 instance.Swap(*other);
             },
             R"pbdoc(
			Swaps the contents with `other` mesh.
		)pbdoc",
             pybind11::arg("other"))
        .def("Area", &TriangleMesh3::Area,
             R"pbdoc(
			Returns area of this mesh.
		)pbdoc")
        .def("Volume", &TriangleMesh3::Volume,
             R"pbdoc(
			Returns volume of this mesh.
		)pbdoc")
        .def("GetPoint",
             [](const TriangleMesh3& instance, size_t i) -> Vector3D {
                 return instance.Point(i);
             },
             R"pbdoc(
			Returns i-th point.
		)pbdoc",
             pybind11::arg("i"))
        .def("SetPoint",
             [](TriangleMesh3& instance, size_t i, const Vector3D& pt) {
                 instance.Point(i) = pt;
             },
             R"pbdoc(
			Sets `i`-th point with `pt`.
		)pbdoc",
             pybind11::arg("i"), pybind11::arg("pt"))
        .def("GetNormal",
             [](const TriangleMesh3& instance, size_t i) -> Vector3D {
                 return instance.Normal(i);
             },
             R"pbdoc(
			Returns i-th normal.
		)pbdoc",
             pybind11::arg("i"))
        .def("SetNormal",
             [](TriangleMesh3& instance, size_t i, const Vector3D& n) {
                 instance.Normal(i) = n;
             },
             R"pbdoc(
			Sets `i`-th normal with `pt`.
		)pbdoc",
             pybind11::arg("i"), pybind11::arg("n"))
        .def("GetPointIndex",
             [](const TriangleMesh3& instance, size_t i) -> Point3UI {
                 return instance.PointIndex(i);
             },
             R"pbdoc(
			Returns i-th pointIndex.
		)pbdoc",
             pybind11::arg("i"))
        .def("SetPointIndex",
             [](TriangleMesh3& instance, size_t i, const Point3UI& idx) {
                 instance.PointIndex(i) = idx;
             },
             R"pbdoc(
			Sets `i`-th pointIndex with `idx`.
		)pbdoc",
             pybind11::arg("i"), pybind11::arg("idx"))
        .def("GetNormalIndex",
             [](const TriangleMesh3& instance, size_t i) -> Point3UI {
                 return instance.NormalIndex(i);
             },
             R"pbdoc(
			Returns i-th normalIndexIndex.
		)pbdoc",
             pybind11::arg("i"))
        .def("SetNormalIndex",
             [](TriangleMesh3& instance, size_t i, const Point3UI& idx) {
                 instance.NormalIndex(i) = idx;
             },
             R"pbdoc(
			Sets `i`-th normalIndexIndex with `idx`.
		)pbdoc",
             pybind11::arg("i"), pybind11::arg("idx"))
        .def("GetUVIndex",
             [](const TriangleMesh3& instance, size_t i) -> Point3UI {
                 return instance.UVIndex(i);
             },
             R"pbdoc(
			Returns i-th uvIndexIndex.
		)pbdoc",
             pybind11::arg("i"))
        .def("SetUVIndex",
             [](TriangleMesh3& instance, size_t i, const Point3UI& idx) {
                 instance.UVIndex(i) = idx;
             },
             R"pbdoc(
			Sets `i`-th uvIndexIndex with `idx`.
		)pbdoc",
             pybind11::arg("i"), pybind11::arg("idx"))
        .def("Triangle",
             [](const TriangleMesh3& instance, size_t i) {
                 return std::make_shared<Triangle3>(instance.Triangle(i));
             },
             R"pbdoc(
			Returns `i`-th triangle.
		)pbdoc",
             pybind11::arg("i"))
        .def("NumberOfPoints", &TriangleMesh3::NumberOfPoints,
             R"pbdoc(
			Returns number of points.
		)pbdoc")
        .def("NumberOfNormals", &TriangleMesh3::NumberOfNormals,
             R"pbdoc(
			Returns number of normals.
		)pbdoc")
        .def("NumberOfUVs", &TriangleMesh3::NumberOfUVs,
             R"pbdoc(
			Returns number of UV coordinates.
		)pbdoc")
        .def("NumberOfTriangles", &TriangleMesh3::NumberOfTriangles,
             R"pbdoc(
			Returns number of triangles.
		)pbdoc")
        .def("HasNormals", &TriangleMesh3::HasNormals,
             R"pbdoc(
			Returns true if the mesh has normals.
		)pbdoc")
        .def("HasUVs", &TriangleMesh3::HasUVs,
             R"pbdoc(
			Returns true if the mesh has UV coordinates.
		)pbdoc")
        .def("AddPoint",
             [](TriangleMesh3& instance, pybind11::object obj) {
                 instance.AddPoint(ObjectToVector3D(obj));
             },
             R"pbdoc(
			Adds a point.
		)pbdoc",
             pybind11::arg("pt"))
        .def("AddNormal",
             [](TriangleMesh3& instance, pybind11::object obj) {
                 instance.AddNormal(ObjectToVector3D(obj));
             },
             R"pbdoc(
			Adds a normal.
		)pbdoc",
             pybind11::arg("n"))
        .def("AddUV",
             [](TriangleMesh3& instance, pybind11::object obj) {
                 instance.AddUV(ObjectToVector2D(obj));
             },
             R"pbdoc(
			Adds a UV.
		)pbdoc",
             pybind11::arg("uv"))
        .def("AddPointTriangle",
             [](TriangleMesh3& instance, pybind11::object obj) {
                 instance.AddPointTriangle(ObjectToPoint3UI(obj));
             },
             R"pbdoc(
			Adds a triangle with point.
		)pbdoc",
             pybind11::arg("newPointIndices"))
        .def("AddNormalTriangle",
             [](TriangleMesh3& instance, pybind11::object obj) {
                 instance.AddNormalTriangle(ObjectToPoint3UI(obj));
             },
             R"pbdoc(
			Adds a triangle with normal.
		)pbdoc",
             pybind11::arg("newNormalIndices"))
        .def("AddUVTriangle",
             [](TriangleMesh3& instance, pybind11::object obj) {
                 instance.AddUVTriangle(ObjectToPoint3UI(obj));
             },
             R"pbdoc(
			Adds a triangle with UV.
		)pbdoc",
             pybind11::arg("newUVIndices"))
        .def("AddTriangle", &TriangleMesh3::AddTriangle,
             R"pbdoc(
			Add a triangle.
		)pbdoc",
             pybind11::arg("tri"))
        .def("SetFaceNormal", &TriangleMesh3::SetFaceNormal,
             R"pbdoc(
			Sets entire normals to the face normals.
		)pbdoc")
        .def("SetAngleWeightedVertexNormal",
             &TriangleMesh3::SetAngleWeightedVertexNormal,
             R"pbdoc(
			Sets angle weighted vertex normal.
		)pbdoc")
        .def("Scale", &TriangleMesh3::Scale,
             R"pbdoc(
			Scales the mesh by given factor.
		)pbdoc",
             pybind11::arg("factor"))
        .def("Translate",
             [](TriangleMesh3& instance, pybind11::object obj) {
                 instance.Translate(ObjectToVector3D(obj));
             },
             R"pbdoc(
			Translates the mesh.
		)pbdoc",
             pybind11::arg("t"))
        .def("Rotate",
             [](TriangleMesh3& instance, pybind11::object obj) {
                 instance.Rotate(ObjectToQuaternionD(obj));
             },
             R"pbdoc(
			Rotates the mesh.
		)pbdoc",
             pybind11::arg("rot"))
        .def("WriteObj",
             [](const TriangleMesh3& instance, const std::string& fileName) {
                 instance.WriteObj(fileName);
             },
             R"pbdoc(
			Writes the mesh in obj format to the file.
		)pbdoc",
             pybind11::arg("fileName"))
        .def("ReadObj",
             [](TriangleMesh3& instance, const std::string& fileName) {
                 instance.ReadObj(fileName);
             },
             R"pbdoc(
			Reads the mesh in obj format from the file.
		)pbdoc",
             pybind11::arg("fileName"));
}