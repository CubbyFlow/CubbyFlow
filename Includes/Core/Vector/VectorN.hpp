// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VECTORN_HPP
#define CUBBYFLOW_VECTORN_HPP

#include <Core/Array/ArrayAccessor1.hpp>
#include <Core/Vector/VectorExpression.hpp>

namespace CubbyFlow
{
//!
//! \brief General purpose dynamically-sizedN-D vector class.
//!
//! This class defines N-D vector data where its size can be defined
//! dynamically.
//!
//! \tparam T Type of the element.
//!
template <typename T>
class VectorN final : public VectorExpression<T, VectorN<T>>
{
 public:
    static_assert(std::is_floating_point<T>::value,
                  "VectorN only can be instantiated with floating point types");

    using ContainerType = std::vector<T>;

    //! Constructs empty vector.
    VectorN() = default;

    //! Constructs default vector (val, val, ... , val).
    VectorN(size_t n, const T& val = 0);

    //! Constructs vector with given initializer list.
    template <typename U>
    VectorN(const std::initializer_list<U>& list);

    //! Constructs vector with expression template.
    template <typename E>
    VectorN(const VectorExpression<T, E>& other);

    //! Copy constructor.
    VectorN(const VectorN& other);

    //! Move constructor.
    VectorN(VectorN&& other) noexcept;

    //! Default destructor.
    ~VectorN() = default;

    //! Copy assignment operator.
    VectorN& operator=(const VectorN& other);

    //! Move assignment operator.
    VectorN& operator=(VectorN&& other) noexcept;

    //! Resizes to \p n dimensional vector with initial value \p val.
    void Resize(size_t n, const T& val = 0);

    //! Clears the vector and make it zero-dimensional.
    void Clear();

    //! Sets all elements to \p s.
    void Set(const T& s);

    //! Sets all elements with given initializer list.
    template <typename U>
    void Set(const std::initializer_list<U>& list);

    //! Sets vector with expression template.
    template <typename E>
    void Set(const VectorExpression<T, E>& other);

    //! Adds an element.
    void Append(const T& val);

    //! Swaps the content of the vector with \p other vector.
    void Swap(VectorN& other);

    //! Sets all elements to zero.
    void SetZero();

    //! Normalizes this vector.
    void Normalize();

    //! Returns the size of the vector.
    [[nodiscard]] size_t size() const;

    //! Returns the raw pointer to the vector data.
    [[nodiscard]] T* data();

    //! Returns the const raw pointer to the vector data.
    [[nodiscard]] const T* data() const;

    //! Returns the begin iterator of the vector.
    [[nodiscard]] typename ContainerType::iterator begin();

    //! Returns the begin const iterator of the vector.
    [[nodiscard]] typename ContainerType::const_iterator begin() const;

    //! Returns the end iterator of the vector.
    [[nodiscard]] typename ContainerType::iterator end();

    //! Returns the end const iterator of the vector.
    [[nodiscard]] typename ContainerType::const_iterator end() const;

    //! Returns the array accessor.
    [[nodiscard]] ArrayAccessor1<T> Accessor();

    //! Returns the const array accessor.
    [[nodiscard]] ConstArrayAccessor1<T> ConstAccessor() const;

    //! Returns const reference to the \p i -th element of the vector.
    [[nodiscard]] T At(size_t i) const;

    //! Returns reference to the \p i -th element of the vector.
    [[nodiscard]] T& At(size_t i);

    //! Returns the sum of all the elements.
    [[nodiscard]] T Sum() const;

    //! Returns the average of all the elements.
    [[nodiscard]] T Avg() const;

    //! Returns the minimum element.
    [[nodiscard]] T Min() const;

    //! Returns the maximum element.
    [[nodiscard]] T Max() const;

    //! Returns the absolute minimum element.
    [[nodiscard]] T AbsMin() const;

    //! Returns the absolute maximum element.
    [[nodiscard]] T AbsMax() const;

    //! Returns the index of the dominant axis.
    [[nodiscard]] size_t DominantAxis() const;

