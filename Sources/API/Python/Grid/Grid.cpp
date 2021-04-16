// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Grid/Grid.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Grid/Grid.hpp>

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddGrid2(pybind11::module& m)
{
    pybind11::class_<GridDataPositionFunc<2>>(m, "GridDataPositionFunc2")
        .def("__call__",
             [](const GridDataPositionFunc<2>& instance, pybind11::args args) {
                 size_t i, j;
                 if (args.size() == 1)
                 {
                     const Vector2UZ idx = ObjectToVector2UZ(args[0]);
                     i = idx.x;
                     j = idx.y;
                 }
                 else if (args.size() == 2)
                 {
                     i = args[0].cast<size_t>();
                     j = args[1].cast<size_t>();
                 }
                 else
                 {
                     throw std::invalid_argument("Too many arguments.");
                 }

                 return instance(i, j);
             });

    pybind11::class_<Grid2, Grid2Ptr, Serializable>(
        static_cast<pybind11::handle>(m), "Grid2",
        R"pbdoc(
			Abstract base class for 2-D cartesian grid structure.

			This class represents 2-D cartesian grid structure. This class is an
			abstract base class and does not store any data. The class only stores the
			shape of the grid. The grid structure is axis-aligned and can have different
			grid spacing per axis.
		)pbdoc")
        .def_property_readonly("resolution", &Grid2::Resolution,
                               R"pbdoc(Resolution of the grid.)pbdoc")
        .def_property_readonly("gridOrigin", &Grid2::Origin,
                               R"pbdoc(Origin of the grid.)pbdoc")
        .def_property_readonly("gridSpacing", &Grid2::GridSpacing,
                               R"pbdoc(Spacing between grid points.)pbdoc")
        .def_property_readonly("boundingBox", &Grid2::GetBoundingBox,
                               R"pbdoc(Bounding box of the entire grid.)pbdoc")
        .def_property_readonly(
            "cellCenterPosition", &Grid2::CellCenterPosition,
            R"pbdoc(Function that maps grid index to the cell-center position.)pbdoc")
        .def(
            "ForEachCellIndex",
            [](Grid2& instance, pybind11::function func) {
                instance.ForEachCellIndex(func);
            },
            R"pbdoc(
             Invokes the given function `func` for each grid cell.
             This function invokes the given function object `func` for each grid
             cell in serial manner. The input parameters are `i` and `j` indices of a
             grid cell. The order of execution is i-first, j-last.
             )pbdoc",
            pybind11::arg("func"))
        .def(
            "HasSameShape",
            [](const Grid2& instance, const Grid2Ptr& other) {
                return instance.HasSameShape(*other);
            },
            R"pbdoc(
             Returns true if resolution, grid-spacing and origin are same.
             )pbdoc")
        .def(
            "Swap",
            [](Grid2& instance, Grid2Ptr& other) {
                return instance.Swap(other.get());
            },
            R"pbdoc(
             Swaps the data with other grid.
             )pbdoc");
}

void AddGrid3(pybind11::module& m)
{
    pybind11::class_<GridDataPositionFunc<3>>(m, "GridDataPositionFunc3")
        .def("__call__",
             [](const GridDataPositionFunc<3>& instance, pybind11::args args) {
                 size_t i, j, k;
                 if (args.size() == 1)
                 {
                     const Vector3UZ idx = ObjectToVector3UZ(args[0]);
                     i = idx.x;
                     j = idx.y;
                     k = idx.z;
                 }
                 else if (args.size() == 3)
                 {
                     i = args[0].cast<size_t>();
                     j = args[1].cast<size_t>();
                     k = args[2].cast<size_t>();
                 }
                 else
                 {
                     throw std::invalid_argument("Too few/many arguments.");
                 }

                 return instance(i, j, k);
             });

    pybind11::class_<Grid3, Grid3Ptr, Serializable>(
        static_cast<pybind11::handle>(m), "Grid3",
        R"pbdoc(
			Abstract base class for 3-D cartesian grid structure.

			This class represents 3-D cartesian grid structure. This class is an
			abstract base class and does not store any data. The class only stores the
			shape of the grid. The grid structure is axis-aligned and can have different
			grid spacing per axis.
		)pbdoc")
        .def_property_readonly("resolution", &Grid3::Resolution,
                               R"pbdoc(Resolution of the grid.)pbdoc")
        .def_property_readonly("gridOrigin", &Grid3::Origin,
                               R"pbdoc(Origin of the grid.)pbdoc")
        .def_property_readonly("gridSpacing", &Grid3::GridSpacing,
                               R"pbdoc(Spacing between grid points.)pbdoc")
        .def_property_readonly("boundingBox", &Grid3::GetBoundingBox,
                               R"pbdoc(Bounding box of the entire grid.)pbdoc")
        .def_property_readonly(
            "cellCenterPosition", &Grid3::CellCenterPosition,
            R"pbdoc(Function that maps grid index to the cell-center position.)pbdoc")
        .def(
            "ForEachCellIndex",
            [](Grid3& instance, pybind11::function func) {
                instance.ForEachCellIndex(func);
            },
            R"pbdoc(
             Invokes the given function `func` for each grid cell.
             This function invokes the given function object `func` for each grid
             cell in serial manner. The input parameters are `i`, `j` and `k` indices of a
             grid cell. The order of execution is i-first, k-last.
             )pbdoc",
            pybind11::arg("func"))
        .def(
            "HasSameShape",
            [](const Grid3& instance, const Grid3Ptr& other) {
                return instance.HasSameShape(*other);
            },
            R"pbdoc(
             Returns true if resolution, grid-spacing and origin are same.
             )pbdoc")
        .def(
            "Swap",
            [](Grid3& instance, Grid3Ptr& other) {
                return instance.Swap(other.get());
            },
            R"pbdoc(
             Swaps the data with other grid.
             )pbdoc");
}