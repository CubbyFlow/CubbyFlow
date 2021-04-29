// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CONSTANT_VECTOR_FIELD_HPP
#define CUBBYFLOW_CONSTANT_VECTOR_FIELD_HPP

#include <Core/Field/VectorField.hpp>

namespace CubbyFlow
{
//! N-D constant vector field.
template <size_t N>
class ConstantVectorField final : public VectorField<N>
{
 public:
    class Builder;

    //! Constructs a constant vector field with given \p value.
    explicit ConstantVectorField(const Vector<double, N>& value);

    //! Returns the sampled value at given position \p x.
    [[nodiscard]] Vector<double, N> Sample(
        const Vector<double, N>& x) const override;

    //! Returns the sampler function.
    [[nodiscard]] std::function<Vector<double, N>(const Vector<double, N>&)>
    Sampler() const override;

    //! Returns builder for ConstantVectorField.
    static Builder GetBuilder();

 private:
    Vector<double, N> m_value;
};

//! 2-D ConstantVectorField type.
using ConstantVectorField2 = ConstantVectorField<2>;

//! 3-D ConstantVectorField type.
using ConstantVectorField3 = ConstantVectorField<3>;

//! Shared pointer for the ConstantVectorField2 type.
using ConstantVectorField2Ptr = std::shared_ptr<ConstantVectorField2>;

//! Shared pointer for the ConstantVectorField3 type.
using ConstantVectorField3Ptr = std::shared_ptr<ConstantVectorField3>;

//!
//! \brief Front-end to create ConstantVectorField objects step by step.
//!
template <size_t N>
class ConstantVectorField<N>::Builder final
{
 public:
    //! Returns builder with value.
    Builder& WithValue(const Vector<double, N>& value);

    //! Builds ConstantVectorField.
    ConstantVectorField Build() const;

    //! Builds shared pointer of ConstantVectorField instance.
    std::shared_ptr<ConstantVectorField> MakeShared() const;

 private:
    Vector<double, N> m_value;
};
}  // namespace CubbyFlow

#endif