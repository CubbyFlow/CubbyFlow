// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CONSTANT_SCALAR_FIELD2_HPP
#define CUBBYFLOW_CONSTANT_SCALAR_FIELD2_HPP

#include <Core/Field/ScalarField2.hpp>

namespace CubbyFlow
{
//! 2-D constant scalar field.
class ConstantScalarField2 final : public ScalarField2
{
 public:
    class Builder;

    //! Constructs a constant scalar field with given \p value.
    explicit ConstantScalarField2(double value);

    //! Returns the sampled value at given position \p x.
    double Sample(const Vector2D& x) const override;

    //! Returns the sampler function.
    std::function<double(const Vector2D&)> Sampler() const override;

    //! Returns builder fox ConstantScalarField2.
    static Builder GetBuilder();

 private:
    double m_value = 0.0;
};

//! Shared pointer for the ConstantScalarField2 type.
using ConstantScalarField2Ptr = std::shared_ptr<ConstantScalarField2>;

//!
//! \brief Front-end to create ConstantScalarField2 objects step by step.
//!
class ConstantScalarField2::Builder final
{
 public:
    //! Returns builder with value.
    Builder& WithValue(double value);

    //! Builds ConstantScalarField2.
    ConstantScalarField2 Build() const;

    //! Builds shared pointer of ConstantScalarField2 instance.
    ConstantScalarField2Ptr MakeShared() const;

 private:
    double m_value = 0.0;
};
}  // namespace CubbyFlow

#endif