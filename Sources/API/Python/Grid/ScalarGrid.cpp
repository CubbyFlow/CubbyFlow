// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Grid/ScalarGrid.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Grid/ScalarGrid2.hpp>
#include <Core/Grid/ScalarGrid3.hpp>

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddScalarGrid2(pybind11::module& m)
{
    pybind11::class_<ScalarGrid2, ScalarGrid2Ptr, ScalarField2, Grid2>(
        static_cast<pybind11::handle>(m), "ScalarGrid2",
        R"pbdoc(
			Abstract base class for 2-D scalar grid structure.
		)pbdoc")
        .def_property_readonly("dataSize", &ScalarGrid2::GetDataSize,
                               R"pbdoc(
                               Returns the size of the grid data.
                               This function returns the size of the grid data which is not necessarily
                               equal to the grid resolution if the data is not stored at cell-center.
                               )pbdoc")
        .def_property_readonly("dataOrigin", &ScalarGrid2::GetDataOrigin,
                               R"pbdoc(
                               Returns the origin of the grid data.
                               This function returns data position for the grid point at (0, 0).
                               Note that this is different from `origin()` since `origin()` returns
                               the lower corner point of the bounding box.
                               )pbdoc")
        .def("Clone", &ScalarGrid2::Clone,
             R"pbdoc(Returns the copy of the grid instance.)pbdoc")
        .def("Clear", &ScalarGrid2::Clear,
             R"pbdoc(Clears the contents of the grid.)pbdoc")
        .def(
            "Resize",
            [](ScalarGrid2& instance, pybind11::args args,
               pybind11::kwargs kwargs) {
                Vector2UZ resolution{ 1, 1 };
                Vector2D gridSpacing{ 1, 1 };
                Vector2D gridOrigin{ 0, 0 };

                ParseGridResizeParams(args, kwargs, resolution, gridSpacing,
                                      gridOrigin);
                instance.Resize(resolution, gridSpacing, gridOrigin);
            },
            R"pbdoc(
			Resizes the grid using given parameters.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point at the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc")
        .def(
            "__getitem__",
            [](const ScalarGrid2& instance, pybind11::object obj) -> double {
                if (pybind11::isinstance<pybind11::tuple>(
                        static_cast<pybind11::handle>(obj)))
                {
                    auto tidx = obj.cast<pybind11::tuple>();
                    if (tidx.size() == 2)
                    {
                        return instance(tidx[0].cast<size_t>(),
                                        tidx[1].cast<size_t>());
                    }
                    else
                    {
                        throw std::invalid_argument(
                            "Size of index tuple must be 2.");
                    }
                }
                else if (pybind11::isinstance<pybind11::list>(
                             static_cast<pybind11::handle>(obj)))
                {
                    auto lidx = obj.cast<pybind11::list>();
                    if (lidx.size() == 2)
                    {
                        return instance(lidx[0].cast<size_t>(),
                                        lidx[1].cast<size_t>());
                    }
                    else
                    {
                        throw std::invalid_argument(
                            "Size of index list must be 2.");
                    }
                }
                else
                {
                    throw std::invalid_argument(
                        "Input type must be tuple or list");
                }
            },
            R"pbdoc(
			Returns the grid data at given data point.

			Parameters
			----------
			- idx : Data point index (i, j).
		)pbdoc",
            pybind11::arg("idx"))
        .def(
            "__setitem__",
            [](ScalarGrid2& instance, pybind11::object obj, double val) {
                if (pybind11::isinstance<pybind11::tuple>(
                        static_cast<pybind11::handle>(obj)))
                {
                    auto tidx = obj.cast<pybind11::tuple>();
                    if (tidx.size() == 2)
                    {
                        instance(tidx[0].cast<size_t>(),
                                 tidx[1].cast<size_t>()) = val;
                    }
                    else
                    {
                        throw std::invalid_argument(
                            "Size of index tuple must be 2.");
                    }
                }
                else if (pybind11::isinstance<pybind11::list>(
                             static_cast<pybind11::handle>(obj)))
                {
                    auto lidx = obj.cast<pybind11::list>();
                    if (lidx.size() == 2)
                    {
                        instance(lidx[0].cast<size_t>(),
                                 lidx[1].cast<size_t>()) = val;
                    }
                    else
                    {
                        throw std::invalid_argument(
                            "Size of index list must be 2.");
                    }
                }
                else
                {
                    throw std::invalid_argument(
                        "Input type must be tuple or list");
                }
            },
            R"pbdoc(
			Sets the grid data at given data point.

			Parameters
			----------
			- idx : Data point index (i, j).
			- val : Value to set.
		)pbdoc",
            pybind11::arg("idx"), pybind11::arg("val"))
        .def("GradientAtDataPoint", &ScalarGrid2::GradientAtDataPoint,
             R"pbdoc(
			Returns the gradient vector at given data point.

			Parameters
			----------
			- i : Data point index i.
			- j : Data point index j.
		)pbdoc",
             pybind11::arg("i"), pybind11::arg("j"))
        .def("LaplacianAtDataPoint", &ScalarGrid2::LaplacianAtDataPoint,
             R"pbdoc(
			Returns the Laplacian at given data point.

			Parameters
			----------
			- i : Data point index i.
			- j : Data point index j.
		)pbdoc",
             pybind11::arg("i"), pybind11::arg("j"))
        .def("DataView",
             static_cast<ArrayView2<double> (ScalarGrid2::*)()>(
                 &ScalarGrid2::DataView),
             R"pbdoc(Returns the data array view.)pbdoc")
        .def("DataPosition", &ScalarGrid2::DataPosition,
             R"pbdoc(The function that maps data point to its position.)pbdoc")
        .def(
            "Fill",
            [](ScalarGrid2& instance, double value) {
                instance.Fill(value, ExecutionPolicy::Serial);
            },
            R"pbdoc(Fills the grid with given value.)pbdoc")
        .def(
            "Fill",
            [](ScalarGrid2& instance, pybind11::object obj) {
                if (pybind11::isinstance<pybind11::function>(obj))
                {
                    auto func = obj.cast<pybind11::function>();
                    instance.Fill(
                        [func](const Vector2D& pt) -> double {
                            return func(pt).cast<double>();
                        },
                        ExecutionPolicy::Serial);
                }
                else
                {
                    throw std::invalid_argument(
                        "Input type must be double or function object -> "
                        "double");
                }
            },
            R"pbdoc(
			Fills the grid with given function.
		)pbdoc")
        .def(
            "ForEachDataPointIndex",
            [](ScalarGrid2& instance, pybind11::function func) {
                instance.ForEachDataPointIndex(func);
            },
            R"pbdoc(
			Invokes the given function `func` for each data point.

			This function invokes the given function object `func` for each data
			point in serial manner. The input parameters are i and j indices of a
			data point. The order of execution is i-first, j-last.
		)pbdoc",
            pybind11::arg("func"))
        .def(
            "Sample",
            [](const ScalarGrid2& instance, pybind11::object obj) {
                return instance.Sample(ObjectToVector2D(obj));
            },
            R"pbdoc(
			Returns sampled value at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Gradient",
            [](const ScalarGrid2& instance, pybind11::object obj) {
                return instance.Gradient(ObjectToVector2D(obj));
            },
            R"pbdoc(
			Returns the gradient vector at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Laplacian",
            [](const ScalarGrid2& instance, pybind11::object obj) {
                return instance.Laplacian(ObjectToVector2D(obj));
            },
            R"pbdoc(
			Returns the Laplacian value at given position `x`.
		)pbdoc",
            pybind11::arg("x"));
}

