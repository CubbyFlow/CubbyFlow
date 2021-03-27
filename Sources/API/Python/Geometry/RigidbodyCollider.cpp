// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Geometry/RigidbodyCollider.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Geometry/RigidBodyCollider.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddRigidBodyCollider2(pybind11::module& m)
{
    pybind11::class_<RigidBodyCollider2, RigidBodyCollider2Ptr, Collider2>(
        m, "RigidBodyCollider2", R"pbdoc(
        2-D rigid body collider class.
        This class implements 2-D rigid body collider. The collider can only take
        rigid body motion with linear and rotational velocities.
        )pbdoc")
        .def(
            "__init__",
            [](RigidBodyCollider2& instance, const Surface2Ptr& surface,
               pybind11::object linearVelocity, double angularVelocity) {
                new (&instance) RigidBodyCollider2(
                    surface, ObjectToVector2D(linearVelocity), angularVelocity);
            },
            R"pbdoc(
			Constructs RigidBodyCollider2

			This method constructs RigidBodyCollider2 with surface, linear
			velocity, and angular velocity.
		)pbdoc",
            pybind11::arg("surface"),
            pybind11::arg("linearVelocity") = Vector2D{},
            pybind11::arg("angularVelocity") = 0.0)
        .def_property(
            "linearVelocity",
            [](const RigidBodyCollider2& instance) {
                return instance.linearVelocity;
            },
            [](RigidBodyCollider2& instance, pybind11::object obj) {
                instance.linearVelocity = ObjectToVector2D(obj);
            },
            R"pbdoc(
			Linear velocity of the collider.
		)pbdoc")
        .def_readwrite("angularVelocity", &RigidBodyCollider2::angularVelocity,
                       R"pbdoc(
			Angular velocity of the collider.
		)pbdoc");
}

void AddRigidBodyCollider3(pybind11::module& m)
{
    pybind11::class_<RigidBodyCollider3, RigidBodyCollider3Ptr, Collider3>(
        m, "RigidBodyCollider3", R"pbdoc(
        3-D rigid body collider class.
        This class implements 3-D rigid body collider. The collider can only take
        rigid body motion with linear and rotational velocities.
        )pbdoc")
        .def(
            "__init__",
            [](RigidBodyCollider3& instance, const Surface3Ptr& surface,
               pybind11::object linearVelocity,
               pybind11::object angularVelocity) {
                new (&instance) RigidBodyCollider3(
                    surface, ObjectToVector3D(linearVelocity),
                    ObjectToVector3D(angularVelocity));
            },
            R"pbdoc(
			Constructs RigidBodyCollider3

			This method constructs RigidBodyCollider3 with surface, linear
			velocity, and angular velocity.
		)pbdoc",
            pybind11::arg("surface"),
            pybind11::arg("linearVelocity") = Vector3D{},
            pybind11::arg("angularVelocity") = Vector3D{})
        .def_property(
            "linearVelocity",
            [](const RigidBodyCollider3& instance) {
                return instance.linearVelocity;
            },
            [](RigidBodyCollider3& instance, pybind11::object obj) {
                instance.linearVelocity = ObjectToVector3D(obj);
            },
            R"pbdoc(
			Linear velocity of the collider.
		)pbdoc")
        .def_property(
            "angularVelocity",
            [](const RigidBodyCollider3& instance) {
                return instance.angularVelocity;
            },
            [](RigidBodyCollider3& instance, pybind11::object obj) {
                instance.angularVelocity = ObjectToVector3D(obj);
            },
            R"pbdoc(
			Angular velocity of the collider.
		)pbdoc");
}