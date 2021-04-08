// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CONSTANT_VECTOR_FIELD3_HPP
#define CUBBYFLOW_CONSTANT_VECTOR_FIELD3_HPP

#include <Core/Field/VectorField.hpp>

namespace CubbyFlow
{
//! 3-D constant vector field.
class ConstantVectorField3 final : public VectorField3
{
 public:
    class Builder;

    //! Constructs a constant vector field with given \p value.
    explicit ConstantVectorField3(const Vector3D& value);

    //! Returns the sampled value at given position \p x.
    [[nodiscard]] Vector3D Sample(const Vector3D& x) const override;

    //! Returns the sampler function.
    [[nodiscard]] std::function<Vector3D(const Vector3D&)> Sampler()
        const override;

    //! Returns builder fox ConstantVectorField3.
    [[nodiscard]] static Builder GetBuilder();

 private:
    Vector3D m_value;
};

//! Shared pointer for the ConstantVectorField3 type.
using ConstantVectorField3Ptr = std::shared_ptr<ConstantVectorField3>;

//!
//! \brief Front-end to create ConstantVectorField3 objects step by step.
//!
class ConstantVectorField3::Builder final
{
 public:
    //! Returns builder with value.
    [[nodiscard]] Builder& WithValue(const Vector3D& value);

    //! Builds ConstantVectorField3.
    [[nodiscard]] ConstantVectorField3 Build() const;

    //! Builds shared pointer of ConstantVectorField3 instance.
    [[nodiscard]] ConstantVectorField3Ptr MakeShared() const;

 private:
    Vector3D m_value{ 0, 0, 0 };
};
}  // namespace CubbyFlow

#endif