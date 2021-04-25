// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MATRIX_DENSE_BASE_HPP
#define CUBBYFLOW_MATRIX_DENSE_BASE_HPP

#include <Core/Matrix/MatrixExpression.hpp>

namespace CubbyFlow
{
// Derived type should be constructible.
template <typename T, size_t Rows, size_t Cols, typename Derived>
class MatrixDenseBase
{
 public:
    using ValueType = T;
    using Reference = T&;
    using ConstReference = const T&;

    //! Copies from generic expression.
    template <size_t R, size_t C, typename E>
    void CopyFrom(const MatrixExpression<T, R, C, E>& expression);

    //! Sets diagonal elements with input scalar.
    void SetDiagonal(ConstReference val);

    //! Sets off-diagonal elements with input scalar.
    void SetOffDiagonal(ConstReference val);

    //! Sets i-th row with input column vector.
    template <size_t R, size_t C, typename E>
    void SetRow(size_t i, const MatrixExpression<T, R, C, E>& row);

    //! Sets j-th column with input vector.
    template <size_t R, size_t C, typename E>
    void SetColumn(size_t j, const MatrixExpression<T, R, C, E>& col);

    void Normalize();

    //! Transposes this matrix.
    void Transpose();

    //! Inverts this matrix.
    void Invert();

    Reference operator()(size_t i, size_t j);

    ConstReference operator()(size_t i, size_t j) const;

    //! Copies from generic expression
    template <size_t R, size_t C, typename E>
    MatrixDenseBase& operator=(const MatrixExpression<T, R, C, E>& expression);

    //! Makes a static matrix with zero entries.
    template <typename D = Derived>
    static std::enable_if_t<IsMatrixSizeStatic<Rows, Cols>(), D> MakeZero();

    //! Makes a dynamic matrix with zero entries.
    template <typename D = Derived>
    static std::enable_if_t<IsMatrixSizeDynamic<Rows, Cols>(), D> MakeZero(
        size_t rows, size_t cols);

    //! Makes a static matrix with constant entries.
    template <typename D = Derived>
    static std::enable_if_t<IsMatrixSizeStatic<Rows, Cols>(), D> MakeConstant(
        ValueType val);

    //! Makes a dynamic matrix with constant entries.
    template <typename D = Derived>
    static std::enable_if_t<IsMatrixSizeDynamic<Rows, Cols>(), D> MakeConstant(
        size_t rows, size_t cols, ValueType val);

    //! Makes a static identity matrix.
    template <typename D = Derived>
    static std::enable_if_t<IsMatrixStaticSquare<Rows, Cols>(), D>
    MakeIdentity();

    //! Makes a dynamic identity matrix.
    template <typename D = Derived>
    static std::enable_if_t<IsMatrixSizeDynamic<Rows, Cols>(), D> MakeIdentity(
        size_t rows);

    //! Makes scale matrix.
    template <typename... Args, typename D = Derived>
    static std::enable_if_t<IsMatrixStaticSquare<Rows, Cols>(), D>
    MakeScaleMatrix(ValueType first, Args... rest);

    //! Makes scale matrix.
    template <size_t R, size_t C, typename E, typename D = Derived>
    static std::enable_if_t<IsMatrixStaticSquare<Rows, Cols>(), D>
    MakeScaleMatrix(const MatrixExpression<T, R, C, E>& expression);

    //! Makes rotation matrix.
    //! \warning Input angle should be radian.
    template <typename D = Derived>
    static std::enable_if_t<IsMatrixStaticSquare<Rows, Cols>() && (Rows == 2),
                            D>
    MakeRotationMatrix(T rad);

    //! Makes rotation matrix.
    //! \warning Input angle should be radian.
    template <size_t R, size_t C, typename E, typename D = Derived>
    static std::enable_if_t<
        IsMatrixStaticSquare<Rows, Cols>() && (Rows == 3 || Rows == 4), D>
    MakeRotationMatrix(const MatrixExpression<T, R, C, E>& axis, T rad);

    //! Makes translation matrix.
    template <size_t R, size_t C, typename E, typename D = Derived>
    static std::enable_if_t<IsMatrixStaticSquare<Rows, Cols>() && (Rows == 4),
                            D>
    MakeTranslationMatrix(const MatrixExpression<T, R, C, E>& t);

 protected:
    MatrixDenseBase() = default;

 private:
    [[nodiscard]] constexpr size_t GetRows() const;

    [[nodiscard]] constexpr size_t GetCols() const;

    [[nodiscard]] auto begin();

    [[nodiscard]] constexpr auto begin() const;

    [[nodiscard]] auto end();

    [[nodiscard]] constexpr auto end() const;

    Reference operator[](size_t i);

    ConstReference operator[](size_t i) const;

    [[nodiscard]] Derived& GetDerived();

    [[nodiscard]] const Derived& GetDerived() const;
};
}  // namespace CubbyFlow

#include <Core/Matrix/MatrixDenseBase-Impl.hpp>

#endif