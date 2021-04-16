// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Grid/FaceCenteredGrid.hpp>
#include <API/Python/Utils/pybind11Utils.hpp>
#include <Core/Grid/FaceCenteredGrid.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddFaceCenteredGrid2(pybind11::module& m)
{
    using ValueAtCenterFunc =
        Vector2D (FaceCenteredGrid2::*)(const Vector2UZ&) const;
    using DivergenceAtCellCenterFunc =
        double (FaceCenteredGrid2::*)(const Vector2UZ&) const;
    using CurlAtCellCenterFunc =
        double (FaceCenteredGrid2::*)(const Vector2UZ&) const;

    pybind11::class_<FaceCenteredGrid2, FaceCenteredGrid2Ptr, VectorGrid2>(
        static_cast<pybind11::handle>(m), "FaceCenteredGrid2",
        R"pbdoc(
			2-D face-centered (a.k.a MAC or staggered) grid.

			This class implements face-centered grid which is also known as
			marker-and-cell (MAC) or staggered grid. This vector grid stores each vector
			component at face center. Thus, u and v components are not collocated.
		)pbdoc")
        .def(
            "__init__",
            [](FaceCenteredGrid2& instance, pybind11::args args,
               pybind11::kwargs kwargs) {
                Vector2UZ resolution{ 1, 1 };
                Vector2D gridSpacing{ 1, 1 };
                Vector2D gridOrigin{ 0, 0 };

                ParseGridResizeParams(args, kwargs, resolution, gridSpacing,
                                      gridOrigin);
                new (&instance)
                    FaceCenteredGrid2(resolution, gridSpacing, gridOrigin);
            },
            R"pbdoc(
			Constructs grid.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point at the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc")
        .def("Set", &FaceCenteredGrid2::Set,
             R"pbdoc(
			Sets the contents with the given grid.

			This method copies the given grid to this grid.

			Parameters
			----------
			- other : Other grid to copy from.
		)pbdoc",
             pybind11::arg("other"))
        .def(
            "U",
            [](const FaceCenteredGrid2& instance, pybind11::object obj)
                -> double { return instance.U(ObjectToVector2UZ(obj)); },
            R"pbdoc(
			Returns u-value at given data point.

			Parameters
			----------
            - idx : Data point index (i, j).
		)pbdoc",
            pybind11::arg("idx"))
        .def(
            "V",
            [](const FaceCenteredGrid2& instance, pybind11::object obj)
                -> double { return instance.V(ObjectToVector2UZ(obj)); },
            R"pbdoc(
			Returns v-value at given data point.

			Parameters
			----------
            - idx : Data point index (i, j).
		)pbdoc",
            pybind11::arg("idx"))
        .def(
            "SetU",
            [](FaceCenteredGrid2& instance, pybind11::object obj, double val) {
                instance.U(ObjectToVector2UZ(obj)) = val;
            },
            R"pbdoc(
			Sets u-value at given data point.

			Parameters
			----------
            - idx : Data point index (i, j).
			- val : Value to set.
		)pbdoc",
            pybind11::arg("idx"), pybind11::arg("val"))
        .def(
            "SetV",
            [](FaceCenteredGrid2& instance, pybind11::object obj, double val) {
                instance.V(ObjectToVector2UZ(obj)) = val;
            },
            R"pbdoc(
			Sets v-value at given data point.

			Parameters
			----------
            - idx : Data point index (i, j).
			- val : Value to set.
		)pbdoc",
            pybind11::arg("idx"), pybind11::arg("val"))
        .def("ValueAtCellCenter",
             static_cast<ValueAtCenterFunc>(
                 &FaceCenteredGrid2::ValueAtCellCenter),
             R"pbdoc(
			Returns interpolated value at cell center.

			Parameters
			----------
            - idx : Data point index (i, j).
		)pbdoc",
             pybind11::arg("idx"))
        .def("DivergenceAtCellCenter",
             static_cast<DivergenceAtCellCenterFunc>(
                 &FaceCenteredGrid2::DivergenceAtCellCenter),
             R"pbdoc(
			Returns divergence at cell center.

			Parameters
			----------
            - idx : Data point index (i, j).
		)pbdoc",
             pybind11::arg("idx"))
        .def("CurlAtCellCenter",
             static_cast<CurlAtCellCenterFunc>(
                 &FaceCenteredGrid2::CurlAtCellCenter),
             R"pbdoc(
			Returns curl at cell center.

			Parameters
			----------
            - idx : Data point index (i, j).
		)pbdoc",
             pybind11::arg("idx"))
        .def("UView",
             static_cast<ArrayView2<double> (FaceCenteredGrid2::*)()>(
                 &FaceCenteredGrid2::UView),
             R"pbdoc(Returns u data view.)pbdoc")
        .def("VView",
             static_cast<ArrayView2<double> (FaceCenteredGrid2::*)()>(
                 &FaceCenteredGrid2::VView),
             R"pbdoc(Returns v data view.)pbdoc")
        .def("UPosition", &FaceCenteredGrid2::UPosition,
             R"pbdoc(
			The function object that maps u data point to its actual position.
		)pbdoc")
        .def("VPosition", &FaceCenteredGrid2::VPosition,
             R"pbdoc(
			The function object that maps v data point to its actual position.
		)pbdoc")
        .def("USize", &FaceCenteredGrid2::USize,
             R"pbdoc(
			Returns data size of the u component.
		)pbdoc")
        .def("VSize", &FaceCenteredGrid2::VSize,
             R"pbdoc(
			Returns data size of the v component.
		)pbdoc")
        .def("UOrigin", &FaceCenteredGrid2::UOrigin,
             R"pbdoc(
			Returns u-data position for the grid point at (0, 0).

			Note that this is different from origin() since origin() returns
			the lower corner point of the bounding box.
		)pbdoc")
        .def("VOrigin", &FaceCenteredGrid2::VOrigin,
             R"pbdoc(
			Returns v-data position for the grid point at (0, 0).

			Note that this is different from origin() since origin() returns
			the lower corner point of the bounding box.
		)pbdoc")
        .def(
            "Fill",
            [](FaceCenteredGrid2& instance, pybind11::object obj) {
                if (pybind11::isinstance<Vector2D>(obj))
                {
                    instance.Fill(obj.cast<Vector2D>());
                }
                else if (pybind11::isinstance<pybind11::tuple>(obj))
                {
                    instance.Fill(ObjectToVector2D(obj));
                }
                else if (pybind11::isinstance<pybind11::function>(obj))
                {
                    auto func = obj.cast<pybind11::function>();
                    instance.Fill(
                        [func](const Vector2D& pt) {
                            return ObjectToVector2D(func(pt));
                        },
                        ExecutionPolicy::Serial);
                }
                else
                {
                    throw std::invalid_argument(
                        "Input type must be Vector2D or function object -> "
                        "Vector2D");
                }
            },
            R"pbdoc(
			Fills the grid with given value or function.
		)pbdoc")
        .def(
            "ForEachUIndex",
            [](FaceCenteredGrid2& instance, pybind11::function func) {
                instance.ForEachUIndex(func);
            },
            R"pbdoc(
			Invokes the given function func for each u-data point.

			This function invokes the given function object `func` for each u-data
			point in serial manner. The input parameters are i and j indices of a
			u-data point. The order of execution is i-first, j-last.
		)pbdoc",
            pybind11::arg("func"))
        .def(
            "ForEachVIndex",
            [](FaceCenteredGrid2& instance, pybind11::function func) {
                instance.ForEachVIndex(func);
            },
            R"pbdoc(
			Invokes the given function func for each v-data point.

			This function invokes the given function object `func` for each v-data
			point in serial manner. The input parameters are i and j indices of a
			v-data point. The order of execution is i-first, j-last.
		)pbdoc",
            pybind11::arg("func"))
        .def(
            "Sample",
            [](const FaceCenteredGrid2& instance, pybind11::object obj) {
                return instance.Sample(ObjectToVector2D(obj));
            },
            R"pbdoc(
			Returns sampled value at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Divergence",
            [](const FaceCenteredGrid2& instance, pybind11::object obj) {
                return instance.Divergence(ObjectToVector2D(obj));
            },
            R"pbdoc(
			Returns divergence at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Curl",
            [](const FaceCenteredGrid2& instance, pybind11::object obj) {
                return instance.Curl(ObjectToVector2D(obj));
            },
            R"pbdoc(
			Returns curl at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def("Sampler", &FaceCenteredGrid2::Sampler,
             R"pbdoc(
			Returns the sampler function.

			This function returns the data sampler function object. The sampling
			function is linear.
		)pbdoc");
}

