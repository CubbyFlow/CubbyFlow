// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Utils/pybind11Utils.hpp>
#include <API/Python/Vector/Vector.hpp>
#include <Core/Vector/Vector3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddVector2F(pybind11::module& m)
{
    pybind11::class_<Vector2F>(static_cast<pybind11::handle>(m), "Vector2F")
        .def(
            "__init__",
            [](Vector2F& instance, float x, float y) {
                new (&instance) Vector2F(x, y);
            },
            R"pbdoc(
			Constructs Vector2F.

			This method constructs float-type 2-D vector with x and y.
		)pbdoc",
            pybind11::arg("x") = 0.0f, pybind11::arg("y") = 0.0f)
        .def_readwrite("x", &Vector2F::x)
        .def_readwrite("y", &Vector2F::y)
        .def("SetZero", &Vector2F::SetZero)
        .def("Normalize", &Vector2F::Normalize)
        .def("Dot",
             [](const Vector2F& instance, pybind11::object other) {
                 return instance.Dot(ObjectToVector2F(other));
             })
        .def("Cross",
             [](const Vector2F& instance, pybind11::object other) {
                 return instance.Cross(ObjectToVector2F(other));
             })
        .def("Sum", &Vector2F::Sum)
        .def("Avg", &Vector2F::Avg)
        .def("Min", &Vector2F::Min)
        .def("Max", &Vector2F::Max)
        .def("AbsMin", &Vector2F::AbsMin)
        .def("AbsMax", &Vector2F::AbsMax)
        .def("DominantAxis", &Vector2F::DominantAxis)
        .def("SubdominantAxis", &Vector2F::SubdominantAxis)
        .def("Normalized", &Vector2F::Normalized)
        .def("Length", &Vector2F::Length)
        .def("LengthSquared", &Vector2F::LengthSquared)
        .def("DistanceTo",
             [](const Vector2F& instance, pybind11::object other) {
                 return instance.DistanceTo(ObjectToVector2F(other));
             })
        .def("DistanceSquaredTo",
             [](const Vector2F& instance, pybind11::object other) {
                 return instance.DistanceSquaredTo(ObjectToVector2F(other));
             })
        .def("Reflected",
             [](const Vector2F& instance, pybind11::object other) {
                 return instance.Reflected(ObjectToVector2F(other));
             })
        .def("Projected",
             [](const Vector2F& instance, pybind11::object other) {
                 return instance.Projected(ObjectToVector2F(other));
             })
        .def("Tangential", &Vector2F::Tangential)
        .def("__getitem__",
             [](const Vector2F& instance, size_t i) -> float {
                 return instance[i];
             })
        .def("__setitem__",
             [](Vector2F& instance, size_t i, float val) { instance[i] = val; })
        .def("__add__",
             [](const Vector2F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Add(object.cast<float>());
                 }
                 else
                 {
                     return instance.Add(ObjectToVector2F(object));
                 }
             })
        .def("__sub__",
             [](const Vector2F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Sub(object.cast<float>());
                 }
                 else
                 {
                     return instance.Sub(ObjectToVector2F(object));
                 }
             })
        .def("__rsub__",
             [](const Vector2F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.RSub(object.cast<float>());
                 }
                 else
                 {
                     return instance.RSub(ObjectToVector2F(object));
                 }
             })
        .def("__mul__",
             [](const Vector2F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Mul(object.cast<float>());
                 }
                 else
                 {
                     return instance.Mul(ObjectToVector2F(object));
                 }
             })
        .def("__div__",
             [](const Vector2F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Div(object.cast<float>());
                 }
                 else
                 {
                     return instance.Div(ObjectToVector2F(object));
                 }
             })
        .def("__rdiv__",
             [](const Vector2F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.RDiv(object.cast<float>());
                 }
                 else
                 {
                     return instance.RDiv(ObjectToVector2F(object));
                 }
             })
        .def("__truediv__",
             [](const Vector2F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Div(object.cast<float>());
                 }
                 else
                 {
                     return instance.Div(ObjectToVector2F(object));
                 }
             })
        .def("__rtruediv__",
             [](const Vector2F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.RDiv(object.cast<float>());
                 }
                 else
                 {
                     return instance.RDiv(ObjectToVector2F(object));
                 }
             })
        .def("__eq__", [](const Vector2F& instance, pybind11::object obj) {
            Vector2F other = ObjectToVector2F(obj);
            return instance == other;
        });
}

