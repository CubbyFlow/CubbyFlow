// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MATRIX_HPP
#define CUBBYFLOW_MATRIX_HPP

#include <Core/Matrix/MatrixExpression.hpp>

#include <array>
#include <type_traits>

namespace CubbyFlow
{
//!
//! \brief Static-sized M x N matrix class.
//!
//! This class defines M x N row-major matrix data where its size is determined
//! statically at compile time.
//!
//! \tparam T - Real number type.
//! \tparam M - Number of rows.
//! \tparam N - Number of columns.
//!
template <typename T, size_t M, size_t N>
class Matrix final : public MatrixExpression<T, Matrix<T, M, N>>
{
 public:
    static_assert(
        M > 0,
        "Number of rows for static-sized matrix should be greater than zero.");
    static_assert(N > 0,
                  "Number of columns for static-sized matrix should be greater "
                  "than zero.");
    static_assert(!(M == 2 && N == 2) && !(M == 3 && N == 3) &&
                      !(M == 4 && N == 4),
                  "Use specialized matrix for 2z2, 3x3, and 4x4 matrices.");
    static_assert(std::is_floating_point<T>::value,
                  "Matrix only can be instantiated with floating point types.");

    using ContainerType = std::array<T, M * N>;
    using Iterator = typename ContainerType::iterator;
    using ConstIterator = typename ContainerType::const_iterator;

    //! Default constructor.
    //! \warning This constructor will create zero matrix.
    Matrix();

    //! Constructs matrix instance with parameters.
    template <typename... Params>
    explicit Matrix(Params... params);

    //!
    //! \brief Constructs a matrix with given initializer list \p list.
    //!
    //! This constructor will build a matrix with given initializer list \p list
    //! such as
    //!
    //! \code{.cpp}
    //! Matrix<float, 3, 4> mat =
    //! {
    //!     {1.f, 2.f, 4.f, 3.f},
    //!     {9.f, 3.f, 5.f, 1.f},
    //!     {4.f, 8.f, 1.f, 5.f}
    //! };
    //! \endcode
    //!
    //! \param list Initializer list that should be copy to the new matrix.
    //!
    Matrix(const std::initializer_list<std::initializer_list<T>>& list);

    //! Constructs a matrix with expression template.
    template <typename E>
    Matrix(const MatrixExpression<T, E>& other);

    //! Copy constructor.
    Matrix(const Matrix& other);

    //! Default move constructor.
    Matrix(Matrix&&) noexcept = default;

    //! Default destructor.
    ~Matrix() = default;

    //! Copy assignment operator.
    Matrix& operator=(const Matrix& other);

    //! Default move assignment operator.
    Matrix& operator=(Matrix&&) noexcept = default;

    //! Sets whole matrix with input scalar.
    void Set(const T& s);

    //!
    //! \brief Sets a matrix with given initializer list \p list.
    //!
    //! This function will fill the matrix with given initializer list \p list
    //! such as
    //!
    //! \code{.cpp}
    //! Matrix<float, 3, 4> mat;
    //! mat.set(
    //! {
    //!     {1.f, 2.f, 4.f, 3.f},
    //!     {9.f, 3.f, 5.f, 1.f},
    //!     {4.f, 8.f, 1.f, 5.f}
    //! });
    //! \endcode
    //!
    //! \param list Initializer list that should be copy to the new matrix.
    //!
    void Set(const std::initializer_list<std::initializer_list<T>>& list);

    //! Copies from input matrix expression.
    template <typename E>
    void Set(const MatrixExpression<T, E>& other);

    //! Sets diagonal elements with input scalar.
    void SetDiagonal(const T& s);

    //! Sets off-diagonal elements with input scalar.
    void SetOffDiagonal(const T& s);

    //! Sets i-th row with input vector.
    template <typename E>
    void SetRow(size_t i, const VectorExpression<T, E>& row);

    //! Sets j-th column with input vector.
    template <typename E>
    void SetColumn(size_t j, const VectorExpression<T, E>& col);

    template <typename E>
    [[nodiscard]] bool IsEqual(const MatrixExpression<T, E>& other) const;

    //! Returns true if this matrix is similar to the input matrix within the
    //! given tolerance.
    template <typename E>
    [[nodiscard]] bool IsSimilar(
        const MatrixExpression<T, E>& other,
        double tol = std::numeric_limits<double>::epsilon()) const;

    //! Returns true if this matrix is a square matrix.
    [[nodiscard]] static constexpr bool IsSquare();

    //! Returns the size of this matrix.
    [[nodiscard]] static constexpr Size2 size();

    //! Returns number of rows of this matrix.
    [[nodiscard]] static constexpr size_t Rows();

    //! Returns number of columns of this matrix.
    [[nodiscard]] static constexpr size_t Cols();

    //! Returns data pointer of this matrix.
    [[nodiscard]] T* data();

