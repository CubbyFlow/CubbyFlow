// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FRAME_HPP
#define CUBBYFLOW_FRAME_HPP

namespace CubbyFlow
{
//!
//! \brief Representation of an animation frame.
//!
//! This struct holds current animation frame index and frame interval in
//! seconds.
//!
struct Frame final
{
    //! Default constructor: A Frame instance with 1/60 seconds time interval.
    Frame() = default;

    //! Constructs Frame instance with given time interval.
    Frame(int newIndex, double newTimeIntervalInSeconds);

    //! Returns the elapsed time in seconds.
    [[nodiscard]] double TimeInSeconds() const;

    //! Advances single frame.
    void Advance();

    //! Advances multiple frames.
    //! \param delta Number of frames to advance.
    void Advance(int delta);

    //! Advances single frame (prefix).
    Frame& operator++();

    //! Advances single frame (postfix).
    Frame operator++(int);

    //! Frame index.
    int index = 0;

    //! Time interval in seconds between two adjacent frames.
    double timeIntervalInSeconds = 1.0 / 60.0;
};
}  // namespace CubbyFlow

#endif