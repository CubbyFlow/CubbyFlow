/*************************************************************************
> File Name: Point.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Generic N-D point class.
> Created Time: 2017/01/31
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_POINT_H
#define CUBBYFLOW_POINT_H

#include <array>
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
		static_assert(N > 0, "Size of static-sized point should be greater than zero.");
		static_assert(std::is_arithmetic<T>::value, "Point only can be instantiated with arithmetic types");

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

		//! Set point instance with initializer list.
		template <typename U>
		void Set(const std::initializer_list<U>& list);

		//! Set point instance with other point.
		void Set(const Point& other);

		//! Set point instance with initializer list.
		template <typename U>
		Point& operator=(const std::initializer_list<U>& list);

		//! Set point instance with other point.
		Point& operator=(const Point& other);

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
}

#include <Core/Point/Point-Impl.h>

#endif