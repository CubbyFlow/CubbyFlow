// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Field/ScalarField.hpp>

namespace CubbyFlow
{
template <size_t N>
Vector<double, N> ScalarField<N>::Gradient(const Vector<double, N>&) const
{
    return Vector<double, N>{};
}

template <size_t N>
double ScalarField<N>::Laplacian(const Vector<double, N>&) const
{
    return 0.0;
}

template <size_t N>
std::function<double(const Vector<double, N>&)> ScalarField<N>::Sampler() const
{
    const ScalarField* self = this;
    return [self](const Vector<double, N>& x) -> double {
        return self->Sample(x);
    };
}

template class ScalarField<2>;

template class ScalarField<3>;
}  // namespace CubbyFlow