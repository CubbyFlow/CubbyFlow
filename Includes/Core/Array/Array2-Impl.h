/*************************************************************************
> File Name: Array2-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D array class.
> Created Time: 2017/01/25
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ARRAY2_IMPL_H
#define CUBBYFLOW_ARRAY2_IMPL_H

#include <algorithm>
#include <cassert>

namespace CubbyFlow
{
	template <typename T>
	Array<T, 2>::Array()
	{
		// Do nothing
	}

	template <typename T>
	Array<T, 2>::Array(const Size2& size, const T& initVal)
	{
		Resize(size, initVal);
	}

	template <typename T>
	Array<T, 2>::Array(size_t width, size_t height, const T& initVal)
	{
		Resize(width, height, initVal);
	}

	template <typename T>
	Array<T, 2>::Array(const std::initializer_list<std::initializer_list<T>>& list)
	{
		Set(list);
	}

	template <typename T>
	Array<T, 2>::Array(const Array& other)
	{
		Set(other);
	}

	template <typename T>
	Array<T, 2>::Array(Array&& other)
	{
		*this = std::move(other);
	}

	template <typename T>
	void Array<T, 2>::Set(const T& value)
	{
		for (auto& v : m_data)
		{
			v = value;
		}
	}

	template <typename T>
	void Array<T, 2>::Set(const Array& other)
	{
		m_data.resize(other.m_data.size());
		std::copy(other.m_data.begin(), other.m_data.end(), m_data.begin());
		m_size = other.m_size;
	}

	template <typename T>
	void Array<T, 2>::Set(const std::initializer_list<std::initializer_list<T>>& list)
	{
		size_t height = list.size();
		size_t width = (height > 0) ? list.begin()->size() : 0;
		
		Resize(Size2(width, height));
		
		auto rowIter = list.begin();
		for (size_t j = 0; j < height; ++j)
		{
			assert(width == rowIter->size());

			auto colIter = rowIter->begin();
			for (size_t i = 0; i < width; ++i)
			{
				(*this)(i, j) = *colIter;
				++colIter;
			}

			++rowIter;
		}
	}

	template <typename T>
	void Array<T, 2>::Clear()
	{
		m_data.clear();
		m_size = Size2(0, 0);
	}

	template <typename T>
	void Array<T, 2>::Resize(const Size2& size, const T& initVal)
	{
		Array grid;
		grid.m_data.resize(size.x * size.y, initVal);
		grid.m_size = size;
		
		size_t iMin = std::min(size.x, m_size.x);
		size_t jMin = std::min(size.y, m_size.y);
		for (size_t j = 0; j < jMin; ++j)
		{
			for (size_t i = 0; i < iMin; ++i)
			{
				grid(i, j) = At(i, j);
			}
		}

		Swap(grid);
	}

	template <typename T>
	void Array<T, 2>::Resize(size_t width, size_t height, const T& initVal)
	{
		Resize(Size2(width, height), initVal);
	}

	template <typename T>
	T& Array<T, 2>::At(size_t i)
	{
		assert(i < Width() * Height());
		return m_data[i];
	}

	template <typename T>
	const T& Array<T, 2>::At(size_t i) const
	{
		assert(i < Width() * Height());
		return m_data[i];
	}

	template <typename T>
	T& Array<T, 2>::At(const Point2UI& pt)
	{
		return At(pt.x, pt.y);
	}

	template <typename T>
	const T& Array<T, 2>::At(const Point2UI& pt) const
	{
		return At(pt.x, pt.y);
	}

	template <typename T>
	T& Array<T, 2>::At(size_t i, size_t j)
	{
		assert(i < Width() && j < Height());
		return m_data[i + Width() * j];
	}

	template <typename T>
	const T& Array<T, 2>::At(size_t i, size_t j) const
	{
		assert(i < Width() && j < Height());
		return m_data[i + Width() * j];
	}

	template <typename T>
	Size2 Array<T, 2>::size() const
	{
		return m_size;
	}

	template <typename T>
	size_t Array<T, 2>::Width() const
	{
		return m_size.x;
	}

	template <typename T>
	size_t Array<T, 2>::Height() const
	{
		return m_size.y;
	}

	template <typename T>
	T* Array<T, 2>::data()
	{
		return m_data.data();
	}

	template <typename T>
	const T* Array<T, 2>::data() const
	{
		return m_data.data();
	}

	template <typename T>
	typename Array<T, 2>::ContainerType::iterator Array<T, 2>::begin()
	{
		return m_data.begin();
	}

	template <typename T>
	typename Array<T, 2>::ContainerType::const_iterator Array<T, 2>::begin() const
	{
		return m_data.cbegin();
	}

	template <typename T>
	typename Array<T, 2>::ContainerType::iterator Array<T, 2>::end()
	{
		return m_data.end();
	}

	template <typename T>
	typename Array<T, 2>::ContainerType::const_iterator Array<T, 2>::end() const
	{
		return m_data.cend();
	}

	template <typename T>
	ArrayAccessor2<T> Array<T, 2>::Accessor()
	{
		return ArrayAccessor2<T>(size(), data());
	}

	template <typename T>
	ConstArrayAccessor2<T> Array<T, 2>::ConstAccessor() const
	{
		return ConstArrayAccessor2<T>(size(), data());
	}

	template <typename T>
	void Array<T, 2>::Swap(Array& other)
	{
		std::swap(other.m_data, m_data);
		std::swap(other.m_size, m_size);
	}

	template <typename T>
	template <typename Callback>
	void Array<T, 2>::ForEach(Callback func) const
	{
		ConstAccessor().ForEach(func);
	}

	template <typename T>
	template <typename Callback>
	void Array<T, 2>::ForEachIndex(Callback func) const
	{
		ConstAccessor().ForEachIndex(func);
	}

	template <typename T>
	template <typename Callback>
	void Array<T, 2>::ParallelForEach(Callback func)
	{
		Accessor().ParallelForEach(func);
	}

	template <typename T>
	template <typename Callback>
	void Array<T, 2>::ParallelForEachIndex(Callback func) const
	{
		ConstAccessor().ParallelForEachIndex(func);
	}

	template <typename T>
	T& Array<T, 2>::operator[](size_t i)
	{
		return m_data[i];
	}

	template <typename T>
	const T& Array<T, 2>::operator[](size_t i) const
	{
		return m_data[i];
	}

	template <typename T>
	T& Array<T, 2>::operator()(size_t i, size_t j)
	{
		assert(i < Width() && j < Height());
		return m_data[i + Width() * j];
	}

	template <typename T>
	const T& Array<T, 2>::operator()(size_t i, size_t j) const
	{
		assert(i < Width() && j < Height());
		return m_data[i + Width() * j];
	}

	template <typename T>
	T& Array<T, 2>::operator()(const Point2UI &pt)
	{
		assert(pt.x < Width() && pt.y < Height());
		return m_data[pt.x + Width() * pt.y];
	}

	template <typename T>
	const T& Array<T, 2>::operator()(const Point2UI &pt) const
	{
		assert(pt.x < Width() && pt.y < Height());
		return m_data[pt.x + Width() * pt.y];
	}

	template <typename T>
	Array<T, 2>& Array<T, 2>::operator=(const T& value)
	{
		Set(value);
		return *this;
	}

	template <typename T>
	Array<T, 2>& Array<T, 2>::operator=(const Array& other)
	{
		Set(other);
		return *this;
	}

	template <typename T>
	Array<T, 2>& Array<T, 2>::operator=(Array&& other)
	{
		m_data = std::move(other.m_data);
		m_size = other.m_size;
		other.m_size = Size2();
		return *this;
	}

	template <typename T>
	Array<T, 2>& Array<T, 2>::operator=(const std::initializer_list<std::initializer_list<T>>& list)
	{
		Set(list);
		return *this;
	}

	template <typename T>
	Array<T, 2>::operator ArrayAccessor2<T>()
	{
		return Accessor();
	}

	template <typename T>
	Array<T, 2>::operator ConstArrayAccessor2<T>() const
	{
		return ConstAccessor();
	}
}

#endif