// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include "gtest/gtest.h"

#include <Core/CUDA/CUDAStdVector.hpp>

using namespace CubbyFlow;

TEST(CUDAStdVector, Constructors)
{
    {
        CUDAStdVector<int> vec;
        EXPECT_EQ(0u, vec.Size());
        EXPECT_EQ(nullptr, vec.data());
    }

    {
        CUDAStdVector<int> vec(5, 3);
        EXPECT_EQ(5u, vec.Size());

        std::vector<int> ans;
        vec.CopyTo(ans);

        for (size_t i = 0; i < 5; ++i)
        {
            EXPECT_EQ(3, ans[i]);
        }
    }

    {
        std::vector<int> host({ 1, 2, 3, 4, 5 });
        CUDAStdVector<int> vec(host);

        std::vector<int> ans;
        vec.CopyTo(ans);

        for (size_t i = 0; i < 5; ++i)
        {
            EXPECT_EQ(host[i], ans[i]);
        }
    }

    {
        std::vector<int> host({ 1, 2, 3, 4, 5 });
        CUDAStdVector<int> vec(host);
        CUDAStdVector<int> vec2(vec);

        std::vector<int> ans;
        vec2.CopyTo(ans);

        for (size_t i = 0; i < 5; ++i)
        {
            EXPECT_EQ(host[i], ans[i]);
        }
    }

    {
        std::vector<int> host({ 1, 2, 3, 4, 5 });
        CUDAStdVector<int> vec(host);
        CUDAStdVector<int> vec2 = std::move(vec);

        EXPECT_EQ(0u, vec.Size());
        EXPECT_EQ(nullptr, vec.data());

        std::vector<int> ans;
        vec2.CopyTo(ans);

        for (size_t i = 0; i < 5; ++i)
        {
            EXPECT_EQ(host[i], ans[i]);
        }
    }
}

TEST(CUDAStdVector, Getters)
{
    std::vector<int> host({ 1, 2, 3, 4, 5 });
    CUDAStdVector<int> vec(host);

    EXPECT_NE(nullptr, vec.data());
    EXPECT_EQ(5u, vec.Size());

    const auto& vecRef = vec;
    EXPECT_NE(nullptr, vecRef.data());

    for (size_t i = 0; i < 5; ++i)
    {
        EXPECT_EQ(host[i], vecRef.At(i));
    }

    std::vector<int> ans;
    vec.CopyTo(ans);

    for (size_t i = 0; i < 5; ++i)
    {
        EXPECT_EQ(host[i], ans[i]);
    }
}

TEST(CUDAStdVector, Modifiers)
{
    std::vector<int> host({ 1, 2, 3, 4, 5 });
    CUDAStdVector<int> vec(host);

    vec.At(0) = 9;
    vec.At(1) = 8;
    vec.At(2) = 7;
    vec.At(3) = 6;
    vec.At(4) = 5;

    const auto& vecRef = vec;
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(9 - i, vec.At(i));
        EXPECT_EQ(9 - i, vecRef.At(i));
    }

    vec.Fill(10);
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(10, vec.At(i));
    }

    vec.Clear();
    EXPECT_EQ(0u, vec.Size());
    EXPECT_EQ(nullptr, vec.data());

    vec.ResizeUninitialized(4);
    EXPECT_EQ(4u, vec.Size());

    vec.Resize(7, 3);
    EXPECT_EQ(7u, vec.Size());
    for (int i = 4; i < 7; ++i)
    {
        EXPECT_EQ(3, vec.At(i));
    }

    CUDAStdVector<int> vec2(host);
    vec.Swap(vec2);

    EXPECT_EQ(7u, vec2.Size());
    for (int i = 4; i < 7; ++i)
    {
        EXPECT_EQ(3, vec2.At(i));
    }
    EXPECT_EQ(5u, vec.Size());
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(host[i], vec.At(i));
    }

    vec.PushBack(6);
    EXPECT_EQ(6u, vec.Size());
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(host[i], vec.At(i));
    }
    EXPECT_EQ(6, vec.At(5));

    vec2.CopyFrom(host);
    vec.Append(vec2);

    EXPECT_EQ(11u, vec.Size());
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(host[i], vec.At(i));
    }
    EXPECT_EQ(6, vec.At(5));
    for (int i = 6; i < 11; ++i)
    {
        EXPECT_EQ(host[i - 6], vec.At(i));
    }

    vec.CopyFrom(vec2);
    EXPECT_EQ(5u, vec.Size());
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(host[i], vec.At(i));
    }
}

TEST(CUDAStdVector, Operators)
{
    std::vector<int> host({ 1, 2, 3, 4, 5 });
    CUDAStdVector<int> vec(host);

    vec[0] = 9;
    vec[1] = 8;
    vec[2] = 7;
    vec[3] = 6;
    vec[4] = 5;

    const auto& vecRef = vec;
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(9 - i, vec[i]);
        EXPECT_EQ(9 - i, vecRef[i]);
    }

    vec = host;
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(host[i], vec[i]);
    }

    CUDAStdVector<int> vec2(host);
    vec.Fill(42);
    vec = vec2;
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(host[i], vec[i]);
    }
}
