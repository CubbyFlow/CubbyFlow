// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MATRIXMXN_HPP
#define CUBBYFLOW_MATRIXMXN_HPP

#include <Core/Array/Array2.hpp>
#include <Core/Matrix/MatrixExpression.hpp>

namespace CubbyFlow
{
//!
//! \brief M x N matrix class.
//!
//! This class defines M x N row-major matrix.
//!
//! \tparam T Type of the element.
//!
template <typename T>
class MatrixMxN final : public MatrixExpression<T, MatrixMxN<T>>
{
 public:
    static_assert(
        std::is_floating_point<T>::value,
        "MatrixMxN only can be instantiated with floating point types");

    using ContainerType = Array2<T>;
    using Iterator = typename ContainerType::Iterator;
    using ConstIterator = typename ContainerType::ConstIterator;

    //! Constructs an empty matrix.
    MatrixMxN() = default;

    //! Constructs m x n constant value matrix.
    MatrixMxN(size_t m, size_t n, const T& s = T(0));

    //!
    //! \brief Constructs a matrix with given initializer list \p list.
    //!
    //! This constructor will build a matrix with given initializer list \p list
    //! such as
    //!
    //! \code{.cpp}
    //! MatrixMxN<float> mat =
    //! {
    //!     {1.f, 2.f, 4.f, 3.f},
    //!     {9.f, 3.f, 5.f, 1.f},
    //!     {4.f, 8.f, 1.f, 5.f}
    //! };
    //! \endcode
    //!
    //! Note the initializer has 4x3 structure which will create 4x3 matrix.
    //!
    //! \param list Initializer list that should be copy to the new matrix.
    //!
    MatrixMxN(const std::initializer_list<std::initializer_list<T>>& list);

    //! Constructs a matrix with expression template.
    template <typename E>
    MatrixMxN(const MatrixExpression<T, E>& other);

    //! Constructs a m x n matrix with input array.
    //! \warning Ordering of the input elements is row-major.
    MatrixMxN(size_t m, size_t n, const T* arr);

    //! Copy constructor.
    MatrixMxN(const MatrixMxN& other);

    //! Move constructor.
    MatrixMxN(MatrixMxN&& other) noexcept;

    //! Default destructor.
    ~MatrixMxN() = default;

    //! Copy assignment operator.
    MatrixMxN& operator=(const MatrixMxN& other);

    //! Move assignment operator.
    MatrixMxN& operator=(MatrixMxN&& other) noexcept;

    //! Resizes to m x n matrix with initial value \p s.
    void Resize(size_t m, size_t n, const T& s = T(0));

    //! Sets whole matrix with input scalar.
    void Set(const T& s);

    //!
    //! \brief Sets a matrix with given initializer list \p list.
    //!
    //! This function will fill the matrix with given initializer list \p list
    //! such as
    //!
    //! \code{.cpp}
    //! MatrixMxN<float> mat;
    //! mat.set(
    //! {
    //!     {1.f, 2.f, 4.f, 3.f},
    //!     {9.f, 3.f, 5.f, 1.f},
    //!     {4.f, 8.f, 1.f, 5.f}
    //! });
    //! \endcode
    //!
    //! Note the initializer has 4x3 structure which will resize to 4x3 matrix.
    //!
    //! \param list Initializer list that should be copy to the new matrix.
    //!
    void Set(const std::initializer_list<std::initializer_list<T>>& list);

    //! Copies from input matrix expression.
    template <typename E>
    void Set(const MatrixExpression<T, E>& other);

    //! Copies from input array.
    //! \warning Ordering of the input elements is row-major.
    void Set(size_t m, size_t n, const T* arr);

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
    [[nodiscard]] bool IsSquare() const;

    //! Returns the size of this matrix.
    [[nodiscard]] Size2 size() const;

    //! Returns number of rows of this matrix.
    [[nodiscard]] size_t Rows() const;

    //! Returns number of columns of this matrix.
    [[nodiscard]] size_t Cols() const;

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
    [[nodiscard]] MatrixScalarAdd<T, MatrixMxN> Add(const T& s) const;

    //! Returns this matrix + input matrix (element-wise).
    template <typename E>
    [[nodiscard]] MatrixAdd<T, MatrixMxN, E> Add(const E& m) const;

    //! Returns this matrix - input scalar.
    [[nodiscard]] MatrixScalarSub<T, MatrixMxN> Sub(const T& s) const;

    //! Returns this matrix - input matrix (element-wise).
    template <typename E>
    [[nodiscard]] MatrixSub<T, MatrixMxN, E> Sub(const E& m) const;

    //! Returns this matrix * input scalar.
    [[nodiscard]] MatrixScalarMul<T, MatrixMxN> Mul(const T& s) const;

    //! Returns this matrix * input vector.
    template <typename VE>
    [[nodiscard]] MatrixVectorMul<T, MatrixMxN, VE> Mul(
        const VectorExpression<T, VE>& v) const;

    //! Returns this matrix * input matrix.
    template <typename E>
    [[nodiscard]] MatrixMul<T, MatrixMxN, E> Mul(const E& m) const;

    //! Returns this matrix / input scalar.
    [[nodiscard]] MatrixScalarDiv<T, MatrixMxN> Div(const T& s) const;

    //! Returns input scalar + this matrix.
    [[nodiscard]] MatrixScalarAdd<T, MatrixMxN> RAdd(const T& s) const;

    //! Returns input matrix + this matrix (element-wise).
    template <typename E>
    [[nodiscard]] MatrixAdd<T, MatrixMxN, E> RAdd(const E& m) const;