void AddVector2D(pybind11::module& m)
{
    pybind11::class_<Vector2D>(static_cast<pybind11::handle>(m), "Vector2D")
        .def(
            "__init__",
            [](Vector2D& instance, double x, double y) {
                new (&instance) Vector2D(x, y);
            },
            R"pbdoc(
			Constructs Vector2D.

			This method constructs double-type 2-D vector with x and y.
		)pbdoc",
            pybind11::arg("x") = 0.0, pybind11::arg("y") = 0.0)
        .def_readwrite("x", &Vector2D::x)
        .def_readwrite("y", &Vector2D::y)
        .def("SetZero", &Vector2D::SetZero)
        .def("Normalize", &Vector2D::Normalize)
        .def("Dot",
             [](const Vector2D& instance, pybind11::object other) {
                 return instance.Dot(ObjectToVector2D(other));
             })
        .def("Cross",
             [](const Vector2D& instance, pybind11::object other) {
                 return instance.Cross(ObjectToVector2D(other));
             })
        .def("Sum", &Vector2D::Sum)
        .def("Avg", &Vector2D::Avg)
        .def("Min", &Vector2D::Min)
        .def("Max", &Vector2D::Max)
        .def("AbsMin", &Vector2D::AbsMin)
        .def("AbsMax", &Vector2D::AbsMax)
        .def("DominantAxis", &Vector2D::DominantAxis)
        .def("SubdominantAxis", &Vector2D::SubdominantAxis)
        .def("Normalized", &Vector2D::Normalized)
        .def("Length", &Vector2D::Length)
        .def("LengthSquared", &Vector2D::LengthSquared)
        .def("DistanceTo",
             [](const Vector2D& instance, pybind11::object other) {
                 return instance.DistanceTo(ObjectToVector2D(other));
             })
        .def("DistanceSquaredTo",
             [](const Vector2D& instance, pybind11::object other) {
                 return instance.DistanceSquaredTo(ObjectToVector2D(other));
             })
        .def("Reflected",
             [](const Vector2D& instance, pybind11::object other) {
                 return instance.Reflected(ObjectToVector2D(other));
             })
        .def("Projected",
             [](const Vector2D& instance, pybind11::object other) {
                 return instance.Projected(ObjectToVector2D(other));
             })
        .def("Tangential", &Vector2D::Tangential)
        .def("__getitem__",
             [](const Vector2D& instance, size_t i) -> double {
                 return instance[i];
             })
        .def("__setitem__", [](Vector2D& instance, size_t i,
                               double val) { instance[i] = val; })
        .def("__add__",
             [](const Vector2D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Add(object.cast<double>());
                 }
                 else
                 {
                     return instance.Add(ObjectToVector2D(object));
                 }
             })
        .def("__sub__",
             [](const Vector2D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Sub(object.cast<double>());
                 }
                 else
                 {
                     return instance.Sub(ObjectToVector2D(object));
                 }
             })
        .def("__rsub__",
             [](const Vector2D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.RSub(object.cast<double>());
                 }
                 else
                 {
                     return instance.RSub(ObjectToVector2D(object));
                 }
             })
        .def("__mul__",
             [](const Vector2D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Mul(object.cast<double>());
                 }
                 else
                 {
                     return instance.Mul(ObjectToVector2D(object));
                 }
             })
        .def("__div__",
             [](const Vector2D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Div(object.cast<double>());
                 }
                 else
                 {
                     return instance.Div(ObjectToVector2D(object));
                 }
             })
        .def("__rdiv__",
             [](const Vector2D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.RDiv(object.cast<double>());
                 }
                 else
                 {
                     return instance.RDiv(ObjectToVector2D(object));
                 }
             })
        .def("__truediv__",
             [](const Vector2D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Div(object.cast<double>());
                 }
                 else
                 {
                     return instance.Div(ObjectToVector2D(object));
                 }
             })
        .def("__rtruediv__",
             [](const Vector2D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.RDiv(object.cast<double>());
                 }
                 else
                 {
                     return instance.RDiv(ObjectToVector2D(object));
                 }
             })
        .def("__eq__", [](const Vector2D& instance, pybind11::object obj) {
            Vector2D other = ObjectToVector2D(obj);
            return instance == other;
        });
}

