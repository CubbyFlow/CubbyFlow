// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Geometry/ColliderSet.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Geometry/ColliderSet2.hpp>
#include <Core/Geometry/ColliderSet3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddColliderSet2(pybind11::module& m)
{
    pybind11::class_<ColliderSet2, ColliderSet2Ptr, Collider2>(m,
                                                               "ColliderSet2",
                                                               R"pbdoc(
        Collection of 2-D colliders
        )pbdoc")
        .def(pybind11::init([](pybind11::args args) {
                 if (args.size() == 1)
                 {
                     return new ColliderSet2(
                         args[0].cast<std::vector<Collider2Ptr>>());
                 }

                 if (args.size() == 0)
                 {
                     return new ColliderSet2();
                 }

                 throw std::invalid_argument("Invalid number of arguments.");
             }),
             R"pbdoc(
             Constructs ColliderSet2
             This method constructs ColliderSet2 with other colliders.
             Parameters
             ----------
             - `*args` : List of other colliders. Must be size of 0 or 1.
             )pbdoc")
        .def("AddCollider", &ColliderSet2::AddCollider,
             R"pbdoc(Adds a collider to the set.)pbdoc")
        .def_property_readonly("numberOfColliders",
                               &ColliderSet2::NumberOfColliders,
                               R"pbdoc(Number of colliders.)pbdoc")
        .def("Collider", &ColliderSet2::Collider,
             R"pbdoc(Returns collider at index i.)pbdoc");
}

void AddColliderSet3(pybind11::module& m)
{
    pybind11::class_<ColliderSet3, ColliderSet3Ptr, Collider3>(m,
                                                               "ColliderSet3",
                                                               R"pbdoc(
        Collection of 3-D colliders
        )pbdoc")
        .def(pybind11::init([](pybind11::args args) {
                 if (args.size() == 1)
                 {
                     return new ColliderSet3(
                         args[0].cast<std::vector<Collider3Ptr>>());
                 }

                 if (args.size() == 0)
                 {
                     return new ColliderSet3();
                 }

                 throw std::invalid_argument("Invalid number of arguments.");
             }),
             R"pbdoc(
             Constructs ColliderSet3
             This method constructs ColliderSet3 with other colliders.
             Parameters
             ----------
             - `*args` : List of other colliders. Must be size of 0 or 1.
             )pbdoc")
        .def("AddCollider", &ColliderSet3::AddCollider,
             R"pbdoc(Adds a collider to the set.)pbdoc")
        .def_property_readonly("numberOfColliders",
                               &ColliderSet3::NumberOfColliders,
                               R"pbdoc(Number of colliders.)pbdoc")
        .def("Collider", &ColliderSet3::Collider,
             R"pbdoc(Returns collider at index i.)pbdoc");
}