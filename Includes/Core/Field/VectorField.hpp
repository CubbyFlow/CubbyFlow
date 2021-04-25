// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_VECTOR_FIELD_HPP
#define CUBBYFLOW_VECTOR_FIELD_HPP

#include <Core/Field/Field.hpp>
#include <Core/Matrix/Matrix.hpp>

#include <functional>
#include <memory>

namespace CubbyFlow
{
template <size_t N>
struct GetCurl
{
    // Do nothing
};

template <>
struct GetCurl<2>
{
    using Type = double;
};

template <>
struct GetCurl<3>
{
    using Type = Vector3D;
};

//! Abstract base class for N-D vector field.
template <size_t N>
class VectorField : public Field<N>
{
 public:
    //! Default constructor.
    VectorField() = default;

    //! Default destructor.
    ~VectorField() override = default;

    //! Default copy constructor.
    VectorField(const VectorField&) = default;

    //! Default move constructor.
    VectorField(VectorField&&) noexcept = default;

    //! Default copy assignment operator.
    VectorField& operator=(const VectorField&) = default;

    //! Default move assignment operator.
    VectorField& operator=(VectorField&&) noexcept = default;

    //! Returns sampled value at given position \p x.
    [[nodiscard]] virtual Vector<double, N> Sample(
        const Vector<double, N>& x) const = 0;

    //! Returns divergence at given position \p x.
    [[nodiscard]] virtual double Divergence(const Vector<double, N>& x) const;

    //! Returns curl at given position \p x.
    [[nodiscard]] virtual typename GetCurl<N>::Type Curl(
        const Vector<double, N>& x) const;

    //! Returns sampler function object.
    [[nodiscard]] virtual std::function<
        Vector<double, N>(const Vector<double, N>&)>
    Sampler() const;
};

//! 2-D VectorField type.
using VectorField2 = VectorField<2>;

//! 3-D VectorField type.
using VectorField3 = VectorField<3>;

//! Shared pointer for the VectorField2 type.
using VectorField2Ptr = std::shared_ptr<VectorField2>;

//! Shared pointer for the VectorField3 type.
using VectorField3Ptr = std::shared_ptr<VectorField3>;
}  // namespace CubbyFlow

#endif