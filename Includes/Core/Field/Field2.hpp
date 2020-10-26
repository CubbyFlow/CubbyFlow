// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FIELD2_HPP
#define CUBBYFLOW_FIELD2_HPP

#include <memory>

namespace CubbyFlow
{
//! Abstract base class for 2-D fields.
class Field2
{
 public:
    //! Default constructor.
    Field2() = default;

    //! Default copy constructor.
    Field2(const Field2&) = default;

    //! Default move constructor.
    Field2(Field2&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~Field2() = default;

    //! Default copy assignment operator.
    Field2& operator=(const Field2&) = default;

    //! Default move assignment operator.
    Field2& operator=(Field2&&) noexcept = default;
};

using Field2Ptr = std::shared_ptr<Field2>;
}  // namespace CubbyFlow

#endif