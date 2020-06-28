// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Geometry/MarchingCubes.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Geometry/MarchingCubes.hpp>
#include <Core/Grid/ScalarGrid3.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddMarchingCubes(pybind11::module& m)
{
    m.def(
        "MarchingCubes",
        [](ScalarGrid3Ptr grid, pybind11::object gridSize,
           pybind11::object origin, double isoValue,
           int bndFlag) -> TriangleMesh3Ptr {
            auto mesh = TriangleMesh3::Builder().MakeShared();
            MarchingCubes(grid->GetConstDataAccessor(),
                          ObjectToVector3D(gridSize), ObjectToVector3D(origin),
                          mesh.get(), isoValue, bndFlag);
            return mesh;
        },
        R"pbdoc(
          Computes marching cubes and extract triangle mesh from grid.
          )pbdoc");
}