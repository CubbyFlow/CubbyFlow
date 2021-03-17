// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_BOX3_HPP
#define CUBBYFLOW_BOX3_HPP

#include <Core/Geometry/Surface.hpp>

namespace CubbyFlow
{
//!
//! \brief 3-D box geometry.
//!
//! This class represents 3-D box geometry which extends Surface3 by overriding
//! surface-related queries. This box implementation is an axis-aligned box
//! that wraps lower-level primitive type, BoundingBox3D.
//!
class Box3 final : public Surface3
{
 public:
    class Builder;

    //! Constructs (0, 0, 0) x (1, 1, 1) box.
    Box3(const Transform3& transform = Transform3{},
         bool isNormalFlipped = false);

    //! Constructs a box with given \p lowerCorner and \p upperCorner.
    Box3(const Vector3D& lowerCorner, const Vector3D& upperCorner,
         const Transform3& transform = Transform3{},
         bool isNormalFlipped = false);

    //! Constructs a box with BoundingBox3D instance.
    explicit Box3(BoundingBox3D boundingBox,
                  const Transform3& transform = Transform3{},
                  bool isNormalFlipped = false);

    //! Default copy constructor.
    Box3(const Box3& other) = default;

    //! Default move constructor.
    Box3(Box3&& other) noexcept = default;

    //! Default virtual destructor.
    ~Box3() override = default;

    //! Default copy assignment operator.
    Box3& operator=(const Box3& other) = default;

    //! Default move assignment operator.
    Box3& operator=(Box3&& other) noexcept = default;

    //! Returns builder fox Box3.
    [[nodiscard]] static Builder GetBuilder();

    //! Bounding box of this box.
    BoundingBox3D bound =
        BoundingBox3D{ Vector3D{}, Vector3D{ 1.0, 1.0, 1.0 } };

 protected:
    // Surface3 implementations
    [[nodiscard]] Vector3D ClosestPointLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] bool IntersectsLocal(const Ray3D& ray) const override;

    [[nodiscard]] BoundingBox3D BoundingBoxLocal() const override;

    [[nodiscard]] Vector3D ClosestNormalLocal(
        const Vector3D& otherPoint) const override;

    [[nodiscard]] SurfaceRayIntersection3 ClosestIntersectionLocal(
        const Ray3D& ray) const override;
};

//! Shared pointer type for the Box3.
using Box3Ptr = std::shared_ptr<Box3>;

//!
//! \brief Front-end to create Box3 objects step by step.
//!
class Box3::Builder final : public SurfaceBuilderBase3<Builder>
{
 public:
    //! Returns builder with lower corner set.
    [[nodiscard]] Builder& WithLowerCorner(const Vector3D& pt);

    //! Returns builder with upper corner set.
    [[nodiscard]] Builder& WithUpperCorner(const Vector3D& pt);

    //! Returns builder with bounding box.
    [[nodiscard]] Builder& WithBoundingBox(const BoundingBox3D& bbox);

    //! Builds Box3.
    [[nodiscard]] Box3 Build() const;

    //! Builds shared pointer of Box3 instance.
    [[nodiscard]] Box3Ptr MakeShared() const;

 private:
    Vector3D m_lowerCorner{ 0, 0, 0 };
    Vector3D m_upperCorner{ 1, 1, 1 };
};
}  // namespace CubbyFlow

#endif