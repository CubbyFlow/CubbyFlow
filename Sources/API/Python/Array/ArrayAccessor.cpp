/*************************************************************************
> File Name: ArrayAccessor.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: ArrayAccessor functions for CubbyFlow Python API.
> Created Time: 2018/01/26
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#include <API/Python/Array/ArrayAccessor.h>
#include <Core/Array/ArrayAccessor1.h>
#include <Core/Array/ArrayAccessor2.h>
#include <Core/Array/ArrayAccessor3.h>
#include <Core/Vector/Vector2.h>
#include <Core/Vector/Vector3.h>
#include <Core/Vector/Vector4.h>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

template <typename T>
void AddArrayAccessor1Scalar(pybind11::module m, const char* name)
{
	pybind11::class_<ArrayAccessor1<T>>(m, name, pybind11::buffer_protocol())
		.def_buffer([](ArrayAccessor1<T>& m) -> pybind11::buffer_info
	{
		return pybind11::buffer_info(m.data(), sizeof(T),
			pybind11::format_descriptor<T>::format(), 1,
			{ m.size() },
			{ sizeof(T) });
	});
};

template <typename T, size_t N>
void AddArrayAccessor1Vector(pybind11::module m, const char* name)
{
	pybind11::class_<ArrayAccessor1<Vector<T, N>>>(m, name, pybind11::buffer_protocol())
		.def_buffer([](ArrayAccessor1<Vector<T, N>>& m) -> pybind11::buffer_info
	{
		return pybind11::buffer_info(m.data(), sizeof(T),
			pybind11::format_descriptor<T>::format(), 2,
			{ m.size(), N },
			{ sizeof(T) * N, sizeof(T) });
	});
};

template <typename T>
void AddArrayAccessor2Scalar(pybind11::module m, const char* name)
{
	pybind11::class_<ArrayAccessor2<T>>(m, name, pybind11::buffer_protocol())
		.def_buffer([](ArrayAccessor2<T>& m) -> pybind11::buffer_info
	{
		return pybind11::buffer_info(m.data(), sizeof(T),
			pybind11::format_descriptor<T>::format(), 2,
			{ m.Height(), m.Width() },
			{ sizeof(T) * m.Width(), sizeof(T) });
	});
};

template <typename T>
void AddArrayAccessor3Scalar(pybind11::module m, const char* name)
{
	pybind11::class_<ArrayAccessor3<T>>(m, name, pybind11::buffer_protocol())
		.def_buffer([](ArrayAccessor3<T>& m) -> pybind11::buffer_info
	{
		return pybind11::buffer_info(m.data(), sizeof(T),
			pybind11::format_descriptor<T>::format(), 3,
			{ m.Depth(), m.Height(), m.Width() },
			{ sizeof(T) * m.Width() * m.Height(), sizeof(T) * m.Width(), sizeof(T) });
	});
};

void AddArrayAccessor1(pybind11::module& m)
{
	AddArrayAccessor1Scalar<double>(m, "ArrayAccessor1D");
	AddArrayAccessor1Vector<double, 2>(m, "ArrayAccessor1Vector2D");
	AddArrayAccessor1Vector<double, 3>(m, "ArrayAccessor1Vector3D");
	AddArrayAccessor1Vector<double, 4>(m, "ArrayAccessor1Vector4D");
	AddArrayAccessor1Scalar<float>(m, "ArrayAccessor1F");
	AddArrayAccessor1Vector<float, 2>(m, "ArrayAccessor1Vector2F");
	AddArrayAccessor1Vector<float, 3>(m, "ArrayAccessor1Vector3F");
	AddArrayAccessor1Vector<float, 4>(m, "ArrayAccessor1Vector4F");
}

void AddArrayAccessor2(pybind11::module& m)
{
	AddArrayAccessor2Scalar<double>(m, "ArrayAccessor2D");
	AddArrayAccessor2Scalar<float>(m, "ArrayAccessor2F");
}

void AddArrayAccessor3(pybind11::module& m)
{
	AddArrayAccessor3Scalar<double>(m, "ArrayAccessor3D");
	AddArrayAccessor3Scalar<float>(m, "ArrayAccessor3F");
}