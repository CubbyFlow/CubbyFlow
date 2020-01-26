#include "pch.h"

#include <Core/Array/Array1.hpp>
#include <Core/Utils/Serialization.h>

using namespace CubbyFlow;

TEST(Array1, Constructors)
{
	Array1<float> arr1;
	EXPECT_EQ(0u, arr1.size());

	Array1<float> arr2(9, 1.5f);
	EXPECT_EQ(9u, arr2.size());
	for (size_t i = 0; i < 9; ++i)
	{
		EXPECT_FLOAT_EQ(1.5f, arr2[i]);
	}

	Array1<float> arr3({ 1.f,  2.f,  3.f,  4.f });
	EXPECT_EQ(4u, arr3.size());
	for (size_t i = 0; i < 4; ++i)
	{
		EXPECT_FLOAT_EQ(static_cast<float>(i) + 1.f, arr3[i]);
	}

	Array1<float> arr4({ 1.f,  2.f,  3.f,  4.f });
	Array1<float> arr5(arr4);
	EXPECT_EQ(4u, arr5.size());
	for (size_t i = 0; i < 4; ++i)
	{
		EXPECT_FLOAT_EQ(static_cast<float>(i) + 1.f, arr5[i]);
	}
}

TEST(Array1, SetMethods)
{
	Array1<float> arr1(12, -1.f);
	arr1.Set(3.5f);
	for (float a : arr1)
	{
		EXPECT_EQ(3.5f, a);
	}

	Array1<float> arr2;
	arr1.Set(arr2);
	EXPECT_EQ(arr1.size(), arr2.size());
	for (size_t i = 0; i < arr2.size(); ++i)
	{
		EXPECT_EQ(arr1[i], arr2[i]);
	}

	arr2 = { 2.f, 5.f, 9.f, -1.f };
	EXPECT_EQ(4u, arr2.size());
	EXPECT_EQ(2.f, arr2[0]);
	EXPECT_EQ(5.f, arr2[1]);
	EXPECT_EQ(9.f, arr2[2]);
	EXPECT_EQ(-1.f, arr2[3]);
}

TEST(Array1, Clear)
{
	Array1<float> arr1 = { 2.f, 5.f, 9.f, -1.f };
	arr1.Clear();
	EXPECT_EQ(0u, arr1.size());
}

TEST(Array1, ResizeMethod)
{
	Array1<float> arr;
	arr.Resize(9);
	EXPECT_EQ(9u, arr.size());
	for (size_t i = 0; i < 9; ++i)
	{
		EXPECT_FLOAT_EQ(0.f, arr[i]);
	}

	arr.Resize(12, 4.f);
	EXPECT_EQ(12u, arr.size());
	for (size_t i = 0; i < 8; ++i)
	{
		if (i < 9)
		{
			EXPECT_FLOAT_EQ(0.f, arr[i]);
		}
		else
		{
			EXPECT_FLOAT_EQ(4.f, arr[i]);
		}
	}
}

TEST(Array1, Iterators)
{
	Array1<float> arr1 = { 6.f,  4.f,  1.f,  -5.f };

	size_t i = 0;
	for (float& elem : arr1)
	{
		EXPECT_FLOAT_EQ(arr1[i], elem);
		++i;
	}

	i = 0;
	for (const float& elem : arr1)
	{
		EXPECT_FLOAT_EQ(arr1[i], elem);
		++i;
	}
}

TEST(Array1, ForEach)
{
	Array1<float> arr1 = { 6.f,  4.f,  1.f,  -5.f };
	size_t i = 0;
	arr1.ForEach([&](float val)
	{
		EXPECT_FLOAT_EQ(arr1[i], val);
		++i;
	});
}

TEST(Array1, ForEachIndex)
{
	Array1<float> arr1 = { 6.f,  4.f,  1.f,  -5.f };
	size_t cnt = 0;
	arr1.ForEachIndex([&](size_t i)
	{
		EXPECT_EQ(cnt, i);
		++cnt;
	});
}

TEST(Array1, ParallelForEach)
{
	Array1<float> arr1(200);
	arr1.ForEachIndex([&](size_t i)
	{
		arr1[i] = static_cast<float>(200.f - i);
	});

	arr1.ParallelForEach([](float& val)
	{
		val *= 2.f;
	});

	arr1.ForEachIndex([&](size_t i)
	{
		float ans = 2.f * static_cast<float>(200.f - i);
		EXPECT_FLOAT_EQ(ans, arr1[i]);
	});
}

TEST(Array1, ParallelForEachIndex)
{
	Array1<float> arr1(200);
	arr1.ForEachIndex([&](size_t i)
	{
		arr1[i] = static_cast<float>(200.f - i);
	});

	arr1.ParallelForEachIndex([&](size_t i)
	{
		float ans = static_cast<float>(200.f - i);
		EXPECT_EQ(ans, arr1[i]);
	});
}

TEST(Array1, Serialization)
{
	Array1<float> arr1 = { 1.f,  2.f,  3.f,  4.f };

	// Serialize to in-memory stream
	std::vector<uint8_t> buffer1;
	Serialize(arr1.ConstAccessor(), &buffer1);

	// Deserialize to non-zero array
	Array1<float> arr2 = { 5.f, 6.f, 7.f };
	Deserialize(buffer1, &arr2);
	EXPECT_EQ(4u, arr2.size());
	EXPECT_EQ(1.f, arr2[0]);
	EXPECT_EQ(2.f, arr2[1]);
	EXPECT_EQ(3.f, arr2[2]);
	EXPECT_EQ(4.f, arr2[3]);

	// Serialize zero-Sized array
	Array1<float> arr3;
	Serialize(arr3.ConstAccessor(), &buffer1);

	// Deserialize to non-zero array
	Deserialize(buffer1, &arr3);
	EXPECT_EQ(0u, arr3.size());
}