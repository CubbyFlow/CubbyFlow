// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Field/ConstantVectorField2.hpp>
#include <Core/Utils/Macros.hpp>

namespace CubbyFlow
{
ConstantVectorField2::ConstantVectorField2(const Vector2D& value)
    : m_value(value)
{
    // Do nothing
}

Vector2D ConstantVectorField2::Sample(const Vector2D& x) const
{
    UNUSED_VARIABLE(x);

    return m_value;
}

std::function<Vector2D(const Vector2D&)> ConstantVectorField2::Sampler() const
{
    return [this](const Vector2D&) -> Vector2D { return m_value; };
}

ConstantVectorField2::Builder ConstantVectorField2::GetBuilder()
{
    return Builder{};
}

ConstantVectorField2::Builder& ConstantVectorField2::Builder::WithValue(
    const Vector2D& value)
{
    m_value = value;
    return *this;
}

ConstantVectorField2 ConstantVectorField2::Builder::Build() const
{
    return ConstantVectorField2{ m_value };
}

ConstantVectorField2Ptr ConstantVectorField2::Builder::MakeShared() const
{
    return std::shared_ptr<ConstantVectorField2>(
        new ConstantVectorField2{ m_value },
        [](ConstantVectorField2* obj) { delete obj; });
}
}  // namespace CubbyFlow
