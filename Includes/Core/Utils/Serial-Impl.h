/*************************************************************************
> File Name: Serial-Impl.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Serial functions for CubbyFlow.
> Created Time: 2017/07/05
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SERIAL_IMPL_H
#define CUBBYFLOW_SERIAL_IMPL_H

#include <algorithm>

namespace CubbyFlow
{
	template <typename RandomIterator, typename T>
	void SerialFill(const RandomIterator& begin, const RandomIterator& end, const T& value)
	{
		size_t size = static_cast<size_t>(end - begin);

		SerialFor(size_t(0), size, [begin, value](size_t i)
		{
			begin[i] = value;
		});
	}

	template <typename IndexType, typename Function>
	void SerialFor(IndexType beginIndex, IndexType endIndex, const Function& function)
	{
		for (IndexType i = beginIndex; i < endIndex; ++i)
		{
			function(i);
		}
	}

	template <typename IndexType, typename Function>
	void SerialFor(
		IndexType beginIndexX, IndexType endIndexX,
		IndexType beginIndexY, IndexType endIndexY,
		const Function& function)
	{
		for (IndexType j = beginIndexY; j < endIndexY; ++j)
		{
			for (IndexType i = beginIndexX; i < endIndexX; ++i)
			{
				function(i, j);
			}
		}
	}

	template <typename IndexType, typename Function>
	void SerialFor(
		IndexType beginIndexX, IndexType endIndexX,
		IndexType beginIndexY, IndexType endIndexY,
		IndexType beginIndexZ, IndexType endIndexZ,
		const Function& function)
	{
		for (IndexType k = beginIndexZ; k < endIndexZ; ++k)
		{
			for (IndexType j = beginIndexY; j < endIndexY; ++j)
			{
				for (IndexType i = beginIndexX; i < endIndexX; ++i)
				{
					function(i, j, k);
				}
			}
		}
	}

	template<typename RandomIterator>
	void SerialSort(RandomIterator begin, RandomIterator end)
	{
		SerialSort(begin, end, std::less<typename RandomIterator::value_type>());
	}

	template<typename RandomIterator, typename SortingFunction>
	void SerialSort(RandomIterator begin, RandomIterator end,
		const SortingFunction& sortingFunction)
	{
		std::sort(begin, end, sortingFunction);
	}
}

#endif