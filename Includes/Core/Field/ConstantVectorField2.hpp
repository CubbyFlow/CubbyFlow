// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CONSTANT_VECTOR_FIELD2_HPP
#define CUBBYFLOW_CONSTANT_VECTOR_FIELD2_HPP

#include <Core/Field/VectorField.hpp>

namespace CubbyFlow
{
//! 2-D constant vector field.
class ConstantVectorField2 final : public VectorField2
{
 public:
    class Builder;

    //! Constructs a constant vector field with given \p value.
    explicit ConstantVectorField2(const Vector2D& value);

    //! Returns the sampled value at given position \p x.
    [[nodiscard]] Vector2D Sample(const Vector2D& x) const override;

    //! Returns the sampler function.
    [[nodiscard]] std::function<Vector2D(const Vector2D&)> Sampler()
        const override;

    //! Returns builder fox ConstantVectorField2.
    [[nodiscard]] static Builder GetBuilder();

 private:
    Vector2D m_value;
};

//! Shared pointer for the ConstantVectorField2 type.
using ConstantVectorField2Ptr = std::shared_ptr<ConstantVectorField2>;

//!
//! \brief Front-end to create ConstantVectorField2 objects step by step.
//!
class ConstantVectorField2::Builder final
{
 public:
    //! Returns builder with value.
    [[nodiscard]] Builder& WithValue(const Vector2D& value);

    //! Builds ConstantVectorField2.
    [[nodiscard]] ConstantVectorField2 Build() const;

    //! Builds shared pointer of ConstantVectorField2 instance.
    [[nodiscard]] ConstantVectorField2Ptr MakeShared() const;

 private:
    Vector2D m_value{ 0, 0 };
};
}  // namespace CubbyFlow

#endif