    //! Returns the index of the subdominant axis.
    [[nodiscard]] size_t SubdominantAxis() const;

    //! Returns normalized vector.
    [[nodiscard]] VectorScalarDiv<T, VectorN> Normalized() const;

    //! Returns the length of the vector.
    [[nodiscard]] T Length() const;

    //! Returns the squared length of the vector.
    [[nodiscard]] T LengthSquared() const;

    //! Returns the distance to the other vector.
    template <typename E>
    [[nodiscard]] T DistanceTo(const E& other) const;

    //! Returns the squared distance to the other vector.
    template <typename E>
    [[nodiscard]] T DistanceSquaredTo(const E& other) const;

    //! Returns a vector with different value type.
    template <typename U>
    [[nodiscard]] VectorTypeCast<U, VectorN<T>, T> CastTo() const;

    //! Returns true if \p other is the same as this vector.
    template <typename E>
    [[nodiscard]] bool IsEqual(const E& other) const;

    //! Returns true if \p other is similar to this vector.
    template <typename E>
    [[nodiscard]] bool IsSimilar(
        const E& other, T epsilon = std::numeric_limits<T>::epsilon()) const;

    //! Computes this + v.
    template <typename E>
    [[nodiscard]] VectorAdd<T, VectorN, E> Add(const E& v) const;

    //! Computes this + (s, s, ... , s).
    [[nodiscard]] VectorScalarAdd<T, VectorN> Add(const T& s) const;

    //! Computes this - v.
    template <typename E>
    [[nodiscard]] VectorSub<T, VectorN, E> Sub(const E& v) const;

    //! Computes this - (s, s, ... , s).
    [[nodiscard]] VectorScalarSub<T, VectorN> Sub(const T& s) const;

    //! Computes this * v.
    template <typename E>
    [[nodiscard]] VectorMul<T, VectorN, E> Mul(const E& v) const;

    //! Computes this * (s, s, ... , s).
    [[nodiscard]] VectorScalarMul<T, VectorN> Mul(const T& s) const;

    //! Computes this / v.
    template <typename E>
    [[nodiscard]] VectorDiv<T, VectorN, E> Div(const E& v) const;

    //! Computes this / (s, s, ... , s).
    [[nodiscard]] VectorScalarDiv<T, VectorN> Div(const T& s) const;

    //! Computes dot product.
    template <typename E>
    [[nodiscard]] T Dot(const E& v) const;

    //! Computes (s, s, ... , s) - this.
    [[nodiscard]] VectorScalarRSub<T, VectorN> RSub(const T& s) const;

    //! Computes v - this.
    template <typename E>
    [[nodiscard]] VectorSub<T, VectorN, E> RSub(const E& v) const;

    //! Computes (s, s, ... , s) / this.
    [[nodiscard]] VectorScalarRDiv<T, VectorN> RDiv(const T& s) const;

    //! Computes v / this.
    template <typename E>
    [[nodiscard]] VectorDiv<T, VectorN, E> RDiv(const E& v) const;

    //! Computes this += (s, s, ... , s).
    void IAdd(const T& s);

    //! Computes this += v.
    template <typename E>
    void IAdd(const E& v);

    //! Computes this -= (s, s, ... , s).
    void ISub(const T& s);

    //! Computes this -= v.
    template <typename E>
    void ISub(const E& v);

    //! Computes this *= (s, s, ... , s).
    void IMul(const T& s);

    //! Computes this *= v.
    template <typename E>
    void IMul(const E& v);

    //! Computes this /= (s, s, ... , s).
    void IDiv(const T& s);

    //! Computes this /= v.
    template <typename E>
    void IDiv(const E& v);

    //!
    //! \brief Iterates the vector and invoke given \p func for each element.
    //!
    //! This function iterates the vector elements and invoke the callback
    //! function \p func. The callback function takes array's element as its
    //! input. The order of execution will be 0 to N-1 where N is the size of
    //! the vector. Below is the sample usage:
    //!
    //! \code{.cpp}
    //! VectorN<float> vec(10, 4.f);
    //! vec.ForEach([](float elem)
    //! {
    //!     printf("%d\n", elem);
    //! });
    //! \endcode
    //!
    template <typename Callback>
    void ForEach(Callback func) const;