    //! Returns input scalar - this matrix.
    [[nodiscard]] MatrixScalarRSub<T, MatrixMxN> RSub(const T& s) const;

    //! Returns input matrix - this matrix (element-wise).
    template <typename E>
    [[nodiscard]] MatrixSub<T, MatrixMxN, E> RSub(const E& m) const;

    //! Returns input scalar * this matrix.
    [[nodiscard]] MatrixScalarMul<T, MatrixMxN> RMul(const T& s) const;

    //! Returns input matrix * this matrix.
    template <typename E>
    [[nodiscard]] MatrixMul<T, E, MatrixMxN> RMul(const E& m) const;

    //! Returns input matrix / this scalar.
    [[nodiscard]] MatrixScalarRDiv<T, MatrixMxN> RDiv(const T& s) const;

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
    [[nodiscard]] MatrixDiagonal<T, MatrixMxN> Diagonal() const;

    //! Returns off-diagonal part of this matrix.
    [[nodiscard]] MatrixDiagonal<T, MatrixMxN> OffDiagonal() const;

    //! Returns strictly lower triangle part of this matrix.
    [[nodiscard]] MatrixTriangular<T, MatrixMxN> StrictLowerTri() const;

    //! Returns strictly upper triangle part of this matrix.
    [[nodiscard]] MatrixTriangular<T, MatrixMxN> StrictUpperTri() const;

    //! Returns lower triangle part of this matrix (including the diagonal).
    [[nodiscard]] MatrixTriangular<T, MatrixMxN> LowerTri() const;

    //! Returns upper triangle part of this matrix (including the diagonal).
    [[nodiscard]] MatrixTriangular<T, MatrixMxN> UpperTri() const;

    //! Returns transposed matrix.
    [[nodiscard]] MatrixMxN Transposed() const;

    //! Returns inverse matrix.
    [[nodiscard]] MatrixMxN Inverse() const;

    //! Type-casts to different value-typed matrix.
    template <typename U>
    [[nodiscard]] MatrixTypeCast<U, MatrixMxN, T> CastTo() const;

    //! Assigns input matrix.
    template <typename E>
    MatrixMxN& operator=(const E& m);

    //! Addition assignment with input scalar.
    MatrixMxN& operator+=(const T& s);

    //! Addition assignment with input matrix (element-wise).
    template <typename E>
    MatrixMxN& operator+=(const E& m);

    //! Subtraction assignment with input scalar.
    MatrixMxN& operator-=(const T& s);

    //! Subtraction assignment with input matrix (element-wise).
    template <typename E>
    MatrixMxN& operator-=(const E& m);

    //! Multiplication assignment with input scalar.
    MatrixMxN& operator*=(const T& s);

    //! Multiplication assignment with input matrix.
    template <typename E>
    MatrixMxN& operator*=(const E& m);

    //! Division assignment with input scalar.
    MatrixMxN& operator/=(const T& s);

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
    //! for (size_t i = 0; i < mat.Rows(); ++i)
    //! {
    //!     for (size_t j = 0; j < mat.Cols(); ++j)
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
    //! mat.ForEach([](double elem)
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
    //! for (size_t i = 0; i < mat.Rows(); ++i)
    //! {
    //!     for (size_t j = 0; j < mat.Cols(); ++j)
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
    //! mat.ForEachIndex([&](size_t i, size_t j)
    //! {
    //!     mat(i, j) = 4.0 * i + 7.0 * j + 1.5;
    //! });
    //! \endcode
    //!
    template <typename Callback>
    void ForEachIndex(Callback func) const;

    //!
    //! \brief Iterates the matrix and invoke given \p func for each index in
    //! parallel.
    //!
    //! This function iterates the matrix elements and invoke the callback
    //! function \p func. The callback function takes matrix's element as its
    //! input. The order of execution will be non-deterministic since it runs in
    //! parallel. Below is the sample usage:
    //!
    //! \code{.cpp}
    //! MatrixMxN<double> mat(100, 200, 4.0);
    //! mat.ParallelForEach([](double& elem)
    //! {
    //!     elem *= 2.0;
    //! });
    //! \endcode
    //!
    //! The parameter type of the callback function doesn't have to be T&, but
    //! const T& or T can be used as well.
    //!
    template <typename Callback>
    void ParallelForEach(Callback func);

    //!
    //! \brief Iterates the matrix and invoke given \p func for each index in
    //!     parallel using multi-threading.
    //!
    //! This function iterates the matrix elements and invoke the callback
    //! function \p func in parallel using multi-threading. The callback
    //! function takes two parameters which are the (i, j) indices of the
    //! matrix. The order of execution will be non-deterministic since it runs
    //! in parallel. Below is the sample usage:
    //!
    //! \code{.cpp}
    //! MatrixMxN<double> mat(100, 200, 4.0);
    //! mat.ParallelForEachIndex([&](size_t i, size_t j)
    //! {
    //!     mat(i, j) *= 2;
    //! });
    //! \endcode
    //!
    template <typename Callback>
    void ParallelForEachIndex(Callback func) const;

    //! Makes a m x n matrix with zeros.
    [[nodiscard]] static MatrixConstant<T> MakeZero(size_t m, size_t n);

    //! Makes a m x m matrix with all diagonal elements to 1, and other elements
    //! to 0.
    [[nodiscard]] static MatrixIdentity<T> MakeIdentity(size_t m);

 private:
    ContainerType m_elements;
};

//! Float-type M x N matrix.
using MatrixMxNF = MatrixMxN<float>;

//! Double-type M x N matrix.
using MatrixMxND = MatrixMxN<double>;
}  // namespace CubbyFlow

#include <Core/Matrix/MatrixMxN-Impl.hpp>

#endif