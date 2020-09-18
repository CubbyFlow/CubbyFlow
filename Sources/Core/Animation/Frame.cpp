// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Animation/Frame.hpp>
#include <Core/Utils/Macros.hpp>

namespace CubbyFlow
{
Frame::Frame(int newIndex, double newTimeIntervalInSeconds)
    : index(newIndex), timeIntervalInSeconds(newTimeIntervalInSeconds)
{
    // Do nothing
}

double Frame::TimeInSeconds() const
{
    return index * timeIntervalInSeconds;
}

void Frame::Advance()
{
    ++index;
}

void Frame::Advance(int delta)
{
    index += delta;
}

Frame& Frame::operator++()
{
    Advance();
    return *this;
}

Frame Frame::operator++(int i)
{
    UNUSED_VARIABLE(i);

    const Frame result = *this;
    Advance();
    return result;
}
}  // namespace CubbyFlow