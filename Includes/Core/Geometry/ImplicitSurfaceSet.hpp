// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_IMPLICIT_SURFACE_SET_HPP
#define CUBBYFLOW_IMPLICIT_SURFACE_SET_HPP

#include <Core/Array/Array.hpp>
#include <Core/Geometry/BVH.hpp>
#include <Core/Geometry/ImplicitSurface.hpp>

namespace CubbyFlow
{
//!
//! \brief N-D implicit surface set.
//!
//! This class represents N-D implicit surface set which extends
//! ImplicitSurface by overriding implicit surface-related queries. This is
//! class can hold a collection of other implicit surface instances.
//!
template <size_t N>
class ImplicitSurfaceSet final : public ImplicitSurface<N>
{
 public:
    class Builder;

    using ImplicitSurface<N>::transform;
    using ImplicitSurface<N>::isNormalFlipped;

    //! Constructs an empty implicit surface set.
    ImplicitSurfaceSet() = default;

    //! Constructs an implicit surface set using list of other surfaces.
    ImplicitSurfaceSet(
        ConstArrayView1<std::shared_ptr<ImplicitSurface<N>>> surfaces,
        const Transform<N>& _transform = Transform<N>{},
        bool _isNormalFlipped = false);

    //! Constructs an implicit surface set using list of other surfaces.
    ImplicitSurfaceSet(ConstArrayView1<std::shared_ptr<Surface<N>>> surfaces,
                       const Transform<N>& _transform = Transform<N>{},
                       bool _isNormalFlipped = false);

    //! Copy constructor.
    ImplicitSurfaceSet(const ImplicitSurfaceSet& other);

    //! Updates internal spatial query engine.
    void UpdateQueryEngine() override;

    bool IsBounded() const override;

    //! Returns true if the surface is a valid geometry.
    bool IsValidGeometry() const override;

    //! Returns the number of implicit surfaces.
    size_t NumberOfSurfaces() const;

    //! Returns the i-th implicit surface.
    const std::shared_ptr<ImplicitSurface<N>>& SurfaceAt(size_t i) const;

    //! Adds an explicit surface instance.
    void AddExplicitSurface(const std::shared_ptr<Surface<N>>& surface);

    //! Adds an implicit surface instance.
    void AddSurface(const std::shared_ptr<ImplicitSurface<N>>& surface);

    //! Returns builder fox ImplicitSurfaceSet.
    static Builder GetBuilder();

 private:
    Vector<double, N> ClosestPointLocal(
        const Vector<double, N>& otherPoint) const override;

    BoundingBox<double, N> BoundingBoxLocal() const override;

    bool IsInsideLocal(const Vector<double, N>& otherPoint) const override;

    double ClosestDistanceLocal(
        const Vector<double, N>& otherPoint) const override;

    bool IntersectsLocal(const Ray<double, N>& ray) const override;

    Vector<double, N> ClosestNormalLocal(
        const Vector<double, N>& otherPoint) const override;

    SurfaceRayIntersection<N> ClosestIntersectionLocal(
        const Ray<double, N>& ray) const override;

    double SignedDistanceLocal(
        const Vector<double, N>& otherPoint) const override;

    void InvalidateBVH() const;

    void BuildBVH() const;

    Array1<std::shared_ptr<ImplicitSurface<N>>> m_surfaces;
    Array1<std::shared_ptr<ImplicitSurface<N>>> m_unboundedSurfaces;
    mutable BVH<std::shared_ptr<ImplicitSurface<N>>, N> m_bvh;
    mutable bool m_bvhInvalidated = true;
};

//! 2-D ImplicitSurfaceSet type.
using ImplicitSurfaceSet2 = ImplicitSurfaceSet<2>;

//! 3-D ImplicitSurfaceSet type.
using ImplicitSurfaceSet3 = ImplicitSurfaceSet<3>;

//! Shared pointer type for the ImplicitSurfaceSet2.
using ImplicitSurfaceSet2Ptr = std::shared_ptr<ImplicitSurfaceSet2>;

//! Shared pointer type for the ImplicitSurfaceSet3.
using ImplicitSurfaceSet3Ptr = std::shared_ptr<ImplicitSurfaceSet3>;

//!
//! \brief Front-end to create ImplicitSurfaceSet objects step by step.
//!
template <size_t N>
class ImplicitSurfaceSet<N>::Builder final
    : public SurfaceBuilderBase<N, typename ImplicitSurfaceSet<N>::Builder>
{
 public:
    //! Returns builder with surfaces.
    Builder& WithSurfaces(
        const ConstArrayView1<std::shared_ptr<ImplicitSurface<N>>>& surfaces);

    //! Returns builder with explicit surfaces.
    Builder& WithExplicitSurfaces(
        const ConstArrayView1<std::shared_ptr<Surface<N>>>& surfaces);

    //! Builds ImplicitSurfaceSet.
    ImplicitSurfaceSet<N> Build() const;

    //! Builds shared pointer of ImplicitSurfaceSet instance.
    std::shared_ptr<ImplicitSurfaceSet<N>> MakeShared() const;

 private:
    using Base = SurfaceBuilderBase<N, typename ImplicitSurfaceSet<N>::Builder>;
    using Base::m_isNormalFlipped;
    using Base::m_transform;

    Array1<std::shared_ptr<ImplicitSurface<N>>> m_surfaces;
};
}  // namespace CubbyFlow

#endif