void AddVector3F(pybind11::module& m)
{
    pybind11::class_<Vector3F>(static_cast<pybind11::handle>(m), "Vector3F")
        .def(
            "__init__",
            [](Vector3F& instance, float x, float y, float z) {
                new (&instance) Vector3F(x, y, z);
            },
            R"pbdoc(
			Constructs Vector3F.

			This method constructs float-type 3-D vector with x, y, and z.
		)pbdoc",
            pybind11::arg("x") = 0.0f, pybind11::arg("y") = 0.0f,
            pybind11::arg("z") = 0.0f)
        .def_readwrite("x", &Vector3F::x)
        .def_readwrite("y", &Vector3F::y)
        .def_readwrite("z", &Vector3F::z)
        .def("SetZero", &Vector3F::SetZero)
        .def("Normalize", &Vector3F::Normalize)
        .def("Dot",
             [](const Vector3F& instance, pybind11::object other) {
                 return instance.Dot(ObjectToVector3F(other));
             })
        .def("Cross",
             [](const Vector3F& instance, pybind11::object other) {
                 return instance.Cross(ObjectToVector3F(other));
             })
        .def("Sum", &Vector3F::Sum)
        .def("Avg", &Vector3F::Avg)
        .def("Min", &Vector3F::Min)
        .def("Max", &Vector3F::Max)
        .def("AbsMin", &Vector3F::AbsMin)
        .def("AbsMax", &Vector3F::AbsMax)
        .def("DominantAxis", &Vector3F::DominantAxis)
        .def("SubdominantAxis", &Vector3F::SubdominantAxis)
        .def("Normalized", &Vector3F::Normalized)
        .def("Length", &Vector3F::Length)
        .def("LengthSquared", &Vector3F::LengthSquared)
        .def("DistanceTo",
             [](const Vector3F& instance, pybind11::object other) {
                 return instance.DistanceTo(ObjectToVector3F(other));
             })
        .def("DistanceSquaredTo",
             [](const Vector3F& instance, pybind11::object other) {
                 return instance.DistanceSquaredTo(ObjectToVector3F(other));
             })
        .def("Reflected",
             [](const Vector3F& instance, pybind11::object other) {
                 return instance.Reflected(ObjectToVector3F(other));
             })
        .def("Projected",
             [](const Vector3F& instance, pybind11::object other) {
                 return instance.Projected(ObjectToVector3F(other));
             })
        .def("Tangential", &Vector3F::Tangential)
        .def("__getitem__",
             [](const Vector3F& instance, size_t i) -> float {
                 return instance[i];
             })
        .def("__setitem__",
             [](Vector3F& instance, size_t i, float val) { instance[i] = val; })
        .def("__add__",
             [](const Vector3F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Add(object.cast<float>());
                 }
                 else
                 {
                     return instance.Add(ObjectToVector3F(object));
                 }
             })
        .def("__sub__",
             [](const Vector3F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Sub(object.cast<float>());
                 }
                 else
                 {
                     return instance.Sub(ObjectToVector3F(object));
                 }
             })
        .def("__rsub__",
             [](const Vector3F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.RSub(object.cast<float>());
                 }
                 else
                 {
                     return instance.RSub(ObjectToVector3F(object));
                 }
             })
        .def("__mul__",
             [](const Vector3F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Mul(object.cast<float>());
                 }
                 else
                 {
                     return instance.Mul(ObjectToVector3F(object));
                 }
             })
        .def("__div__",
             [](const Vector3F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Div(object.cast<float>());
                 }
                 else
                 {
                     return instance.Div(ObjectToVector3F(object));
                 }
             })
        .def("__rdiv__",
             [](const Vector3F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.RDiv(object.cast<float>());
                 }
                 else
                 {
                     return instance.RDiv(ObjectToVector3F(object));
                 }
             })
        .def("__truediv__",
             [](const Vector3F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Div(object.cast<float>());
                 }
                 else
                 {
                     return instance.Div(ObjectToVector3F(object));
                 }
             })
        .def("__rtruediv__",
             [](const Vector3F& instance, pybind11::object object) {
                 if (pybind11::isinstance<float>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.RDiv(object.cast<float>());
                 }
                 else
                 {
                     return instance.RDiv(ObjectToVector3F(object));
                 }
             })
        .def("__eq__", [](const Vector3F& instance, pybind11::object obj) {
            Vector3F other = ObjectToVector3F(obj);
            return instance == other;
        });
}

