// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MATRIX_IMPL_HPP
#define CUBBYFLOW_MATRIX_IMPL_HPP

namespace CubbyFlow
{
template <typename T, size_t M, size_t N>
Matrix<T, M, N>::Matrix()
{
    for (auto& elem : m_elements)
    {
        elem = 0;
    }
}

template <typename T, size_t M, size_t N>
template <typename... Params>
Matrix<T, M, N>::Matrix(Params... params)
{
    static_assert(sizeof...(params) == M * N, "Invalid number of elements.");

    SetRowAt(0, params...);
}

template <typename T, size_t M, size_t N>
Matrix<T, M, N>::Matrix(
    const std::initializer_list<std::initializer_list<T>>& list)
{
    Set(list);
}

template <typename T, size_t M, size_t N>
template <typename E>
Matrix<T, M, N>::Matrix(const MatrixExpression<T, E>& other)
{
    Set(other);
}

template <typename T, size_t M, size_t N>
Matrix<T, M, N>::Matrix(const Matrix& other)
{
    Set(other);
}

template <typename T, size_t M, size_t N>
Matrix<T, M, N>& Matrix<T, M, N>::operator=(const Matrix& other)
{
    Set(other);
    return *this;
}

template <typename T, size_t M, size_t N>
void Matrix<T, M, N>::Set(const T& s)
{
    m_elements.fill(s);
}

template <typename T, size_t M, size_t N>
void Matrix<T, M, N>::Set(
    const std::initializer_list<std::initializer_list<T>>& list)
{
    const size_t rows = list.size();
    const size_t cols = (rows > 0) ? list.begin()->size() : 0;

    assert(rows == M);
    assert(cols == N);

    auto rowIter = list.begin();
    for (size_t i = 0; i < rows; ++i)
    {
        assert(cols == rowIter->size());

        auto colIter = rowIter->begin();

        for (size_t j = 0; j < cols; ++j)
        {
            (*this)(i, j) = *colIter;
            ++colIter;
        }

        ++rowIter;
    }
}

template <typename T, size_t M, size_t N>
template <typename E>
void Matrix<T, M, N>::Set(const MatrixExpression<T, E>& other)
{
    const E& expression = other();
    ForEachIndex([&](size_t i, size_t j) { (*this)(i, j) = expression(i, j); });
}

template <typename T, size_t M, size_t N>
void Matrix<T, M, N>::SetDiagonal(const T& s)
{
    const size_t l = std::min(Rows(), Cols());

    for (size_t i = 0; i < l; ++i)
    {
        (*this)(i, i) = s;
    }
}

template <typename T, size_t M, size_t N>
void Matrix<T, M, N>::SetOffDiagonal(const T& s)
{
    ForEachIndex([&](size_t i, size_t j) {
        if (i != j)
        {
            (*this)(i, j) = s;
        }
    });
}

template <typename T, size_t M, size_t N>
template <typename E>
void Matrix<T, M, N>::SetRow(size_t i, const VectorExpression<T, E>& row)
{
    assert(Cols() == row.size());

    const E& e = row();

    for (size_t j = 0; j < N; ++j)
    {
        (*this)(i, j) = e[j];
    }
}

template <typename T, size_t M, size_t N>
template <typename E>
void Matrix<T, M, N>::SetColumn(size_t j, const VectorExpression<T, E>& col)
{
    assert(Rows() == col.size());

    const E& e = col();

    for (size_t i = 0; i < M; ++i)
    {
        (*this)(i, j) = e[j];
    }
}

template <typename T, size_t M, size_t N>
template <typename E>
bool Matrix<T, M, N>::IsEqual(const MatrixExpression<T, E>& other) const
{
    if (size() != other.size())
    {
        return false;
    }

    const E& e = other();

    for (size_t i = 0; i < Rows(); ++i)
    {
        for (size_t j = 0; j < Cols(); ++j)
        {
            if ((*this)(i, j) != e(i, j))
            {
                return false;
            }
        }
    }

    return true;
}

template <typename T, size_t M, size_t N>
template <typename E>
bool Matrix<T, M, N>::IsSimilar(const MatrixExpression<T, E>& other,
                                double tol) const
{
    if (size() != other.size())
    {
        return false;
    }

    const E& e = other();

    for (size_t i = 0; i < Rows(); ++i)
    {
        for (size_t j = 0; j < Cols(); ++j)
        {
            if (std::fabs((*this)(i, j) - e(i, j)) > tol)
            {
                return false;
            }
        }
    }

    return true;
}

template <typename T, size_t M, size_t N>
constexpr bool Matrix<T, M, N>::IsSquare()
{
    return M == N;
}

template <typename T, size_t M, size_t N>
constexpr Size2 Matrix<T, M, N>::size()
{
    return Size2{ M, N };
}

template <typename T, size_t M, size_t N>
constexpr size_t Matrix<T, M, N>::Rows()
{
    return M;
}

template <typename T, size_t M, size_t N>
constexpr size_t Matrix<T, M, N>::Cols()
{
    return N;
}

template <typename T, size_t M, size_t N>
T* Matrix<T, M, N>::data()
{
    return m_elements.data();
}

template <typename T, size_t M, size_t N>
const T* Matrix<T, M, N>::data() const
{
    return m_elements.data();
}

template <typename T, size_t M, size_t N>
typename Matrix<T, M, N>::Iterator Matrix<T, M, N>::begin()
{
    return m_elements.begin();
}

template <typename T, size_t M, size_t N>
typename Matrix<T, M, N>::ConstIterator Matrix<T, M, N>::begin() const
{
    return m_elements.begin();
}

template <typename T, size_t M, size_t N>
typename Matrix<T, M, N>::Iterator Matrix<T, M, N>::end()
{
    return m_elements.end();
}

template <typename T, size_t M, size_t N>
typename Matrix<T, M, N>::ConstIterator Matrix<T, M, N>::end() const
{
    return m_elements.end();
}

template <typename T, size_t M, size_t N>
MatrixScalarAdd<T, Matrix<T, M, N>> Matrix<T, M, N>::Add(const T& s) const
{
    return MatrixScalarAdd<T, Matrix<T, M, N>>{ *this, s };
}

template <typename T, size_t M, size_t N>
template <typename E>
MatrixAdd<T, Matrix<T, M, N>, E> Matrix<T, M, N>::Add(const E& m) const
{
    return MatrixAdd<T, Matrix, E>{ *this, m };
}

template <typename T, size_t M, size_t N>
MatrixScalarSub<T, Matrix<T, M, N>> Matrix<T, M, N>::Sub(const T& s) const
{
    return MatrixScalarSub<T, Matrix<T, M, N>>{ *this, s };
}

template <typename T, size_t M, size_t N>
template <typename E>
MatrixSub<T, Matrix<T, M, N>, E> Matrix<T, M, N>::Sub(const E& m) const
{
    return MatrixSub<T, Matrix, E>{ *this, m };
}

template <typename T, size_t M, size_t N>
MatrixScalarMul<T, Matrix<T, M, N>> Matrix<T, M, N>::Mul(const T& s) const
{
    return MatrixScalarMul<T, Matrix>{ *this, s };
}

template <typename T, size_t M, size_t N>
template <typename VE>
MatrixVectorMul<T, Matrix<T, M, N>, VE> Matrix<T, M, N>::Mul(
    const VectorExpression<T, VE>& v) const
{
    return MatrixVectorMul<T, Matrix<T, M, N>, VE>{ *this, v() };
}

template <typename T, size_t M, size_t N>
template <size_t L>
MatrixMul<T, Matrix<T, M, N>, Matrix<T, N, L>> Matrix<T, M, N>::Mul(
    const Matrix<T, N, L>& m) const
{
    return MatrixMul<T, Matrix, Matrix<T, N, L>>{ *this, m };
}

template <typename T, size_t M, size_t N>
MatrixScalarDiv<T, Matrix<T, M, N>> Matrix<T, M, N>::Div(const T& s) const
{
    return MatrixScalarDiv<T, Matrix>{ *this, s };
}

template <typename T, size_t M, size_t N>
MatrixScalarAdd<T, Matrix<T, M, N>> Matrix<T, M, N>::RAdd(const T& s) const
{
    return MatrixScalarAdd<T, Matrix<T, M, N>>{ *this, s };
}

template <typename T, size_t M, size_t N>
template <typename E>
MatrixAdd<T, Matrix<T, M, N>, E> Matrix<T, M, N>::RAdd(const E& m) const
{
    return MatrixAdd<T, Matrix<T, M, N>, E>{ m, *this };
}

template <typename T, size_t M, size_t N>
MatrixScalarRSub<T, Matrix<T, M, N>> Matrix<T, M, N>::RSub(const T& s) const
{
    return MatrixScalarRSub<T, Matrix<T, M, N>>{ *this, s };
}

template <typename T, size_t M, size_t N>
template <typename E>
MatrixSub<T, Matrix<T, M, N>, E> Matrix<T, M, N>::RSub(const E& m) const
{
    return MatrixSub<T, Matrix<T, M, N>, E>{ m, *this };
}

template <typename T, size_t M, size_t N>
MatrixScalarMul<T, Matrix<T, M, N>> Matrix<T, M, N>::RMul(const T& s) const
{
    return MatrixScalarMul<T, Matrix<T, M, N>>{ *this, s };
}

template <typename T, size_t M, size_t N>
template <size_t L>
MatrixMul<T, Matrix<T, N, L>, Matrix<T, M, N>> Matrix<T, M, N>::RMul(
    const Matrix<T, N, L>& m) const
{
    return MatrixMul<T, Matrix<T, N, L>, Matrix>{ m, *this };
}

template <typename T, size_t M, size_t N>
MatrixScalarRDiv<T, Matrix<T, M, N>> Matrix<T, M, N>::RDiv(const T& s) const
{
    return MatrixScalarRDiv<T, Matrix<T, M, N>>{ *this, s };
}

template <typename T, size_t M, size_t N>
void Matrix<T, M, N>::IAdd(const T& s)
{
    Set(Add(s));
}

template <typename T, size_t M, size_t N>
template <typename E>
void Matrix<T, M, N>::IAdd(const E& m)
{
    Set(Add(m));
}

template <typename T, size_t M, size_t N>
void Matrix<T, M, N>::ISub(const T& s)
{
    Set(Sub(s));
}

template <typename T, size_t M, size_t N>
template <typename E>
void Matrix<T, M, N>::ISub(const E& m)
{
    Set(Sub(m));
}

template <typename T, size_t M, size_t N>
void Matrix<T, M, N>::IMul(const T& s)
{
    Set(Mul(s));
}

template <typename T, size_t M, size_t N>
template <typename E>
void Matrix<T, M, N>::IMul(const E& m)
{
    Matrix tmp = Mul(m);
    Set(tmp);
}

template <typename T, size_t M, size_t N>
void Matrix<T, M, N>::IDiv(const T& s)
{
    Set(Div(s));
}

template <typename T, size_t M, size_t N>
void Matrix<T, M, N>::Transpose()
{
    Set(Transposed());
}

template <typename T, size_t M, size_t N>
void Matrix<T, M, N>::Invert()
{
    assert(IsSquare());

    // Computes inverse matrix using Gaussian elimination method.
    // https://martin-thoma.com/solving-linear-equations-with-gaussian-elimination/
    const size_t n = Rows();
    Matrix& a = *this;
    Matrix rhs = MakeIdentity();

    for (size_t i = 0; i < n; ++i)
    {
        // Search for maximum in this column
        T maxEl = std::fabs(a(i, i));
        size_t maxRow = i;

        for (size_t k = i + 1; k < n; ++k)
        {
            if (std::fabs(a(k, i)) > maxEl)
            {
                maxEl = std::fabs(a(k, i));
                maxRow = k;
            }
        }

        // Swap maximum row with current row (column by column)
        if (maxRow != i)
        {
            for (size_t k = i; k < n; ++k)
            {
                std::swap(a(maxRow, k), a(i, k));
            }

            for (size_t k = 0; k < n; ++k)
            {
                std::swap(rhs(maxRow, k), rhs(i, k));
            }
        }

        // Make all rows except this one 0 in current column
        for (size_t k = 0; k < n; ++k)
        {
            if (k == i)
            {
                continue;
            }

            T c = -a(k, i) / a(i, i);

            for (size_t j = 0; j < n; ++j)
            {
                rhs(k, j) += c * rhs(i, j);

                if (i == j)
                {
                    a(k, j) = 0;
                }
                else if (i < j)
                {
                    a(k, j) += c * a(i, j);
                }
            }
        }

        // Scale
        for (size_t k = 0; k < n; ++k)
        {
            T c = 1 / a(k, k);

            for (size_t j = 0; j < n; ++j)
            {
                a(k, j) *= c;
                rhs(k, j) *= c;
            }
        }
    }

    Set(rhs);
}

template <typename T, size_t M, size_t N>
T Matrix<T, M, N>::Sum() const
{
    T ret = 0;

    for (auto v : m_elements)
    {
        ret += v;
    }

    return ret;
}

template <typename T, size_t M, size_t N>
T Matrix<T, M, N>::Avg() const
{
    return Sum() / (Rows() * Cols());
}

template <typename T, size_t M, size_t N>
T Matrix<T, M, N>::Min() const
{
    T ret = m_elements.front();

    for (auto v : m_elements)
    {
        ret = std::min(ret, v);
    }

    return ret;
}

template <typename T, size_t M, size_t N>
T Matrix<T, M, N>::Max() const
{
    T ret = m_elements.front();

    for (auto v : m_elements)
    {
        ret = std::max(ret, v);
    }

    return ret;
}

template <typename T, size_t M, size_t N>
T Matrix<T, M, N>::AbsMin() const
{
    T ret = m_elements.front();

    for (auto v : m_elements)
    {
        ret = CubbyFlow::AbsMin(ret, v);
    }

    return ret;
}

template <typename T, size_t M, size_t N>
T Matrix<T, M, N>::AbsMax() const
{
    T ret = m_elements.front();

    for (auto v : m_elements)
    {
        ret = CubbyFlow::AbsMax(ret, v);
    }

    return ret;
}

template <typename T, size_t M, size_t N>
T Matrix<T, M, N>::Trace() const
{
    assert(IsSquare());

    T ret = 0;

    for (size_t i = 0; i < M; ++i)
    {
        ret += (*this)(i, i);
    }

    return ret;
}

template <typename T, size_t M, size_t N>
T Matrix<T, M, N>::Determinant() const
{
    assert(IsSquare());

    // Computes inverse matrix using Gaussian elimination method.
    // https://martin-thoma.com/solving-linear-equations-with-gaussian-elimination/
    const size_t n = Rows();
    Matrix a(*this);
    T result = 1;

    for (size_t i = 0; i < n; ++i)
    {
        // Search for maximum in this column
        T maxEl = std::fabs(a(i, i));
        size_t maxRow = i;

        for (size_t k = i + 1; k < n; ++k)
        {
            if (std::fabs(a(k, i)) > maxEl)
            {
                maxEl = std::fabs(a(k, i));
                maxRow = k;
            }
        }

        // Swap maximum row with current row (column by column)
        if (maxRow != i)
        {
            for (size_t k = i; k < n; ++k)
            {
                std::swap(a(maxRow, k), a(i, k));
            }

            result *= -1;
        }

        // Make all rows below this one 0 in current column
        for (size_t k = i + 1; k < n; ++k)
        {
            T c = -a(k, i) / a(i, i);

            for (size_t j = i; j < n; ++j)
            {
                if (i == j)
                {
                    a(k, j) = 0;
                }
                else
                {
                    a(k, j) += c * a(i, j);
                }
            }
        }
    }

    for (size_t i = 0; i < n; ++i)
    {
        result *= a(i, i);
    }

    return result;
}

template <typename T, size_t M, size_t N>
MatrixDiagonal<T, Matrix<T, M, N>> Matrix<T, M, N>::Diagonal() const
{
    return MatrixDiagonal<T, Matrix>{ *this, true };
}

template <typename T, size_t M, size_t N>
MatrixDiagonal<T, Matrix<T, M, N>> Matrix<T, M, N>::OffDiagonal() const
{
    return MatrixDiagonal<T, Matrix>{ *this, false };
}

template <typename T, size_t M, size_t N>
MatrixTriangular<T, Matrix<T, M, N>> Matrix<T, M, N>::StrictLowerTri() const
{
    return MatrixTriangular<T, Matrix<T, M, N>>{ *this, false, true };
}

template <typename T, size_t M, size_t N>
MatrixTriangular<T, Matrix<T, M, N>> Matrix<T, M, N>::StrictUpperTri() const
{
    return MatrixTriangular<T, Matrix<T, M, N>>{ *this, true, true };
}

template <typename T, size_t M, size_t N>
MatrixTriangular<T, Matrix<T, M, N>> Matrix<T, M, N>::LowerTri() const
{
    return MatrixTriangular<T, Matrix<T, M, N>>{ *this, false, false };
}

template <typename T, size_t M, size_t N>
MatrixTriangular<T, Matrix<T, M, N>> Matrix<T, M, N>::UpperTri() const
{
    return MatrixTriangular<T, Matrix<T, M, N>>{ *this, true, false };
}

template <typename T, size_t M, size_t N>
Matrix<T, N, M> Matrix<T, M, N>::Transposed() const
{
    Matrix<T, N, M> mt;
    ForEachIndex([&](size_t i, size_t j) { mt(j, i) = (*this)(i, j); });
    return mt;
}

template <typename T, size_t M, size_t N>
Matrix<T, M, N> Matrix<T, M, N>::Inverse() const
{
    Matrix mInv{ *this };
    mInv.Invert();
    return mInv;
}

template <typename T, size_t M, size_t N>
template <typename U>
MatrixTypeCast<U, Matrix<T, M, N>, T> Matrix<T, M, N>::CastTo() const
{
    return MatrixTypeCast<U, Matrix, T>{ *this };
}

template <typename T, size_t M, size_t N>
template <typename E>
Matrix<T, M, N>& Matrix<T, M, N>::operator=(const E& m)
{
    Set(m);
    return *this;
}

template <typename T, size_t M, size_t N>
Matrix<T, M, N>& Matrix<T, M, N>::operator+=(const T& s)
{
    IAdd(s);
    return *this;
}

template <typename T, size_t M, size_t N>
template <typename E>
Matrix<T, M, N>& Matrix<T, M, N>::operator+=(const E& m)
{
    IAdd(m);
    return *this;
}

template <typename T, size_t M, size_t N>
Matrix<T, M, N>& Matrix<T, M, N>::operator-=(const T& s)
{
    ISub(s);
    return *this;
}

template <typename T, size_t M, size_t N>
template <typename E>
Matrix<T, M, N>& Matrix<T, M, N>::operator-=(const E& m)
{
    ISub(m);
    return *this;
}

template <typename T, size_t M, size_t N>
Matrix<T, M, N>& Matrix<T, M, N>::operator*=(const T& s)
{
    IMul(s);
    return *this;
}

template <typename T, size_t M, size_t N>
template <typename E>
Matrix<T, M, N>& Matrix<T, M, N>::operator*=(const E& m)
{
    IMul(m);
    return *this;
}

template <typename T, size_t M, size_t N>
Matrix<T, M, N>& Matrix<T, M, N>::operator/=(const T& s)
{
    IDiv(s);
    return *this;
}

template <typename T, size_t M, size_t N>
T& Matrix<T, M, N>::operator[](size_t i)
{
    return m_elements[i];
}

template <typename T, size_t M, size_t N>
const T& Matrix<T, M, N>::operator[](size_t i) const
{
    return m_elements[i];
}

template <typename T, size_t M, size_t N>
T& Matrix<T, M, N>::operator()(size_t i, size_t j)
{
    return m_elements[i * N + j];
}

template <typename T, size_t M, size_t N>
const T& Matrix<T, M, N>::operator()(size_t i, size_t j) const
{
    return m_elements[i * N + j];
}

template <typename T, size_t M, size_t N>
template <typename E>
bool Matrix<T, M, N>::operator==(const MatrixExpression<T, E>& m) const
{
    return IsEqual(m);
}

template <typename T, size_t M, size_t N>
template <typename E>
bool Matrix<T, M, N>::operator!=(const MatrixExpression<T, E>& m) const
{
    return !IsEqual(m);
}

template <typename T, size_t M, size_t N>
template <typename Callback>
void Matrix<T, M, N>::ForEach(Callback func) const
{
    for (size_t i = 0; i < Rows(); ++i)
    {
        for (size_t j = 0; j < Cols(); ++j)
        {
            func((*this)(i, j));
        }
    }
}

template <typename T, size_t M, size_t N>
template <typename Callback>
void Matrix<T, M, N>::ForEachIndex(Callback func)
{
    for (size_t i = 0; i < Rows(); ++i)
    {
        for (size_t j = 0; j < Cols(); ++j)
        {
            func(i, j);
        }
    }
}

template <typename T, size_t M, size_t N>
MatrixConstant<T> Matrix<T, M, N>::MakeZero()
{
    return MatrixConstant<T>{ M, N, 0 };
}

template <typename T, size_t M, size_t N>
MatrixIdentity<T> Matrix<T, M, N>::MakeIdentity()
{
    static_assert(M == N, "Should be a square matrix.");
    return MatrixIdentity<T>{ M };
}

template <typename T, size_t M, size_t N>
template <typename... Params>
void Matrix<T, M, N>::SetRowAt(size_t i, T v, Params... params)
{
    m_elements[i] = v;
    SetRowAt(i + 1, params...);
}

template <typename T, size_t M, size_t N>
void Matrix<T, M, N>::SetRowAt(size_t i, T v)
{
    m_elements[i] = v;
}
}  // namespace CubbyFlow

#endif