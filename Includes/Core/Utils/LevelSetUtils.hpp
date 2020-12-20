// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_LEVEL_SET_UTILS_HPP
#define CUBBYFLOW_LEVEL_SET_UTILS_HPP

namespace CubbyFlow
{
//!
//! \brief      Returns true if \p phi is inside the implicit surface (< 0).
//!
//! \param[in]  phi   The level set value.
//!
//! \tparam     T     Value type.
//!
//! \return     True if inside the implicit surface, false otherwise.
//!
template <typename T>
bool IsInsideSDF(T phi);

//!
//! \brief      Returns smeared Heaviside function.
//!
//! This function returns smeared (or smooth) Heaviside (or step) function
//! between 0 and 1. If \p phi is less than -1.5, it will return 0. If \p phi
//! is greater than 1.5, it will return 1. Between -1.5 and 1.5, the function
//! will return smooth profile between 0 and 1. Derivative of this function is
//! smearedDeltaSdf.
//!
//! \param[in]  phi   The level set value.
//!
//! \tparam     T     Value type.
//!
//! \return     Smeared Heaviside function.
//!
template <typename T>
T SmearedHeavisideSDF(T phi);

//!
//! \brief      Returns smeared delta function.
//!
//! This function returns smeared (or smooth) delta function between 0 and 1.
//! If \p phi is less than -1.5, it will return 0. If \p phi is greater than
//! 1.5, it will also return 0. Between -1.5 and 1.5, the function will return
//! smooth delta function. Integral of this function is smearedHeavisideSdf.
//!
//! \param[in]  phi   The level set value.
//!
//! \tparam     T     Value type.
//!
//! \return     Smeared delta function.
//!
template <typename T>
T SmearedDeltaSDF(T phi);

//!
//! \brief      Returns the fraction occupied by the implicit surface.
//!
//! The input parameters, \p phi0 and \p phi1, are the level set values,
//! measured from two nearby points. This function computes how much the
//! implicit surface occupies the line between two points. For example, if both
//! \p phi0 and \p phi1 are negative, it means the points are both inside the
//! surface, thus the function will return 1. If both are positive, it will
//! return 0 because both are outside the surface. If the signs are different,
//! then only one of the points is inside the surface and the function will
//! return a value between 0 and 1.
//!
//! \param[in]  phi0  The level set value from the first point.
//! \param[in]  phi1  The level set value from the second point.
//!
//! \tparam     T     Value type.
//!
//! \return     The fraction occupied by the implicit surface.
//!
template <typename T>
T FractionInsideSDF(T phi0, T phi1);

//!
//! \brief      Returns the fraction occupied by the implicit surface.
//!
//! Given four signed distance values (square corners), determine what fraction
//! of the square is "inside". The original implementation can be found from
//! Christopher Batty's variational fluid code at
//! https://github.com/christopherbatty/Fluid3D.
//!
//! \tparam T               Value type.
//!
//! \param phiBottomLeft    The level set value on the bottom-left corner.
//! \param phiBottomRight   The level set value on the bottom-right corner.
//! \param phiTopLeft       The level set value on the top-left corner.
//! \param phiTopRight      The level set value on the top-right corner.
//!
//! \return                 The fraction occupied by the implicit surface.
//!
template <typename T>
T FractionInside(T phiBottomLeft, T phiBottomRight, T phiTopLeft,
                 T phiTopRight);
}  // namespace CubbyFlow

#include <Core/Utils/LevelSetUtils-Impl.hpp>

#endif