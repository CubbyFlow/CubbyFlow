// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include "doctest_proxy.hpp"

#include <Core/Animation/Animation.hpp>

using namespace CubbyFlow;

TEST_CASE("[Frame] - Constructors")
{
    Frame frame;
    CHECK_EQ(0, frame.index);
    CHECK_EQ(doctest::Approx(1.0 / 60.0), frame.timeIntervalInSeconds);
}

TEST_CASE("[Frame] - TimeInSeconds")
{
    Frame frame;

    frame.index = 180;

    CHECK_EQ(doctest::Approx(3.0), frame.TimeInSeconds());
}

TEST_CASE("[Frame] - Advance")
{
    Frame frame;

    frame.index = 45;

    for (int i = 0; i < 9; ++i)
    {
        frame.Advance();
    }

    CHECK_EQ(54, frame.index);

    frame.Advance(23);

    CHECK_EQ(77, frame.index);

    CHECK_EQ(78, (++frame).index);

    CHECK_EQ(78, (frame++).index);

    CHECK_EQ(79, frame.index);
}