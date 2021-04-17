// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Field/VectorField.hpp>

namespace CubbyFlow
{
template <size_t N>
double VectorField<N>::Divergence(const Vector<double, N>&) const
{
    return 0.0;
}

template <size_t N>
typename GetCurl<N>::Type VectorField<N>::Curl(const Vector<double, N>&) const
{
    return typename GetCurl<N>::Type{};
}

template <size_t N>
std::function<Vector<double, N>(const Vector<double, N>&)>
VectorField<N>::Sampler() const
{
    const VectorField* self = this;
    return [self](const Vector<double, N>& x) -> Vector<double, N> {
        return self->Sample(x);
    };
}

template class VectorField<2>;

template class VectorField<3>;
}  // namespace CubbyFlow