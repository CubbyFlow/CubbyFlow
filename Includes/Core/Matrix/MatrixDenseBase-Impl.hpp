// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MATRIX_DENSE_BASE_IMPL_HPP
#define CUBBYFLOW_MATRIX_DENSE_BASE_IMPL_HPP

#include <algorithm>
#include <cassert>

namespace CubbyFlow
{
template <typename T, size_t Rows, size_t Cols, typename D>
template <size_t R, size_t C, typename E>
void MatrixDenseBase<T, Rows, Cols, D>::CopyFrom(
    const MatrixExpression<T, R, C, E>& expression)
{
    for (size_t i = 0; i < GetRows(); ++i)
    {
        for (size_t j = 0; j < GetCols(); ++j)
        {
            (*this)(i, j) = expression.Eval(i, j);
        }
    }
}

template <typename T, size_t Rows, size_t Cols, typename D>
void MatrixDenseBase<T, Rows, Cols, D>::SetDiagonal(ConstReference val)
{
    const size_t n = std::min(GetRows(), GetCols());

    for (size_t i = 0; i < n; ++i)
    {
        (*this)(i, i) = val;
    }
}

template <typename T, size_t Rows, size_t Cols, typename D>
void MatrixDenseBase<T, Rows, Cols, D>::SetOffDiagonal(ConstReference val)
{
    for (size_t i = 0; i < GetRows(); ++i)
    {
        for (size_t j = 0; j < GetCols(); ++j)
        {
            if (i != j)
            {
                (*this)(i, j) = val;
            }
        }
    }
}

template <typename T, size_t Rows, size_t Cols, typename D>
template <size_t R, size_t C, typename E>
void MatrixDenseBase<T, Rows, Cols, D>::SetRow(
    size_t i, const MatrixExpression<T, R, C, E>& row)
{
    assert(row.GetRows() == GetCols() && row.GetCols() == 1);

    for (size_t j = 0; j < GetCols(); ++j)
    {
        (*this)(i, j) = row.Eval(j, 0);
    }
}

template <typename T, size_t Rows, size_t Cols, typename D>
template <size_t R, size_t C, typename E>
void MatrixDenseBase<T, Rows, Cols, D>::SetColumn(
    size_t j, const MatrixExpression<T, R, C, E>& col)
{
    assert(col.GetRows() == GetRows() && col.GetCols() == 1);

    for (size_t i = 0; i < GetRows(); ++i)
    {
        (*this)(i, j) = col.Eval(i, 0);
    }
}

template <typename T, size_t Rows, size_t Cols, typename D>
void MatrixDenseBase<T, Rows, Cols, D>::Normalize()
{
    GetDerived() /= GetDerived().Norm();
}

template <typename T, size_t Rows, size_t Cols, typename D>
void MatrixDenseBase<T, Rows, Cols, D>::Transpose()
{
    D tmp = GetDerived().Transposed();

    CopyFrom(tmp);
}

template <typename T, size_t Rows, size_t Cols, typename D>
void MatrixDenseBase<T, Rows, Cols, D>::Invert()
{
    CopyFrom(GetDerived().Inverse());
}

template <typename T, size_t Rows, size_t Cols, typename D>
typename MatrixDenseBase<T, Rows, Cols, D>::Reference
MatrixDenseBase<T, Rows, Cols, D>::operator()(size_t i, size_t j)
{
    assert(i < GetRows() && j < GetCols());

    return GetDerived()[j + i * GetCols()];
}

template <typename T, size_t Rows, size_t Cols, typename D>
typename MatrixDenseBase<T, Rows, Cols, D>::ConstReference
MatrixDenseBase<T, Rows, Cols, D>::operator()(size_t i, size_t j) const
{
    assert(i < GetRows() && j < GetCols());

    return GetDerived()[j + i * GetCols()];
}

template <typename T, size_t Rows, size_t Cols, typename D>
template <size_t R, size_t C, typename E>
MatrixDenseBase<T, Rows, Cols, D>& MatrixDenseBase<T, Rows, Cols, D>::operator=(
    const MatrixExpression<T, R, C, E>& expression)
{
    CopyFrom(expression);

    return *this;
}

template <typename T, size_t Rows, size_t Cols, typename Derived>
template <typename D>
std::enable_if_t<IsMatrixSizeStatic<Rows, Cols>(), D>
MatrixDenseBase<T, Rows, Cols, Derived>::MakeZero()
{
    return MatrixConstant<T, Rows, Cols>{ Rows, Cols, 0 };
}

template <typename T, size_t Rows, size_t Cols, typename Derived>
template <typename D>
std::enable_if_t<IsMatrixSizeDynamic<Rows, Cols>(), D>
MatrixDenseBase<T, Rows, Cols, Derived>::MakeZero(size_t rows, size_t cols)
{
    return MatrixConstant<T, Rows, Cols>{ rows, cols, 0 };
}

template <typename T, size_t Rows, size_t Cols, typename Derived>
template <typename D>
std::enable_if_t<IsMatrixSizeStatic<Rows, Cols>(), D>
MatrixDenseBase<T, Rows, Cols, Derived>::MakeConstant(ValueType val)
{
    return MatrixConstant<T, Rows, Cols>{ Rows, Cols, val };
}

template <typename T, size_t Rows, size_t Cols, typename Derived>
template <typename D>
std::enable_if_t<IsMatrixSizeDynamic<Rows, Cols>(), D>
MatrixDenseBase<T, Rows, Cols, Derived>::MakeConstant(size_t rows, size_t cols,
                                                      ValueType val)
{
    return MatrixConstant<T, Rows, Cols>{ rows, cols, val };
}

template <typename T, size_t Rows, size_t Cols, typename Derived>
template <typename D>
std::enable_if_t<IsMatrixStaticSquare<Rows, Cols>(), D>
MatrixDenseBase<T, Rows, Cols, Derived>::MakeIdentity()
{
    using ConstType = MatrixConstant<T, Rows, Cols>;

    return MatrixDiagonal<T, Rows, Cols, ConstType>{ ConstType{ Rows, Cols,
                                                                1 } };
}

template <typename T, size_t Rows, size_t Cols, typename Derived>
template <typename D>
std::enable_if_t<IsMatrixSizeDynamic<Rows, Cols>(), D>
MatrixDenseBase<T, Rows, Cols, Derived>::MakeIdentity(size_t rows)
{
    using ConstType = MatrixConstant<T, Rows, Cols>;

    return MatrixDiagonal<T, Rows, Cols, ConstType>{ ConstType{ rows, rows,
                                                                1 } };
}

template <typename T, size_t Rows, size_t Cols, typename Derived>
template <typename... Args, typename D>
std::enable_if_t<IsMatrixStaticSquare<Rows, Cols>(), D>
MatrixDenseBase<T, Rows, Cols, Derived>::MakeScaleMatrix(ValueType first,
                                                         Args... rest)
{
    static_assert(sizeof...(rest) == Rows - 1,
                  "Number of parameters should match the size of diagonal.");

    D m{};
    std::array<T, Rows> diag{ { first, rest... } };

    for (size_t i = 0; i < Rows; ++i)
    {
        m(i, i) = diag[i];
    }

    return m;
}

template <typename T, size_t Rows, size_t Cols, typename Derived>
template <size_t R, size_t C, typename E, typename D>
std::enable_if_t<IsMatrixStaticSquare<Rows, Cols>(), D>
MatrixDenseBase<T, Rows, Cols, Derived>::MakeScaleMatrix(
    const MatrixExpression<T, R, C, E>& expression)
{
    assert(expression.GetCols() == 1);

    D m{};

    for (size_t i = 0; i < Rows; ++i)
    {
        m(i, i) = expression.Eval(i, 0);
    }

    return m;
}

template <typename T, size_t Rows, size_t Cols, typename Derived>
template <typename D>
std::enable_if_t<IsMatrixStaticSquare<Rows, Cols>() && (Rows == 2), D>
MatrixDenseBase<T, Rows, Cols, Derived>::MakeRotationMatrix(T rad)
{
    return D{ std::cos(rad), -std::sin(rad), std::sin(rad), std::cos(rad) };
}

template <typename T, size_t Rows, size_t Cols, typename Derived>
template <size_t R, size_t C, typename E, typename D>
std::enable_if_t<IsMatrixStaticSquare<Rows, Cols>() && (Rows == 3 || Rows == 4),
                 D>
MatrixDenseBase<T, Rows, Cols, Derived>::MakeRotationMatrix(
    const MatrixExpression<T, R, C, E>& axis, T rad)
{
    assert(axis.GetRows() == 3 && axis.GetCols() == 1);

    D result = MakeIdentity();

    result(0, 0) =
        1 + (1 - std::cos(rad)) * (axis.Eval(0, 0) * axis.Eval(0, 0) - 1);
    result(0, 1) = -axis.Eval(2, 0) * std::sin(rad) +
                   (1 - std::cos(rad)) * axis.Eval(0, 0) * axis.Eval(1, 0);
    result(0, 2) = axis.Eval(1, 0) * std::sin(rad) +
                   (1 - std::cos(rad)) * axis.Eval(0, 0) * axis.Eval(2, 0);

    result(1, 0) = axis.Eval(2, 0) * std::sin(rad) +
                   (1 - std::cos(rad)) * axis.Eval(0, 0) * axis.Eval(1, 0);
    result(1, 1) =
        1 + (1 - std::cos(rad)) * (axis.Eval(1, 0) * axis.Eval(1, 0) - 1);
    result(1, 2) = -axis.Eval(0, 0) * std::sin(rad) +
                   (1 - std::cos(rad)) * axis.Eval(1, 0) * axis.Eval(2, 0);

    result(2, 0) = -axis.Eval(1, 0) * std::sin(rad) +
                   (1 - std::cos(rad)) * axis.Eval(0, 0) * axis.Eval(2, 0);
    result(2, 1) = axis.Eval(0, 0) * std::sin(rad) +
                   (1 - std::cos(rad)) * axis.Eval(1, 0) * axis.Eval(2, 0);
    result(2, 2) =
        1 + (1 - std::cos(rad)) * (axis.Eval(2, 0) * axis.Eval(2, 0) - 1);

    return result;
}

template <typename T, size_t Rows, size_t Cols, typename Derived>
template <size_t R, size_t C, typename E, typename D>
std::enable_if_t<IsMatrixStaticSquare<Rows, Cols>() && (Rows == 4), D>
MatrixDenseBase<T, Rows, Cols, Derived>::MakeTranslationMatrix(
    const MatrixExpression<T, R, C, E>& t)
{
    assert(t.GetRows() == 3 && t.GetCols() == 1);

    D result = MakeIdentity();

    result(0, 3) = t.Eval(0, 0);
    result(1, 3) = t.Eval(1, 0);
    result(2, 3) = t.Eval(2, 0);

    return result;
}

template <typename T, size_t Rows, size_t Cols, typename D>
constexpr size_t MatrixDenseBase<T, Rows, Cols, D>::GetRows() const
{
    return static_cast<const D&>(*this).GetRows();
}

template <typename T, size_t Rows, size_t Cols, typename D>
constexpr size_t MatrixDenseBase<T, Rows, Cols, D>::GetCols() const
{
    return static_cast<const D&>(*this).GetCols();
}

template <typename T, size_t Rows, size_t Cols, typename D>
auto MatrixDenseBase<T, Rows, Cols, D>::begin()
{
    return GetDerived().begin();
}

template <typename T, size_t Rows, size_t Cols, typename D>
constexpr auto MatrixDenseBase<T, Rows, Cols, D>::begin() const
{
    return GetDerived().begin();
}

template <typename T, size_t Rows, size_t Cols, typename D>
auto MatrixDenseBase<T, Rows, Cols, D>::end()
{
    return GetDerived().end();
}

template <typename T, size_t Rows, size_t Cols, typename D>
constexpr auto MatrixDenseBase<T, Rows, Cols, D>::end() const
{
    return GetDerived().end();
}

template <typename T, size_t Rows, size_t Cols, typename D>
typename MatrixDenseBase<T, Rows, Cols, D>::Reference
MatrixDenseBase<T, Rows, Cols, D>::operator[](size_t i)
{
    assert(i < GetRows() * GetCols());

    return GetDerived()[i];
}

template <typename T, size_t Rows, size_t Cols, typename D>
typename MatrixDenseBase<T, Rows, Cols, D>::ConstReference
MatrixDenseBase<T, Rows, Cols, D>::operator[](size_t i) const
{
    assert(i < GetRows() * GetCols());

    return GetDerived()[i];
}

template <typename T, size_t Rows, size_t Cols, typename D>
D& MatrixDenseBase<T, Rows, Cols, D>::GetDerived()
{
    return static_cast<D&>(*this);
}

template <typename T, size_t Rows, size_t Cols, typename D>
const D& MatrixDenseBase<T, Rows, Cols, D>::GetDerived() const
{
    return static_cast<const D&>(*this);
}
}  // namespace CubbyFlow

#endif