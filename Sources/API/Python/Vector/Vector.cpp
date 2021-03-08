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
#include <Core/Matrix/Matrix.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

template <typename T, size_t R>
auto Object2Vector(const pybind11::object& obj, const std::string& name)
{
    using VectorType = Matrix<T, R, 1>;

    if (pybind11::isinstance<VectorType>(obj))
    {
        return obj.cast<VectorType>();
    }
    else if (pybind11::isinstance<pybind11::tuple>(obj))
    {
        return TupleToVector<T, R>(pybind11::tuple(obj));
    }
    else if (pybind11::isinstance<pybind11::list>(obj))
    {
        return TupleToVector<T, R>(pybind11::list(obj));
    }
    else
    {
        throw std::invalid_argument(
            ("Cannot convert to " + name + ".").c_str());
    }
}

template <typename PyBindClass, typename T, size_t R>
void AddVector(PyBindClass& cls, const std::string& name)
{
    using VectorType = Matrix<T, R, 1>;

    cls.def("Fill",
            static_cast<void (VectorType::*)(const T&)>(&VectorType::Fill))
        .def("Dot",
             [name](const VectorType& instance, pybind11::object other) {
                 return instance.Dot(Object2Vector<T, R>(other, name));
             })
        .def("Cross",
             [name](const VectorType& instance, pybind11::object other) {
                 return instance.Cross(Object2Vector<T, R>(other, name));
             })
        .def("Sum", &VectorType::Sum)
        .def("Avg", &VectorType::Avg)
        .def("Min", &VectorType::Min)
        .def("Max", &VectorType::Max)
        .def("AbsMin", &VectorType::AbsMin)
        .def("AbsMax", &VectorType::AbsMax)
        .def("DominantAxis", &VectorType::DominantAxis)
        .def("SubdominantAxis", &VectorType::SubdominantAxis)
        .def("LengthSquared", &VectorType::LengthSquared)
        .def("DistanceSquaredTo",
             [name](const VectorType& instance, pybind11::object other) {
                 return instance.DistanceSquaredTo(
                     Object2Vector<T, R>(other, name));
             })
        .def("__getitem__",
             [](const VectorType& instance, size_t i) -> T {
                 return instance[i];
             })
        .def("__setitem__",
             [](VectorType& instance, size_t i, T val) { instance[i] = val; })
        .def("__add__",
             [name](const VectorType& instance,
                    pybind11::object object) -> VectorType {
                 if (pybind11::isinstance<T>(object))
                 {
                     return instance + object.cast<T>();
                 }
                 else
                 {
                     return instance + Object2Vector<T, R>(object, name);
                 }
             })
        .def("__sub__",
             [name](const VectorType& instance,
                    pybind11::object object) -> VectorType {
                 if (pybind11::isinstance<T>(object))
                 {
                     return instance - object.cast<T>();
                 }
                 else
                 {
                     return instance - Object2Vector<T, R>(object, name);
                 }
             })
        .def("__rsub__",
             [name](const VectorType& instance,
                    pybind11::object object) -> VectorType {
                 if (pybind11::isinstance<T>(object))
                 {
                     return object.cast<T>() - instance;
                 }
                 else
                 {
                     return Object2Vector<T, R>(object, name) - instance;
                 }
             })
        .def("__mul__",
             [name](const VectorType& instance,
                    pybind11::object object) -> VectorType {
                 if (pybind11::isinstance<T>(object))
                 {
                     return instance * object.cast<T>();
                 }
                 else
                 {
                     // TODO: Should be deprecated
                     return ElemMul(instance,
                                    Object2Vector<T, R>(object, name));
                 }
             })
        .def("__div__",
             [name](const VectorType& instance,
                    pybind11::object object) -> VectorType {
                 if (pybind11::isinstance<T>(object))
                 {
                     return instance / object.cast<T>();
                 }
                 else
                 {
                     // TODO: Should be deprecated
                     return ElemDiv(instance,
                                    Object2Vector<T, R>(object, name));
                 }
             })
        .def("__rdiv__",
             [name](const VectorType& instance,
                    pybind11::object object) -> VectorType {
                 if (pybind11::isinstance<T>(object))
                 {
                     return object.cast<T>() / instance;
                 }
                 else
                 {
                     // TODO: Should be deprecated
                     return ElemDiv(Object2Vector<T, R>(object, name),
                                    instance);
                 }
             })
        .def("__truediv__",
             [name](const VectorType& instance,
                    pybind11::object object) -> VectorType {
                 if (pybind11::isinstance<T>(object))
                 {
                     return instance / object.cast<T>();
                 }
                 else
                 {
                     // TODO: Should be deprecated
                     return ElemDiv(instance,
                                    Object2Vector<T, R>(object, name));
                 }
             })
        .def("__eq__",
             [name](const VectorType& instance, pybind11::object obj) {
                 VectorType other = Object2Vector<T, R>(obj, name);
                 return instance == other;
             });
}

