// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUSTOM_IMPLICIT_SURFACE3_HPP
#define CUBBYFLOW_CUSTOM_IMPLICIT_SURFACE3_HPP

#include <Core/Surface/ImplicitSurface3.hpp>

#include <functional>

namespace CubbyFlow
{
//! Custom 3-D implicit surface using arbitrary function.
class CustomImplicitSurface3 final : public ImplicitSurface3
{
 public:
    class Builder;

    //!
    //! Constructs an implicit surface using the given signed-distance function.
    //!
    //! \param func Custom SDF function object.
    //! \param domain Bounding box of the SDF if exists.
    //! \param resolution Finite differencing resolution for derivatives.
    //! \param rayMarchingResolution Ray marching resolution for ray tests.
    //! \param maxNumberOfIterations Number of iterations for closest point
    //! search. \param transform Local-to-world transform. \param
    //! isNormalFlipped True if normal is flipped.
    //!
    CustomImplicitSurface3(const std::function<double(const Vector3D&)>& func,
                           const BoundingBox3D& domain = BoundingBox3D(),
                           double resolution = 1e-3,
                           double rayMarchingResolution = 1e-6,
                           unsigned int maxNumberOfIterations = 5,
                           const Transform3& transform = Transform3(),
                           bool isNormalFlipped = false);

    //! Destructor.
    virtual ~CustomImplicitSurface3();

    //! Returns builder for CustomImplicitSurface3.
    static Builder GetBuilder();

 private:
    std::function<double(const Vector3D&)> m_func;
    BoundingBox3D m_domain;
    double m_resolution = 1e-3;
    double m_rayMarchingResolution = 1e-6;
    unsigned int m_maxNumberOfIterations = 5;

    Vector3D ClosestPointLocal(const Vector3D& otherPoint) const override;

    bool IntersectsLocal(const Ray3D& ray) const override;

    BoundingBox3D BoundingBoxLocal() const override;

    Vector3D ClosestNormalLocal(const Vector3D& otherPoint) const override;

    double SignedDistanceLocal(const Vector3D& otherPoint) const override;

    SurfaceRayIntersection3 ClosestIntersectionLocal(
        const Ray3D& ray) const override;

    Vector3D GradientLocal(const Vector3D& x) const;
};

//! Shared pointer type for the CustomImplicitSurface3.
using CustomImplicitSurface3Ptr = std::shared_ptr<CustomImplicitSurface3>;

//!
//! \brief Front-end to create CustomImplicitSurface3 objects step by step.
//!
class CustomImplicitSurface3::Builder final
    : public SurfaceBuilderBase3<CustomImplicitSurface3::Builder>
{
 public:
    //! Returns builder with custom signed-distance function
    Builder& WithSignedDistanceFunction(
        const std::function<double(const Vector3D&)>& func);

    //! Returns builder with domain.
    Builder& WithDomain(const BoundingBox3D& domain);

    //! Returns builder with finite differencing resolution.
    Builder& WithResolution(double resolution);

    //! Returns builder with ray marching resolution which determines the ray
    //! intersection quality.
    Builder& WithRayMarchingResolution(double rayMarchingResolution);

    //! Returns builder with number of iterations for closest point/normal
    //! searches.
    Builder& WithMaxNumberOfIterations(unsigned int numIter);

    //! Builds CustomImplicitSurface3.
    CustomImplicitSurface3 Build() const;

    //! Builds shared pointer of CustomImplicitSurface3 instance.
    CustomImplicitSurface3Ptr MakeShared() const;

 private:
    std::function<double(const Vector3D&)> m_func;
    BoundingBox3D m_domain;
    double m_resolution = 1e-3;
    double m_rayMarchingResolution = 1e-6;
    unsigned int m_maxNumberOfIterations = 5;
};
}  // namespace CubbyFlow

#endif