void AddVector3D(pybind11::module& m)
{
    pybind11::class_<Vector3D>(static_cast<pybind11::handle>(m), "Vector3D")
        .def(
            "__init__",
            [](Vector3D& instance, double x, double y, double z) {
                new (&instance) Vector3D(x, y, z);
            },
            R"pbdoc(
			Constructs Vector3D.

			This method constructs double-type 3-D vector with x, y, and z.
		)pbdoc",
            pybind11::arg("x") = 0.0, pybind11::arg("y") = 0.0,
            pybind11::arg("z") = 0.0)
        .def_readwrite("x", &Vector3D::x)
        .def_readwrite("y", &Vector3D::y)
        .def_readwrite("z", &Vector3D::z)
        .def("SetZero", &Vector3D::SetZero)
        .def("Normalize", &Vector3D::Normalize)
        .def("Dot",
             [](const Vector3D& instance, pybind11::object other) {
                 return instance.Dot(ObjectToVector3D(other));
             })
        .def("Cross",
             [](const Vector3D& instance, pybind11::object other) {
                 return instance.Cross(ObjectToVector3D(other));
             })
        .def("Sum", &Vector3D::Sum)
        .def("Avg", &Vector3D::Avg)
        .def("Min", &Vector3D::Min)
        .def("Max", &Vector3D::Max)
        .def("AbsMin", &Vector3D::AbsMin)
        .def("AbsMax", &Vector3D::AbsMax)
        .def("DominantAxis", &Vector3D::DominantAxis)
        .def("SubdominantAxis", &Vector3D::SubdominantAxis)
        .def("Normalized", &Vector3D::Normalized)
        .def("Length", &Vector3D::Length)
        .def("LengthSquared", &Vector3D::LengthSquared)
        .def("DistanceTo",
             [](const Vector3D& instance, pybind11::object other) {
                 return instance.DistanceTo(ObjectToVector3D(other));
             })
        .def("DistanceSquaredTo",
             [](const Vector3D& instance, pybind11::object other) {
                 return instance.DistanceSquaredTo(ObjectToVector3D(other));
             })
        .def("Reflected",
             [](const Vector3D& instance, pybind11::object other) {
                 return instance.Reflected(ObjectToVector3D(other));
             })
        .def("Projected",
             [](const Vector3D& instance, pybind11::object other) {
                 return instance.Projected(ObjectToVector3D(other));
             })
        .def("Tangential", &Vector3D::Tangential)
        .def("__getitem__",
             [](const Vector3D& instance, size_t i) -> double {
                 return instance[i];
             })
        .def("__setitem__", [](Vector3D& instance, size_t i,
                               double val) { instance[i] = val; })
        .def("__add__",
             [](const Vector3D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Add(object.cast<double>());
                 }
                 else
                 {
                     return instance.Add(ObjectToVector3D(object));
                 }
             })
        .def("__sub__",
             [](const Vector3D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Sub(object.cast<double>());
                 }
                 else
                 {
                     return instance.Sub(ObjectToVector3D(object));
                 }
             })
        .def("__rsub__",
             [](const Vector3D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.RSub(object.cast<double>());
                 }
                 else
                 {
                     return instance.RSub(ObjectToVector3D(object));
                 }
             })
        .def("__mul__",
             [](const Vector3D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Mul(object.cast<double>());
                 }
                 else
                 {
                     return instance.Mul(ObjectToVector3D(object));
                 }
             })
        .def("__div__",
             [](const Vector3D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Div(object.cast<double>());
                 }
                 else
                 {
                     return instance.Div(ObjectToVector3D(object));
                 }
             })
        .def("__rdiv__",
             [](const Vector3D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.RDiv(object.cast<double>());
                 }
                 else
                 {
                     return instance.RDiv(ObjectToVector3D(object));
                 }
             })
        .def("__truediv__",
             [](const Vector3D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.Div(object.cast<double>());
                 }
                 else
                 {
                     return instance.Div(ObjectToVector3D(object));
                 }
             })
        .def("__rtruediv__",
             [](const Vector3D& instance, pybind11::object object) {
                 if (pybind11::isinstance<double>(
                         static_cast<pybind11::handle>(object)))
                 {
                     return instance.RDiv(object.cast<double>());
                 }
                 else
                 {
                     return instance.RDiv(ObjectToVector3D(object));
                 }
             })
        .def("__eq__", [](const Vector3D& instance, pybind11::object obj) {
            Vector3D other = ObjectToVector3D(obj);
            return instance == other;
        });
}