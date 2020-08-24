// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Utils/Logging.hpp>
#include <Core/Utils/Logging.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddLogging(pybind11::module& m)
{
    pybind11::enum_<LogLevel>(m, "LogLevel")
        .value("ALL", LogLevel::All)
        .value("DEBUG", LogLevel::Debug)
        .value("INFO", LogLevel::Info)
        .value("WARN", LogLevel::Warn)
        .value("ERROR", LogLevel::Error)
        .value("OFF", LogLevel::Off)
        .export_values();

    pybind11::class_<Logging>(m, "Logging")
        .def_static("SetLevel", &Logging::SetLevel)
        .def_static("Mute", &Logging::Mute)
        .def_static("Unmute", &Logging::Unmute);
}