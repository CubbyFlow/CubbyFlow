// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_TRIANGLE3_HPP
#define CUBBYFLOW_TRIANGLE3_HPP

#include <Core/Surface/Surface3.hpp>

namespace CubbyFlow
{
//!
//! \brief 3-D triangle geometry.
//!
//! This class represents 3-D triangle geometry which extends Surface3 by
//! overriding surface-related queries.
//!
class Triangle3 final : public Surface3
{
 public:
    class Builder;

    //! Three points.
    std::array<Vector3D, 3> points;

    //! Three normals.
    std::array<Vector3D, 3> normals;

    //! Three UV coordinates.
    std::array<Vector2D, 3> uvs;

    //! Constructs an empty triangle.
    Triangle3(const Transform3& transform = Transform3(),
              bool isNormalFlipped = false);

    //! Constructs a triangle with given \p points, \p normals, and \p uvs.
    Triangle3(const std::array<Vector3D, 3>& points,
              const std::array<Vector3D, 3>& normals,
              const std::array<Vector2D, 3>& uvs,
              const Transform3& transform = Transform3(),
              bool isNormalFlipped = false);

    //! Copy constructor
    Triangle3(const Triangle3& other);

    //! Returns the area of this triangle.
    double Area() const;

    //! Returns barycentric coordinates for the given point \p pt.
    void GetBarycentricCoords(const Vector3D& pt, double* b0, double* b1,
                              double* b2) const;

    //! Returns the face normal of the triangle.
    Vector3D FaceNormal() const;

    //! Set Triangle3::normals to the face normal.
    void SetNormalsToFaceNormal();

    //! Returns builder fox Triangle3.
    static Builder GetBuilder();

 protected:
    Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

    bool IntersectsLocal(const Ray3D& ray) const override;

    BoundingBox3D BoundingBoxLocal() const override;

    Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const override;

    SurfaceRayIntersection3 ClosestIntersectionLocal(
        const Ray3D& ray) const override;
};

//! Shared pointer for the Triangle3 type.
using Triangle3Ptr = std::shared_ptr<Triangle3>;

//!
//! \brief Front-end to create Triangle3 objects step by step.
//!
class Triangle3::Builder final : public SurfaceBuilderBase3<Triangle3::Builder>
{
 public:
    //! Returns builder with points.
    Builder& WithPoints(const std::array<Vector3D, 3>& points);

    //! Returns builder with normals.
    Builder& WithNormals(const std::array<Vector3D, 3>& normals);

    //! Returns builder with uvs.
    Builder& WithUVs(const std::array<Vector2D, 3>& uvs);

    //! Builds Triangle3.
    Triangle3 Build() const;

    //! Builds shared pointer of Triangle3 instance.
    Triangle3Ptr MakeShared() const;

 private:
    std::array<Vector3D, 3> m_points;
    std::array<Vector3D, 3> m_normals;
    std::array<Vector2D, 3> m_uvs;
};
}  // namespace CubbyFlow

#endif