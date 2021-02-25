// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MATRIX_EXPRESSION_HPP
#define CUBBYFLOW_MATRIX_EXPRESSION_HPP

namespace CubbyFlow
{
static constexpr size_t MATRIX_SIZE_DYNAMIC = 0;

template <size_t Rows, size_t Cols>
constexpr bool IsMatrixSizeDynamic()
{
    return (Rows == MATRIX_SIZE_DYNAMIC) || (Cols == MATRIX_SIZE_DYNAMIC);
}

template <size_t Rows, size_t Cols>
constexpr bool IsMatrixSizeStatic()
{
    return !IsMatrixSizeDynamic<Rows, Cols>();
}

template <size_t Rows, size_t Cols>
constexpr bool isMatrixStaticSquare()
{
    return IsMatrixSizeStatic<Rows, Cols>() && (Rows == Cols);
}

template <size_t Rows, size_t Cols>
struct IsMatrixSizeDynamic
{
    static const bool value = IsMatrixSizeDynamic<Rows, Cols>();
};

template <size_t Rows, size_t Cols>
struct IsMatrixSizeStatic
{
    static const bool value = IsMatrixSizeStatic<Rows, Cols>();
};

template <size_t Rows, size_t Cols>
struct IsMatrixSizeSquare
{
    static const bool value = isMatrixStaticSquare<Rows, Cols>();
};

template <typename T, size_t Rows, size_t Cols>
class Matrix;

template <typename T, size_t Rows, size_t Cols, typename M1>
class MatrixDiagonal;

template <typename T, size_t Rows, size_t Cols, typename M1>
class MatrixOffDiagonal;

template <typename T, size_t Rows, size_t Cols, typename M1>
class MatrixTri;

template <typename T, size_t Rows, size_t Cols, typename M1>
class MatrixTranspose;

template <typename T, size_t Rows, size_t Cols, typename M1,
          typename UnaryOperation>
class MatrixUnaryOp;

template <typename T, size_t Rows, size_t Cols, typename M1,
          typename BinaryOperation>
class MatrixScalarElemWiseBinaryOp;
}  // namespace CubbyFlow

#include <Core/Matrix/MatrixExpression-Impl.hpp>

#endif