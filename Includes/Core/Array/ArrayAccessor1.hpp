// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_ACCESSOR1_HPP
#define CUBBYFLOW_ARRAY_ACCESSOR1_HPP

#include <Core/Array/ArrayAccessor.hpp>

namespace CubbyFlow
{
//!
//! \brief 1-D array accessor class.
//!
//! This class represents 1-D array accessor. Array accessor provides array-like
//! data read/write functions, but does not handle memory management. Thus, it
//! is more like a random access iterator, but with multi-dimension support.
//!
//! \see Array1<T, 2>
//!
//! \tparam T - Array value type.
//!
template <typename T>
class ArrayAccessor<T, 1> final
{
 public:
    //! Constructs empty 1-D array accessor.
    ArrayAccessor();

    //! Constructs an array accessor that wraps given array.
    ArrayAccessor(size_t size, T* data);

    //! Copy constructor.
    ArrayAccessor(const ArrayAccessor& other);

    //! Move constructor.
    ArrayAccessor(ArrayAccessor&& other) noexcept;

    //! Default destructor.
    ~ArrayAccessor() = default;

    //! Replaces the content with given \p other array accessor.
    void Set(const ArrayAccessor& other);

    //! Resets the array.
    void Reset(size_t size, T* data);

    //! Returns the reference to the i-th element.
    T& At(size_t i);

    //! Returns the const reference to the i-th element.
    const T& At(size_t i) const;

    //! Returns the begin iterator of the array.
    T* begin() const;

    //! Returns the end iterator of the array.
    T* end() const;

    //! Returns the begin iterator of the array.
    T* begin();

    //! Returns the end iterator of the array.
    T* end();

    //! Returns size of the array.
    [[nodiscard]] size_t size() const;

    //! Returns the raw pointer to the array data.
    T* data() const;

    //! Swaps the content of with \p other array accessor.
    void Swap(ArrayAccessor& other);

    //!
    //! \brief Iterates the array and invoke given \p func for each element.
    //!
    //! This function iterates the array elements and invoke the callback
    //! function \p func. The callback function takes array's element as its
    //! input. The order of execution will be 0 to N-1 where N is the size of
    //! the array. Below is the sample usage:
    //!
    //! \code{.cpp}
    //! int data = {1, 2, 3, 4, 5, 6};
    //! ArrayAccessor<int, 1> acc(6, data);
    //! acc.ForEach([](int elem) {
    //!     printf("%d\n", elem);
    //! });
    //! \endcode
    //!
    template <typename Callback>
    void ForEach(Callback func) const;

    //!
    //! \brief Iterates the array and invoke given \p func for each index.
    //!
    //! This function iterates the array elements and invoke the callback
    //! function \p func. The callback function takes one parameter which is the
    //! index of the array. The order of execution will be 0 to N-1 where N is
    //! the size of the array. Below is the sample usage:
    //!
    //! \code{.cpp}
    //! int data = {1, 2, 3, 4, 5, 6};
    //! ArrayAccessor<int, 1> acc(6, data);
    //! acc.ForEachIndex([&](size_t i) {
    //!     acc[i] = 4.f * i + 1.5f;
    //! });
    //! \endcode
    //!
    template <typename Callback>
    void ForEachIndex(Callback func) const;

    //!
    //! \brief Iterates the array and invoke given \p func for each element in
    //!     parallel using multi-threading.
    //!
    //! This function iterates the array elements and invoke the callback
    //! function \p func in parallel using multi-threading. The callback
    //! function takes array's element as its input. The order of execution will
    //! be non-deterministic since it runs in parallel.
    //! Below is the sample usage:
    //!
    //! \code{.cpp}
    //! int data = {1, 2, 3, 4, 5, 6};
    //! ArrayAccessor<int, 1> acc(6, data);
    //! acc.ParallelForEach([](int& elem) {
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
    //! \brief Iterates the array and invoke given \p func for each index in
    //!     parallel using multi-threading.
    //!
    //! This function iterates the array elements and invoke the callback
    //! function \p func in parallel using multi-threading. The callback
    //! function takes one parameter which is the index of the array. The order
    //! of execution will be non-deterministic since it runs in parallel.
    //! Below is the sample usage:
    //!
    //! \code{.cpp}
    //! int data = {1, 2, 3, 4, 5, 6};
    //! ArrayAccessor<int, 1> acc(6, data);
    //! acc.ParallelForEachIndex([](size_t i) {
    //!     acc[i] *= 2;
    //! });
    //! \endcode
    //!
    template <typename Callback>
    void ParallelForEachIndex(Callback func) const;

    //! Returns the reference to i-th element.
    T& operator[](size_t i);

    //! Returns the const reference to i-th element.
    const T& operator[](size_t i) const;

