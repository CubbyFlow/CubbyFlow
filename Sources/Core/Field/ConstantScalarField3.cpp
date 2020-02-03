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
#include <Core/Field/ConstantScalarField3.hpp>

namespace CubbyFlow
{
ConstantScalarField3::ConstantScalarField3(double value) : m_value(value)
{
    // Do nothing
}

double ConstantScalarField3::Sample(const Vector3D& x) const
{
    UNUSED_VARIABLE(x);

    return m_value;
}

std::function<double(const Vector3D&)> ConstantScalarField3::Sampler() const
{
    double value = m_value;

    return [value](const Vector3D&) -> double { return value; };
}

ConstantScalarField3::Builder ConstantScalarField3::GetBuilder()
{
    return Builder();
}

ConstantScalarField3::Builder& ConstantScalarField3::Builder::WithValue(
    double value)
{
    m_value = value;
    return *this;
}

ConstantScalarField3 ConstantScalarField3::Builder::Build() const
{
    return ConstantScalarField3(m_value);
}

ConstantScalarField3Ptr ConstantScalarField3::Builder::MakeShared() const
{
    return std::shared_ptr<ConstantScalarField3>(
        new ConstantScalarField3(m_value),
        [](ConstantScalarField3* obj) { delete obj; });
}
}  // namespace CubbyFlow
