// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <API/Python/Array/ArrayView.hpp>
#include <Core/Array/ArrayView.hpp>
#include <Core/Matrix/Matrix.hpp>

#include <pybind11/pybind11.h>

using namespace CubbyFlow;

template <typename T>
void AddArrayView1Scalar(pybind11::module m, const char* name)
{
    pybind11::class_<ArrayView1<T>>(m, name, pybind11::buffer_protocol())
        .def_buffer([](ArrayView1<T>& m) -> pybind11::buffer_info {
            return pybind11::buffer_info(
                m.data(), sizeof(T), pybind11::format_descriptor<T>::format(),
                1, { m.Length() }, { sizeof(T) });
        });
};

template <typename T, size_t N>
void AddArrayView1Vector(pybind11::module m, const char* name)
{
    pybind11::class_<ArrayView1<Vector<T, N>>>(m, name,
                                               pybind11::buffer_protocol())
        .def_buffer([](ArrayView1<Vector<T, N>>& m) -> pybind11::buffer_info {
            return pybind11::buffer_info(
                m.data(), sizeof(T), pybind11::format_descriptor<T>::format(),
                2, { m.Length(), N }, { sizeof(T) * N, sizeof(T) });
        });
};

template <typename T>
void AddArrayView2Scalar(pybind11::module m, const char* name)
{
    pybind11::class_<ArrayView2<T>>(m, name, pybind11::buffer_protocol())
        .def_buffer([](ArrayView2<T>& m) -> pybind11::buffer_info {
            return pybind11::buffer_info(
                m.data(), sizeof(T), pybind11::format_descriptor<T>::format(),
                2, { m.Height(), m.Width() },
                { sizeof(T) * m.Width(), sizeof(T) });
        });
};

template <typename T>
void AddArrayView3Scalar(pybind11::module m, const char* name)
{
    pybind11::class_<ArrayView3<T>>(m, name, pybind11::buffer_protocol())
        .def_buffer([](ArrayView3<T>& m) -> pybind11::buffer_info {
            return pybind11::buffer_info(
                m.data(), sizeof(T), pybind11::format_descriptor<T>::format(),
                3, { m.Depth(), m.Height(), m.Width() },
                { sizeof(T) * m.Width() * m.Height(), sizeof(T) * m.Width(),
                  sizeof(T) });
        });
};

void AddArrayView1(pybind11::module& m)
{
    AddArrayView1Scalar<double>(m, "ArrayView1D");
    AddArrayView1Vector<double, 2>(m, "ArrayView1Vector2D");
    AddArrayView1Vector<double, 3>(m, "ArrayView1Vector3D");
    AddArrayView1Vector<double, 4>(m, "ArrayView1Vector4D");
    AddArrayView1Scalar<float>(m, "ArrayView1F");
    AddArrayView1Vector<float, 2>(m, "ArrayView1Vector2F");
    AddArrayView1Vector<float, 3>(m, "ArrayView1Vector3F");
    AddArrayView1Vector<float, 4>(m, "ArrayView1Vector4F");
}

void AddArrayView2(pybind11::module& m)
{
    AddArrayView2Scalar<double>(m, "ArrayView2D");
    AddArrayView2Scalar<float>(m, "ArrayView2F");
}

void AddArrayView3(pybind11::module& m)
{
    AddArrayView3Scalar<double>(m, "ArrayView3D");
    AddArrayView3Scalar<float>(m, "ArrayView3F");
}