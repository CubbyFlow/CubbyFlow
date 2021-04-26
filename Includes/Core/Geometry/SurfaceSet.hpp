// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SURFACE_SET_HPP
#define CUBBYFLOW_SURFACE_SET_HPP

#include <Core/Geometry/BVH.hpp>
#include <Core/Geometry/Surface.hpp>

namespace CubbyFlow
{
//!
//! \brief N-D surface set.
//!
//! This class represents N-D surface set which extends Surface by overriding
//! surface-related queries. This is class can hold a collection of other
//! surface instances.
//!
template <size_t N>
class SurfaceSet final : public Surface<N>
{
 public:
    class Builder;

    //! Constructs an empty surface set.
    SurfaceSet() = default;

    //! Constructs with a list of other surfaces.
    explicit SurfaceSet(
        const ConstArrayView1<std::shared_ptr<Surface<N>>>& others,
        const Transform<N>& _transform = Transform<N>{},
        bool _isNormalFlipped = false);

    //! Default virtual destructor.
    ~SurfaceSet() override = default;

    //! Copy constructor.
    SurfaceSet(const SurfaceSet& other);

    //! Move constructor.
    SurfaceSet(SurfaceSet&& other) noexcept;

    //! Copy assignment operator.
    SurfaceSet& operator=(const SurfaceSet& other);

    //! Move assignment operator.
    SurfaceSet& operator=(SurfaceSet&& other) noexcept;

    //! Updates internal spatial query engine.
    void UpdateQueryEngine() override;

    //! Returns true if bounding box can be defined.
    [[nodiscard]] bool IsBounded() const override;

    //! Returns true if the surface is a valid geometry.
    [[nodiscard]] bool IsValidGeometry() const override;

    //! Returns the number of surfaces.
    [[nodiscard]] size_t NumberOfSurfaces() const;

    //! Returns the i-th surface.
    [[nodiscard]] const std::shared_ptr<Surface<N>>& SurfaceAt(size_t i) const;

    //! Adds a surface instance.
    void AddSurface(const std::shared_ptr<Surface<N>>& surface);

    //! Returns builder for SurfaceSet.
    static Builder GetBuilder();

 private:
    [[nodiscard]] Vector<double, N> ClosestPointLocal(
        const Vector<double, N>& otherPoint) const override;

    [[nodiscard]] BoundingBox<double, N> BoundingBoxLocal() const override;

    [[nodiscard]] SurfaceRayIntersection<N> ClosestIntersectionLocal(
        const Ray<double, N>& ray) const override;

    [[nodiscard]] Vector<double, N> ClosestNormalLocal(
        const Vector<double, N>& otherPoint) const override;

    [[nodiscard]] bool IntersectsLocal(
        const Ray<double, N>& ray) const override;

    [[nodiscard]] double ClosestDistanceLocal(
        const Vector<double, N>& otherPoint) const override;

    [[nodiscard]] bool IsInsideLocal(
        const Vector<double, N>& otherPoint) const override;

    void InvalidateBVH() const;

    void BuildBVH() const;

    Array1<std::shared_ptr<Surface<N>>> m_surfaces;
    Array1<std::shared_ptr<Surface<N>>> m_unboundedSurfaces;
    mutable BVH<std::shared_ptr<Surface<N>>, N> m_bvh;
    mutable bool m_bvhInvalidated = true;
};

//! 2-D SurfaceSet type.
using SurfaceSet2 = SurfaceSet<2>;

//! 3-D SurfaceSet type.
using SurfaceSet3 = SurfaceSet<3>;

//! Shared pointer for the SurfaceSet2 type.
using SurfaceSet2Ptr = std::shared_ptr<SurfaceSet2>;

//! Shared pointer for the SurfaceSet3 type.
using SurfaceSet3Ptr = std::shared_ptr<SurfaceSet3>;

//!
//! \brief Front-end to create SurfaceSet objects step by step.
//!
template <size_t N>
class SurfaceSet<N>::Builder final
    : public SurfaceBuilderBase<N, typename SurfaceSet<N>::Builder>
{
 public:
    //! Returns builder with other surfaces.
    Builder& WithSurfaces(
        const ConstArrayView1<std::shared_ptr<Surface<N>>>& others);

    //! Builds SurfaceSet.
    SurfaceSet Build() const;

    //! Builds shared pointer of SurfaceSet instance.
    std::shared_ptr<SurfaceSet<N>> MakeShared() const;

 private:
    using Base = SurfaceBuilderBase<N, typename SurfaceSet<N>::Builder>;
    using Base::m_isNormalFlipped;
    using Base::m_transform;

    Array1<std::shared_ptr<Surface<N>>> m_surfaces;
};
}  // namespace CubbyFlow

#endif