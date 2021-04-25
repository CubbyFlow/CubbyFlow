// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FIELD_HPP
#define CUBBYFLOW_FIELD_HPP

#include <memory>

namespace CubbyFlow
{
//! Abstract base class for N-D fields.
template <size_t N>
class Field
{
 public:
    //! Default constructor.
    Field() = default;

    //! Default virtual destructor.
    virtual ~Field() = default;

    //! Default copy constructor.
    Field(const Field&) = default;

    //! Default move constructor.
    Field(Field&&) noexcept = default;

    //! Default copy assignment operator.
    Field& operator=(const Field&) = default;

    //! Default move assignment operator.
    Field& operator=(Field&&) noexcept = default;
};

//! 2-D Field type.
using Field2 = Field<2>;

//! 3-D Field type.
using Field3 = Field<3>;

//! Shared pointer type for Field2.
using Field2Ptr = std::shared_ptr<Field2>;

//! Shared pointer type for Field3.
using Field3Ptr = std::shared_ptr<Field3>;
}  // namespace CubbyFlow

#endif