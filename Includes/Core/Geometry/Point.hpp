// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT_HPP
#define CUBBYFLOW_POINT_HPP

#include <array>
#include <cstddef>
#include <type_traits>

namespace CubbyFlow
{
//!
//! \brief Generic N-D point class.
//!
//! \tparam T - Number type.
//! \tparam N - Dimension.
//!
template <typename T, size_t N>
class Point final
{
 public:
    static_assert(N > 0,
                  "Size of static-sized point should be greater than zero.");
    static_assert(std::is_arithmetic<T>::value,
                  "Point only can be instantiated with arithmetic types");

    //! Constructs a point with zeros.
    Point();

    //! Constructs point instance with parameters.
    template <typename... Params>
    explicit Point(Params... params);

    //! Constructs point instance with initializer list.
    template <typename U>
    explicit Point(const std::initializer_list<U>& list);

    //! Copy constructor.
    Point(const Point& other);

    //! Move constructor.
    Point(Point&& other) noexcept;

    //! Default destructor.
    ~Point() = default;

    //! Copy assignment operator.
    Point& operator=(const Point& other);

    //! Move assignment operator.
    Point& operator=(Point&& other) noexcept;

    //! Set point instance with initializer list.
    template <typename U>
    void Set(const std::initializer_list<U>& list);

    //! Set point instance with other point.
    void Set(const Point& other);

    //! Set point instance with initializer list.
    template <typename U>
    Point& operator=(const std::initializer_list<U>& list);

    //! Returns the const reference to the \p i -th element.
    const T& operator[](size_t i) const;

    //! Returns the reference to the \p i -th element.
    T& operator[](size_t);

 private:
    std::array<T, N> m_elements;

    template <typename... Params>
    void SetAt(size_t i, T v, Params... params);

    void SetAt(size_t i, T v);
};
}  // namespace CubbyFlow

#include <Core/Geometry/Point-Impl.hpp>

#endif