template <typename PyBindClass, typename T, size_t R>
void AddFloatVector(PyBindClass& cls, const std::string& name)
{
    using VectorType = Matrix<T, R, 1>;

    cls.def("Normalize", &VectorType::Normalize)
        .def("Avg", &VectorType::Avg)
        .def("Normalized", &VectorType::Normalized)
        .def("Length", &VectorType::Length)
        .def("DistanceTo",
             [name](const VectorType& instance, pybind11::object other) {
                 return instance.DistanceTo(Object2Vector<T, R>(other, name));
             })
        .def("Reflected",
             [name](const VectorType& instance, pybind11::object other) {
                 return instance.Reflected(Object2Vector<T, R>(other, name));
             })
        .def("Projected",
             [name](const VectorType& instance, pybind11::object other) {
                 return instance.Projected(Object2Vector<T, R>(other, name));
             });
}

#define ADD_VECTOR2(NAME, SCALAR)                          \
    pybind11::class_<NAME> cls(m, #NAME);                  \
    cls.def(                                               \
           "__init__",                                     \
           [](NAME& instance, SCALAR x, SCALAR y) {        \
               new (&instance) NAME(x, y);                 \
           },                                              \
           "Constructs " #NAME                             \
           ".\n\n"                                         \
           "This method constructs " #SCALAR               \
           "-type 2-D vector with x and y.\n",             \
           pybind11::arg("x") = 0, pybind11::arg("y") = 0) \
        .def_readwrite("x", &NAME::x)                      \
        .def_readwrite("y", &NAME::y);                     \
    AddVector<pybind11::class_<NAME>, SCALAR, 2>(cls, #NAME);

#define ADD_FLOAT_VECTOR2(NAME, SCALAR)               \
    ADD_VECTOR2(NAME, SCALAR);                        \
    cls.def("Tangential", &NAME::Tangential<SCALAR>); \
    AddFloatVector<pybind11::class_<NAME>, SCALAR, 2>(cls, #NAME);

#define ADD_VECTOR3(NAME, SCALAR)                             \
    pybind11::class_<NAME> cls(m, #NAME);                     \
    cls.def(                                                  \
           "__init__",                                        \
           [](NAME& instance, SCALAR x, SCALAR y, SCALAR z) { \
               new (&instance) NAME(x, y, z);                 \
           },                                                 \
           "Constructs " #NAME                                \
           ".\n\n"                                            \
           "This method constructs " #SCALAR                  \
           "-type 3-D vector with x, y, and z.\n",            \
           pybind11::arg("x") = 0, pybind11::arg("y") = 0,    \
           pybind11::arg("z") = 0)                            \
        .def_readwrite("x", &NAME::x)                         \
        .def_readwrite("y", &NAME::y)                         \
        .def_readwrite("z", &NAME::z);                        \
    AddVector<pybind11::class_<NAME>, SCALAR, 3>(cls, #NAME);

#define ADD_FLOAT_VECTOR3(NAME, SCALAR)                 \
    ADD_VECTOR3(NAME, SCALAR);                          \
    cls.def("Tangentials", &NAME::Tangentials<SCALAR>); \
    AddFloatVector<pybind11::class_<NAME>, SCALAR, 3>(cls, #NAME);

void AddVector2F(pybind11::module& m)
{
    ADD_FLOAT_VECTOR2(Vector2F, float);
}
void AddVector2D(pybind11::module& m)
{
    ADD_FLOAT_VECTOR2(Vector2D, double);
}
void AddVector2Z(pybind11::module& m)
{
    ADD_VECTOR2(Vector2Z, ssize_t);
}
void AddVector2UZ(pybind11::module& m)
{
    ADD_VECTOR2(Vector2UZ, size_t);
}
void AddVector3F(pybind11::module& m)
{
    ADD_FLOAT_VECTOR3(Vector3F, float);
}
void AddVector3D(pybind11::module& m)
{
    ADD_FLOAT_VECTOR3(Vector3D, double);
}
void AddVector3Z(pybind11::module& m)
{
    ADD_VECTOR3(Vector3Z, ssize_t);
}
void AddVector3UZ(pybind11::module& m)
{
    ADD_VECTOR3(Vector3UZ, size_t);
}

void AddVectors(pybind11::module& m)
{
    AddVector2F(m);
    AddVector2D(m);
    AddVector2Z(m);
    AddVector2UZ(m);
    AddVector3F(m);
    AddVector3D(m);
    AddVector3Z(m);
    AddVector3UZ(m);
}