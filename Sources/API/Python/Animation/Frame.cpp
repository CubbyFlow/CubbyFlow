// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Animation/Frame.hpp>
#include <Core/Animation/Frame.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddFrame(pybind11::module& m)
{
    pybind11::class_<Frame>(m, "Frame",
                            R"pbdoc(
			Representation of an animation frame.

			This struct holds current animation frame index and frame interval in seconds.
		)pbdoc")
        .def(
            "__init__",
            [](Frame& instance, int index, double timeIntervalInSeconds) {
                new (&instance) Frame(index, timeIntervalInSeconds);
            },
            R"pbdoc(
			Constructs Frame
			
			This method constructs Frame with index and time interval (in seconds).

			Parameters
			----------
			- index : The index (default is 0).
			- timeIntervalInSeconds : The time interval in seconds (default is 1.0 / 60.0).
		)pbdoc",
            pybind11::arg("index") = 0,
            pybind11::arg("timeIntervalInSeconds") = 1.0 / 60.0)
        .def_readwrite("index", &Frame::index,
                       R"pbdoc(
			Index of the frame
		)pbdoc")
        .def_readwrite("timeIntervalInSeconds", &Frame::timeIntervalInSeconds,
                       R"pbdoc(
			Time interval of the frame in seconds
		)pbdoc")
        .def("TimeInSeconds", &Frame::TimeInSeconds,
             R"pbdoc(
			Elapsed time in seconds
		)pbdoc")
        .def(
            "Advance",
            [](Frame& instance, int delta) { instance.Advance(delta); },
            R"pbdoc(
			Advances multiple frames.

			Parameters
			----------
			- delta : Number of frames to advance.
		)pbdoc",
            pybind11::arg("delta") = 1);
}