    //!
    //! \brief Iterates the vector and invoke given \p func for each index.
    //!
    //! This function iterates the vector elements and invoke the callback
    //! function \p func. The callback function takes one parameter which is the
    //! index of the vector. The order of execution will be 0 to N-1 where N is
    //! the size of the array. Below is the sample usage:
    //!
    //! \code{.cpp}
    //! VectorN<float> vec(10, 4.f);
    //! vec.ForEachIndex([&](size_t i)
    //! {
    //!     vec[i] = 4.f * i + 1.5f;
    //! });
    //! \endcode
    //!
    template <typename Callback>
    void ForEachIndex(Callback func) const;

    //!
    //! \brief Iterates the vector and invoke given \p func for each element in
    //!     parallel using multi-threading.
    //!
    //! This function iterates the vector elements and invoke the callback
    //! function \p func in parallel using multi-threading. The callback
    //! function takes vector's element as its input. The order of execution
    //! will be non-deterministic since it runs in parallel.
    //! Below is the sample usage:
    //!
    //! \code{.cpp}
    //! VectorN<float> vec(1000, 4.f);
    //! vec.ParallelForEach([](float& elem)
    //! {
    //!     elem *= 2;
    //! });
    //! \endcode
    //!
    //! The parameter type of the callback function doesn't have to be T&, but
    //! const T& or T can be used as well.
    //!
    template <typename Callback>
    void ParallelForEach(Callback func);

    //!
    //! \brief Iterates the vector and invoke given \p func for each index in
    //!     parallel using multi-threading.
    //!
    //! This function iterates the vector elements and invoke the callback
    //! function \p func in parallel using multi-threading. The callback
    //! function takes one parameter which is the index of the vector. The order
    //! of execution will be non-deterministic since it runs in parallel.
    //! Below is the sample usage:
    //!
    //! \code{.cpp}
    //! VectorN<float> vec(1000, 4.f);
    //! vec.ParallelForEachIndex([](size_t i)
    //! {
    //!     array[i] *= 2;
    //! });
    //! \endcode
    //!
    template <typename Callback>
    void ParallelForEachIndex(Callback func) const;

    //! Returns the \p i -th element.
    T operator[](size_t i) const;

    //! Returns the reference to the \p i -th element.
    T& operator[](size_t i);

    //! Sets vector with given initializer list.
    template <typename U>
    VectorN& operator=(const std::initializer_list<U>& list);

    //! Sets vector with expression template.
    template <typename E>
    VectorN& operator=(const VectorExpression<T, E>& other);

    //! Computes this += (s, s, ... , s)
    VectorN& operator+=(const T& s);

    //! Computes this += v
    template <typename E>
    VectorN& operator+=(const E& v);

    //! Computes this -= (s, s, ... , s)
    VectorN& operator-=(const T& s);

    //! Computes this -= v
    template <typename E>
    VectorN& operator-=(const E& v);

    //! Computes this *= (s, s, ... , s)
    VectorN& operator*=(const T& s);

    //! Computes this *= v
    template <typename E>
    VectorN& operator*=(const E& v);

    //! Computes this /= (s, s, ... , s)
    VectorN& operator/=(const T& s);

    //! Computes this /= v
    template <typename E>
    VectorN& operator/=(const E& v);

    //! Returns true if \p other is the same as this vector.
    template <typename E>
    bool operator==(const E& v) const;

    //! Returns true if \p other is the not same as this vector.
    template <typename E>
    bool operator!=(const E& v) const;

 private:
    ContainerType m_elements;
};

//! Float-type N-D vector.
using VectorNF = VectorN<float>;

//! Double-type N-D vector.
using VectorND = VectorN<double>;
}  // namespace CubbyFlow

#include <Core/Vector/VectorN-Impl.hpp>

#endif