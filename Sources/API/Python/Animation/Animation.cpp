// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Animation/Animation.hpp>
#include <Core/Animation/Animation.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

class PyAnimation : public Animation
{
 public:
    using Animation::Animation;

    void OnUpdate(const Frame& frame) override
    {
        PYBIND11_OVERLOAD_PURE(void, Animation, OnUpdate, frame);
    }
};

void AddAnimation(pybind11::module& m)
{
    pybind11::class_<Animation, PyAnimation, AnimationPtr>(m, "Animation",
                                                           R"pbdoc(
			Abstract base class for animation-related class.

			This class represents the animation logic in very abstract level.
			Generally animation is a function of time and/or its previous state.
			This base class provides a virtual function update() which can be
			overriden by its sub-classes to implement their own state update logic.
		)pbdoc")
        .def(pybind11::init<>())
        .def("Update", &Animation::Update,
             R"pbdoc(
			Updates animation state for given `frame`.

			Parameters
			----------
			- frame : Number of frames to advance.
		)pbdoc",
             pybind11::arg("frame"));
}