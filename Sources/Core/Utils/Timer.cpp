// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Utils/Timer.hpp>

namespace CubbyFlow
{
Timer::Timer()
{
    m_startingPoint = std::chrono::steady_clock::now();
}

double Timer::DurationInSeconds() const
{
    const std::chrono::time_point<std::chrono::steady_clock> end =
        std::chrono::steady_clock::now();
    const long long count =
        std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                              m_startingPoint)
            .count();
    return static_cast<double>(count) / 1000000.0;
}

void Timer::Reset()
{
    m_startingPoint = std::chrono::steady_clock::now();
}

}  // namespace CubbyFlow