    //! Returns constant pointer of this matrix.
    [[nodiscard]] const T* data() const;

    //! Returns the begin iterator of the matrix.
    [[nodiscard]] Iterator begin();

    //! Returns the begin const iterator of the matrix.
    [[nodiscard]] ConstIterator begin() const;

    //! Returns the end iterator of the matrix.
    [[nodiscard]] Iterator end();

    //! Returns the end const iterator of the matrix.
    [[nodiscard]] ConstIterator end() const;

    //! Returns this matrix + input scalar.
    [[nodiscard]] MatrixScalarAdd<T, Matrix> Add(const T& s) const;

    //! Returns this matrix + input matrix (element-wise).
    template <typename E>
    [[nodiscard]] MatrixAdd<T, Matrix, E> Add(const E& m) const;

    //! Returns this matrix - input scalar.
    [[nodiscard]] MatrixScalarSub<T, Matrix> Sub(const T& s) const;

    //! Returns this matrix - input matrix (element-wise).
    template <typename E>
    [[nodiscard]] MatrixSub<T, Matrix, E> Sub(const E& m) const;

    //! Returns this matrix * input scalar.
    [[nodiscard]] MatrixScalarMul<T, Matrix> Mul(const T& s) const;

    //! Returns this matrix * input vector.
    template <typename VE>
    [[nodiscard]] MatrixVectorMul<T, Matrix, VE> Mul(
        const VectorExpression<T, VE>& v) const;

    //! Returns this matrix * input matrix.
    template <size_t L>
    [[nodiscard]] MatrixMul<T, Matrix, Matrix<T, N, L>> Mul(
        const Matrix<T, N, L>& m) const;

    //! Returns this matrix / input scalar.
    [[nodiscard]] MatrixScalarDiv<T, Matrix> Div(const T& s) const;

    //! Returns input scalar + this matrix.
    [[nodiscard]] MatrixScalarAdd<T, Matrix> RAdd(const T& s) const;

    //! Returns input matrix + this matrix (element-wise).
    template <typename E>
    [[nodiscard]] MatrixAdd<T, Matrix, E> RAdd(const E& m) const;

    //! Returns input scalar - this matrix.
    [[nodiscard]] MatrixScalarRSub<T, Matrix> RSub(const T& s) const;

    //! Returns input matrix - this matrix (element-wise).
    template <typename E>
    [[nodiscard]] MatrixSub<T, Matrix, E> RSub(const E& m) const;

    //! Returns input scalar * this matrix.
    [[nodiscard]] MatrixScalarMul<T, Matrix> RMul(const T& s) const;

    //! Returns input matrix * this matrix.
    template <size_t L>
    [[nodiscard]] MatrixMul<T, Matrix<T, N, L>, Matrix> RMul(
        const Matrix<T, N, L>& m) const;

    //! Returns input matrix / this scalar.
    [[nodiscard]] MatrixScalarRDiv<T, Matrix> RDiv(const T& s) const;

    //! Adds input scalar to this matrix.
    void IAdd(const T& s);

    //! Adds input matrix to this matrix (element-wise).
    template <typename E>
    void IAdd(const E& m);

    //! Subtracts input scalar from this matrix.
    void ISub(const T& s);

    //! Subtracts input matrix from this matrix (element-wise).
    template <typename E>
    void ISub(const E& m);

    //! Multiplies input scalar to this matrix.
    void IMul(const T& s);

    //! Multiplies input matrix to this matrix.
    template <typename E>
    void IMul(const E& m);

    //! Divides this matrix with input scalar.
    void IDiv(const T& s);

    //! Transposes this matrix.
    void Transpose();

    //!
    //! \brief Inverts this matrix.
    //!
    //! This function computes the inverse using Gaussian elimination method.
    //!
    void Invert();

    //! Returns sum of all elements.
    [[nodiscard]] T Sum() const;

    //! Returns average of all elements.
    [[nodiscard]] T Avg() const;

    //! Returns minimum among all elements.
    [[nodiscard]] T Min() const;

    //! Returns maximum among all elements.
    [[nodiscard]] T Max() const;

    //! Returns absolute minimum among all elements.
    [[nodiscard]] T AbsMin() const;

    //! Returns absolute maximum among all elements.
    [[nodiscard]] T AbsMax() const;

    //! Returns sum of all diagonal elements.
    //! \warning Should be a square matrix.
    [[nodiscard]] T Trace() const;

    //! Returns determinant of this matrix.
    [[nodiscard]] T Determinant() const;

    //! Returns diagonal part of this matrix.
    [[nodiscard]] MatrixDiagonal<T, Matrix> Diagonal() const;

    //! Returns off-diagonal part of this matrix.
    [[nodiscard]] MatrixDiagonal<T, Matrix> OffDiagonal() const;