    //! Copies given array accessor \p other.
    ArrayAccessor& operator=(const ArrayAccessor& other);

    //! Moves given array accessor \p other.
    ArrayAccessor& operator=(ArrayAccessor&& other) noexcept;

    //! Casts type to ConstArrayAccessor.
    operator ConstArrayAccessor<T, 1>() const;

 private:
    size_t m_size;
    T* m_data;
};

//! Type alias for 1-D array accessor.
template <typename T>
using ArrayAccessor1 = ArrayAccessor<T, 1>;

//!
//! \brief 1-D read-only array accessor class.
//!
//! This class represents 1-D read-only array accessor. Array accessor provides
//! array-like data read/write functions, but does not handle memory management.
//! Thus, it is more like a random access iterator, but with multi-dimension
//! support.
//!
template <typename T>
class ConstArrayAccessor<T, 1>
{
 public:
    //! Constructs empty 1-D array accessor.
    ConstArrayAccessor();

    //! Constructs an read-only array accessor that wraps given array.
    ConstArrayAccessor(size_t size, const T* data);

    //! Constructs a read-only array accessor from read/write accessor.
    explicit ConstArrayAccessor(const ArrayAccessor<T, 1>& other);

    //! Default destructor.
    ~ConstArrayAccessor() = default;

    //! Copy constructor.
    ConstArrayAccessor(const ConstArrayAccessor& other);

    //! Move constructor.
    ConstArrayAccessor(ConstArrayAccessor&& other) noexcept;

    //! Returns the const reference to the i-th element.
    [[nodiscard]] const T& At(size_t i) const;

    //! Returns the begin iterator of the array.
    [[nodiscard]] const T* begin() const;

    //! Returns the end iterator of the array.
    [[nodiscard]] const T* end() const;

    //! Returns size of the array.
    [[nodiscard]] size_t size() const;

    //! Returns the raw pointer to the array data.
    const T* data() const;

    //!
    //! \brief Iterates the array and invoke given \p func for each element.
    //!
    //! This function iterates the array elements and invoke the callback
    //! function \p func. The callback function takes array's element as its
    //! input. The order of execution will be 0 to N-1 where N is the size of
    //! the array. Below is the sample usage:
    //!
    //! \code{.cpp}
    //! int data = {1, 2, 3, 4, 5, 6};
    //! ConstArrayAccessor<int, 1> acc(6, data);
    //! acc.ForEach([](int elem) {
    //!     printf("%d\n", elem);
    //! });
    //! \endcode
    //!
    template <typename Callback>
    void ForEach(Callback func) const;

    //!
    //! \brief Iterates the array and invoke given \p func for each index.
    //!
    //! This function iterates the array elements and invoke the callback
    //! function \p func. The callback function takes one parameter which is the
    //! index of the array. The order of execution will be 0 to N-1 where N is
    //! the size of the array. Below is the sample usage:
    //!
    //! \code{.cpp}
    //! int data = {1, 2, 3, 4, 5, 6};
    //! ConstArrayAccessor<int, 1> acc(6, data);
    //! acc.ForEachIndex([&](size_t i) {
    //!     data[i] = acc[i] * acc[i];
    //! });
    //! \endcode
    //!
    template <typename Callback>
    void ForEachIndex(Callback func) const;

    //!
    //! \brief Iterates the array and invoke given \p func for each index in
    //!     parallel using multi-threading.
    //!
    //! This function iterates the array elements and invoke the callback
    //! function \p func in parallel using multi-threading. The callback
    //! function takes one parameter which is the index of the array. The order
    //! of execution will be non-deterministic since it runs in parallel.
    //! Below is the sample usage:
    //!
    //! \code{.cpp}
    //! int data = {1, 2, 3, 4, 5, 6};
    //! ConstArrayAccessor<int, 1> acc(6, data);
    //! accessor.ParallelForEachIndex([](size_t i) {
    //!     data[i] = acc[i] * acc[i];
    //! });
    //! \endcode
    //!
    template <typename Callback>
    void ParallelForEachIndex(Callback func) const;

    //! Returns the const reference to i-th element.
    const T& operator[](size_t i) const;

    //! Copies given read-only array accessor \p other.
    ConstArrayAccessor& operator=(const ConstArrayAccessor& other);

    //! Moves given read-only array accessor \p other.
    ConstArrayAccessor& operator=(ConstArrayAccessor&& other) noexcept;

 private:
    size_t m_size;
    const T* m_data;
};

//! Type alias for 1-D const array accessor.
template <typename T>
using ConstArrayAccessor1 = ConstArrayAccessor<T, 1>;
}  // namespace CubbyFlow

#include <Core/Array/ArrayAccessor1-Impl.hpp>

#endif