void AddFaceCenteredGrid3(pybind11::module& m)
{
    using ValueAtCenterFunc =
        Vector3D (FaceCenteredGrid3::*)(const Vector3UZ&) const;
    using DivergenceAtCellCenterFunc =
        double (FaceCenteredGrid3::*)(const Vector3UZ&) const;
    using CurlAtCellCenterFunc =
        Vector3D (FaceCenteredGrid3::*)(const Vector3UZ&) const;
    using PositionFunc = GridDataPositionFunc<3> (FaceCenteredGrid3::*)() const;
    using SizeFunc = Vector3UZ (FaceCenteredGrid3::*)() const;
    using OriginFunc = Vector3D (FaceCenteredGrid3::*)() const;

    pybind11::class_<FaceCenteredGrid3, FaceCenteredGrid3Ptr, VectorGrid3>(
        static_cast<pybind11::handle>(m), "FaceCenteredGrid3",
        R"pbdoc(
			3-D face-centered (a.k.a MAC or staggered) grid.

			This class implements face-centered grid which is also known as
			marker-and-cell (MAC) or staggered grid. This vector grid stores each vector
			component at face center. Thus, u, v, and w components are not collocated.
		)pbdoc")
        .def(
            "__init__",
            [](FaceCenteredGrid3& instance, pybind11::args args,
               pybind11::kwargs kwargs) {
                Vector3UZ resolution{ 1, 1, 1 };
                Vector3D gridSpacing{ 1, 1, 1 };
                Vector3D gridOrigin{ 0, 0, 0 };

                ParseGridResizeParams(args, kwargs, resolution, gridSpacing,
                                      gridOrigin);
                new (&instance)
                    FaceCenteredGrid3(resolution, gridSpacing, gridOrigin);
            },
            R"pbdoc(
			Constructs grid.

			Parameters
			----------
			- `*args` : resolution, gridSpacing, and gridOrigin arguments.
			- `**kwargs`
				- resolution : Grid resolution.
				- gridSpacing : Grid spacing.
				- gridOrigin : Origin point ot the grid.
				- domainSizeX : Domain size in x-direction.
		)pbdoc")
        .def("Set", &FaceCenteredGrid3::Set,
             R"pbdoc(
			Sets the contents with the given grid.

			This method copies the given grid to this grid.

			Parameters
			----------
			- other : Other grid to copy from.
		)pbdoc",
             pybind11::arg("other"))
        .def(
            "U",
            [](const FaceCenteredGrid3& instance, pybind11::object obj)
                -> double { return instance.U(ObjectToVector3UZ(obj)); },
            R"pbdoc(
			Returns u-value at given data point.

			Parameters
			----------
            - idx : Data point index (i, j, k).
		)pbdoc",
            pybind11::arg("idx"))
        .def(
            "V",
            [](const FaceCenteredGrid3& instance, pybind11::object obj)
                -> double { return instance.V(ObjectToVector3UZ(obj)); },
            R"pbdoc(
			Returns v-value at given data point.

			Parameters
			----------
            - idx : Data point index (i, j, k).
		)pbdoc",
            pybind11::arg("idx"))
        .def(
            "W",
            [](const FaceCenteredGrid3& instance, pybind11::object obj)
                -> double { return instance.W(ObjectToVector3UZ(obj)); },
            R"pbdoc(
			Returns v-value at given data point.

			Parameters
			----------
			- idx : Data point index (i, j, k).
		)pbdoc",
            pybind11::arg("idx"))
        .def(
            "SetU",
            [](FaceCenteredGrid3& instance, pybind11::object obj, double val) {
                instance.U(ObjectToVector3UZ(obj)) = val;
            },
            R"pbdoc(
			Sets u-value at given data point.

			Parameters
			----------
            - idx : Data point index (i, j, k).
			- val : Value to set.
		)pbdoc",
            pybind11::arg("idx"), pybind11::arg("val"))
        .def(
            "SetV",
            [](FaceCenteredGrid3& instance, pybind11::object obj, double val) {
                instance.V(ObjectToVector3UZ(obj)) = val;
            },
            R"pbdoc(
			Sets v-value at given data point.

			Parameters
			----------
            - idx : Data point index (i, j, k).
			- val : Value to set.
		)pbdoc",
            pybind11::arg("idx"), pybind11::arg("val"))
        .def(
            "SetW",
            [](FaceCenteredGrid3& instance, pybind11::object obj, double val) {
                instance.W(ObjectToVector3UZ(obj)) = val;
            },
            R"pbdoc(
			Sets w-value at given data point.

			Parameters
			----------
            - idx : Data point index (i, j, k).
			- val : Value to set.
		)pbdoc",
            pybind11::arg("idx"), pybind11::arg("val"))
        .def("ValueAtCellCenter",
             static_cast<ValueAtCenterFunc>(
                 &FaceCenteredGrid3::ValueAtCellCenter),
             R"pbdoc(
			Returns interpolated value at cell center.

			Parameters
			----------
			- idx : Data point index (i, j, k).
		)pbdoc",
             pybind11::arg("idx"))
        .def("DivergenceAtCellCenter",
             static_cast<DivergenceAtCellCenterFunc>(
                 &FaceCenteredGrid3::DivergenceAtCellCenter),
             R"pbdoc(
			Returns divergence at cell center.

			Parameters
			----------
            - idx : Data point index (i, j, k).
		)pbdoc",
             pybind11::arg("idx"))
        .def("CurlAtCellCenter",
             static_cast<CurlAtCellCenterFunc>(
                 &FaceCenteredGrid3::CurlAtCellCenter),
             R"pbdoc(
			Returns curl at cell center.

			Parameters
			----------
            - idx : Data point index (i, j, k).
		)pbdoc",
             pybind11::arg("idx"))
        .def("UView",
             static_cast<ArrayView3<double> (FaceCenteredGrid3::*)()>(
                 &FaceCenteredGrid3::UView),
             R"pbdoc(Returns u data view.)pbdoc")
        .def("VView",
             static_cast<ArrayView3<double> (FaceCenteredGrid3::*)()>(
                 &FaceCenteredGrid3::VView),
             R"pbdoc(Returns v data view.)pbdoc")
        .def("WView",
             static_cast<ArrayView3<double> (FaceCenteredGrid3::*)()>(
                 &FaceCenteredGrid3::WView),
             R"pbdoc(Returns w data view.)pbdoc")
        .def("UPosition", &FaceCenteredGrid3::UPosition,
             R"pbdoc(
			The function object that maps u data point to its actual position.
		)pbdoc")
        .def("VPosition", &FaceCenteredGrid3::VPosition,
             R"pbdoc(
			The function object that maps v data point to its actual position.
		)pbdoc")
        .def("WPosition",
             static_cast<PositionFunc>(&FaceCenteredGrid3::WPosition),
             R"pbdoc(
			The function object that maps w data point to its actual position.
		)pbdoc")
        .def("USize", &FaceCenteredGrid3::USize,
             R"pbdoc(
			Returns data size of the u component.
		)pbdoc")
        .def("VSize", &FaceCenteredGrid3::VSize,
             R"pbdoc(
			Returns data size of the v component.
		)pbdoc")
        .def("WSize", static_cast<SizeFunc>(&FaceCenteredGrid3::WSize),
             R"pbdoc(
			Returns data size of the w component.
		)pbdoc")
        .def("UOrigin", &FaceCenteredGrid3::UOrigin,
             R"pbdoc(
			Returns u-data position for the grid point at (0, 0).

			Note that this is different from origin() since origin() returns
			the lower corner point of the bounding box.
		)pbdoc")
        .def("VOrigin", &FaceCenteredGrid3::VOrigin,
             R"pbdoc(
			Returns v-data position for the grid point at (0, 0).

			Note that this is different from origin() since origin() returns
			the lower corner point of the bounding box.
		 )pbdoc")
        .def("WOrigin", static_cast<OriginFunc>(&FaceCenteredGrid3::WOrigin),
             R"pbdoc(
			Returns w-data position for the grid point at (0, 0).

			Note that this is different from origin() since origin() returns
			the lower corner point of the bounding box.
		)pbdoc")
        .def(
            "Fill",
            [](FaceCenteredGrid3& instance, pybind11::object obj) {
                if (pybind11::isinstance<Vector3D>(obj))
                {
                    instance.Fill(obj.cast<Vector3D>());
                }
                else if (pybind11::isinstance<pybind11::tuple>(obj))
                {
                    instance.Fill(ObjectToVector3D(obj));
                }
                else if (pybind11::isinstance<pybind11::function>(obj))
                {
                    auto func = obj.cast<pybind11::function>();
                    instance.Fill(
                        [func](const Vector3D& pt) {
                            return ObjectToVector3D(func(pt));
                        },
                        ExecutionPolicy::Serial);
                }
                else
                {
                    throw std::invalid_argument(
                        "Input type must be Vector3D or function object -> "
                        "Vector3D");
                }
            },
            R"pbdoc(
			Fills the grid with given value or function.
		)pbdoc")
        .def(
            "ForEachUIndex",
            [](FaceCenteredGrid3& instance, pybind11::function func) {
                instance.ForEachUIndex(func);
            },
            R"pbdoc(
			Invokes the given function func for each u-data point.

			This function invokes the given function object `func` for each u-data
			point in serial manner. The input parameters are i, j, and k indices of a
			u-data point. The order of execution is i-first, k-last.
		)pbdoc",
            pybind11::arg("func"))
        .def(
            "ForEachVIndex",
            [](FaceCenteredGrid3& instance, pybind11::function func) {
                instance.ForEachVIndex(func);
            },
            R"pbdoc(
			Invokes the given function func for each v-data point.

			This function invokes the given function object `func` for each v-data
			point in serial manner. The input parameters are i, j, and k indices of a
			u-data point. The order of execution is i-first, k-last.
		)pbdoc",
            pybind11::arg("func"))
        .def(
            "ForEachWIndex",
            [](FaceCenteredGrid3& instance, pybind11::function func) {
                instance.ForEachWIndex(func);
            },
            R"pbdoc(
			Invokes the given function func for each w-data point.

			This function invokes the given function object `func` for each w-data
			point in serial manner. The input parameters are i, j, and k indices of a
			u-data point. The order of execution is i-first, k-last.
		)pbdoc",
            pybind11::arg("func"))
        .def(
            "Sample",
            [](const FaceCenteredGrid3& instance, pybind11::object obj) {
                return instance.Sample(ObjectToVector3D(obj));
            },
            R"pbdoc(
			Returns sampled value at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Divergence",
            [](const FaceCenteredGrid3& instance, pybind11::object obj) {
                return instance.Divergence(ObjectToVector3D(obj));
            },
            R"pbdoc(
			Returns divergence at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def(
            "Curl",
            [](const FaceCenteredGrid3& instance, pybind11::object obj) {
                return instance.Curl(ObjectToVector3D(obj));
            },
            R"pbdoc(
			Returns curl at given position `x`.
		)pbdoc",
            pybind11::arg("x"))
        .def("Sampler", &FaceCenteredGrid3::Sampler,
             R"pbdoc(
			Returns the sampler function.

			This function returns the data sampler function object. The sampling
			function is linear.
		)pbdoc");
}