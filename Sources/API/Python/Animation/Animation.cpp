/*************************************************************************
> File Name: Animation.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Animation functions for CubbyFlow Python API.
> Created Time: 2018/01/20
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Animation/Animation.h>
#include <Core/Animation/Animation.h>

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
	.def("Update",
		&Animation::Update,
		R"pbdoc(
			Updates animation state for given `frame`.

			Parameters
			----------
			- frame : Number of frames to advance.
		)pbdoc",
		pybind11::arg("frame"));
}