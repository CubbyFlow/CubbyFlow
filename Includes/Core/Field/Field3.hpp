// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FIELD3_HPP
#define CUBBYFLOW_FIELD3_HPP

#include <memory>

namespace CubbyFlow
{
//! Abstract base class for 3-D fields.
class Field3
{
 public:
    //! Default constructor.
    Field3() = default;

    //! Default copy constructor.
    Field3(const Field3&) = default;

    //! Default move constructor.
    Field3(Field3&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~Field3() = default;

    //! Default copy assignment operator.
    Field3& operator=(const Field3&) = default;

    //! Default move assignment operator.
    Field3& operator=(Field3&&) noexcept = default;
};

using Field3Ptr = std::shared_ptr<Field3>;
}  // namespace CubbyFlow

#endif