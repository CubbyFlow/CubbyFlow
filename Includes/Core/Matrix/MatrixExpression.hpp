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

#include <Core/Utils/Functors.hpp>

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

//!
//! \brief Base class for matrix expression.
//!
//! Matrix expression is a meta type that enables template expression
//! pattern.
//!
//! \tparam T           Real number type.
//! \tparam Rows        The number of rows.
//! \tparam Cols        The number of columns.
//! \tparam Derived     Subclass type.
//!
template <typename T, size_t Rows, size_t Cols, typename Derived>
class MatrixExpression
{
 public:
    using ValueType = T;

    //! Returns the number of rows.
    constexpr size_t GetRows() const;

    //! Returns the number of columns.
    constexpr size_t GetCols() const;

    //! Returns the evaluated value for (i, j).
    T Eval(size_t i, size_t j) const;

    Matrix<T, Rows, Cols> Eval() const;

    //! Returns true if this matrix is similar to the input matrix within the
    //! given tolerance.
    template <size_t R, size_t C, typename E>
    bool IsSimilar(const MatrixExpression<T, R, C, E>& m,
                   double tol = std::numeric_limits<double>::epsilon()) const;

    //! Returns true if this matrix is a square matrix.
    constexpr bool IsSquare() const;

    ValueType Sum() const;

    ValueType Avg() const;

    ValueType Min() const;

    ValueType Max() const;

    ValueType AbsMin() const;

    ValueType AbsMax() const;

    ValueType Trace() const;

    ValueType Determinant() const;

    size_t DominantAxis() const;

    size_t SubdominantAxis() const;

    ValueType Norm() const;

    ValueType NormSquared() const;

    ValueType FrobeniusNorm() const;

    ValueType Length() const;

    ValueType LengthSquared() const;

    //! Returns the distance to the other vector.
    template <size_t R, size_t C, typename E>
    ValueType DistanceTo(const MatrixExpression<T, R, C, E>& other) const;

    //! Returns the squared distance to the other vector.
    template <size_t R, size_t C, typename E>
    ValueType DistanceSquaredTo(
        const MatrixExpression<T, R, C, E>& other) const;

    MatrixScalarElemWiseBinaryOp<T, Rows, Cols, const Derived&, std::divides<T>>
    Normalized() const;

    //! Returns diagonal part of this matrix.
    MatrixDiagonal<T, Rows, Cols, const Derived&> Diagonal() const;

    //! Returns off-diagonal part of this matrix.
    MatrixOffDiagonal<T, Rows, Cols, const Derived&> OffDiagonal() const;

    //! Returns strictly lower triangle part of this matrix.
    MatrixTri<T, Rows, Cols, const Derived&> StrictLowerTri() const;

    //! Returns strictly upper triangle part of this matrix.
    MatrixTri<T, Rows, Cols, const Derived&> StrictUpperTri() const;

    //! Returns lower triangle part of this matrix (including the diagonal).
    MatrixTri<T, Rows, Cols, const Derived&> LowerTri() const;

    //! Returns upper triangle part of this matrix (including the diagonal).
    MatrixTri<T, Rows, Cols, const Derived&> UpperTri() const;

    MatrixTranspose<T, Rows, Cols, const Derived&> Transposed() const;

    //! Returns inverse matrix.
    Matrix<T, Rows, Cols> Inverse() const;

    template <typename U>
    MatrixUnaryOp<U, Rows, Cols, const Derived&, TypeCast<T, U>> CastTo() const;

    template <size_t R, size_t C, typename E, typename U = ValueType>
    std::enable_if_t<(IsMatrixSizeDynamic<Rows, Cols>() || Cols == 1) &&
                         (IsMatrixSizeDynamic<R, C>() || C == 1),
                     U>
    Dot(const MatrixExpression<T, R, C, E>& expression) const;

    template <size_t R, size_t C, typename E, typename U = ValueType>
    std::enable_if_t<(IsMatrixSizeDynamic<Rows, Cols>() ||
                      (Rows == 2 && Cols == 1)) &&
                         (IsMatrixSizeDynamic<R, C>() || (R == 2 && C == 1)),
                     U>
    Cross(const MatrixExpression<T, R, C, E>& expression) const;

