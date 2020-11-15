// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_TRANSFORM2_HPP
#define CUBBYFLOW_TRANSFORM2_HPP

#include <Core/Geometry/BoundingBox2.hpp>
#include <Core/Geometry/Ray2.hpp>
#include <Core/Vector/Vector2.hpp>

namespace CubbyFlow
{
//!
//! \brief Represents 2-D rigid body transform.
//!
class Transform2
{
 public:
    //! Constructs identity transform.
    Transform2() = default;

    //! Constructs a transform with translation and orientation.
    Transform2(const Vector2D& translation, double orientation);

    //! Returns the translation.
    [[nodiscard]] const Vector2D& GetTranslation() const;

    //! Sets the translation.
    void SetTranslation(const Vector2D& translation);

    //! Returns the orientation in radians.
    [[nodiscard]] double GetOrientation() const;

    //! Sets the orientation in radians.
    void SetOrientation(double orientation);

    //! Transforms a point in world coordinate to the local frame.
    [[nodiscard]] Vector2D ToLocal(const Vector2D& pointInWorld) const;

    //! Transforms a direction in world coordinate to the local frame.
    [[nodiscard]] Vector2D ToLocalDirection(const Vector2D& dirInWorld) const;

    //! Transforms a ray in world coordinate to the local frame.
    [[nodiscard]] Ray2D ToLocal(const Ray2D& rayInWorld) const;

    //! Transforms a bounding box in world coordinate to the local frame.
    [[nodiscard]] BoundingBox2D ToLocal(const BoundingBox2D& bboxInWorld) const;

    //! Transforms a point in local space to the world coordinate.
    [[nodiscard]] Vector2D ToWorld(const Vector2D& pointInLocal) const;

    //! Transforms a direction in local space to the world coordinate.
    [[nodiscard]] Vector2D ToWorldDirection(const Vector2D& dirInLocal) const;

    //! Transforms a ray in local space to the world coordinate.
    [[nodiscard]] Ray2D ToWorld(const Ray2D& rayInLocal) const;

    //! Transforms a bounding box in local space to the world coordinate.
    [[nodiscard]] BoundingBox2D ToWorld(const BoundingBox2D& bboxInLocal) const;

 private:
    Vector2D m_translation;
    double m_orientation = 0.0;
    double m_cosAngle = 1.0;
    double m_sinAngle = 0.0;
};
}  // namespace CubbyFlow

#endif