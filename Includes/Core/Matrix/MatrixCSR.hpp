// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MATRIX_CSR_HPP
#define CUBBYFLOW_MATRIX_CSR_HPP

#include <Core/Matrix/Matrix.hpp>

namespace CubbyFlow
{
template <typename T>
class MatrixCSR;

//!
//! \brief Matrix expression for CSR matrix-matrix multiplication.
//!
//! This matrix expression represents a CSR matrix-matrix operation that
//! takes one CSR input matrix expression and one (probably dense) matrix
//! expression.
//!
//! \tparam T   Element value type.
//! \tparam ME  Matrix expression.
//!
template <typename T, typename ME>
class MatrixCSRMatrixMul
    : public MatrixExpression<T, MATRIX_SIZE_DYNAMIC, MATRIX_SIZE_DYNAMIC,
                              MatrixCSRMatrixMul<T, ME>>
{
 public:
    MatrixCSRMatrixMul(const MatrixCSR<T>& m1, const ME& m2);

    //! Number of rows.
    [[nodiscard]] size_t GetRows() const;

    //! Number of columns.
    [[nodiscard]] size_t GetCols() const;

    //! Returns matrix element at (i, j).
    T operator()(size_t i, size_t j) const;

 private:
    const MatrixCSR<T>& m_m1;
    const ME& m_m2;
    const T* const m_nnz;
    const size_t* const m_rp;
    const size_t* const m_ci;
};

//!
//! \brief Compressed Sparse Row (CSR) matrix class.
//!
//! This class defines Compressed Sparse Row (CSR) matrix using arrays of
//! non-zero elements, row pointers, and column indices.
//!
//! \see http://www.netlib.org/utk/people/JackDongarra/etemplates/node373.html
//!
//! \tparam T Type of the element.
//!
template <typename T>
class MatrixCSR final
    : public MatrixExpression<T, MATRIX_SIZE_DYNAMIC, MATRIX_SIZE_DYNAMIC,
                              MatrixCSR<T>>
{
 public:
    static_assert(
        std::is_floating_point<T>::value,
        "MatrixCSR only can be instantiated with floating point types");

    struct Element
    {
        size_t i;
        size_t j;
        T value;

        Element();

        Element(size_t i, size_t j, const T& value);
    };

    using NonZeroContainerType = std::vector<T>;
    using NonZeroIterator = typename NonZeroContainerType::iterator;
    using ConstNonZeroIterator = typename NonZeroContainerType::const_iterator;

    using IndexContainerType = std::vector<size_t>;
    using IndexIterator = IndexContainerType::iterator;
    using ConstIndexIterator = IndexContainerType::const_iterator;

    //! Constructs an empty matrix.
    MatrixCSR();

    //!
    //! \brief Compresses given initializer list \p lst into a sparse matrix.
    //!
    //! This constructor will build a matrix with given initializer list \p lst
    //! such as
    //!
    //! \code{.cpp}
    //! MatrixCSR<float> mat = {
    //!     {1.f, 0.f, 0.f, 3.f},
    //!     {0.f, 3.f, 5.f, 1.f},
    //!     {4.f, 0.f, 1.f, 5.f}
    //! };
    //! \endcode
    //!
    //! Note the initializer has 4x3 structure which will create 4x3 matrix.
    //! During the process, zero elements (less than \p epsilon) will not be
    //! stored.
    //!
    //! \param lst Initializer list that should be copy to the new matrix.
    //!
    MatrixCSR(const std::initializer_list<std::initializer_list<T>>& lst,
              T epsilon = std::numeric_limits<T>::epsilon());

    //!
    //! \brief Compresses input (dense) matrix expression into a sparse matrix.
    //!
    //! This function sets this sparse matrix with dense input matrix.
    //! During the process, zero elements (less than \p epsilon) will not be
    //! stored.
    //!
    template <size_t R, size_t C, typename E>
    MatrixCSR(const MatrixExpression<T, R, C, E>& other,
              T epsilon = std::numeric_limits<T>::epsilon());

    //! Default destructor.
    ~MatrixCSR() = default;

    //! Copy constructor.
    MatrixCSR(const MatrixCSR& other);

    //! Move constructor.
    MatrixCSR(MatrixCSR&& other) noexcept;

    //! Copies to this matrix.
    MatrixCSR& operator=(const MatrixCSR& other);

    //! Moves to this matrix.
    MatrixCSR& operator=(MatrixCSR&& other) noexcept;

    //! Clears the matrix and make it zero-dimensional.
    void Clear();

    //! Sets whole matrix with input scalar.
    void Set(const T& s);

    //! Copy from given sparse matrix.
    void Set(const MatrixCSR& other);

    //! Reserves memory space of this matrix.
    void Reserve(size_t rows, size_t cols, size_t numNonZeros);

    //!
    //! \brief Compresses given initializer list \p lst into a sparse matrix.
    //!
    //! This function will fill the matrix with given initializer list \p lst
    //! such as
    //!
    //! \code{.cpp}
    //! MatrixCSR<float> mat;
    //! mat.compress({
    //!     {1.f, 0.f, 0.f, 3.f},
    //!     {0.f, 3.f, 5.f, 1.f},
    //!     {4.f, 0.f, 1.f, 5.f}
    //! });
    //! \endcode
    //!
    //! Note the initializer has 4x3 structure which will resize to 4x3 matrix.
    //! During the process, zero elements (less than \p epsilon) will not be
    //! stored.
    //!
    //! \param lst Initializer list that should be copy to the new matrix.
    //!
    void Compress(const std::initializer_list<std::initializer_list<T>>& lst,
                  T epsilon = std::numeric_limits<T>::epsilon());

    //!
    //! \brief Compresses input (dense) matrix expression into a sparse matrix.
    //!
    //! This function sets this sparse matrix with dense input matrix.
    //! During the process, zero elements (less than \p epsilon) will not be
    //! stored.
    //!
    template <size_t R, size_t C, typename E>
    void Compress(const MatrixExpression<T, R, C, E>& other,
                  T epsilon = std::numeric_limits<T>::epsilon());

    //! Adds non-zero element to (i, j).
    void AddElement(size_t i, size_t j, const T& value);

    //! Adds non-zero element.
    void AddElement(const Element& element);

    //!
    //! Adds a row to the sparse matrix.
    //!
    //! \param nonZeros - Array of non-zero elements for the row.
    //! \param columnIndices - Array of column indices for the row.
    //!
    void AddRow(const NonZeroContainerType& nonZeros,
                const IndexContainerType& columnIndices);

    //! Sets non-zero element to (i, j).
    void SetElement(size_t i, size_t j, const T& value);

    //! Sets non-zero element.
    void SetElement(const Element& element);

    [[nodiscard]] bool IsEqual(const MatrixCSR& other) const;

    //! Returns true if this matrix is similar to the input matrix within the
    //! given tolerance.
    [[nodiscard]] bool IsSimilar(
        const MatrixCSR& other,
        double tol = std::numeric_limits<double>::epsilon()) const;

    //! Returns true if this matrix is a square matrix.
    [[nodiscard]] bool IsSquare() const;

    //! Returns the size of this matrix.
    [[nodiscard]] Vector2UZ Size() const;

    //! Returns number of rows of this matrix.
    [[nodiscard]] size_t GetRows() const;

    //! Returns number of columns of this matrix.
    [[nodiscard]] size_t GetCols() const;

    //! Returns the number of non-zero elements.
    [[nodiscard]] size_t NumberOfNonZeros() const;

    //! Returns i-th non-zero element.
    [[nodiscard]] const T& NonZero(size_t i) const;

    //! Returns i-th non-zero element.
    [[nodiscard]] T& NonZero(size_t i);

    //! Returns i-th row pointer.
    [[nodiscard]] const size_t& RowPointer(size_t i) const;

    //! Returns i-th column index.
    [[nodiscard]] const size_t& ColumnIndex(size_t i) const;

    //! Returns pointer of the non-zero elements data.
    [[nodiscard]] T* NonZeroData();

    //! Returns constant pointer of the non-zero elements data.
    [[nodiscard]] const T* NonZeroData() const;

    //! Returns constant pointer of the row pointers data.
    [[nodiscard]] const size_t* RowPointersData() const;

    //! Returns constant pointer of the column indices data.
    [[nodiscard]] const size_t* ColumnIndicesData() const;

    //! Returns the begin iterator of the non-zero elements.
    [[nodiscard]] NonZeroIterator NonZeroBegin();

    //! Returns the begin const iterator of the non-zero elements.
    [[nodiscard]] ConstNonZeroIterator NonZeroBegin() const;

    //! Returns the end iterator of the non-zero elements.
    [[nodiscard]] NonZeroIterator NonZeroEnd();

    //! Returns the end const iterator of the non-zero elements.
    [[nodiscard]] ConstNonZeroIterator NonZeroEnd() const;

    //! Returns the begin iterator of the row pointers.
    [[nodiscard]] IndexIterator RowPointersBegin();

    //! Returns the begin const iterator of the row pointers.
    [[nodiscard]] ConstIndexIterator RowPointersBegin() const;

    //! Returns the end iterator of the row pointers.
    [[nodiscard]] IndexIterator RowPointersEnd();

    //! Returns the end const iterator of the row pointers.
    [[nodiscard]] ConstIndexIterator RowPointersEnd() const;

    //! Returns the begin iterator of the column indices.
    [[nodiscard]] IndexIterator ColumnIndicesBegin();

    //! Returns the begin const iterator of the column indices.
    [[nodiscard]] ConstIndexIterator ColumnIndicesBegin() const;

    //! Returns the end iterator of the column indices.
    [[nodiscard]] IndexIterator ColumnIndicesEnd();

    //! Returns the end const iterator of the column indices.
    [[nodiscard]] ConstIndexIterator ColumnIndicesEnd() const;

    //! Returns this matrix + input scalar.
    [[nodiscard]] MatrixCSR Add(const T& s) const;

    //! Returns this matrix + input matrix (element-wise).
    [[nodiscard]] MatrixCSR Add(const MatrixCSR& m) const;

    //! Returns this matrix - input scalar.
    [[nodiscard]] MatrixCSR Sub(const T& s) const;

    //! Returns this matrix - input matrix (element-wise).
    [[nodiscard]] MatrixCSR Sub(const MatrixCSR& m) const;

    //! Returns this matrix * input scalar.
    [[nodiscard]] MatrixCSR Mul(const T& s) const;

    //! Returns this matrix * input matrix.
    template <size_t R, size_t C, typename ME>
    [[nodiscard]] MatrixCSRMatrixMul<T, ME> Mul(
        const MatrixExpression<T, R, C, ME>& m) const;

    //! Returns this matrix / input scalar.
    [[nodiscard]] MatrixCSR Div(const T& s) const;

    //! Returns input scalar + this matrix.
    [[nodiscard]] MatrixCSR RAdd(const T& s) const;

    //! Returns input matrix + this matrix (element-wise).
    [[nodiscard]] MatrixCSR RAdd(const MatrixCSR& m) const;

    //! Returns input scalar - this matrix.
    [[nodiscard]] MatrixCSR RSub(const T& s) const;

    //! Returns input matrix - this matrix (element-wise).
    [[nodiscard]] MatrixCSR RSub(const MatrixCSR& m) const;

    //! Returns input scalar * this matrix.
    [[nodiscard]] MatrixCSR RMul(const T& s) const;

    //! Returns input matrix / this scalar.
    [[nodiscard]] MatrixCSR RDiv(const T& s) const;

    //! Adds input scalar to this matrix.
    void IAdd(const T& s);

    //! Adds input matrix to this matrix (element-wise).
    void IAdd(const MatrixCSR& m);

    //! Subtracts input scalar from this matrix.
    void ISub(const T& s);

    //! Subtracts input matrix from this matrix (element-wise).
    void ISub(const MatrixCSR& m);

    //! Multiplies input scalar to this matrix.
    void IMul(const T& s);

    //! Multiplies input matrix to this matrix.
    template <size_t R, size_t C, typename ME>
    void IMul(const MatrixExpression<T, R, C, ME>& m);

    //! Divides this matrix with input scalar.
    void IDiv(const T& s);

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

    //! Type-casts to different value-typed matrix.
    template <typename U>
    MatrixCSR<U> CastTo() const;

    //!
    //! \brief Compresses input (dense) matrix expression into a sparse matrix.
    //!
    //! This function sets this sparse matrix with dense input matrix.
    //! During the process, zero elements (less than \p epsilon) will not be
    //! stored.
    //!
    template <size_t R, size_t C, typename ME>
    MatrixCSR& operator=(const MatrixExpression<T, R, C, ME>& m);

    //! Addition assignment with input scalar.
    MatrixCSR& operator+=(const T& s);

    //! Addition assignment with input matrix (element-wise).
    MatrixCSR& operator+=(const MatrixCSR& m);

    //! Subtraction assignment with input scalar.
    MatrixCSR& operator-=(const T& s);

    //! Subtraction assignment with input matrix (element-wise).
    MatrixCSR& operator-=(const MatrixCSR& m);

    //! Multiplication assignment with input scalar.
    MatrixCSR& operator*=(const T& s);

    //! Multiplication assignment with input matrix.
    template <size_t R, size_t C, typename ME>
    MatrixCSR& operator*=(const MatrixExpression<T, R, C, ME>& m);

    //! Division assignment with input scalar.
    MatrixCSR& operator/=(const T& s);

    //! Returns (i,j) element.
    T operator()(size_t i, size_t j) const;

    //! Returns true if is equal to m.
    bool operator==(const MatrixCSR& m) const;

    //! Returns true if is not equal to m.
    bool operator!=(const MatrixCSR& m) const;

    //! Makes a m x m matrix with all diagonal elements to 1, and other elements
    //! to 0.
    static MatrixCSR<T> MakeIdentity(size_t m);

 private:
    [[nodiscard]] size_t HasElement(size_t i, size_t j) const;

    template <typename Op>
    MatrixCSR BinaryOp(const MatrixCSR& m, Op op) const;

    Vector2UZ m_size;
    NonZeroContainerType m_nonZeros;
    IndexContainerType m_rowPointers;
    IndexContainerType m_columnIndices;
};

//! Float-type CSR matrix.
typedef MatrixCSR<float> MatrixCSRF;

//! Double-type CSR matrix.
typedef MatrixCSR<double> MatrixCSRD;
}  // namespace CubbyFlow

#include <Core/Matrix/MatrixCSR-Impl.hpp>

#endif