    template <size_t R, size_t C, typename E, typename U = ValueType>
    std::enable_if_t<(IsMatrixSizeDynamic<Rows, Cols>() ||
                      (Rows == 3 && Cols == 1)) &&
                         (IsMatrixSizeDynamic<R, C>() || (R == 3 && C == 1)),
                     Matrix<U, 3, 1>>
    Cross(const MatrixExpression<T, R, C, E>& expression) const;

    //! Returns the reflection vector to the surface with given surface normal.
    template <size_t R, size_t C, typename E, typename U = ValueType>
    std::enable_if_t<(IsMatrixSizeDynamic<Rows, Cols>() ||
                      ((Rows == 2 || Rows == 3) && Cols == 1)) &&
                         (IsMatrixSizeDynamic<R, C>() ||
                          ((R == 2 || R == 3) && C == 1)),
                     Matrix<U, Rows, 1>>
    Reflected(const MatrixExpression<T, R, C, E>& normal) const;

    //! Returns the projected vector to the surface with given surface normal.
    template <size_t R, size_t C, typename E, typename U = ValueType>
    std::enable_if_t<(IsMatrixSizeDynamic<Rows, Cols>() ||
                      ((Rows == 2 || Rows == 3) && Cols == 1)) &&
                         (IsMatrixSizeDynamic<R, C>() ||
                          ((R == 2 || R == 3) && C == 1)),
                     Matrix<U, Rows, 1>>
    Projected(const MatrixExpression<T, R, C, E>& normal) const;

    //! Returns the tangential vector for this vector.
    template <typename U = ValueType>
    std::enable_if_t<(IsMatrixSizeDynamic<Rows, Cols>() ||
                      (Rows == 2 && Cols == 1)),
                     Matrix<U, 2, 1>>
    Tangential() const;

    //! Returns the tangential vectors for this vector.
    template <typename U = ValueType>
    std::enable_if_t<(IsMatrixSizeDynamic<Rows, Cols>() ||
                      (Rows == 3 && Cols == 1)),
                     std::tuple<Matrix<U, 3, 1>, Matrix<U, 3, 1>>>
    Tangentials() const;

    //! Returns actual implementation (the subclass).
    Derived& GetDerived();

    //! Returns actual implementation (the subclass).
    const Derived& GetDerived() const;

 protected:
    // Prohibits constructing this class instance.
    MatrixExpression() = default;

    constexpr static T Determinant(const MatrixExpression<T, 1, 1, Derived>& m);

    constexpr static T Determinant(const MatrixExpression<T, 2, 2, Derived>& m);

    constexpr static T Determinant(const MatrixExpression<T, 3, 3, Derived>& m);

    constexpr static T Determinant(const MatrixExpression<T, 4, 4, Derived>& m);

    template <typename U = ValueType>
    static std::enable_if_t<
        (Rows > 4 && Cols > 4) || IsMatrixSizeDynamic<Rows, Cols>(), U>
    Determinant(const MatrixExpression<T, Rows, Cols, Derived>& m);

    static void Inverse(const MatrixExpression<T, 1, 1, Derived>& m,
                        Matrix<T, Rows, Cols>& result);

    static void Inverse(const MatrixExpression<T, 2, 2, Derived>& m,
                        Matrix<T, Rows, Cols>& result);

    static void Inverse(const MatrixExpression<T, 3, 3, Derived>& m,
                        Matrix<T, Rows, Cols>& result);

    static void Inverse(const MatrixExpression<T, 4, 4, Derived>& m,
                        Matrix<T, Rows, Cols>& result);

    template <typename M = Matrix<T, Rows, Cols>>
    static void Inverse(const MatrixExpression& m,
                        std::enable_if_t<(Rows > 4 && Cols > 4) ||
                                             IsMatrixSizeDynamic<Rows, Cols>(),
                                         M>& result);
};

template <typename T, size_t Rows, size_t Cols>
class MatrixConstant
    : public MatrixExpression<T, Rows, Cols, MatrixConstant<T, Rows, Cols>>
{
 public:
    constexpr MatrixConstant(size_t r, size_t c, const T& val)
        : m_rows(r), m_cols(c), m_val(val)
    {
        // Do nothing
    }

    constexpr size_t GetRows() const;

    constexpr size_t GetCols() const;

    constexpr T operator()(size_t, size_t) const;

 private:
    size_t m_rows;
    size_t m_cols;
    T m_val;
};
}  // namespace CubbyFlow

#include <Core/Matrix/MatrixExpression-Impl.hpp>

#endif