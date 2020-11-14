// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_MATRIXMXN_IMPL_HPP
#define CUBBYFLOW_MATRIXMXN_IMPL_HPP

namespace CubbyFlow
{
template <typename T>
MatrixMxN<T>::MatrixMxN(size_t m, size_t n, const T& s)
{
    Resize(m, n, s);
}

template <typename T>
MatrixMxN<T>::MatrixMxN(
    const std::initializer_list<std::initializer_list<T>>& list)
{
    m_elements.Set(list);
}

template <typename T>
template <typename E>
MatrixMxN<T>::MatrixMxN(const MatrixExpression<T, E>& other)
{
    Set(other);
}

template <typename T>
MatrixMxN<T>::MatrixMxN(const MatrixMxN& other)
{
    Set(other);
}

template <typename T>
MatrixMxN<T>::MatrixMxN(MatrixMxN&& other) noexcept
{
    (*this) = std::move(other);
}

template <typename T>
MatrixMxN<T>& MatrixMxN<T>::operator=(const MatrixMxN& other)
{
    Set(other);
    return *this;
}

template <typename T>
MatrixMxN<T>& MatrixMxN<T>::operator=(MatrixMxN&& other) noexcept
{
    m_elements = std::move(other.m_elements);
    return *this;
}

template <typename T>
MatrixMxN<T>::MatrixMxN(size_t m, size_t n, const T* arr)
{
    Set(m, n, arr);
}

template <typename T>
void MatrixMxN<T>::Resize(size_t m, size_t n, const T& s)
{
    // Note that m and n are flipped.
    m_elements.Resize(n, m, s);
}

template <typename T>
void MatrixMxN<T>::Set(const T& s)
{
    m_elements.Set(s);
}

template <typename T>
void MatrixMxN<T>::Set(
    const std::initializer_list<std::initializer_list<T>>& list)
{
    m_elements.Set(list);
}

template <typename T>
template <typename E>
void MatrixMxN<T>::Set(const MatrixExpression<T, E>& other)
{
    Resize(other.Rows(), other.Cols());

    // Parallel evaluation of the expression
    const E& expression = other();
    ParallelForEachIndex(
        [&](size_t i, size_t j) { (*this)(i, j) = expression(i, j); });
}

template <typename T>
void MatrixMxN<T>::Set(size_t m, size_t n, const T* arr)
{
    Resize(m, n);

    const size_t sz = m * n;
    for (size_t i = 0; i < sz; ++i)
    {
        m_elements[i] = arr[i];
    }
}

template <typename T>
void MatrixMxN<T>::SetDiagonal(const T& s)
{
    const size_t l = std::min(Rows(), Cols());
    for (size_t i = 0; i < l; ++i)
    {
        (*this)(i, i) = s;
    }
}

template <typename T>
void MatrixMxN<T>::SetOffDiagonal(const T& s)
{
    ParallelForEachIndex([&](size_t i, size_t j) {
        if (i != j)
        {
            (*this)(i, j) = s;
        }
    });
}

template <typename T>
template <typename E>
void MatrixMxN<T>::SetRow(size_t i, const VectorExpression<T, E>& row)
{
    assert(Cols() == row.size());

    const E& e = row();
    ParallelFor(ZERO_SIZE, Cols(), [&](size_t j) { (*this)(i, j) = e[j]; });
}

template <typename T>
template <typename E>
void MatrixMxN<T>::SetColumn(size_t j, const VectorExpression<T, E>& col)
{
    assert(Rows() == col.size());

    const E& e = col();
    ParallelFor(ZERO_SIZE, Rows(), [&](size_t i) { (*this)(i, j) = e[i]; });
}

template <typename T>
template <typename E>
bool MatrixMxN<T>::IsEqual(const MatrixExpression<T, E>& other) const
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

template <typename T>
template <typename E>
bool MatrixMxN<T>::IsSimilar(const MatrixExpression<T, E>& other,
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

template <typename T>
bool MatrixMxN<T>::IsSquare() const
{
    return Rows() == Cols();
}

template <typename T>
Size2 MatrixMxN<T>::size() const
{
    return Size2(Rows(), Cols());
}

template <typename T>
size_t MatrixMxN<T>::Rows() const
{
    return m_elements.Height();
}

template <typename T>
size_t MatrixMxN<T>::Cols() const
{
    return m_elements.Width();
}

template <typename T>
T* MatrixMxN<T>::data()
{
    return m_elements.data();
}

template <typename T>
const T* MatrixMxN<T>::data() const
{
    return m_elements.data();
}

template <typename T>
typename MatrixMxN<T>::Iterator MatrixMxN<T>::begin()
{
    return m_elements.begin();
}

template <typename T>
typename MatrixMxN<T>::ConstIterator MatrixMxN<T>::begin() const
{
    return m_elements.begin();
}

template <typename T>
typename MatrixMxN<T>::Iterator MatrixMxN<T>::end()
{
    return m_elements.end();
}

template <typename T>
typename MatrixMxN<T>::ConstIterator MatrixMxN<T>::end() const
{
    return m_elements.end();
}

template <typename T>
MatrixScalarAdd<T, MatrixMxN<T>> MatrixMxN<T>::Add(const T& s) const
{
    return MatrixScalarAdd<T, MatrixMxN<T>>{ *this, s };
}

template <typename T>
template <typename E>
MatrixAdd<T, MatrixMxN<T>, E> MatrixMxN<T>::Add(const E& m) const
{
    return MatrixAdd<T, MatrixMxN, E>{ *this, m };
}

template <typename T>
MatrixScalarSub<T, MatrixMxN<T>> MatrixMxN<T>::Sub(const T& s) const
{
    return MatrixScalarSub<T, MatrixMxN<T>>{ *this, s };
}

template <typename T>
template <typename E>
MatrixSub<T, MatrixMxN<T>, E> MatrixMxN<T>::Sub(const E& m) const
{
    return MatrixSub<T, MatrixMxN, E>{ *this, m };
}

template <typename T>
MatrixScalarMul<T, MatrixMxN<T>> MatrixMxN<T>::Mul(const T& s) const
{
    return MatrixScalarMul<T, MatrixMxN>{ *this, s };
}

template <typename T>
template <typename VE>
MatrixVectorMul<T, MatrixMxN<T>, VE> MatrixMxN<T>::Mul(
    const VectorExpression<T, VE>& v) const
{
    return MatrixVectorMul<T, MatrixMxN<T>, VE>{ *this, v() };
}

template <typename T>
template <typename E>
MatrixMul<T, MatrixMxN<T>, E> MatrixMxN<T>::Mul(const E& m) const
{
    return MatrixMul<T, MatrixMxN, E>{ *this, m };
}

template <typename T>
MatrixScalarDiv<T, MatrixMxN<T>> MatrixMxN<T>::Div(const T& s) const
{
    return MatrixScalarDiv<T, MatrixMxN>{ *this, s };
}

template <typename T>
MatrixScalarAdd<T, MatrixMxN<T>> MatrixMxN<T>::RAdd(const T& s) const
{
    return MatrixScalarAdd<T, MatrixMxN<T>>{ *this, s };
}

template <typename T>
template <typename E>
MatrixAdd<T, MatrixMxN<T>, E> MatrixMxN<T>::RAdd(const E& m) const
{
    return MatrixAdd<T, MatrixMxN<T>, E>{ m, *this };
}

template <typename T>
MatrixScalarRSub<T, MatrixMxN<T>> MatrixMxN<T>::RSub(const T& s) const
{
    return MatrixScalarRSub<T, MatrixMxN<T>>{ *this, s };
}

template <typename T>
template <typename E>
MatrixSub<T, MatrixMxN<T>, E> MatrixMxN<T>::RSub(const E& m) const
{
    return MatrixSub<T, MatrixMxN<T>, E>{ m, *this };
}

template <typename T>
MatrixScalarMul<T, MatrixMxN<T>> MatrixMxN<T>::RMul(const T& s) const
{
    return MatrixScalarMul<T, MatrixMxN<T>>{ *this, s };
}

template <typename T>
template <typename E>
MatrixMul<T, E, MatrixMxN<T>> MatrixMxN<T>::RMul(const E& m) const
{
    return MatrixMul<T, E, MatrixMxN<T>>{ m, *this };
}

template <typename T>
MatrixScalarRDiv<T, MatrixMxN<T>> MatrixMxN<T>::RDiv(const T& s) const
{
    return MatrixScalarRDiv<T, MatrixMxN<T>>{ *this, s };
}

template <typename T>
void MatrixMxN<T>::IAdd(const T& s)
{
    Set(Add(s));
}

template <typename T>
template <typename E>
void MatrixMxN<T>::IAdd(const E& m)
{
    Set(Add(m));
}

template <typename T>
void MatrixMxN<T>::ISub(const T& s)
{
    Set(Sub(s));
}

template <typename T>
template <typename E>
void MatrixMxN<T>::ISub(const E& m)
{
    Set(Sub(m));
}

template <typename T>
void MatrixMxN<T>::IMul(const T& s)
{
    Set(Mul(s));
}

template <typename T>
template <typename E>
void MatrixMxN<T>::IMul(const E& m)
{
    MatrixMxN tmp = Mul(m);
    Set(tmp);
}

template <typename T>
void MatrixMxN<T>::IDiv(const T& s)
{
    Set(Div(s));
}

template <typename T>
void MatrixMxN<T>::Transpose()
{
    Set(Transposed());
}

template <typename T>
void MatrixMxN<T>::Invert()
{
    assert(IsSquare());

    // Computes inverse matrix using Gaussian elimination method.
    // https://martin-thoma.com/solving-linear-equations-with-gaussian-elimination/
    const size_t n = Rows();
    MatrixMxN& a = *this;
    MatrixMxN rhs = MakeIdentity(n);

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

template <typename T>
T MatrixMxN<T>::Sum() const
{
    return ParallelReduce(
        ZERO_SIZE, Rows() * Cols(), T(0),
        [&](size_t start, size_t end, T init) {
            T result = init;

            for (size_t i = start; i < end; ++i)
            {
                result += m_elements[i];
            }

            return result;
        },
        std::plus<T>());
}

template <typename T>
T MatrixMxN<T>::Avg() const
{
    return Sum() / (Rows() * Cols());
}

template <typename T>
T MatrixMxN<T>::Min() const
{
    const T& (*_min)(const T&, const T&) = std::min<T>;

    return ParallelReduce(
        ZERO_SIZE, Rows() * Cols(), std::numeric_limits<T>::max(),
        [&](size_t start, size_t end, T init) {
            T result = init;

            for (size_t i = start; i < end; ++i)
            {
                result = std::min(result, m_elements[i]);
            }

            return result;
        },
        _min);
}

template <typename T>
T MatrixMxN<T>::Max() const
{
    const T& (*_max)(const T&, const T&) = std::max<T>;

    return ParallelReduce(
        ZERO_SIZE, Rows() * Cols(), std::numeric_limits<T>::min(),
        [&](size_t start, size_t end, T init) {
            T result = init;

            for (size_t i = start; i < end; ++i)
            {
                result = std::max(result, m_elements[i]);
            }

            return result;
        },
        _max);
}

template <typename T>
T MatrixMxN<T>::AbsMin() const
{
    return ParallelReduce(
        ZERO_SIZE, Rows() * Cols(), std::numeric_limits<T>::max(),
        [&](size_t start, size_t end, T init) {
            T result = init;

            for (size_t i = start; i < end; ++i)
            {
                result = CubbyFlow::AbsMin(result, m_elements[i]);
            }

            return result;
        },
        CubbyFlow::AbsMin<T>);
}

template <typename T>
T MatrixMxN<T>::AbsMax() const
{
    return ParallelReduce(
        ZERO_SIZE, Rows() * Cols(), T(0),
        [&](size_t start, size_t end, T init) {
            T result = init;

            for (size_t i = start; i < end; ++i)
            {
                result = CubbyFlow::AbsMax(result, m_elements[i]);
            }

            return result;
        },
        CubbyFlow::AbsMax<T>);
}

template <typename T>
T MatrixMxN<T>::Trace() const
{
    assert(IsSquare());

    return ParallelReduce(
        ZERO_SIZE, Rows(), T(0),
        [&](size_t start, size_t end, T init) {
            T result = init;

            for (size_t i = start; i < end; ++i)
            {
                result += m_elements(i, i);
            }

            return result;
        },
        std::plus<T>());
}

template <typename T>
T MatrixMxN<T>::Determinant() const
{
    assert(IsSquare());

    // Computes inverse matrix using Gaussian elimination method.
    // https://martin-thoma.com/solving-linear-equations-with-gaussian-elimination/
    const size_t n = Rows();
    MatrixMxN a(*this);
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

template <typename T>
MatrixDiagonal<T, MatrixMxN<T>> MatrixMxN<T>::Diagonal() const
{
    return MatrixDiagonal<T, MatrixMxN>{ *this, true };
}

template <typename T>
MatrixDiagonal<T, MatrixMxN<T>> MatrixMxN<T>::OffDiagonal() const
{
    return MatrixDiagonal<T, MatrixMxN>{ *this, false };
}

template <typename T>
MatrixTriangular<T, MatrixMxN<T>> MatrixMxN<T>::StrictLowerTri() const
{
    return MatrixTriangular<T, MatrixMxN<T>>{ *this, false, true };
}

template <typename T>
MatrixTriangular<T, MatrixMxN<T>> MatrixMxN<T>::StrictUpperTri() const
{
    return MatrixTriangular<T, MatrixMxN<T>>{ *this, true, true };
}

template <typename T>
MatrixTriangular<T, MatrixMxN<T>> MatrixMxN<T>::LowerTri() const
{
    return MatrixTriangular<T, MatrixMxN<T>>{ *this, false, false };
}

template <typename T>
MatrixTriangular<T, MatrixMxN<T>> MatrixMxN<T>::UpperTri() const
{
    return MatrixTriangular<T, MatrixMxN<T>>{ *this, true, false };
}

template <typename T>
MatrixMxN<T> MatrixMxN<T>::Transposed() const
{
    MatrixMxN mt{ Cols(), Rows() };
    ParallelForEachIndex([&](size_t i, size_t j) { mt(j, i) = (*this)(i, j); });
    return mt;
}

template <typename T>
MatrixMxN<T> MatrixMxN<T>::Inverse() const
{
    MatrixMxN mInv{ *this };
    mInv.Invert();
    return mInv;
}

template <typename T>
template <typename U>
MatrixTypeCast<U, MatrixMxN<T>, T> MatrixMxN<T>::CastTo() const
{
    return MatrixTypeCast<U, MatrixMxN, T>{ *this };
}

template <typename T>
template <typename E>
MatrixMxN<T>& MatrixMxN<T>::operator=(const E& m)
{
    Set(m);
    return *this;
}

template <typename T>
MatrixMxN<T>& MatrixMxN<T>::operator+=(const T& s)
{
    IAdd(s);
    return *this;
}

template <typename T>
template <typename E>
MatrixMxN<T>& MatrixMxN<T>::operator+=(const E& m)
{
    IAdd(m);
    return *this;
}

template <typename T>
MatrixMxN<T>& MatrixMxN<T>::operator-=(const T& s)
{
    ISub(s);
    return *this;
}

template <typename T>
template <typename E>
MatrixMxN<T>& MatrixMxN<T>::operator-=(const E& m)
{
    ISub(m);
    return *this;
}

template <typename T>
MatrixMxN<T>& MatrixMxN<T>::operator*=(const T& s)
{
    IMul(s);
    return *this;
}

template <typename T>
template <typename E>
MatrixMxN<T>& MatrixMxN<T>::operator*=(const E& m)
{
    IMul(m);
    return *this;
}

template <typename T>
MatrixMxN<T>& MatrixMxN<T>::operator/=(const T& s)
{
    IDiv(s);
    return *this;
}

template <typename T>
T& MatrixMxN<T>::operator[](size_t i)
{
    return m_elements[i];
}

template <typename T>
const T& MatrixMxN<T>::operator[](size_t i) const
{
    return m_elements[i];
}

template <typename T>
T& MatrixMxN<T>::operator()(size_t i, size_t j)
{
    return m_elements(j, i);
}

template <typename T>
const T& MatrixMxN<T>::operator()(size_t i, size_t j) const
{
    return m_elements(j, i);
}

template <typename T>
template <typename E>
bool MatrixMxN<T>::operator==(const MatrixExpression<T, E>& m) const
{
    return IsEqual(m);
}

template <typename T>
template <typename E>
bool MatrixMxN<T>::operator!=(const MatrixExpression<T, E>& m) const
{
    return !IsEqual(m);
}

template <typename T>
template <typename Callback>
void MatrixMxN<T>::ForEach(Callback func) const
{
    for (size_t i = 0; i < Rows(); ++i)
    {
        for (size_t j = 0; j < Cols(); ++j)
        {
            func((*this)(i, j));
        }
    }
}

template <typename T>
template <typename Callback>
void MatrixMxN<T>::ForEachIndex(Callback func) const
{
    for (size_t i = 0; i < Rows(); ++i)
    {
        for (size_t j = 0; j < Cols(); ++j)
        {
            func(i, j);
        }
    }
}

template <typename T>
template <typename Callback>
void MatrixMxN<T>::ParallelForEach(Callback func)
{
    ParallelFor(ZERO_SIZE, Cols(), ZERO_SIZE, Rows(),
                [&](size_t j, size_t i) { func((*this)(i, j)); });
}

template <typename T>
template <typename Callback>
void MatrixMxN<T>::ParallelForEachIndex(Callback func) const
{
    ParallelFor(ZERO_SIZE, Cols(), ZERO_SIZE, Rows(),
                [&](size_t j, size_t i) { func(i, j); });
}

// MARK: Builders
template <typename T>
MatrixConstant<T> MatrixMxN<T>::MakeZero(size_t m, size_t n)
{
    return MatrixConstant<T>{ m, n, 0 };
}

template <typename T>
MatrixIdentity<T> MatrixMxN<T>::MakeIdentity(size_t m)
{
    return MatrixIdentity<T>{ m };
}
}  // namespace CubbyFlow

#endif