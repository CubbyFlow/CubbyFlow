// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Field/ConstantVectorField3.hpp>
#include <Core/Utils/Macros.hpp>

namespace CubbyFlow
{
ConstantVectorField3::ConstantVectorField3(const Vector3D& value)
    : m_value(value)
{
    // Do nothing
}

Vector3D ConstantVectorField3::Sample(const Vector3D& x) const
{
    UNUSED_VARIABLE(x);

    return m_value;
}

std::function<Vector3D(const Vector3D&)> ConstantVectorField3::Sampler() const
{
    return [this](const Vector3D&) -> Vector3D { return m_value; };
}

ConstantVectorField3::Builder ConstantVectorField3::GetBuilder()
{
    return Builder();
}

ConstantVectorField3::Builder& ConstantVectorField3::Builder::WithValue(
    const Vector3D& value)
{
    m_value = value;
    return *this;
}

ConstantVectorField3 ConstantVectorField3::Builder::Build() const
{
    return ConstantVectorField3(m_value);
}

ConstantVectorField3Ptr ConstantVectorField3::Builder::MakeShared() const
{
    return std::shared_ptr<ConstantVectorField3>(
        new ConstantVectorField3(m_value),
        [](ConstantVectorField3* obj) { delete obj; });
}
}  // namespace CubbyFlow
