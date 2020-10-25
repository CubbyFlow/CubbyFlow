// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VECTOR_FIELD3_HPP
#define CUBBYFLOW_VECTOR_FIELD3_HPP

#include <Core/Field/Field3.hpp>
#include <Core/Vector/Vector3.hpp>

#include <functional>
#include <memory>

namespace CubbyFlow
{
//! Abstract base class for 3-D vector field.
class VectorField3 : public Field3
{
 public:
    //! Default constructor.
    VectorField3() = default;

    //! Default copy constructor.
    VectorField3(const VectorField3&) = default;

    //! Default move constructor.
    VectorField3(VectorField3&&) noexcept = default;

    //! Default virtual destructor.
    virtual ~VectorField3() = default;

    //! Default copy assignment operator.
    VectorField3& operator=(const VectorField3&) = default;

    //! Default move assignment operator.
    VectorField3& operator=(VectorField3&&) noexcept = default;

    //! Returns sampled value at given position \p x.
    [[nodiscard]] virtual Vector3D Sample(const Vector3D& x) const = 0;

    //! Returns divergence at given position \p x.
    [[nodiscard]] virtual double Divergence(const Vector3D& x) const;

    //! Returns curl at given position \p x.
    [[nodiscard]] virtual Vector3D Curl(const Vector3D& x) const;

    //! Returns sampler function object.
    [[nodiscard]] virtual std::function<Vector3D(const Vector3D&)> Sampler()
        const;
};

//! Shared pointer for the VectorField3 type.
using VectorField3Ptr = std::shared_ptr<VectorField3>;
}  // namespace CubbyFlow

#endif