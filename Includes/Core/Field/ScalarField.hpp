// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SCALAR_FIELD_HPP
#define CUBBYFLOW_SCALAR_FIELD_HPP

#include <Core/Field/Field.hpp>
#include <Core/Matrix/Matrix.hpp>

#include <functional>
#include <memory>

namespace CubbyFlow
{
//! Abstract base class for N-D scalar field.
template <size_t N>
class ScalarField : public Field<N>
{
 public:
    //! Default constructor.
    ScalarField() = default;

    //! Default destructor.
    ~ScalarField() override = default;

    //! Default copy constructor.
    ScalarField(const ScalarField&) = default;

    //! Default move constructor.
    ScalarField(ScalarField&&) noexcept = default;

    //! Default copy assignment operator.
    ScalarField& operator=(const ScalarField&) = default;

    //! Default move assignment operator.
    ScalarField& operator=(ScalarField&&) noexcept = default;

    //! Returns sampled value at given position \p x.
    [[nodiscard]] virtual double Sample(const Vector<double, N>& x) const = 0;

    //! Returns gradient vector at given position \p x.
    [[nodiscard]] virtual Vector<double, N> Gradient(
        const Vector<double, N>& x) const;

    //! Returns Laplacian at given position \p x.
    [[nodiscard]] virtual double Laplacian(const Vector<double, N>& x) const;

    //! Returns sampler function object.
    [[nodiscard]] virtual std::function<double(const Vector<double, N>&)>
    Sampler() const;
};

//! 2-D ScalarField type.
using ScalarField2 = ScalarField<2>;

//! 3-D ScalarField type.
using ScalarField3 = ScalarField<3>;

//! Shared pointer for the ScalarField2 type.
using ScalarField2Ptr = std::shared_ptr<ScalarField2>;

//! Shared pointer for the ScalarField3 type.
using ScalarField3Ptr = std::shared_ptr<ScalarField3>;
}  // namespace CubbyFlow

#endif