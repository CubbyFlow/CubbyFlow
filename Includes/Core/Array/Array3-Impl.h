/*************************************************************************
> File Name: Array3-Impl.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk, Dongmin Kim
> Purpose: 3-D array class.
> Created Time: 2017/01/25
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ARRAY3_IMPL_H
#define CUBBYFLOW_ARRAY3_IMPL_H

#include <algorithm>
#include <cassert>

namespace CubbyFlow
{
	template <typename T>
	Array<T, 3>::Array()
	{
		// Do nothing
	}

	template <typename T>
	Array<T, 3>::Array(const Size3& size, const T& initVal)
	{
		Resize(size, initVal);
	}

	template <typename T>
	Array<T, 3>::Array(size_t width, size_t height, size_t depth, const T& initVal)
	{
		Resize(width, height, depth, initVal);
	}

	template <typename T>
	Array<T, 3>::Array(const std::initializer_list<std::initializer_list<std::initializer_list<T>>>& list)
	{
		Set(list);
	}

	template <typename T>
	Array<T, 3>::Array(const Array& other)
	{
		Set(other);
	}

	template <typename T>
	Array<T, 3>::Array(Array&& other)
	{
		*this = std::move(other);
	}

	template <typename T>
	void Array<T, 3>::Set(const T& value)
	{
		for (auto& v : m_data)
		{
			v = value;
		}
	}

	template <typename T>
	void Array<T, 3>::Set(const Array& other)
	{
		m_data.resize(other.m_data.size());
		std::copy(other.m_data.begin(), other.m_data.end(), m_data.begin());
		m_size = other.m_size;
	}

	template <typename T>
	void Array<T, 3>::Set(const std::initializer_list<std::initializer_list<std::initializer_list<T>>>& list)
	{
		size_t depth = list.size();
		size_t height = (depth > 0) ? list.begin()->size() : 0;
		size_t width = (height > 0) ? list.begin()->begin()->size() : 0;

		Resize(Size3(width, height, depth));

		auto depthIter = list.begin();
		for (size_t k = 0; k < depth; ++k)
		{
			assert(height == depthIter->size());

			auto heightIter = depthIter->begin();
			for (size_t j = 0; j < height; ++j)
			{
				assert(width == heightIter->size());
			
				auto widthIter = heightIter->begin();
				for (size_t i = 0; i < width; ++i)
				{
					(*this)(i, j, k) = *widthIter;
					++widthIter;
				}
				++heightIter;
			}
			++depthIter;
		}
	}

	template <typename T>
	void Array<T, 3>::Clear()
	{
		m_size = Size3(0, 0, 0);
		m_data.clear();
	}

	template <typename T>
	void Array<T, 3>::Resize(const Size3& size, const T& initVal)
	{
		Array grid;
		grid.m_data.resize(size.x * size.y * size.z, initVal);
		grid.m_size = size;

		size_t iMin = std::min(size.x, m_size.x);
		size_t jMin = std::min(size.y, m_size.y);
		size_t kMin = std::min(size.z, m_size.z);

		for (size_t k = 0; k < kMin; ++k)
		{
			for (size_t j = 0; j < jMin; ++j)
			{
				for (size_t i = 0; i < iMin; ++i)
				{
					grid(i, j, k) = At(i, j, k);
				}
			}
		}

		Swap(grid);
	}

	template <typename T>
	void Array<T, 3>::Resize(size_t width, size_t height, size_t depth, const T& initVal)
	{
		Resize(Size3(width, height, depth), initVal);
	}

	template <typename T>
	T& Array<T, 3>::At(size_t i)
	{
		assert(i < Width() * Height() * Depth());
		return m_data[i];
	}

	template <typename T>
	const T& Array<T, 3>::At(size_t i) const
	{
		assert(i < Width() * Height() * Depth());
		return m_data[i];
	}

	template <typename T>
	T& Array<T, 3>::At(const Point3UI& pt)
	{
		return At(pt.x, pt.y, pt.z);
	}

	template <typename T>
	const T& Array<T, 3>::At(const Point3UI& pt) const
	{
		return At(pt.x, pt.y, pt.z);
	}

	template <typename T>
	T& Array<T, 3>::At(size_t i, size_t j, size_t k)
	{
		assert(i < Width() && j < Height() && k < Depth());
		return m_data[i + Width() * j + Width() * Height() * k];
	}

	template <typename T>
	const T& Array<T, 3>::At(size_t i, size_t j, size_t k) const
	{
		assert(i < Width() && j < Height() && k < Depth());
		return m_data[i + Width() * j + Width() * Height() * k];
	}

	template <typename T>
	Size3 Array<T, 3>::size() const
	{
		return m_size;
	}

	template <typename T>
	size_t Array<T, 3>::Width() const
	{
		return m_size.x;
	}

	template <typename T>
	size_t Array<T, 3>::Height() const
	{
		return m_size.y;
	}

	template <typename T>
	size_t Array<T, 3>::Depth() const
	{
		return m_size.z;
	}

	template <typename T>
	T* Array<T, 3>::data()
	{
		return m_data.data();
	}

	template <typename T>
	const T* const Array<T, 3>::data() const
	{
		return m_data.data();
	}

	template <typename T>
	typename Array<T, 3>::ContainerType::iterator Array<T, 3>::begin()
	{
		return m_data.begin();
	}

	template <typename T>
	typename Array<T, 3>::ContainerType::const_iterator Array<T, 3>::begin() const
	{
		return m_data.cbegin();
	}

	template <typename T>
	typename Array<T, 3>::ContainerType::iterator Array<T, 3>::end()
	{
		return m_data.end();
	}

	template <typename T>
	typename Array<T, 3>::ContainerType::const_iterator Array<T, 3>::end() const
	{
		return m_data.cend();
	}

	template <typename T>
	ArrayAccessor3<T> Array<T, 3>::Accessor()
	{
		return ArrayAccessor3<T>(size(), data());
	}

	template <typename T>
	ConstArrayAccessor3<T> Array<T, 3>::ConstAccessor() const
	{
		return ConstArrayAccessor3<T>(size(), data());
	}

	template <typename T>
	void Array<T, 3>::Swap(Array& other)
	{
		std::swap(other.m_data, m_data);
		std::swap(other.m_size, m_size);
	}

	template <typename T>
	template <typename Callback>
	void Array<T, 3>::ForEach(Callback func) const
	{
		ConstAccessor().ForEach(func);
	}

	template <typename T>
	template <typename Callback>
	void Array<T, 3>::ForEachIndex(Callback func) const
	{
		ConstAccessor().ForEachIndex(func);
	}

	template <typename T>
	template <typename Callback>
	void Array<T, 3>::ParallelForEach(Callback func)
	{
		Accessor().ParallelForEach(func);
	}

	template <typename T>
	template <typename Callback>
	void Array<T, 3>::ParallelForEachIndex(Callback func) const
	{
		ConstAccessor().ParallelForEachIndex(func);
	}

	template <typename T>
	T& Array<T, 3>::operator[](size_t i)
	{
		return m_data[i];
	}

	template <typename T>
	const T& Array<T, 3>::operator[](size_t i) const
	{
		return m_data[i];
	}

	template <typename T>
	T& Array<T, 3>::operator()(size_t i, size_t j, size_t k)
	{
		assert(i < Width() && j < Height() && k < Depth());
		return m_data[i + Width() * j + Width() * Height() * k];
	}

	template <typename T>
	const T& Array<T, 3>::operator()(size_t i, size_t j, size_t k) const
	{
		assert(i < Width() && j < Height() && k < Depth());
		return m_data[i + Width() * j + Width() * Height() * k];
	}

	template <typename T>
	T& Array<T, 3>::operator()(const Point3UI &pt)
	{
		assert(pt.x < Width() && pt.y < Height() && pt.z < Depth());
		return m_data[pt.x + Width() * pt.y + Width() * Height() * pt.z];
	}

	template <typename T>
	const T& Array<T, 3>::operator()(const Point3UI &pt) const
	{
		assert(pt.x < Width() && pt.y < Height() && pt.z < Depth());
		return m_data[pt.x + Width() * pt.y + Width() * Height() * pt.z];
	}

	template <typename T>
	Array<T, 3>& Array<T, 3>::operator=(const T& value)
	{
		Set(value);
		return *this;
	}

	template <typename T>
	Array<T, 3>& Array<T, 3>::operator=(const Array& other)
	{
		Set(other);
		return *this;
	}

	template <typename T>
	Array<T, 3>& Array<T, 3>::operator=(Array&& other)
	{
		m_data = std::move(other.m_data);
		m_size = other.m_size;
		other.m_size = Size3();
		return *this;
	}

	template <typename T>
	Array<T, 3>& Array<T, 3>::operator=(const std::initializer_list<std::initializer_list<std::initializer_list<T>>>& list)
	{
		Set(list);
		return *this;
	}

	template <typename T>
	Array<T, 3>::operator ArrayAccessor3<T>()
	{
		return Accessor();
	}

	template <typename T>
	Array<T, 3>::operator ConstArrayAccessor3<T>() const
	{
		return ConstAccessor();
	}
}

#endif