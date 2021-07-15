// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CONSTANTS_HPP
#define CUBBYFLOW_CONSTANTS_HPP

#include <Core/Utils/Macros.hpp>

namespace CubbyFlow
{
// MARK: Zero
//! Zero size_t.
constexpr size_t ZERO_SIZE = 0;
//! Zero ssize_t.
constexpr ssize_t ZERO_SSIZE = 0;

//! Zero for type T.
template <typename T>
constexpr T Zero()
{
    return 0;
}

//! Zero for float.
template <>
constexpr float Zero<float>()
{
    return 0.f;
}

//! Zero for double.
template <>
constexpr double Zero<double>()
{
    return 0.0;
}

// MARK: One
//! One size_t.
constexpr size_t ONE_SIZE = 1;

//! One ssize_t.
constexpr ssize_t ONE_SSIZE = 1;

//! One for type T.
template <typename T>
constexpr T One()
{
    return 1;
}

//! One for float.
template <>
constexpr float One<float>()
{
    return 1.f;
}

//! One for double.
template <>
constexpr double One<double>()
{
    return 1.0;
}

// MARK: PI
//! Float-type PI.
constexpr float PI_FLOAT = 3.14159265358979323846264338327950288f;
//! Double-type PI.
constexpr double PI_DOUBLE = 3.14159265358979323846264338327950288;

//! PI for type T.
template <typename T>
constexpr T PI()
{
    return static_cast<T>(PI_DOUBLE);
}

//! Pi for float.
template <>
constexpr float PI<float>()
{
    return PI_FLOAT;
}

//! Pi for double.
template <>
constexpr double PI<double>()
{
    return PI_DOUBLE;
}

// MARK: PI/2
//! Float-type PI/2.
constexpr float HALF_PI_FLOAT = 1.57079632679489661923132169163975144f;

//! Double-type PI/2.
constexpr double HALF_PI_DOUBLE = 1.57079632679489661923132169163975144;

//! PI/2 for type T.
template <typename T>
constexpr T HalfPI()
{
    return static_cast<T>(HALF_PI_DOUBLE);
}

//! PI/2 for float.
template <>
constexpr float HalfPI<float>()
{
    return HALF_PI_FLOAT;
}

//! PI/2 for double.
template <>
constexpr double HalfPI<double>()
{
    return HALF_PI_DOUBLE;
}

// MARK: PI/4
//! Float-type PI/4.
constexpr float QUARTER_PI_FLOAT = 0.785398163397448309615660845819875721f;

//! Double-type PI/4.
constexpr double QUARTER_PI_DOUBLE = 0.785398163397448309615660845819875721;

//! PI/4 for type T.
template <typename T>
constexpr T QuarterPI()
{
    return static_cast<T>(QUARTER_PI_DOUBLE);
}

//! PI/4 for float.
template <>
constexpr float QuarterPI<float>()
{
    return QUARTER_PI_FLOAT;
}

//! PI/4 for double.
template <>
constexpr double QuarterPI<double>()
{
    return QUARTER_PI_DOUBLE;
}

// MARK: 2*PI
//! Float-type 2*PI.
constexpr float TWO_PI_FLOAT = static_cast<float>(2.0 * PI_DOUBLE);

//! Double-type 2*PI.
constexpr double TWO_PI_DOUBLE = 2.0 * PI_DOUBLE;

//! 2*PI for type T.
template <typename T>
constexpr T TwoPI()
{
    return static_cast<T>(TWO_PI_DOUBLE);
}

//! 2*PI for float.
template <>
constexpr float TwoPI<float>()
{
    return TWO_PI_FLOAT;
}

//! 2*PI for double.
template <>
constexpr double TwoPI<double>()
{
    return TWO_PI_DOUBLE;
}

// MARK: 4*PI
//! Float-type 4*PI.
constexpr float FOUR_PI_FLOAT = static_cast<float>(4.0 * PI_DOUBLE);

//! Double-type 4*pi.
constexpr double FOUR_PI_DOUBLE = 4.0 * PI_DOUBLE;

//! 4*PI for type T.
template <typename T>
constexpr T FourPI()
{
    return static_cast<T>(FOUR_PI_DOUBLE);
}

//! 4*PI for float.
template <>
constexpr float FourPI<float>()
{
    return FOUR_PI_FLOAT;
}

//! 4*PI for double.
template <>
constexpr double FourPI<double>()
{
    return FOUR_PI_DOUBLE;
}

// MARK: 1/PI
//! Float-type 1/PI.
constexpr float INV_PI_FLOAT = static_cast<float>(1.0 / PI_DOUBLE);

//! Double-type 1/PI.
constexpr double INV_PI_DOUBLE = 1.0 / PI_DOUBLE;

//! 1/PI for type T.
template <typename T>
constexpr T InvPI()
{
    return static_cast<T>(INV_PI_DOUBLE);
}

//! 1/PI for float.
template <>
constexpr float InvPI<float>()
{
    return INV_PI_FLOAT;
}

//! 1/PI for double.
template <>
constexpr double InvPI<double>()
{
    return INV_PI_DOUBLE;
}

// MARK: 1/2*PI
//! Float-type 1/2*PI.
constexpr float INV_TWO_PI_FLOAT = static_cast<float>(0.5 / PI_DOUBLE);

//! Double-type 1/2*PI.
constexpr double INV_TWO_PI_DOUBLE = 0.5 / PI_DOUBLE;

//! 1/2*PI for type T.
template <typename T>
constexpr T InvTwoPI()
{
    return static_cast<T>(INV_TWO_PI_DOUBLE);
}

//! 1/2*PI for float.
template <>
constexpr float InvTwoPI<float>()
{
    return INV_TWO_PI_FLOAT;
}

//! 1/2*PI for double.
template <>
constexpr double InvTwoPI<double>()
{
    return INV_TWO_PI_DOUBLE;
}

// MARK: 1/4*PI
//! Float-type 1/4*PI.
constexpr float INV_FOUR_PI_FLOAT = static_cast<float>(0.25 / PI_DOUBLE);

//! Double-type 1/4*PI.
constexpr double INV_FOUR_PI_DOUBLE = 0.25 / PI_DOUBLE;

//! 1/4*PI for type T.
template <typename T>
constexpr T InvFourPI()
{
    return static_cast<T>(INV_FOUR_PI_DOUBLE);
}

//! 1/4*PI for float.
template <>
constexpr float InvFourPI<float>()
{
    return INV_FOUR_PI_FLOAT;
}

//! 1/4*PI for double.
template <>
constexpr double InvFourPI<double>()
{
    return INV_FOUR_PI_DOUBLE;
}

// MARK: Physics
//! Gravity.
constexpr float GRAVITY_FLOAT = -9.8f;
constexpr double GRAVITY = -9.8;

//! Water density.
constexpr float WATER_DENSITY_FLOAT = 1000.0f;
constexpr double WATER_DENSITY = 1000.0;

//! Speed of sound in water at 20 degrees Celsius.
constexpr float SPEED_OF_SOUND_IN_WATER_FLOAT = 1482.0f;
constexpr double SPEED_OF_SOUND_IN_WATER = 1482.0;

// MARK: Common enums
//! No direction.
constexpr int DIRECTION_NONE = 0;

//! Left direction.
constexpr int DIRECTION_LEFT = 1 << 0;

//! Right direction.
constexpr int DIRECTION_RIGHT = 1 << 1;

//! Down direction.
constexpr int DIRECTION_DOWN = 1 << 2;

//! Up direction.
constexpr int DIRECTION_UP = 1 << 3;

//! Back direction.
constexpr int DIRECTION_BACK = 1 << 4;

//! Front direction.
constexpr int DIRECTION_FRONT = 1 << 5;

//! All direction.
constexpr int DIRECTION_ALL = DIRECTION_LEFT | DIRECTION_RIGHT |
                              DIRECTION_DOWN | DIRECTION_UP | DIRECTION_BACK |
                              DIRECTION_FRONT;
}  // namespace CubbyFlow

#endif