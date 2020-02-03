// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Utils/Macros.h>
#include <Core/Field/ConstantScalarField2.hpp>

namespace CubbyFlow
{
ConstantScalarField2::ConstantScalarField2(double value) : m_value(value)
{
    // Do nothing
}

double ConstantScalarField2::Sample(const Vector2D& x) const
{
    UNUSED_VARIABLE(x);

    return m_value;
}

std::function<double(const Vector2D&)> ConstantScalarField2::Sampler() const
{
    double value = m_value;

    return [value](const Vector2D&) -> double { return value; };
}

ConstantScalarField2::Builder ConstantScalarField2::GetBuilder()
{
    return Builder();
}

ConstantScalarField2::Builder& ConstantScalarField2::Builder::WithValue(
    double value)
{
    m_value = value;
    return *this;
}

ConstantScalarField2 ConstantScalarField2::Builder::Build() const
{
    return ConstantScalarField2(m_value);
}

ConstantScalarField2Ptr ConstantScalarField2::Builder::MakeShared() const
{
    return std::shared_ptr<ConstantScalarField2>(
        new ConstantScalarField2(m_value),
        [](ConstantScalarField2* obj) { delete obj; });
}
}  // namespace CubbyFlow
