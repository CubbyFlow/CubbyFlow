#include "pch.hpp"

#include <Core/Array/Array2.hpp>
#include <Core/Utils/Serialization.hpp>

using namespace CubbyFlow;

TEST(Array2, Constructors)
{
    Array2<float> arr1;
    EXPECT_EQ(0u, arr1.Width());
    EXPECT_EQ(0u, arr1.Height());

    Array2<float> arr2(Size2(3, 7));
    EXPECT_EQ(3u, arr2.Width());
    EXPECT_EQ(7u, arr2.Height());
    for (size_t i = 0; i < 21; ++i)
    {
        EXPECT_FLOAT_EQ(0.f, arr2[i]);
    }

    Array2<float> arr3(Size2(1, 9), 1.5f);
    EXPECT_EQ(1u, arr3.Width());
    EXPECT_EQ(9u, arr3.Height());
    for (size_t i = 0; i < 9; ++i)
    {
        EXPECT_FLOAT_EQ(1.5f, arr3[i]);
    }

    Array2<float> arr4(5, 2);
    EXPECT_EQ(5u, arr4.Width());
    EXPECT_EQ(2u, arr4.Height());
    for (size_t i = 0; i < 10; ++i)
    {
        EXPECT_FLOAT_EQ(0.f, arr4[i]);
    }

    Array2<float> arr5(3, 4, 7.f);
    EXPECT_EQ(3u, arr5.Width());
    EXPECT_EQ(4u, arr5.Height());
    for (size_t i = 0; i < 12; ++i)
    {
        EXPECT_FLOAT_EQ(7.f, arr5[i]);
    }

    Array2<float> arr6({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });
    EXPECT_EQ(4u, arr6.Width());
    EXPECT_EQ(3u, arr6.Height());
    for (size_t i = 0; i < 12; ++i)
    {
        EXPECT_FLOAT_EQ(static_cast<float>(i) + 1.f, arr6[i]);
    }

    Array2<float> arr7({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });
    Array2<float> arr8(arr7);
    EXPECT_EQ(4u, arr8.Width());
    EXPECT_EQ(3u, arr8.Height());
    for (size_t i = 0; i < 12; ++i)
    {
        EXPECT_FLOAT_EQ(static_cast<float>(i) + 1.f, arr8[i]);
    }
}

TEST(Array2, Clear)
{
    Array2<float> arr({ { 1.f, 2.f, 3.f, 4.f },
                        { 5.f, 6.f, 7.f, 8.f },
                        { 9.f, 10.f, 11.f, 12.f } });

    arr.Clear();
    EXPECT_EQ(0u, arr.Width());
    EXPECT_EQ(0u, arr.Height());
}

TEST(Array2, ResizeMethod)
{
    Array2<float> arr1;
    arr1.Resize(Size2(2, 9));
    EXPECT_EQ(2u, arr1.Width());
    EXPECT_EQ(9u, arr1.Height());
    for (size_t i = 0; i < 18; ++i)
    {
        EXPECT_FLOAT_EQ(0.f, arr1[i]);
    }

    arr1.Resize(Size2(8, 13), 4.f);
    EXPECT_EQ(8u, arr1.Width());
    EXPECT_EQ(13u, arr1.Height());
    for (size_t i = 0; i < 8; ++i)
    {
        for (size_t j = 0; j < 13; ++j)
        {
            if (i < 2 && j < 9)
            {
                EXPECT_FLOAT_EQ(0.f, arr1(i, j));
            }
            else
            {
                EXPECT_FLOAT_EQ(4.f, arr1(i, j));
            }
        }
    }

    Array2<float> arr2;
    arr2.Resize(7, 6);
    EXPECT_EQ(7u, arr2.Width());
    EXPECT_EQ(6u, arr2.Height());
    for (size_t i = 0; i < 42; ++i)
    {
        EXPECT_FLOAT_EQ(0.f, arr2[i]);
    }

    arr2.Resize(1, 9, 3.f);
    EXPECT_EQ(1u, arr2.Width());
    EXPECT_EQ(9u, arr2.Height());
    for (size_t i = 0; i < 1; ++i)
    {
        for (size_t j = 0; j < 9; ++j)
        {
            if (j < 6)
            {
                EXPECT_FLOAT_EQ(0.f, arr2(i, j));
            }
            else
            {
                EXPECT_FLOAT_EQ(3.f, arr2(i, j));
            }
        }
    }
}

TEST(Array2, AtMethod)
{
    float values[12] = { 0.f, 1.f, 2.f, 3.f, 4.f,  5.f,
                         6.f, 7.f, 8.f, 9.f, 10.f, 11.f };
    Array2<float> arr(4, 3);
    for (size_t i = 0; i < 12; ++i)
    {
        arr[i] = values[i];
    }

    // Test row-major
    EXPECT_FLOAT_EQ(0.f, arr(0, 0));
    EXPECT_FLOAT_EQ(1.f, arr(1, 0));
    EXPECT_FLOAT_EQ(2.f, arr(2, 0));
    EXPECT_FLOAT_EQ(3.f, arr(3, 0));
    EXPECT_FLOAT_EQ(4.f, arr(0, 1));
    EXPECT_FLOAT_EQ(5.f, arr(1, 1));
    EXPECT_FLOAT_EQ(6.f, arr(2, 1));
    EXPECT_FLOAT_EQ(7.f, arr(3, 1));
    EXPECT_FLOAT_EQ(8.f, arr(0, 2));
    EXPECT_FLOAT_EQ(9.f, arr(1, 2));
    EXPECT_FLOAT_EQ(10.f, arr(2, 2));
    EXPECT_FLOAT_EQ(11.f, arr(3, 2));
}

TEST(Array2, Iterators)
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });

    float cnt = 1.f;
    for (float& elem : arr1)
    {
        EXPECT_FLOAT_EQ(cnt, elem);
        cnt += 1.f;
    }

    cnt = 1.f;
    for (const float& elem : arr1)
    {
        EXPECT_FLOAT_EQ(cnt, elem);
        cnt += 1.f;
    }
}

TEST(Array2, ForEach)
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });

    size_t i = 0;
    arr1.ForEach([&](float val) {
        EXPECT_FLOAT_EQ(arr1[i], val);
        ++i;
    });
}

TEST(Array2, ForEachIndex)
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });

    arr1.ForEachIndex([&](size_t i, size_t j) {
        size_t idx = i + (4 * j) + 1;
        EXPECT_FLOAT_EQ(static_cast<float>(idx), arr1(i, j));
    });
}

TEST(Array2, ParallelForEach)
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });

    arr1.ParallelForEach([&](float& val) { val *= 2.f; });

    arr1.ForEachIndex([&](size_t i, size_t j) {
        size_t idx = i + (4 * j) + 1;
        float ans = 2.f * static_cast<float>(idx);
        EXPECT_FLOAT_EQ(ans, arr1(i, j));
    });
}

TEST(Array2, ParallelForEachIndex)
{
    Array2<float> arr1({ { 1.f, 2.f, 3.f, 4.f },
                         { 5.f, 6.f, 7.f, 8.f },
                         { 9.f, 10.f, 11.f, 12.f } });

    arr1.ParallelForEachIndex([&](size_t i, size_t j) {
        size_t idx = i + (4 * j) + 1;
        EXPECT_FLOAT_EQ(static_cast<float>(idx), arr1(i, j));
    });
}