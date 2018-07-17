/*************************************************************************
> File Name: ArrayUtils-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: N-D array util functions.
> Created Time: 2017/04/30
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ARRAY_UTILS_IMPL_H
#define CUBBYFLOW_ARRAY_UTILS_IMPL_H

#include <Array/Array2.h>
#include <Array/Array3.h>
#include <Utils/Parallel.h>
#include <Utils/TypeHelpers.h>

#include <iostream>

namespace CubbyFlow
{
	template <typename ArrayType, typename T>
	void SetRange1(size_t size, const T& value, ArrayType* output)
	{
		SetRange1(ZERO_SIZE, size, value, output);
	}

	template <typename ArrayType, typename T>
	void SetRange1(size_t begin, size_t end, const T& value, ArrayType* output)
	{
		ParallelFor(begin, end,	[&](size_t i)
		{
			(*output)[i] = value;
		});
	}

	template <typename ArrayType1, typename ArrayType2>
	void CopyRange1(const ArrayType1& input, size_t size, ArrayType2* output)
	{
		CopyRange1(input, 0, size, output);
	}

	template <typename ArrayType1, typename ArrayType2>
	void CopyRange1(const ArrayType1& input, size_t begin, size_t end, ArrayType2* output)
	{
		ParallelFor(begin, end, [&input, &output](size_t i)
		{
			(*output)[i] = input[i];
		});
	}

	template <typename ArrayType1, typename ArrayType2>
	void CopyRange2(const ArrayType1& input, size_t sizeX, size_t sizeY, ArrayType2* output)
	{
		CopyRange2(input, ZERO_SIZE, sizeX, ZERO_SIZE, sizeY, output);
	}

	template <typename ArrayType1, typename ArrayType2>
	void CopyRange2(const ArrayType1& input, size_t beginX, size_t endX, size_t beginY, size_t endY, ArrayType2* output)
	{
		ParallelFor(beginX, endX, beginY, endY,	[&input, &output](size_t i, size_t j)
		{
			(*output)(i, j) = input(i, j);
		});
	}

	template <typename ArrayType1, typename ArrayType2>
	void CopyRange3(const ArrayType1& input, size_t sizeX, size_t sizeY, size_t sizeZ, ArrayType2* output)
	{
		CopyRange3(input, ZERO_SIZE, sizeX, ZERO_SIZE, sizeY, ZERO_SIZE, sizeZ, output);
	}

	template <typename ArrayType1, typename ArrayType2>
	void CopyRange3(const ArrayType1& input, size_t beginX, size_t endX, size_t beginY, size_t endY, size_t beginZ, size_t endZ, ArrayType2* output)
	{
		ParallelFor(beginX, endX, beginY, endY, beginZ, endZ, [&input, &output](size_t i, size_t j, size_t k)
		{
			(*output)(i, j, k) = input(i, j, k);
		});
	}

	template <typename T>
	void ExtrapolateToRegion(const ConstArrayAccessor2<T>& input, const ConstArrayAccessor2<char>& valid, unsigned int numberOfIterations, ArrayAccessor2<T> output)
	{
		const Size2 size = input.size();

		assert(size == valid.size());
		assert(size == output.size());

		Array2<char> valid0(size);
		Array2<char> valid1(size);

		valid0.ParallelForEachIndex([&](size_t i, size_t j)
		{
			valid0(i, j) = valid(i, j);
			output(i, j) = input(i, j);
		});

		for (unsigned int iter = 0; iter < numberOfIterations; ++iter)
		{
			valid0.ForEachIndex([&](size_t i, size_t j)
			{
				T sum = Zero<T>();
				unsigned int count = 0;

				if (!valid0(i, j))
				{
					if (i + 1 < size.x && valid0(i + 1, j))
					{
						sum += output(i + 1, j);
						++count;
					}

					if (i > 0 && valid0(i - 1, j))
					{
						sum += output(i - 1, j);
						++count;
					}

					if (j + 1 < size.y && valid0(i, j + 1))
					{
						sum += output(i, j + 1);
						++count;
					}

					if (j > 0 && valid0(i, j - 1))
					{
						sum += output(i, j - 1);
						++count;
					}

					if (count > 0)
					{
						output(i, j) = sum / static_cast<typename ScalarType<T>::value>(count);
						valid1(i, j) = 1;
					}
				}
				else
				{
					valid1(i, j) = 1;
				}
			});

			valid0.Swap(valid1);
		}
	}

	template <typename T>
	void ExtrapolateToRegion(const ConstArrayAccessor3<T>& input, const ConstArrayAccessor3<char>& valid, unsigned int numberOfIterations, ArrayAccessor3<T> output)
	{
		const Size3 size = input.size();

		assert(size == valid.size());
		assert(size == output.size());

		Array3<char> valid0(size);
		Array3<char> valid1(size);

		valid0.ParallelForEachIndex([&](size_t i, size_t j, size_t k)
		{
			valid0(i, j, k) = valid(i, j, k);
			output(i, j, k) = input(i, j, k);
		});

		for (unsigned int iter = 0; iter < numberOfIterations; ++iter)
		{
			valid0.ForEachIndex([&](size_t i, size_t j, size_t k)
			{
				T sum = Zero<T>();
				unsigned int count = 0;

				if (!valid0(i, j, k))
				{
					if (i + 1 < size.x && valid0(i + 1, j, k))
					{
						sum += output(i + 1, j, k);
						++count;
					}

					if (i > 0 && valid0(i - 1, j, k))
					{
						sum += output(i - 1, j, k);
						++count;
					}

					if (j + 1 < size.y && valid0(i, j + 1, k))
					{
						sum += output(i, j + 1, k);
						++count;
					}

					if (j > 0 && valid0(i, j - 1, k))
					{
						sum += output(i, j - 1, k);
						++count;
					}

					if (k + 1 < size.z && valid0(i, j, k + 1))
					{
						sum += output(i, j, k + 1);
						++count;
					}

					if (k > 0 && valid0(i, j, k - 1))
					{
						sum += output(i, j, k - 1);
						++count;
					}

					if (count > 0)
					{
						output(i, j, k) = sum / static_cast<typename ScalarType<T>::value>(count);
						valid1(i, j, k) = 1;
					}
				}
				else {
					valid1(i, j, k) = 1;
				}
			});

			valid0.Swap(valid1);
		}
	}

	template <typename ArrayType>
	void ConvertToCSV(const ArrayType& data, std::ostream* stream)
	{
		Size2 size = data.size();

		for (size_t j = 0; j < size.y; ++j)
		{
			for (size_t i = 0; i < size.x; ++i)
			{
				auto val = data(i, j);

				// TODO: Hack to handle char and unsigned char
				if constexpr (sizeof(decltype(val)) == 1)
				{
					*stream << static_cast<int>(val);
				}
				else
				{
					*stream << val;
				}

				if (i + 1 < size.x)
				{
					*stream << ", ";
				}
			}

			*stream << std::endl;
		}
	}
}

#endif