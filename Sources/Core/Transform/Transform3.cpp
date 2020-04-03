// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Transform/Transform3.hpp>

namespace CubbyFlow
{
Transform3::Transform3()
{
    // Do nothing
}

Transform3::Transform3(const Vector3D& translation,
                       const QuaternionD& orientation)
{
    SetTranslation(translation);
    SetOrientation(orientation);
}

const Vector3D& Transform3::GetTranslation() const
{
    return m_translation;
}

void Transform3::SetTranslation(const Vector3D& translation)
{
    m_translation = translation;
}

const QuaternionD& Transform3::GetOrientation() const
{
    return m_orientation;
}

void Transform3::SetOrientation(const QuaternionD& orientation)
{
    m_orientation = orientation;
    m_orientationMat3 = orientation.Matrix3();
    m_inverseOrientationMat3 = orientation.Inverse().Matrix3();
}

Vector3D Transform3::ToLocal(const Vector3D& pointInWorld) const
{
    return m_inverseOrientationMat3 * (pointInWorld - m_translation);
}

Vector3D Transform3::ToLocalDirection(const Vector3D& dirInWorld) const
{
    return m_inverseOrientationMat3 * dirInWorld;
}

Ray3D Transform3::ToLocal(const Ray3D& rayInWorld) const
{
    return Ray3D(ToLocal(rayInWorld.origin),
                 ToLocalDirection(rayInWorld.direction));
}

BoundingBox3D Transform3::ToLocal(const BoundingBox3D& bboxInWorld) const
{
    BoundingBox3D bboxInLocal;

    for (size_t i = 0; i < 8; ++i)
    {
        auto cornerInLocal = ToLocal(bboxInWorld.Corner(i));

        bboxInLocal.lowerCorner = Min(bboxInLocal.lowerCorner, cornerInLocal);
        bboxInLocal.upperCorner = Max(bboxInLocal.upperCorner, cornerInLocal);
    }

    return bboxInLocal;
}

Vector3D Transform3::ToWorld(const Vector3D& pointInLocal) const
{
    return (m_orientationMat3 * pointInLocal) + m_translation;
}

Vector3D Transform3::ToWorldDirection(const Vector3D& dirInLocal) const
{
    return m_orientationMat3 * dirInLocal;
}

Ray3D Transform3::ToWorld(const Ray3D& rayInLocal) const
{
    return Ray3D(ToWorld(rayInLocal.origin),
                 ToWorldDirection(rayInLocal.direction));
}

BoundingBox3D Transform3::ToWorld(const BoundingBox3D& bboxInLocal) const
{
    BoundingBox3D bboxInWorld;

    for (size_t i = 0; i < 8; ++i)
    {
        auto cornerInWorld = ToWorld(bboxInLocal.Corner(i));
        bboxInWorld.lowerCorner = Min(bboxInWorld.lowerCorner, cornerInWorld);
        bboxInWorld.upperCorner = Max(bboxInWorld.upperCorner, cornerInWorld);
    }

    return bboxInWorld;
}
}  // namespace CubbyFlow