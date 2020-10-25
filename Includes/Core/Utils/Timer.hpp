// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_TIMER_HPP
#define CUBBYFLOW_TIMER_HPP

#include <chrono>

namespace CubbyFlow
{
//! Simple timer class.
class Timer
{
 public:
    //! Constructs the timer and start ticking.
    Timer();

    //! Returns the time duration since the creation or reset in seconds.
    [[nodiscard]] double DurationInSeconds() const;

    //! Resets the timer.
    void Reset();

 private:
    std::chrono::steady_clock::time_point m_startingPoint;
};
}  // namespace CubbyFlow

#endif