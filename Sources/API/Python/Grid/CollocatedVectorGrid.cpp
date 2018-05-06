/*************************************************************************
> File Name: CollocatedVectorGrid.cpp
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: CollocatedVectorGrid functions for CubbyFlow Python API.
> Created Time: 2018/02/05
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Grid/CollocatedVectorGrid.h>
#include <API/Python/Utils/pybind11Utils.h>
#include <Core/Grid/CollocatedVectorGrid2.h>
#include <Core/Grid/CollocatedVectorGrid3.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

void AddCollocatedVectorGrid2(pybind11::module& m)
{
	pybind11::class_<CollocatedVectorGrid2, CollocatedVectorGrid2Ptr, VectorGrid2>(static_cast<pybind11::handle>(m), "CollocatedVectorGrid2",
		R"pbdoc(
			Abstract base class for 2-D collocated vector grid structure.
		)pbdoc")
	.def("__getitem__", [](const CollocatedVectorGrid2& instance, pybind11::object obj) -> Vector2D
	{
		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			auto tidx = obj.cast<pybind11::tuple>();
			if (tidx.size() == 2)
			{
				return instance(tidx[0].cast<size_t>(), tidx[1].cast<size_t>());
			}
			else
			{
				throw std::invalid_argument("Size of index tuple must be 2.");
			}
		}
		else if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			auto lidx = obj.cast<pybind11::list>();
			if (lidx.size() == 2)
			{
				return instance(lidx[0].cast<size_t>(), lidx[1].cast<size_t>());
			}
			else
			{
				throw std::invalid_argument("Size of index list must be 2.");
			}
		}
		else
		{
			throw std::invalid_argument("Input type must be tuple or list");
		}
	},
		R"pbdoc(
			Returns the grid data at given data point.

			Parameters
			----------
			- idx : Data point index (i, j).
		)pbdoc",
		pybind11::arg("idx"))
	.def("__setitem__", [](CollocatedVectorGrid2& instance, pybind11::object obj, const Vector2D& val)
	{
		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			auto tidx = obj.cast<pybind11::tuple>();
			if (tidx.size() == 2)
			{
				instance(tidx[0].cast<size_t>(), tidx[1].cast<size_t>()) = val;
			}
			else
			{
				throw std::invalid_argument("Size of index tuple must be 2.");
			}
		}
		else if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			auto lidx = obj.cast<pybind11::list>();
			if (lidx.size() == 2)
			{
				instance(lidx[0].cast<size_t>(), lidx[1].cast<size_t>()) = val;
			}
			else
			{
				throw std::invalid_argument("Size of index list must be 2.");
			}
		}
		else
		{
			throw std::invalid_argument("Input type must be tuple or list");
		}
	},
		R"pbdoc(
			Sets the grid data at given data point.

			Parameters
			----------
			- idx : Data point index (i, j).
			- val : Value to set.
		)pbdoc",
		pybind11::arg("idx"),
		pybind11::arg("val"))
	.def("DivergenceAtDataPoint", &CollocatedVectorGrid2::DivergenceAtDataPoint,
		R"pbdoc(
			Returns divergence at data point location.

			Parameters
			----------
			- i : Data point index i.
			- j : Data point index j.
		)pbdoc",
		pybind11::arg("i"),
		pybind11::arg("j"))
	.def("CurlAtDataPoint", &CollocatedVectorGrid2::CurlAtDataPoint,
		R"pbdoc(
			Returns curl at data point location.

			Parameters
			----------
			- i : Data point index i.
			- j : Data point index j.
		)pbdoc",
		pybind11::arg("i"),
		pybind11::arg("j"))
	.def("GetDataAccessor", &CollocatedVectorGrid2::GetDataAccessor,
		R"pbdoc(
			Returns the data array accessor.
		)pbdoc")
	.def("GetDataPosition", &CollocatedVectorGrid2::GetDataPosition,
		R"pbdoc(
			Returns the function that maps data point to its position.
		)pbdoc")
	.def("ForEachDataPointIndex", [](CollocatedVectorGrid2& instance, pybind11::function func)
	{
		instance.ForEachDataPointIndex(func);
	},
		R"pbdoc(
			Invokes the given function `func` for each data point.

			This function invokes the given function object `func` for each data
			point in serial manner. The input parameters are i and j indices of a
			data point. The order of execution is i-first, j-last.
		)pbdoc",
		pybind11::arg("func"))
	.def("Sample", [](const CollocatedVectorGrid2& instance, pybind11::object obj)
	{
		return instance.Sample(ObjectToVector2D(obj));
	},
		R"pbdoc(
			Returns sampled value at given position `x`.
		)pbdoc",
		pybind11::arg("x"))
	.def("Divergence", [](const CollocatedVectorGrid2& instance, pybind11::object obj)
	{
		return instance.Divergence(ObjectToVector2D(obj));
	},
		R"pbdoc(
			Returns divergence at given position `x`.
		)pbdoc",
		pybind11::arg("x"))
	.def("Curl", [](const CollocatedVectorGrid2& instance, pybind11::object obj)
	{
		return instance.Curl(ObjectToVector2D(obj));
	},
		R"pbdoc(
			Returns curl at given position `x`.
		)pbdoc",
		pybind11::arg("x"))
	.def("Sampler", [](const CollocatedVectorGrid2& instance)
	{
		return instance.Sampler();
	},
		R"pbdoc(
			Returns the sampler function.

			This function returns the data sampler function object. The sampling
			function is linear.
		)pbdoc");
}

void AddCollocatedVectorGrid3(pybind11::module& m)
{
	pybind11::class_<CollocatedVectorGrid3, CollocatedVectorGrid3Ptr, VectorGrid3>(static_cast<pybind11::handle>(m), "CollocatedVectorGrid3",
		R"pbdoc(
			Abstract base class for 3-D collocated vector grid structure.
		)pbdoc")
	.def("__getitem__", [](const CollocatedVectorGrid3& instance, pybind11::object obj) -> Vector3D
	{
		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			auto tidx = obj.cast<pybind11::tuple>();
			if (tidx.size() == 3)
			{
				return instance(tidx[0].cast<size_t>(), tidx[1].cast<size_t>(), tidx[2].cast<size_t>());
			}
			else
			{
				throw std::invalid_argument("Size of index tuple must be 3.");
			}
		}
		else if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			auto lidx = obj.cast<pybind11::list>();
			if (lidx.size() == 3)
			{
				return instance(lidx[0].cast<size_t>(), lidx[1].cast<size_t>(), lidx[2].cast<size_t>());
			}
			else
			{
				throw std::invalid_argument("Size of index list must be 3.");
			}
		}
		else
		{
			throw std::invalid_argument("Input type must be tuple or list");
		}
	},
		R"pbdoc(
			Returns the grid data at given data point.

			Parameters
			----------
			- idx : Data point index (i, j, k).
		)pbdoc",
		pybind11::arg("idx"))
	.def("__setitem__", [](CollocatedVectorGrid3& instance, pybind11::object obj, const Vector3D& val)
	{
		if (pybind11::isinstance<pybind11::tuple>(static_cast<pybind11::handle>(obj)))
		{
			auto tidx = obj.cast<pybind11::tuple>();
			if (tidx.size() == 3)
			{
				instance(tidx[0].cast<size_t>(), tidx[1].cast<size_t>(), tidx[2].cast<size_t>()) = val;
			}
			else
			{
				throw std::invalid_argument("Size of index tuple must be 3.");
			}
		}
		else if (pybind11::isinstance<pybind11::list>(static_cast<pybind11::handle>(obj)))
		{
			auto lidx = obj.cast<pybind11::list>();
			if (lidx.size() == 3)
			{
				instance(lidx[0].cast<size_t>(), lidx[1].cast<size_t>(), lidx[2].cast<size_t>()) = val;
			}
			else
			{
				throw std::invalid_argument("Size of index list must be 3.");
			}
		}
		else
		{
			throw std::invalid_argument("Input type must be tuple or list");
		}
	},
		R"pbdoc(
			Sets the grid data at given data point.

			Parameters
			----------
			- idx : Data point index (i, j, k).
			- val : Value to set.
		)pbdoc",
		pybind11::arg("idx"),
		pybind11::arg("val"))
	.def("DivergenceAtDataPoint", &CollocatedVectorGrid3::DivergenceAtDataPoint,
		R"pbdoc(
			Returns divergence at data point location.

			Parameters
			----------
			- i : Data point index i.
			- j : Data point index j.
			- k : Data point index k.
		)pbdoc",
		pybind11::arg("i"),
		pybind11::arg("j"),
		pybind11::arg("k"))
	.def("CurlAtDataPoint", &CollocatedVectorGrid3::CurlAtDataPoint,
		R"pbdoc(
			Returns curl at data point location.

			Parameters
			----------
			- i : Data point index i.
			- j : Data point index j.
			- k : Data point index k.
		)pbdoc",
		pybind11::arg("i"),
		pybind11::arg("j"),
		pybind11::arg("k"))
	.def("GetDataAccessor", &CollocatedVectorGrid3::GetDataAccessor,
		R"pbdoc(
			Returns the data array accessor.
		)pbdoc")
	.def("GetDataPosition", &CollocatedVectorGrid3::GetDataPosition,
		R"pbdoc(
			Returns the function that maps data point to its position.
		)pbdoc")
	.def("ForEachDataPointIndex", [](CollocatedVectorGrid3& instance, pybind11::function func)
	{
		instance.ForEachDataPointIndex(func);
	},
		R"pbdoc(
			Invokes the given function `func` for each data point.

			This function invokes the given function object `func` for each data
			point in serial manner. The input parameters are i and j indices of a
			data point. The order of execution is i-first, j-last.
		)pbdoc",
		pybind11::arg("func"))
	.def("Sample", [](const CollocatedVectorGrid3& instance, pybind11::object obj)
	{
		return instance.Sample(ObjectToVector3D(obj));
	},
		R"pbdoc(
			Returns sampled value at given position `x`.
		)pbdoc",
		pybind11::arg("x"))
	.def("Divergence", [](const CollocatedVectorGrid3& instance, pybind11::object obj)
	{
		return instance.Divergence(ObjectToVector3D(obj));
	},
		R"pbdoc(
			Returns divergence at given position `x`.
		)pbdoc",
		pybind11::arg("x"))
	.def("Curl", [](const CollocatedVectorGrid3& instance, pybind11::object obj)
	{
		return instance.Curl(ObjectToVector3D(obj));
	},
		R"pbdoc(
			Returns curl at given position `x`.
		)pbdoc",
		pybind11::arg("x"))
	.def("Sampler", [](const CollocatedVectorGrid3& instance)
	{
		return instance.Sampler();
	},
		R"pbdoc(
			Returns the sampler function.

			This function returns the data sampler function object. The sampling
			function is linear.
		)pbdoc");
}