    //! Returns strictly lower triangle part of this matrix.
    [[nodiscard]] MatrixTriangular<T, Matrix> StrictLowerTri() const;

    //! Returns strictly upper triangle part of this matrix.
    [[nodiscard]] MatrixTriangular<T, Matrix> StrictUpperTri() const;

    //! Returns lower triangle part of this matrix (including the diagonal).
    [[nodiscard]] MatrixTriangular<T, Matrix> LowerTri() const;

    //! Returns upper triangle part of this matrix (including the diagonal).
    [[nodiscard]] MatrixTriangular<T, Matrix> UpperTri() const;

    //! Returns transposed matrix.
    [[nodiscard]] Matrix<T, N, M> Transposed() const;

    //! Returns inverse matrix.
    [[nodiscard]] Matrix Inverse() const;

    template <typename U>
    [[nodiscard]] MatrixTypeCast<U, Matrix, T> CastTo() const;

    //! Assigns input matrix.
    template <typename E>
    Matrix& operator=(const E& m);

    //! Addition assignment with input scalar.
    Matrix& operator+=(const T& s);

    //! Addition assignment with input matrix (element-wise).
    template <typename E>
    Matrix& operator+=(const E& m);

    //! Subtraction assignment with input scalar.
    Matrix& operator-=(const T& s);

    //! Subtraction assignment with input matrix (element-wise).
    template <typename E>
    Matrix& operator-=(const E& m);

    //! Multiplication assignment with input scalar.
    Matrix& operator*=(const T& s);

    //! Multiplication assignment with input matrix.
    template <typename E>
    Matrix& operator*=(const E& m);

    //! Division assignment with input scalar.
    Matrix& operator/=(const T& s);

    //! Returns reference of i-th element.
    T& operator[](size_t i);

    //! Returns constant reference of i-th element.
    const T& operator[](size_t i) const;

    //! Returns reference of (i,j) element.
    T& operator()(size_t i, size_t j);

    //! Returns constant reference of (i,j) element.
    const T& operator()(size_t i, size_t j) const;

    //! Returns true if is equal to m.
    template <typename E>
    bool operator==(const MatrixExpression<T, E>& m) const;

    //! Returns true if is not equal to m.
    template <typename E>
    bool operator!=(const MatrixExpression<T, E>& m) const;

    //!
    //! \brief Iterates the matrix and invoke given \p func for each index.
    //!
    //! This function iterates the matrix elements and invoke the callback
    //! function \p func. The callback function takes matrix's element as its
    //! input. The order of execution will be the same as the nested for-loop
    //! below:
    //!
    //! \code{.cpp}
    //! MatrixMxN<double> mat(100, 200, 4.0);
    //! for (size_t i = 0; i < mat.rows(); ++i)
    //! {
    //!     for (size_t j = 0; j < mat.cols(); ++j)
    //!     {
    //!         func(mat(i, j));
    //!     }
    //! }
    //! \endcode
    //!
    //! Below is the sample usage:
    //!
    //! \code{.cpp}
    //! MatrixMxN<double> mat(100, 200, 4.0);
    //! mat.forEach([](double elem)
    //! {
    //!     printf("%d\n", elem);
    //! });
    //! \endcode
    //!
    template <typename Callback>
    void ForEach(Callback func) const;

    //!
    //! \brief Iterates the matrix and invoke given \p func for each index.
    //!
    //! This function iterates the matrix elements and invoke the callback
    //! function \p func. The callback function takes two parameters which are
    //! the (i, j) indices of the matrix. The order of execution will be the
    //! same as the nested for-loop below:
    //!
    //! \code{.cpp}
    //! MatrixMxN<double> mat(100, 200, 4.0);
    //! for (size_t i = 0; i < mat.rows(); ++i)
    //! {
    //!     for (size_t j = 0; j < mat.cols(); ++j)
    //!     {
    //!         func(i, j);
    //!     }
    //! }
    //! \endcode
    //!
    //! Below is the sample usage:
    //!
    //! \code{.cpp}
    //! MatrixMxN<double> mat(100, 200, 4.0);
    //! mat.forEachIndex([&](size_t i, size_t j)
    //! {
    //!     mat(i, j) = 4.0 * i + 7.0 * j + 1.5;
    //! });
    //! \endcode
    //!
    template <typename Callback>
    static void ForEachIndex(Callback func);

    //! Makes a M x N matrix with zeros.
    [[nodiscard]] static MatrixConstant<T> MakeZero();

    //! Makes a M x N matrix with all diagonal elements to 1, and other elements
    //! to 0.
    [[nodiscard]] static MatrixIdentity<T> MakeIdentity();

 private:
    ContainerType m_elements;

    template <typename... Params>
    void SetRowAt(size_t i, T v, Params... params);
    void SetRowAt(size_t i, T v);
};
}  // namespace CubbyFlow

#include <Core/Matrix/Matrix-Impl.hpp>

#endif