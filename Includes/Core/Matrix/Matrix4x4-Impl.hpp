// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MATRIX4X4_IMPL_HPP
#define CUBBYFLOW_MATRIX4X4_IMPL_HPP

#include <cassert>

namespace CubbyFlow
{
template <typename T>
Matrix<T, 4, 4>::Matrix()
{
    Set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

template <typename T>
Matrix<T, 4, 4>::Matrix(T s)
{
    Set(s);
}

template <typename T>
Matrix<T, 4, 4>::Matrix(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21,
                        T m22)
{
    Set(m00, m01, m02, m10, m11, m12, m20, m21, m22);
}

template <typename T>
Matrix<T, 4, 4>::Matrix(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13,
                        T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33)
{
    Set(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31,
        m32, m33);
}

template <typename T>
template <typename U>
Matrix<T, 4, 4>::Matrix(
    const std::initializer_list<std::initializer_list<U>>& list)
{
    Set(list);
}

template <typename T>
Matrix<T, 4, 4>::Matrix(const Matrix3x3<T>& m33)
{
    Set(m33);
}

template <typename T>
Matrix<T, 4, 4>::Matrix(const T* arr)
{
    Set(arr);
}

template <typename T>
Matrix<T, 4, 4>::Matrix(const Matrix& m)
{
    Set(m);
}

template <typename T>
Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator=(const Matrix& m)
{
    Set(m);
    return *this;
}

template <typename T>
void Matrix<T, 4, 4>::Set(T s)
{
    m_elements[0] = s;
    m_elements[1] = s;
    m_elements[2] = s;
    m_elements[3] = s;
    m_elements[4] = s;
    m_elements[5] = s;
    m_elements[6] = s;
    m_elements[7] = s;
    m_elements[8] = s;
    m_elements[9] = s;
    m_elements[10] = s;
    m_elements[11] = s;
    m_elements[12] = s;
    m_elements[13] = s;
    m_elements[14] = s;
    m_elements[15] = s;
}

template <typename T>
void Matrix<T, 4, 4>::Set(T m00, T m01, T m02, T m10, T m11, T m12, T m20,
                          T m21, T m22)
{
    m_elements[0] = m00;
    m_elements[1] = m01;
    m_elements[2] = m02;
    m_elements[3] = 0;
    m_elements[4] = m10;
    m_elements[5] = m11;
    m_elements[6] = m12;
    m_elements[7] = 0;
    m_elements[8] = m20;
    m_elements[9] = m21;
    m_elements[10] = m22;
    m_elements[11] = 0;
    m_elements[12] = 0;
    m_elements[13] = 0;
    m_elements[14] = 0;
    m_elements[15] = 1;
}

template <typename T>
void Matrix<T, 4, 4>::Set(T m00, T m01, T m02, T m03, T m10, T m11, T m12,
                          T m13, T m20, T m21, T m22, T m23, T m30, T m31,
                          T m32, T m33)
{
    m_elements[0] = m00;
    m_elements[1] = m01;
    m_elements[2] = m02;
    m_elements[3] = m03;
    m_elements[4] = m10;
    m_elements[5] = m11;
    m_elements[6] = m12;
    m_elements[7] = m13;
    m_elements[8] = m20;
    m_elements[9] = m21;
    m_elements[10] = m22;
    m_elements[11] = m23;
    m_elements[12] = m30;
    m_elements[13] = m31;
    m_elements[14] = m32;
    m_elements[15] = m33;
}

template <typename T>
template <typename U>
void Matrix<T, 4, 4>::Set(
    const std::initializer_list<std::initializer_list<U>>& list)
{
    const size_t height = list.size();
    const size_t width = (height > 0) ? list.begin()->size() : 0;

    assert(width == 4);
    assert(height == 4);

    auto rowIter = list.begin();
    for (size_t i = 0; i < height; ++i)
    {
        assert(width == rowIter->size());

        auto colIter = rowIter->begin();
        for (size_t j = 0; j < width; ++j)
        {
            (*this)(i, j) = static_cast<T>(*colIter);
            ++colIter;
        }

        ++rowIter;
    }
}

template <typename T>
void Matrix<T, 4, 4>::Set(const Matrix<T, 3, 3>& m33)
{
    Set(m33[0], m33[1], m33[2], 0, m33[3], m33[4], m33[5], 0, m33[6], m33[7],
        m33[8], 0, 0, 0, 0, 1);
}

template <typename T>
void Matrix<T, 4, 4>::Set(const Matrix& m)
{
    for (size_t i = 0; i < 16; ++i)
    {
        m_elements[i] = m.m_elements[i];
    }
}

template <typename T>
void Matrix<T, 4, 4>::Set(const T* arr)
{
    for (size_t i = 0; i < 16; ++i)
    {
        m_elements[i] = arr[i];
    }
}

template <typename T>
void Matrix<T, 4, 4>::SetDiagonal(T s)
{
    m_elements[0] = s;
    m_elements[5] = s;
    m_elements[10] = s;
    m_elements[15] = s;
}

template <typename T>
void Matrix<T, 4, 4>::SetOffDiagonal(T s)
{
    m_elements[1] = s;
    m_elements[2] = s;
    m_elements[3] = s;
    m_elements[4] = s;
    m_elements[6] = s;
    m_elements[7] = s;
    m_elements[8] = s;
    m_elements[9] = s;
    m_elements[11] = s;
    m_elements[12] = s;
    m_elements[13] = s;
    m_elements[14] = s;
}

template <typename T>
void Matrix<T, 4, 4>::SetRow(size_t i, const Vector4<T>& row)
{
    m_elements[i * 4] = row.x;
    m_elements[i * 4 + 1] = row.y;
    m_elements[i * 4 + 2] = row.z;
    m_elements[i * 4 + 3] = row.w;
}

template <typename T>
void Matrix<T, 4, 4>::SetColumn(size_t i, const Vector4<T>& col)
{
    m_elements[i] = col.x;
    m_elements[i + 4] = col.y;
    m_elements[i + 8] = col.z;
    m_elements[i + 12] = col.w;
}

template <typename T>
bool Matrix<T, 4, 4>::IsSimilar(const Matrix& m, double tol) const
{
    return std::fabs(m_elements[0] - m.m_elements[0]) < tol &&
           std::fabs(m_elements[1] - m.m_elements[1]) < tol &&
           std::fabs(m_elements[2] - m.m_elements[2]) < tol &&
           std::fabs(m_elements[3] - m.m_elements[3]) < tol &&
           std::fabs(m_elements[4] - m.m_elements[4]) < tol &&
           std::fabs(m_elements[5] - m.m_elements[5]) < tol &&
           std::fabs(m_elements[6] - m.m_elements[6]) < tol &&
           std::fabs(m_elements[7] - m.m_elements[7]) < tol &&
           std::fabs(m_elements[8] - m.m_elements[8]) < tol &&
           std::fabs(m_elements[9] - m.m_elements[9]) < tol &&
           std::fabs(m_elements[10] - m.m_elements[10]) < tol &&
           std::fabs(m_elements[11] - m.m_elements[11]) < tol &&
           std::fabs(m_elements[12] - m.m_elements[12]) < tol &&
           std::fabs(m_elements[13] - m.m_elements[13]) < tol &&
           std::fabs(m_elements[14] - m.m_elements[14]) < tol &&
           std::fabs(m_elements[15] - m.m_elements[15]) < tol;
}

template <typename T>
bool Matrix<T, 4, 4>::IsSquare()
{
    return true;
}

template <typename T>
size_t Matrix<T, 4, 4>::Rows()
{
    return 4;
}

template <typename T>
size_t Matrix<T, 4, 4>::Cols()
{
    return 4;
}

template <typename T>
T* Matrix<T, 4, 4>::data()
{
    return m_elements.data();
}

template <typename T>
const T* Matrix<T, 4, 4>::data() const
{
    return m_elements.data();
}

template <typename T>
Matrix<T, 3, 3> Matrix<T, 4, 4>::Matrix3() const
{
    return Matrix<T, 3, 3>{ m_elements[0], m_elements[1], m_elements[2],
                            m_elements[4], m_elements[5], m_elements[6],
                            m_elements[8], m_elements[9], m_elements[10] };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::Add(T s) const
{
    return Matrix<T, 4, 4>{ m_elements[0] + s,  m_elements[1] + s,
                            m_elements[2] + s,  m_elements[3] + s,
                            m_elements[4] + s,  m_elements[5] + s,
                            m_elements[6] + s,  m_elements[7] + s,
                            m_elements[8] + s,  m_elements[9] + s,
                            m_elements[10] + s, m_elements[11] + s,
                            m_elements[12] + s, m_elements[13] + s,
                            m_elements[14] + s, m_elements[15] + s };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::Add(const Matrix& m) const
{
    return Matrix<T, 4, 4>{
        m_elements[0] + m.m_elements[0],   m_elements[1] + m.m_elements[1],
        m_elements[2] + m.m_elements[2],   m_elements[3] + m.m_elements[3],
        m_elements[4] + m.m_elements[4],   m_elements[5] + m.m_elements[5],
        m_elements[6] + m.m_elements[6],   m_elements[7] + m.m_elements[7],
        m_elements[8] + m.m_elements[8],   m_elements[9] + m.m_elements[9],
        m_elements[10] + m.m_elements[10], m_elements[11] + m.m_elements[11],
        m_elements[12] + m.m_elements[12], m_elements[13] + m.m_elements[13],
        m_elements[14] + m.m_elements[14], m_elements[15] + m.m_elements[15]
    };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::Sub(T s) const
{
    return Matrix<T, 4, 4>{ m_elements[0] - s,  m_elements[1] - s,
                            m_elements[2] - s,  m_elements[3] - s,
                            m_elements[4] - s,  m_elements[5] - s,
                            m_elements[6] - s,  m_elements[7] - s,
                            m_elements[8] - s,  m_elements[9] - s,
                            m_elements[10] - s, m_elements[11] - s,
                            m_elements[12] - s, m_elements[13] - s,
                            m_elements[14] - s, m_elements[15] - s };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::Sub(const Matrix& m) const
{
    return Matrix<T, 4, 4>{
        m_elements[0] - m.m_elements[0],   m_elements[1] - m.m_elements[1],
        m_elements[2] - m.m_elements[2],   m_elements[3] - m.m_elements[3],
        m_elements[4] - m.m_elements[4],   m_elements[5] - m.m_elements[5],
        m_elements[6] - m.m_elements[6],   m_elements[7] - m.m_elements[7],
        m_elements[8] - m.m_elements[8],   m_elements[9] - m.m_elements[9],
        m_elements[10] - m.m_elements[10], m_elements[11] - m.m_elements[11],
        m_elements[12] - m.m_elements[12], m_elements[13] - m.m_elements[13],
        m_elements[14] - m.m_elements[14], m_elements[15] - m.m_elements[15]
    };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::Mul(T s) const
{
    return Matrix<T, 4, 4>{ m_elements[0] * s,  m_elements[1] * s,
                            m_elements[2] * s,  m_elements[3] * s,
                            m_elements[4] * s,  m_elements[5] * s,
                            m_elements[6] * s,  m_elements[7] * s,
                            m_elements[8] * s,  m_elements[9] * s,
                            m_elements[10] * s, m_elements[11] * s,
                            m_elements[12] * s, m_elements[13] * s,
                            m_elements[14] * s, m_elements[15] * s };
}

template <typename T>
Vector4<T> Matrix<T, 4, 4>::Mul(const Vector4<T>& v) const
{
    return Vector4<T>{ m_elements[0] * v.x + m_elements[1] * v.y +
                           m_elements[2] * v.z + m_elements[3] * v.w,
                       m_elements[4] * v.x + m_elements[5] * v.y +
                           m_elements[6] * v.z + m_elements[7] * v.w,
                       m_elements[8] * v.x + m_elements[9] * v.y +
                           m_elements[10] * v.z + m_elements[11] * v.w,
                       m_elements[12] * v.x + m_elements[13] * v.y +
                           m_elements[14] * v.z + m_elements[15] * v.w };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::Mul(const Matrix& m) const
{
    return Matrix{
        m_elements[0] * m.m_elements[0] + m_elements[1] * m.m_elements[4] +
            m_elements[2] * m.m_elements[8] + m_elements[3] * m.m_elements[12],
        m_elements[0] * m.m_elements[1] + m_elements[1] * m.m_elements[5] +
            m_elements[2] * m.m_elements[9] + m_elements[3] * m.m_elements[13],
        m_elements[0] * m.m_elements[2] + m_elements[1] * m.m_elements[6] +
            m_elements[2] * m.m_elements[10] + m_elements[3] * m.m_elements[14],
        m_elements[0] * m.m_elements[3] + m_elements[1] * m.m_elements[7] +
            m_elements[2] * m.m_elements[11] + m_elements[3] * m.m_elements[15],

        m_elements[4] * m.m_elements[0] + m_elements[5] * m.m_elements[4] +
            m_elements[6] * m.m_elements[8] + m_elements[7] * m.m_elements[12],
        m_elements[4] * m.m_elements[1] + m_elements[5] * m.m_elements[5] +
            m_elements[6] * m.m_elements[9] + m_elements[7] * m.m_elements[13],
        m_elements[4] * m.m_elements[2] + m_elements[5] * m.m_elements[6] +
            m_elements[6] * m.m_elements[10] + m_elements[7] * m.m_elements[14],
        m_elements[4] * m.m_elements[3] + m_elements[5] * m.m_elements[7] +
            m_elements[6] * m.m_elements[11] + m_elements[7] * m.m_elements[15],

        m_elements[8] * m.m_elements[0] + m_elements[9] * m.m_elements[4] +
            m_elements[10] * m.m_elements[8] +
            m_elements[11] * m.m_elements[12],
        m_elements[8] * m.m_elements[1] + m_elements[9] * m.m_elements[5] +
            m_elements[10] * m.m_elements[9] +
            m_elements[11] * m.m_elements[13],
        m_elements[8] * m.m_elements[2] + m_elements[9] * m.m_elements[6] +
            m_elements[10] * m.m_elements[10] +
            m_elements[11] * m.m_elements[14],
        m_elements[8] * m.m_elements[3] + m_elements[9] * m.m_elements[7] +
            m_elements[10] * m.m_elements[11] +
            m_elements[11] * m.m_elements[15],

        m_elements[12] * m.m_elements[0] + m_elements[13] * m.m_elements[4] +
            m_elements[14] * m.m_elements[8] +
            m_elements[15] * m.m_elements[12],
        m_elements[12] * m.m_elements[1] + m_elements[13] * m.m_elements[5] +
            m_elements[14] * m.m_elements[9] +
            m_elements[15] * m.m_elements[13],
        m_elements[12] * m.m_elements[2] + m_elements[13] * m.m_elements[6] +
            m_elements[14] * m.m_elements[10] +
            m_elements[15] * m.m_elements[14],
        m_elements[12] * m.m_elements[3] + m_elements[13] * m.m_elements[7] +
            m_elements[14] * m.m_elements[11] +
            m_elements[15] * m.m_elements[15]
    };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::Div(T s) const
{
    return Matrix<T, 4, 4>{ m_elements[0] / s,  m_elements[1] / s,
                            m_elements[2] / s,  m_elements[3] / s,
                            m_elements[4] / s,  m_elements[5] / s,
                            m_elements[6] / s,  m_elements[7] / s,
                            m_elements[8] / s,  m_elements[9] / s,
                            m_elements[10] / s, m_elements[11] / s,
                            m_elements[12] / s, m_elements[13] / s,
                            m_elements[14] / s, m_elements[15] / s };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::RAdd(T s) const
{
    return Matrix<T, 4, 4>{ s + m_elements[0],  s + m_elements[1],
                            s + m_elements[2],  s + m_elements[3],
                            s + m_elements[4],  s + m_elements[5],
                            s + m_elements[6],  s + m_elements[7],
                            s + m_elements[8],  s + m_elements[9],
                            s + m_elements[10], s + m_elements[11],
                            s + m_elements[12], s + m_elements[13],
                            s + m_elements[14], s + m_elements[15] };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::RAdd(const Matrix& m) const
{
    return Matrix<T, 4, 4>{
        m.m_elements[0] + m_elements[0],   m.m_elements[1] + m_elements[1],
        m.m_elements[2] + m_elements[2],   m.m_elements[3] + m_elements[3],
        m.m_elements[4] + m_elements[4],   m.m_elements[5] + m_elements[5],
        m.m_elements[6] + m_elements[6],   m.m_elements[7] + m_elements[7],
        m.m_elements[8] + m_elements[8],   m.m_elements[9] + m_elements[9],
        m.m_elements[10] + m_elements[10], m.m_elements[11] + m_elements[11],
        m.m_elements[12] + m_elements[12], m.m_elements[13] + m_elements[13],
        m.m_elements[14] + m_elements[14], m.m_elements[15] + m_elements[15]
    };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::RSub(T s) const
{
    return Matrix<T, 4, 4>{ s - m_elements[0],  s - m_elements[1],
                            s - m_elements[2],  s - m_elements[3],
                            s - m_elements[4],  s - m_elements[5],
                            s - m_elements[6],  s - m_elements[7],
                            s - m_elements[8],  s - m_elements[9],
                            s - m_elements[10], s - m_elements[11],
                            s - m_elements[12], s - m_elements[13],
                            s - m_elements[14], s - m_elements[15] };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::RSub(const Matrix& m) const
{
    return Matrix<T, 4, 4>{
        m.m_elements[0] - m_elements[0],   m.m_elements[1] - m_elements[1],
        m.m_elements[2] - m_elements[2],   m.m_elements[3] - m_elements[3],
        m.m_elements[4] - m_elements[4],   m.m_elements[5] - m_elements[5],
        m.m_elements[6] - m_elements[6],   m.m_elements[7] - m_elements[7],
        m.m_elements[8] - m_elements[8],   m.m_elements[9] - m_elements[9],
        m.m_elements[10] - m_elements[10], m.m_elements[11] - m_elements[11],
        m.m_elements[12] - m_elements[12], m.m_elements[13] - m_elements[13],
        m.m_elements[14] - m_elements[14], m.m_elements[15] - m_elements[15]
    };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::RMul(T s) const
{
    return Matrix<T, 4, 4>{ s * m_elements[0],  s * m_elements[1],
                            s * m_elements[2],  s * m_elements[3],
                            s * m_elements[4],  s * m_elements[5],
                            s * m_elements[6],  s * m_elements[7],
                            s * m_elements[8],  s * m_elements[9],
                            s * m_elements[10], s * m_elements[11],
                            s * m_elements[12], s * m_elements[13],
                            s * m_elements[14], s * m_elements[15] };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::RMul(const Matrix& m) const
{
    return m.Mul(*this);
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::RDiv(T s) const
{
    return Matrix<T, 4, 4>{ s / m_elements[0],  s / m_elements[1],
                            s / m_elements[2],  s / m_elements[3],
                            s / m_elements[4],  s / m_elements[5],
                            s / m_elements[6],  s / m_elements[7],
                            s / m_elements[8],  s / m_elements[9],
                            s / m_elements[10], s / m_elements[11],
                            s / m_elements[12], s / m_elements[13],
                            s / m_elements[14], s / m_elements[15] };
}

template <typename T>
void Matrix<T, 4, 4>::IAdd(T s)
{
    m_elements[0] += s;
    m_elements[1] += s;
    m_elements[2] += s;
    m_elements[3] += s;
    m_elements[4] += s;
    m_elements[5] += s;
    m_elements[6] += s;
    m_elements[7] += s;
    m_elements[8] += s;
    m_elements[9] += s;
    m_elements[10] += s;
    m_elements[11] += s;
    m_elements[12] += s;
    m_elements[13] += s;
    m_elements[14] += s;
    m_elements[15] += s;
}

template <typename T>
void Matrix<T, 4, 4>::IAdd(const Matrix& m)
{
    m_elements[0] += m.m_elements[0];
    m_elements[1] += m.m_elements[1];
    m_elements[2] += m.m_elements[2];
    m_elements[3] += m.m_elements[3];
    m_elements[4] += m.m_elements[4];
    m_elements[5] += m.m_elements[5];
    m_elements[6] += m.m_elements[6];
    m_elements[7] += m.m_elements[7];
    m_elements[8] += m.m_elements[8];
    m_elements[9] += m.m_elements[9];
    m_elements[10] += m.m_elements[10];
    m_elements[11] += m.m_elements[11];
    m_elements[12] += m.m_elements[12];
    m_elements[13] += m.m_elements[13];
    m_elements[14] += m.m_elements[14];
    m_elements[15] += m.m_elements[15];
}

template <typename T>
void Matrix<T, 4, 4>::ISub(T s)
{
    m_elements[0] -= s;
    m_elements[1] -= s;
    m_elements[2] -= s;
    m_elements[3] -= s;
    m_elements[4] -= s;
    m_elements[5] -= s;
    m_elements[6] -= s;
    m_elements[7] -= s;
    m_elements[8] -= s;
    m_elements[9] -= s;
    m_elements[10] -= s;
    m_elements[11] -= s;
    m_elements[12] -= s;
    m_elements[13] -= s;
    m_elements[14] -= s;
    m_elements[15] -= s;
}

template <typename T>
void Matrix<T, 4, 4>::ISub(const Matrix& m)
{
    m_elements[0] -= m.m_elements[0];
    m_elements[1] -= m.m_elements[1];
    m_elements[2] -= m.m_elements[2];
    m_elements[3] -= m.m_elements[3];
    m_elements[4] -= m.m_elements[4];
    m_elements[5] -= m.m_elements[5];
    m_elements[6] -= m.m_elements[6];
    m_elements[7] -= m.m_elements[7];
    m_elements[8] -= m.m_elements[8];
    m_elements[9] -= m.m_elements[9];
    m_elements[10] -= m.m_elements[10];
    m_elements[11] -= m.m_elements[11];
    m_elements[12] -= m.m_elements[12];
    m_elements[13] -= m.m_elements[13];
    m_elements[14] -= m.m_elements[14];
    m_elements[15] -= m.m_elements[15];
}

template <typename T>
void Matrix<T, 4, 4>::IMul(T s)
{
    m_elements[0] *= s;
    m_elements[1] *= s;
    m_elements[2] *= s;
    m_elements[3] *= s;
    m_elements[4] *= s;
    m_elements[5] *= s;
    m_elements[6] *= s;
    m_elements[7] *= s;
    m_elements[8] *= s;
    m_elements[9] *= s;
    m_elements[10] *= s;
    m_elements[11] *= s;
    m_elements[12] *= s;
    m_elements[13] *= s;
    m_elements[14] *= s;
    m_elements[15] *= s;
}

template <typename T>
void Matrix<T, 4, 4>::IMul(const Matrix& m)
{
    Set(Mul(m));
}

template <typename T>
void Matrix<T, 4, 4>::IDiv(T s)
{
    m_elements[0] /= s;
    m_elements[1] /= s;
    m_elements[2] /= s;
    m_elements[3] /= s;
    m_elements[4] /= s;
    m_elements[5] /= s;
    m_elements[6] /= s;
    m_elements[7] /= s;
    m_elements[8] /= s;
    m_elements[9] /= s;
    m_elements[10] /= s;
    m_elements[11] /= s;
    m_elements[12] /= s;
    m_elements[13] /= s;
    m_elements[14] /= s;
    m_elements[15] /= s;
}

template <typename T>
void Matrix<T, 4, 4>::Transpose()
{
    std::swap(m_elements[1], m_elements[4]);
    std::swap(m_elements[2], m_elements[8]);
    std::swap(m_elements[3], m_elements[12]);
    std::swap(m_elements[6], m_elements[9]);
    std::swap(m_elements[7], m_elements[13]);
    std::swap(m_elements[11], m_elements[14]);
}

template <typename T>
void Matrix<T, 4, 4>::Invert()
{
    T d = Determinant();

    Matrix m;
    m.m_elements[0] = m_elements[5] * m_elements[10] * m_elements[15] +
                      m_elements[6] * m_elements[11] * m_elements[13] +
                      m_elements[7] * m_elements[9] * m_elements[14] -
                      m_elements[5] * m_elements[11] * m_elements[14] -
                      m_elements[6] * m_elements[9] * m_elements[15] -
                      m_elements[7] * m_elements[10] * m_elements[13];
    m.m_elements[1] = m_elements[1] * m_elements[11] * m_elements[14] +
                      m_elements[2] * m_elements[9] * m_elements[15] +
                      m_elements[3] * m_elements[10] * m_elements[13] -
                      m_elements[1] * m_elements[10] * m_elements[15] -
                      m_elements[2] * m_elements[11] * m_elements[13] -
                      m_elements[3] * m_elements[9] * m_elements[14];
    m.m_elements[2] = m_elements[1] * m_elements[6] * m_elements[15] +
                      m_elements[2] * m_elements[7] * m_elements[13] +
                      m_elements[3] * m_elements[5] * m_elements[14] -
                      m_elements[1] * m_elements[7] * m_elements[14] -
                      m_elements[2] * m_elements[5] * m_elements[15] -
                      m_elements[3] * m_elements[6] * m_elements[13];
    m.m_elements[3] = m_elements[1] * m_elements[7] * m_elements[10] +
                      m_elements[2] * m_elements[5] * m_elements[11] +
                      m_elements[3] * m_elements[6] * m_elements[9] -
                      m_elements[1] * m_elements[6] * m_elements[11] -
                      m_elements[2] * m_elements[7] * m_elements[9] -
                      m_elements[3] * m_elements[5] * m_elements[10];
    m.m_elements[4] = m_elements[4] * m_elements[11] * m_elements[14] +
                      m_elements[6] * m_elements[8] * m_elements[15] +
                      m_elements[7] * m_elements[10] * m_elements[12] -
                      m_elements[4] * m_elements[10] * m_elements[15] -
                      m_elements[6] * m_elements[11] * m_elements[12] -
                      m_elements[7] * m_elements[8] * m_elements[14];
    m.m_elements[5] = m_elements[0] * m_elements[10] * m_elements[15] +
                      m_elements[2] * m_elements[11] * m_elements[12] +
                      m_elements[3] * m_elements[8] * m_elements[14] -
                      m_elements[0] * m_elements[11] * m_elements[14] -
                      m_elements[2] * m_elements[8] * m_elements[15] -
                      m_elements[3] * m_elements[10] * m_elements[12];
    m.m_elements[6] = m_elements[0] * m_elements[7] * m_elements[14] +
                      m_elements[2] * m_elements[4] * m_elements[15] +
                      m_elements[3] * m_elements[6] * m_elements[12] -
                      m_elements[0] * m_elements[6] * m_elements[15] -
                      m_elements[2] * m_elements[7] * m_elements[12] -
                      m_elements[3] * m_elements[4] * m_elements[14];
    m.m_elements[7] = m_elements[0] * m_elements[6] * m_elements[11] +
                      m_elements[2] * m_elements[7] * m_elements[8] +
                      m_elements[3] * m_elements[4] * m_elements[10] -
                      m_elements[0] * m_elements[7] * m_elements[10] -
                      m_elements[2] * m_elements[4] * m_elements[11] -
                      m_elements[3] * m_elements[6] * m_elements[8];
    m.m_elements[8] = m_elements[4] * m_elements[9] * m_elements[15] +
                      m_elements[5] * m_elements[11] * m_elements[12] +
                      m_elements[7] * m_elements[8] * m_elements[13] -
                      m_elements[4] * m_elements[11] * m_elements[13] -
                      m_elements[5] * m_elements[8] * m_elements[15] -
                      m_elements[7] * m_elements[9] * m_elements[12];
    m.m_elements[9] = m_elements[0] * m_elements[11] * m_elements[13] +
                      m_elements[1] * m_elements[8] * m_elements[15] +
                      m_elements[3] * m_elements[9] * m_elements[12] -
                      m_elements[0] * m_elements[9] * m_elements[15] -
                      m_elements[1] * m_elements[11] * m_elements[12] -
                      m_elements[3] * m_elements[8] * m_elements[13];
    m.m_elements[10] = m_elements[0] * m_elements[5] * m_elements[15] +
                       m_elements[1] * m_elements[7] * m_elements[12] +
                       m_elements[3] * m_elements[4] * m_elements[13] -
                       m_elements[0] * m_elements[7] * m_elements[13] -
                       m_elements[1] * m_elements[4] * m_elements[15] -
                       m_elements[3] * m_elements[5] * m_elements[12];
    m.m_elements[11] = m_elements[0] * m_elements[7] * m_elements[9] +
                       m_elements[1] * m_elements[4] * m_elements[11] +
                       m_elements[3] * m_elements[5] * m_elements[8] -
                       m_elements[0] * m_elements[5] * m_elements[11] -
                       m_elements[1] * m_elements[7] * m_elements[8] -
                       m_elements[3] * m_elements[4] * m_elements[9];
    m.m_elements[12] = m_elements[4] * m_elements[10] * m_elements[13] +
                       m_elements[5] * m_elements[8] * m_elements[14] +
                       m_elements[6] * m_elements[9] * m_elements[12] -
                       m_elements[4] * m_elements[9] * m_elements[14] -
                       m_elements[5] * m_elements[10] * m_elements[12] -
                       m_elements[6] * m_elements[8] * m_elements[13];
    m.m_elements[13] = m_elements[0] * m_elements[9] * m_elements[14] +
                       m_elements[1] * m_elements[10] * m_elements[12] +
                       m_elements[2] * m_elements[8] * m_elements[13] -
                       m_elements[0] * m_elements[10] * m_elements[13] -
                       m_elements[1] * m_elements[8] * m_elements[14] -
                       m_elements[2] * m_elements[9] * m_elements[12];
    m.m_elements[14] = m_elements[0] * m_elements[6] * m_elements[13] +
                       m_elements[1] * m_elements[4] * m_elements[14] +
                       m_elements[2] * m_elements[5] * m_elements[12] -
                       m_elements[0] * m_elements[5] * m_elements[14] -
                       m_elements[1] * m_elements[6] * m_elements[12] -
                       m_elements[2] * m_elements[4] * m_elements[13];
    m.m_elements[15] = m_elements[0] * m_elements[5] * m_elements[10] +
                       m_elements[1] * m_elements[6] * m_elements[8] +
                       m_elements[2] * m_elements[4] * m_elements[9] -
                       m_elements[0] * m_elements[6] * m_elements[9] -
                       m_elements[1] * m_elements[4] * m_elements[10] -
                       m_elements[2] * m_elements[5] * m_elements[8];
    m.IDiv(d);

    Set(m);
}

template <typename T>
T Matrix<T, 4, 4>::Sum() const
{
    T sum = 0;

    for (size_t i = 0; i < 16; ++i)
    {
        sum += m_elements[i];
    }

    return sum;
}

template <typename T>
T Matrix<T, 4, 4>::Avg() const
{
    return Sum() / 16;
}

template <typename T>
T Matrix<T, 4, 4>::Min() const
{
    return m_elements[std::distance(
        std::begin(m_elements),
        std::min_element(std::begin(m_elements), std::end(m_elements)))];
}

template <typename T>
T Matrix<T, 4, 4>::Max() const
{
    return m_elements[std::distance(
        std::begin(m_elements),
        std::max_element(std::begin(m_elements), std::end(m_elements)))];
}

template <typename T>
T Matrix<T, 4, 4>::AbsMin() const
{
    return CubbyFlow::AbsMinN(data(), 16);
}

template <typename T>
T Matrix<T, 4, 4>::AbsMax() const
{
    return CubbyFlow::AbsMaxN(data(), 16);
}

template <typename T>
T Matrix<T, 4, 4>::Trace() const
{
    return m_elements[0] + m_elements[5] + m_elements[10] + m_elements[15];
}

template <typename T>
T Matrix<T, 4, 4>::Determinant() const
{
    return m_elements[0] * m_elements[5] * m_elements[10] * m_elements[15] +
           m_elements[0] * m_elements[6] * m_elements[11] * m_elements[13] +
           m_elements[0] * m_elements[7] * m_elements[9] * m_elements[14] +
           m_elements[1] * m_elements[4] * m_elements[11] * m_elements[14] +
           m_elements[1] * m_elements[6] * m_elements[8] * m_elements[15] +
           m_elements[1] * m_elements[7] * m_elements[10] * m_elements[12] +
           m_elements[2] * m_elements[4] * m_elements[9] * m_elements[15] +
           m_elements[2] * m_elements[5] * m_elements[11] * m_elements[12] +
           m_elements[2] * m_elements[7] * m_elements[8] * m_elements[13] +
           m_elements[3] * m_elements[4] * m_elements[10] * m_elements[13] +
           m_elements[3] * m_elements[5] * m_elements[8] * m_elements[14] +
           m_elements[3] * m_elements[6] * m_elements[9] * m_elements[12] -
           m_elements[0] * m_elements[5] * m_elements[11] * m_elements[14] -
           m_elements[0] * m_elements[6] * m_elements[9] * m_elements[15] -
           m_elements[0] * m_elements[7] * m_elements[10] * m_elements[13] -
           m_elements[1] * m_elements[4] * m_elements[10] * m_elements[15] -
           m_elements[1] * m_elements[6] * m_elements[11] * m_elements[12] -
           m_elements[1] * m_elements[7] * m_elements[8] * m_elements[14] -
           m_elements[2] * m_elements[4] * m_elements[11] * m_elements[13] -
           m_elements[2] * m_elements[5] * m_elements[8] * m_elements[15] -
           m_elements[2] * m_elements[7] * m_elements[9] * m_elements[12] -
           m_elements[3] * m_elements[4] * m_elements[9] * m_elements[14] -
           m_elements[3] * m_elements[5] * m_elements[10] * m_elements[12] -
           m_elements[3] * m_elements[6] * m_elements[8] * m_elements[13];
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::Diagonal() const
{
    return Matrix<T, 4, 4>{ m_elements[0],  0, 0, 0, 0,
                            m_elements[5],  0, 0, 0, 0,
                            m_elements[10], 0, 0, 0, 0,
                            m_elements[15] };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::OffDiagonal() const
{
    return Matrix<T, 4, 4>{
        0, m_elements[1],  m_elements[2],  m_elements[3],  m_elements[4],
        0, m_elements[6],  m_elements[7],  m_elements[8],  m_elements[9],
        0, m_elements[11], m_elements[12], m_elements[13], m_elements[14],
        0
    };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::StrictLowerTriangle() const
{
    return Matrix<T, 4, 4>{ 0,
                            0,
                            0,
                            0,
                            m_elements[4],
                            0,
                            0,
                            0,
                            m_elements[8],
                            m_elements[9],
                            0,
                            0,
                            m_elements[12],
                            m_elements[13],
                            m_elements[14],
                            0 };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::StrictUpperTriangle() const
{
    return Matrix<T, 4, 4>{ 0,
                            m_elements[1],
                            m_elements[2],
                            m_elements[3],
                            0,
                            0,
                            m_elements[6],
                            m_elements[7],
                            0,
                            0,
                            0,
                            m_elements[11],
                            0,
                            0,
                            0,
                            0 };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::LowerTriangle() const
{
    return Matrix<T, 4, 4>{ m_elements[0],
                            0,
                            0,
                            0,
                            m_elements[4],
                            m_elements[5],
                            0,
                            0,
                            m_elements[8],
                            m_elements[9],
                            m_elements[10],
                            0,
                            m_elements[12],
                            m_elements[13],
                            m_elements[14],
                            m_elements[15] };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::UpperTriangle() const
{
    return Matrix<T, 4, 4>{ m_elements[0],
                            m_elements[1],
                            m_elements[2],
                            m_elements[3],
                            0,
                            m_elements[5],
                            m_elements[6],
                            m_elements[7],
                            0,
                            0,
                            m_elements[10],
                            m_elements[11],
                            0,
                            0,
                            0,
                            m_elements[15] };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::Transposed() const
{
    return Matrix<T, 4, 4>{ m_elements[0],  m_elements[4],  m_elements[8],
                            m_elements[12], m_elements[1],  m_elements[5],
                            m_elements[9],  m_elements[13], m_elements[2],
                            m_elements[6],  m_elements[10], m_elements[14],
                            m_elements[3],  m_elements[7],  m_elements[11],
                            m_elements[15] };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::Inverse() const
{
    Matrix m{ *this };
    m.Invert();
    return m;
}

template <typename T>
template <typename U>
Matrix<U, 4, 4> Matrix<T, 4, 4>::CastTo() const
{
    return Matrix<U, 4, 4>{
        static_cast<U>(m_elements[0]),  static_cast<U>(m_elements[1]),
        static_cast<U>(m_elements[2]),  static_cast<U>(m_elements[3]),
        static_cast<U>(m_elements[4]),  static_cast<U>(m_elements[5]),
        static_cast<U>(m_elements[6]),  static_cast<U>(m_elements[7]),
        static_cast<U>(m_elements[8]),  static_cast<U>(m_elements[9]),
        static_cast<U>(m_elements[10]), static_cast<U>(m_elements[11]),
        static_cast<U>(m_elements[12]), static_cast<U>(m_elements[13]),
        static_cast<U>(m_elements[14]), static_cast<U>(m_elements[15])
    };
}

template <typename T>
Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator+=(T s)
{
    IAdd(s);
    return *this;
}

template <typename T>
Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator+=(const Matrix& m)
{
    IAdd(m);
    return *this;
}

template <typename T>
Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator-=(T s)
{
    ISub(s);
    return *this;
}

template <typename T>
Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator-=(const Matrix& m)
{
    ISub(m);
    return *this;
}

template <typename T>
Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator*=(const T s)
{
    IMul(s);
    return *this;
}

template <typename T>
Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator*=(const Matrix& m)
{
    IMul(m);
    return *this;
}

template <typename T>
Matrix<T, 4, 4>& Matrix<T, 4, 4>::operator/=(T s)
{
    IDiv(s);
    return *this;
}

template <typename T>
T& Matrix<T, 4, 4>::operator[](size_t i)
{
    return m_elements[i];
}

template <typename T>
const T& Matrix<T, 4, 4>::operator[](size_t i) const
{
    return m_elements[i];
}

template <typename T>
T& Matrix<T, 4, 4>::operator()(size_t i, size_t j)
{
    return m_elements[i * 4 + j];
}

template <typename T>
const T& Matrix<T, 4, 4>::operator()(size_t i, size_t j) const
{
    return m_elements[i * 4 + j];
}

template <typename T>
bool Matrix<T, 4, 4>::operator==(const Matrix& m) const
{
    return m_elements[0] == m.m_elements[0] &&
           m_elements[1] == m.m_elements[1] &&
           m_elements[2] == m.m_elements[2] &&
           m_elements[3] == m.m_elements[3] &&
           m_elements[4] == m.m_elements[4] &&
           m_elements[5] == m.m_elements[5] &&
           m_elements[6] == m.m_elements[6] &&
           m_elements[7] == m.m_elements[7] &&
           m_elements[8] == m.m_elements[8] &&
           m_elements[9] == m.m_elements[9] &&
           m_elements[10] == m.m_elements[10] &&
           m_elements[11] == m.m_elements[11] &&
           m_elements[12] == m.m_elements[12] &&
           m_elements[13] == m.m_elements[13] &&
           m_elements[14] == m.m_elements[14] &&
           m_elements[15] == m.m_elements[15];
}

template <typename T>
bool Matrix<T, 4, 4>::operator!=(const Matrix& m) const
{
    return m_elements[0] != m.m_elements[0] ||
           m_elements[1] != m.m_elements[1] ||
           m_elements[2] != m.m_elements[2] ||
           m_elements[3] != m.m_elements[3] ||
           m_elements[4] != m.m_elements[4] ||
           m_elements[5] != m.m_elements[5] ||
           m_elements[6] != m.m_elements[6] ||
           m_elements[7] != m.m_elements[7] ||
           m_elements[8] != m.m_elements[8] ||
           m_elements[9] != m.m_elements[9] ||
           m_elements[10] != m.m_elements[10] ||
           m_elements[11] != m.m_elements[11] ||
           m_elements[12] != m.m_elements[12] ||
           m_elements[13] != m.m_elements[13] ||
           m_elements[14] != m.m_elements[14] ||
           m_elements[15] != m.m_elements[15];
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::MakeZero()
{
    return Matrix<T, 4, 4>{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::MakeIdentity()
{
    return Matrix<T, 4, 4>{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::MakeScaleMatrix(T sx, T sy, T sz)
{
    return Matrix<T, 4, 4>{ sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1 };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::MakeScaleMatrix(const Vector3<T>& s)
{
    return MakeScaleMatrix(s.x, s.y, s.z);
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::MakeRotationMatrix(const Vector3<T>& axis,
                                                    const T rad)
{
    return Matrix<T, 4, 4>{ Matrix<T, 3, 3>::MakeRotationMatrix(axis, rad) };
}

template <typename T>
Matrix<T, 4, 4> Matrix<T, 4, 4>::MakeTranslationMatrix(const Vector3<T>& t)
{
    return Matrix<T, 4, 4>{
        1, 0, 0, t.x, 0, 1, 0, t.y, 0, 0, 1, t.z, 0, 0, 0, 1
    };
}

template <typename T>
Matrix<T, 4, 4> operator-(const Matrix<T, 4, 4>& a)
{
    return a.Mul(-1);
}

template <typename T>
Matrix<T, 4, 4> operator+(const Matrix<T, 4, 4>& a, const Matrix<T, 4, 4>& b)
{
    return a.Add(b);
}

template <typename T>
Matrix<T, 4, 4> operator+(const Matrix<T, 4, 4>& a, const T b)
{
    return a.Add(b);
}

template <typename T>
Matrix<T, 4, 4> operator+(const T a, const Matrix<T, 4, 4>& b)
{
    return b.RAdd(a);
}

template <typename T>
Matrix<T, 4, 4> operator-(const Matrix<T, 4, 4>& a, const Matrix<T, 4, 4>& b)
{
    return a.Sub(b);
}

template <typename T>
Matrix<T, 4, 4> operator-(const Matrix<T, 4, 4>& a, const T b)
{
    return a.Sub(b);
}

template <typename T>
Matrix<T, 4, 4> operator-(const T a, const Matrix<T, 4, 4>& b)
{
    return b.RSub(a);
}

template <typename T>
Matrix<T, 4, 4> operator*(const Matrix<T, 4, 4>& a, T b)
{
    return a.Mul(b);
}

template <typename T>
Matrix<T, 4, 4> operator*(T a, const Matrix<T, 4, 4>& b)
{
    return b.RMul(a);
}

template <typename T>
Vector<T, 3> operator*(const Matrix<T, 4, 4>& a, const Vector<T, 3>& b)
{
    return a.Mul(b);
}

template <typename T>
Vector<T, 4> operator*(const Matrix<T, 4, 4>& a, const Vector<T, 4>& b)
{
    return a.Mul(b);
}

template <typename T>
Matrix<T, 4, 4> operator*(const Matrix<T, 4, 4>& a, const Matrix<T, 4, 4>& b)
{
    return a.Mul(b);
}

template <typename T>
Matrix<T, 4, 4> operator/(const Matrix<T, 4, 4>& a, T b)
{
    return a.Div(b);
}

template <typename T>
Matrix<T, 4, 4> operator/(const T& a, const Matrix<T, 4, 4>& b)
{
    return b.RDiv(a);
}
}  // namespace CubbyFlow

#endif