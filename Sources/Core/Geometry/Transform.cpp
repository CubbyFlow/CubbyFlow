// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Geometry/Transform.hpp>

namespace CubbyFlow
{
Orientation<2>::Orientation() : Orientation{ 0.0 }
{
    // Do nothing
}

Orientation<2>::Orientation(double angleInRadian)
{
    SetRotation(angleInRadian);
}

double Orientation<2>::GetRotation() const
{
    return m_angle;
}

void Orientation<2>::SetRotation(double angleInRadian)
{
    m_angle = angleInRadian;
    m_cosAngle = std::cos(angleInRadian);
    m_sinAngle = std::sin(angleInRadian);
}

Vector2D Orientation<2>::ToLocal(const Vector2D& pointInWorld) const
{
    // Convert to the local frame
    return Vector2D{ m_cosAngle * pointInWorld.x + m_sinAngle * pointInWorld.y,
                     -m_sinAngle * pointInWorld.x +
                         m_cosAngle * pointInWorld.y };
}

Vector2D Orientation<2>::ToWorld(const Vector2D& pointInLocal) const
{
    // Convert to the world frame
    return Vector2D{ m_cosAngle * pointInLocal.x - m_sinAngle * pointInLocal.y,
                     m_sinAngle * pointInLocal.x +
                         m_cosAngle * pointInLocal.y };
}

Orientation<3>::Orientation() : Orientation{ QuaternionD{} }
{
    // Do nothing
}

Orientation<3>::Orientation(const QuaternionD& quat)
{
    SetRotation(quat);
}

const QuaternionD& Orientation<3>::GetRotation() const
{
    return m_quat;
}

void Orientation<3>::SetRotation(const QuaternionD& quat)
{
    m_quat = quat;
    m_rotationMat3 = quat.Matrix3();
    m_inverseRotationMat3 = quat.Inverse().Matrix3();
}

Vector3D Orientation<3>::ToLocal(const Vector3D& pointInWorld) const
{
    return m_inverseRotationMat3 * pointInWorld;
}

Vector3D Orientation<3>::ToWorld(const Vector3D& pointInLocal) const
{
    return m_rotationMat3 * pointInLocal;
}

template <size_t N>
Transform<N>::Transform(const Vector<double, N>& translation,
                        const Orientation<N>& orientation)
{
    SetTranslation(translation);
    SetOrientation(orientation);
}

template <size_t N>
const Vector<double, N>& Transform<N>::GetTranslation() const
{
    return m_translation;
}

template <size_t N>
void Transform<N>::SetTranslation(const Vector<double, N>& translation)
{
    m_translation = translation;
}

template <size_t N>
const Orientation<N>& Transform<N>::GetOrientation() const
{
    return m_orientation;
}

template <size_t N>
void Transform<N>::SetOrientation(const Orientation<N>& orientation)
{
    m_orientation = orientation;
}

template <size_t N>
Vector<double, N> Transform<N>::ToLocal(
    const Vector<double, N>& pointInWorld) const
{
    return m_orientation.ToLocal(pointInWorld - m_translation);
}

template <size_t N>
Vector<double, N> Transform<N>::ToLocalDirection(
    const Vector<double, N>& dirInWorld) const
{
    return m_orientation.ToLocal(dirInWorld);
}

template <size_t N>
Ray<double, N> Transform<N>::ToLocal(const Ray<double, N>& rayInWorld) const
{
    return Ray<double, N>{ ToLocal(rayInWorld.origin),
                           ToLocalDirection(rayInWorld.direction) };
}

template <size_t N>
BoundingBox<double, N> Transform<N>::ToLocal(
    const BoundingBox<double, N>& bboxInWorld) const
{
    BoundingBox<double, N> bboxInLocal;
    const int numCorners = 2 << N;

    for (int i = 0; i < numCorners; ++i)
    {
        Vector<double, N> cornerInLocal = ToLocal(bboxInWorld.Corner(i));

        bboxInLocal.lowerCorner = Min(bboxInLocal.lowerCorner, cornerInLocal);
        bboxInLocal.upperCorner = Max(bboxInLocal.upperCorner, cornerInLocal);
    }

    return bboxInLocal;
}

template <size_t N>
Vector<double, N> Transform<N>::ToWorld(
    const Vector<double, N>& pointInLocal) const
{
    return m_orientation.ToWorld(pointInLocal) + m_translation;
}

template <size_t N>
Vector<double, N> Transform<N>::ToWorldDirection(
    const Vector<double, N>& dirInLocal) const
{
    return m_orientation.ToWorld(dirInLocal);
}

template <size_t N>
Ray<double, N> Transform<N>::ToWorld(const Ray<double, N>& rayInLocal) const
{
    return Ray<double, N>{ ToWorld(rayInLocal.origin),
                           ToWorldDirection(rayInLocal.direction) };
}

template <size_t N>
BoundingBox<double, N> Transform<N>::ToWorld(
    const BoundingBox<double, N>& bboxInLocal) const
{
    BoundingBox<double, N> bboxInWorld;
    const int numCorners = 2 << N;

    for (int i = 0; i < numCorners; ++i)
    {
        Vector<double, N> cornerInWorld = ToWorld(bboxInLocal.Corner(i));

        bboxInWorld.lowerCorner = Min(bboxInWorld.lowerCorner, cornerInWorld);
        bboxInWorld.upperCorner = Max(bboxInWorld.upperCorner, cornerInWorld);
    }

    return bboxInWorld;
}

template class Transform<2>;
template class Transform<3>;
}  // namespace CubbyFlow