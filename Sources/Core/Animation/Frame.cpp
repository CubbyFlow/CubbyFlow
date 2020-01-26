// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.
#include <Core/Utils/Macros.h>
#include <Core/Animation/Frame.hpp>

namespace CubbyFlow
{
Frame::Frame()
{
    // Do nothing
}

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

    Frame result = *this;
    Advance();
    return result;
}
}  // namespace CubbyFlow