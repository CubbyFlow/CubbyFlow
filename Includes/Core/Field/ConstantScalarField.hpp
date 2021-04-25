// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CONSTANT_SCALAR_FIELD_HPP
#define CUBBYFLOW_CONSTANT_SCALAR_FIELD_HPP

#include <Core/Field/ScalarField.hpp>

namespace CubbyFlow
{
//! N-D constant scalar field.
template <size_t N>
class ConstantScalarField final : public ScalarField<N>
{
 public:
    class Builder;

    //! Constructs a constant scalar field with given \p value.
    explicit ConstantScalarField(double value);

    //! Returns the sampled value at given position \p x.
    [[nodiscard]] double Sample(const Vector<double, N>& x) const override;

    //! Returns the sampler function.
    [[nodiscard]] std::function<double(const Vector<double, N>&)> Sampler()
        const override;

    //! Returns builder for ConstantScalarField.
    static Builder GetBuilder();

 private:
    double m_value = 0.0;
};

//! 2-D ConstantScalarField type.
using ConstantScalarField2 = ConstantScalarField<2>;

//! 3-D ConstantScalarField type.
using ConstantScalarField3 = ConstantScalarField<3>;

//! Shared pointer for the ConstantScalarField2 type.
using ConstantScalarField2Ptr = std::shared_ptr<ConstantScalarField2>;

//! Shared pointer for the ConstantScalarField3 type.
using ConstantScalarField3Ptr = std::shared_ptr<ConstantScalarField3>;

//!
//! \brief Front-end to create ConstantScalarField objects step by step.
//!
template <size_t N>
class ConstantScalarField<N>::Builder final
{
 public:
    //! Returns builder with value.
    Builder& WithValue(double value);

    //! Builds ConstantScalarField.
    ConstantScalarField Build() const;

    //! Builds shared pointer of ConstantScalarField instance.
    std::shared_ptr<ConstantScalarField> MakeShared() const;

 private:
    double m_value = 0.0;
};
}  // namespace CubbyFlow

#endif