// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CONSTANT_SCALAR_FIELD3_HPP
#define CUBBYFLOW_CONSTANT_SCALAR_FIELD3_HPP

#include <Core/Field/ScalarField3.hpp>

namespace CubbyFlow
{
//! 3-D constant scalar field.
class ConstantScalarField3 final : public ScalarField3
{
 public:
    class Builder;

    //! Constructs a constant scalar field with given \p value.
    explicit ConstantScalarField3(double value);

    //! Returns the sampled value at given position \p x.
    double Sample(const Vector3D& x) const override;

    //! Returns the sampler function.
    std::function<double(const Vector3D&)> Sampler() const override;

    //! Returns builder fox ConstantScalarField3.
    static Builder GetBuilder();

 private:
    double m_value = 0.0;
};

//! Shared pointer for the ConstantScalarField3 type.
using ConstantScalarField3Ptr = std::shared_ptr<ConstantScalarField3>;

//!
//! \brief Front-end to create ConstantScalarField3 objects step by step.
//!
class ConstantScalarField3::Builder final
{
 public:
    //! Returns builder with value.
    Builder& WithValue(double value);

    //! Builds ConstantScalarField3.
    ConstantScalarField3 Build() const;

    //! Builds shared pointer of ConstantScalarField3 instance.
    ConstantScalarField3Ptr MakeShared() const;

 private:
    double m_value = 0.0;
};
}  // namespace CubbyFlow

#endif