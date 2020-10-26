// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SURFACE_SET2_HPP
#define CUBBYFLOW_SURFACE_SET2_HPP

#include <Core/Geometry/BVH2.hpp>
#include <Core/Surface/Surface2.hpp>

#include <vector>

namespace CubbyFlow
{
//!
//! \brief 2-D surface set.
//!
//! This class represents 2-D surface set which extends Surface2 by overriding
//! surface-related queries. This is class can hold a collection of other
//! surface instances.
//!
class SurfaceSet2 final : public Surface2
{
 public:
    class Builder;

    //! Constructs an empty surface set.
    SurfaceSet2() = default;

    //! Constructs with a list of other surfaces.
    explicit SurfaceSet2(std::vector<Surface2Ptr> others,
                         const Transform2& _transform = Transform2{},
                         bool _isNormalFlipped = false);

    //! Copy constructor.
    SurfaceSet2(const SurfaceSet2& other);

    //! Move constructor.
    SurfaceSet2(SurfaceSet2&& other) noexcept;

    //! Default virtual destructor.
    ~SurfaceSet2() override = default;

    //! Copy assignment operator.
    SurfaceSet2& operator=(const SurfaceSet2& other);

    //! Default move assignment operator.
    SurfaceSet2& operator=(SurfaceSet2&&) = default;

    //! Updates internal spatial query engine.
    void UpdateQueryEngine() override;

    //! Returns true if bounding box can be defined.
    [[nodiscard]] bool IsBounded() const override;

    //! Returns true if the surface is a valid geometry.
    [[nodiscard]] bool IsValidGeometry() const override;

    //! Returns the number of surfaces.
    [[nodiscard]] size_t NumberOfSurfaces() const;

    //! Returns the i-th surface.
    [[nodiscard]] const Surface2Ptr& SurfaceAt(size_t i) const;

    //! Adds a surface instance.
    void AddSurface(const Surface2Ptr& surface);

    //! Returns builder for SurfaceSet2.
    [[nodiscard]] static Builder GetBuilder();

 private:
    // Surface2 implementations
    [[nodiscard]] Vector2D ClosestPointLocal(
        const Vector2D& otherPoint) const override;

    [[nodiscard]] BoundingBox2D BoundingBoxLocal() const override;

    [[nodiscard]] double ClosestDistanceLocal(
        const Vector2D& otherPoint) const override;

    [[nodiscard]] bool IntersectsLocal(const Ray2D& ray) const override;

    [[nodiscard]] Vector2D ClosestNormalLocal(
        const Vector2D& otherPoint) const override;

    [[nodiscard]] SurfaceRayIntersection2 ClosestIntersectionLocal(
        const Ray2D& ray) const override;

    [[nodiscard]] bool IsInsideLocal(const Vector2D& otherPoint) const override;

    void InvalidateBVH() const;

    void BuildBVH() const;

    std::vector<Surface2Ptr> m_surfaces;
    std::vector<Surface2Ptr> m_unboundedSurfaces;
    mutable BVH2<Surface2Ptr> m_bvh;
    mutable bool m_bvhInvalidated = true;
};

//! Shared pointer for the SurfaceSet2 type.
using SurfaceSet2Ptr = std::shared_ptr<SurfaceSet2>;

//!
//! \brief Front-end to create SurfaceSet2 objects step by step.
//!
class SurfaceSet2::Builder final : public SurfaceBuilderBase2<Builder>
{
 public:
    //! Returns builder with other surfaces.
    [[nodiscard]] Builder& WithSurfaces(const std::vector<Surface2Ptr>& others);

    //! Builds SurfaceSet2.
    [[nodiscard]] SurfaceSet2 Build() const;

    //! Builds shared pointer of SurfaceSet2 instance.
    [[nodiscard]] SurfaceSet2Ptr MakeShared() const;

 private:
    std::vector<Surface2Ptr> m_surfaces;
};
}  // namespace CubbyFlow

#endif