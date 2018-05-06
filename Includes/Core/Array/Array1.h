/*************************************************************************
> File Name: Array1.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 1-D array class.
> Created Time: 2017/01/23
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ARRAY1_H
#define CUBBYFLOW_ARRAY1_H

#include <Core/Array/Array.h>
#include <Core/Array/ArrayAccessor1.h>

#include <vector>

namespace CubbyFlow
{
	//!
	//! \brief 1-D array class.
	//!
	//! This class represents 1-D array data structure. This class is a simple
	//! wrapper around std::vector with some additional features such as the array
	//! accessor object and parallel for-loop.
	//!
	//! \tparam T - Type to store in the array.
	//!
	template <typename T>
	class Array<T, 1> final
	{
	public:
		using ContainerType = std::vector<T>;
		using Iterator = typename ContainerType::iterator;
		using ConstIterator = typename ContainerType::const_iterator;

		//! Constructs zero-sized 1-D array.
		Array();

		//! Constructs 1-D array with given \p size and fill it with \p initVal.
		//! \param size Initial size of the array.
		//! \param initVal Initial value of each array element.
		explicit Array(size_t size, const T& initVal = T());

		//!
		//! \brief Constructs 1-D array with given initializer list \p list.
		//!
		//! This constructor will build 1-D array with given initializer list \p list
		//! such as
		//!
		//! \code{.cpp}
		//! Array<int, 1> arr = { 1, 2, 4, 9, 3 };
		//! \endcode
		//!
		//! \param list Initializer list that should be copy to the new array.
		//!
		Array(const std::initializer_list<T>& list);

		//! Copy constructor.
		Array(const Array& other);
		
		//! Move constructor.
		Array(Array&& other);

		//! Sets entire array with given \p value.
		void Set(const T& value);

		//! Copies given array \p other to this array.
		void Set(const Array& other);

		//! Copies given initializer list \p list to this array.
		void Set(const std::initializer_list<T>& list);

		//! Clears the array and resizes to zero.
		void Clear();

		//! Resizes the array with \p size and fill the new element with \p initVal.
		void Resize(size_t size, const T& initVal = T());

		//! Returns the reference to the i-th element.
		T& At(size_t i);

		//! Returns the const reference to the i-th element.
		const T& At(size_t i) const;

		//! Returns size of the array.
		size_t size() const;

		//! Returns the raw pointer to the array data.
		T* data();

		//! Returns the const raw pointer to the array data.
		const T* data() const;

		//! Returns the begin iterator of the array.
		Iterator begin();

		//! Returns the begin const iterator of the array.
		ConstIterator begin() const;

		//! Returns the end iterator of the array.
		Iterator end();

		//! Returns the end const iterator of the array.
		ConstIterator end() const;

		//! Returns the array accessor.
		ArrayAccessor1<T> Accessor();

		//! Returns the const array accessor.
		ConstArrayAccessor1<T> ConstAccessor() const;

		//! Swaps the content of the array with \p other array.
		void Swap(Array& other);

		//! Appends single value \p newVal at the end of the array.
		void Append(const T& newVal);

		//! Appends \p other array at the end of the array.
		void Append(const Array& other);

		//!
		//! \brief Iterates the array and invoke given \p func for each element.
		//!
		//! This function iterates the array elements and invoke the callback
		//! function \p func. The callback function takes array's element as its
		//! input. The order of execution will be 0 to N-1 where N is the size of
		//! the array. Below is the sample usage:
		//!
		//! \code{.cpp}
		//! Array<int, 1> array(10, 4);
		//! array.ForEach([](int elem)
		//! {
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
		//! Array<int, 1> array(10, 4);
		//! array.ForEachIndex([&](size_t i)
		//! {
		//!     array[i] = 4.f * i + 1.5f;
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
		//! Array<int, 1> array(1000, 4);
		//! array.ParallelForEach([](int& elem)
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
		//! Array<int, 1> array(1000, 4);
		//! array.ParallelForEachIndex([](size_t i)
		//! {
		//!     array[i] *= 2;
		//! });
		//! \endcode
		//!
		template <typename Callback>
		void ParallelForEachIndex(Callback func) const;

		//! Returns the reference to i-th element.
		T& operator[](size_t i);

		//! Returns the const reference to i-th element.
		const T& operator[](size_t i) const;

		//! Sets entire array with given \p value.
		Array& operator=(const T& other);

		//! Copies given array \p other to this array.
		Array& operator=(const Array& other);

		//! Moves given array \p other to this array.
		Array& operator=(Array&& other);

		//! Copies given initializer list \p list to this array.
		Array& operator=(const std::initializer_list<T>& list);

		//! Casts to array accessor.
		operator ArrayAccessor1<T>();

		//! Casts to const array accessor.
		operator ConstArrayAccessor1<T>() const;
		
	private:
		ContainerType m_data;
	};

	//! Type alias for 1-D array.
	template <typename T>
	using Array1 = Array<T, 1>;
}

#include <Core/Array/Array1-Impl.h>

#endif