void AddScalarGrid3(pybind11::module& m)
{
    pybind11::class_<ScalarGrid3, ScalarGrid3Ptr, ScalarField3, Grid3>(
        static_cast<pybind11::handle>(m), "ScalarGrid3",
        R"pbdoc(
			Abstract base class for 3-D scalar grid structure.
		)pbdoc")
        .def_property_readonly("dataSize", &ScalarGrid3::GetDataSize,
                               R"pbdoc(
                               Returns the size of the grid data.
                               This function returns the size of the grid data which is not necessarily
                               equal to the grid resolution if the data is not stored at cell-center.
                               )pbdoc")
        .def_property_readonly("dataOrigin", &ScalarGrid3::GetDataOrigin,
                               R"pbdoc(
                               Returns the origin of the grid data.
                               This function returns data position for the grid point at (0, 0).
                               Note that this is different from `origin()` since `origin()` returns
                               the lower corner point of the bounding box.
                               )pbdoc")
        .def("Clone", &ScalarGrid3::Clone,
             R"pbdoc(Returns the copy of the grid instance.)pbdoc")
        .def("Clear", &ScalarGrid3::Clear,
             R"pbdoc(Clears the contents of the grid.)pbdoc")
        .def(
            "Resize",
            [](ScalarGrid3& instance, pybind11::args args,
               pybind11::kwargs kwargs) {
                Vector3UZ resolution{ 1, 1, 1 };
                Vector3D gridSpacing{ 1, 1, 1 };
                Vector3D gridOrigin{ 0, 0, 0 };

                ParseGridResizeParams(args, kwargs, resolution, gridSpacing,
                                      gridOrigin);
                instance.Resize(resolution, gridSpacing, gridOrigin);
            },
            R"pbdoc(
			Resizes the grid using given parameters.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point at the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc")
        .def(
            "__getitem__",
            [](const ScalarGrid3& instance, pybind11::object obj) -> double {
                if (pybind11::isinstance<pybind11::tuple>(
                        static_cast<pybind11::handle>(obj)))
                {
                    auto tidx = obj.cast<pybind11::tuple>();
                    if (tidx.size() == 3)
                    {
                        return instance(tidx[0].cast<size_t>(),
                                        tidx[1].cast<size_t>(),
                                        tidx[2].cast<size_t>());
                    }
                    else
                    {
                        throw std::invalid_argument(
                            "Size of index tuple must be 3.");
                    }
                }
                else if (pybind11::isinstance<pybind11::list>(
                             static_cast<pybind11::handle>(obj)))
                {
                    auto lidx = obj.cast<pybind11::list>();
                    if (lidx.size() == 3)
                    {
                        return instance(lidx[0].cast<size_t>(),
                                        lidx[1].cast<size_t>(),
                                        lidx[2].cast<size_t>());
                    }
                    else
                    {
                        throw std::invalid_argument(
                            "Size of index list must be 3.");
                    }
                }
                else
                {
                    throw std::invalid_argument(
                        "Input type must be tuple or list");
                }
            },
            R"pbdoc(
			Returns the grid data at given data point.

			Parameters
			----------
			- idx : Data point index (i, j, k).
		)pbdoc",
            pybind11::arg("idx"))
        .def(
            "__setitem__",
            [](ScalarGrid3& instance, pybind11::object obj, double val) {
                if (pybind11::isinstance<pybind11::tuple>(
                        static_cast<pybind11::handle>(obj)))
                {
                    auto tidx = obj.cast<pybind11::tuple>();
                    if (tidx.size() == 3)
                    {
                        instance(tidx[0].cast<size_t>(), tidx[1].cast<size_t>(),
                                 tidx[2].cast<size_t>()) = val;
                    }
                    else
                    {
                        throw std::invalid_argument(
                            "Size of index tuple must be 3.");
                    }
                }
                else if (pybind11::isinstance<pybind11::list>(
                             static_cast<pybind11::handle>(obj)))
                {
                    auto lidx = obj.cast<pybind11::list>();
                    if (lidx.size() == 3)
                    {
                        instance(lidx[0].cast<size_t>(), lidx[1].cast<size_t>(),
                                 lidx[2].cast<size_t>()) = val;
                    }
                    else
                    {
                        throw std::invalid_argument(
                            "Size of index list must be 3.");
                    }
                }
                else
                {
                    throw std::invalid_argument(
                        "Input type must be tuple or list");
                }
            },
            R"pbdoc(
			Sets the grid data at given data point.

			Parameters
			----------
			- idx : Data point index (i, j, k).
			- val : Value to set.
		)pbdoc",
            pybind11::arg("idx"), pybind11::arg("val"))
        .def("GradientAtDataPoint", &ScalarGrid3::GradientAtDataPoint,
             R"pbdoc(
			Returns the gradient vector at given data point.

			Parameters
			----------
			- i : Data point index i.
			- j : Data point index j.
			- k : Data point index k.
		)pbdoc",
             pybind11::arg("i"), pybind11::arg("j"), pybind11::arg("k"))
        .def("LaplacianAtDataPoint", &ScalarGrid3::LaplacianAtDataPoint,
             R"pbdoc(
			Returns the Laplacian at given data point.

			Parameters
			----------
			- i : Data point index i.
			- j : Data point index j.
			- k : Data point index k.
		)pbdoc",
             pybind11::arg("i"), pybind11::arg("j"), pybind11::arg("k"))
        .def("DataView",
             static_cast<ArrayView3<double> (ScalarGrid3::*)()>(
                 &ScalarGrid3::DataView),
             R"pbdoc(Returns the data array view.)pbdoc")
        .def("DataPosition", &ScalarGrid3::DataPosition,
             R"pbdoc(The function that maps data point to its position.)pbdoc")
        .def(
            "Fill",
            [](ScalarGrid3& instance, double value) {
                instance.Fill(value, ExecutionPolicy::Serial);
            },
            R"pbdoc(Fills the grid with given value.)pbdoc")
        .def(
            "Fill",
            [](ScalarGrid3& instance, pybind11::object obj) {
                if (pybind11::isinstance<pybind11::function>(obj))
                {
                    auto func = obj.cast<pybind11::function>();
                    instance.Fill(
                        [func](const Vector3D& pt) -> double {
                            return func(pt).cast<double>();
                        },
                        ExecutionPolicy::Serial);
                }
                else
                {
                    throw std::invalid_argument(
                        "Input type must be double or function object -> "
                        "double");
                }
            },
            R"pbdoc(
			Fills the grid with given function.
		)pbdoc")
        .def(
            "ForEachDataPointIndex",
            [](ScalarGrid3& instance, pybind11::function func) {
                instance.ForEachDataPointIndex(func);
            },
            R"pbdoc(
			Invokes the given function `func` for each data point.

			This function invokes the given function object `func` for each data
			point in serial manner. The input parameters are i and j indices of a
			data point. The order of execution is i-first, j-last.
		)pbdoc",
            pybind11::arg("func"))
        .def(
            "Sample",
            [](const ScalarGrid3& instance, pybind11::object obj) {
                return instance.Sample(ObjectToVector3D(obj));
            },
            R"pbdoc(
			Returns sampled value at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Gradient",
            [](const ScalarGrid3& instance, pybind11::object obj) {
                return instance.Gradient(ObjectToVector3D(obj));
            },
            R"pbdoc(
			Returns the gradient vector at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Laplacian",
            [](const ScalarGrid3& instance, pybind11::object obj) {
                return instance.Laplacian(ObjectToVector3D(obj));
            },
            R"pbdoc(
			Returns the Laplacian value at given position `x`.
		)pbdoc",
            pybind11::arg("x"));
}