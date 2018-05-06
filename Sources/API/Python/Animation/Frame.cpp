/*************************************************************************
> File Name: Frame.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Frame functions for CubbyFlow Python API.
> Created Time: 2018/04/21
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Animation/Frame.h>
#include <Core/Animation/Frame.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddFrame(pybind11::module& m)
{
	pybind11::class_<Frame>(m, "Frame",
		R"pbdoc(
			Representation of an animation frame.

			This struct holds current animation frame index and frame interval in seconds.
		)pbdoc")
		.def("__init__", [](Frame& instance, int index, double timeIntervalInSeconds)
	{
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
		.def("Advance", [](Frame& instance, int delta)
	{
		instance.Advance(delta);
	},
			R"pbdoc(
			Advances multiple frames.

			Parameters
			----------
			- delta : Number of frames to advance.
		)pbdoc",
		pybind11::arg("delta") = 1);
}