// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUSTOM_IMPLICIT_SURFACE2_HPP
#define CUBBYFLOW_CUSTOM_IMPLICIT_SURFACE2_HPP

#include <Core/Surface/ImplicitSurface2.hpp>

#include <functional>

namespace CubbyFlow
{
//! Custom 2-D implicit surface using arbitrary function.
class CustomImplicitSurface2 final : public ImplicitSurface2
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
    //!                              search.
    //! \param _transform Local-to-world transform.
    //! \param _isNormalFlipped True if normal is flipped.
    //!
    CustomImplicitSurface2(std::function<double(const Vector2D&)> func,
                           BoundingBox2D domain = BoundingBox2D{},
                           double resolution = 1e-3,
                           double rayMarchingResolution = 1e-6,
                           unsigned int maxNumberOfIterations = 5,
                           const Transform2& _transform = Transform2{},
                           bool _isNormalFlipped = false);

    //! Returns builder for CustomImplicitSurface2.
    [[nodiscard]] static Builder GetBuilder();

 private:
    [[nodiscard]] Vector2D ClosestPointLocal(
        const Vector2D& otherPoint) const override;

    [[nodiscard]] bool IntersectsLocal(const Ray2D& ray) const override;

    [[nodiscard]] BoundingBox2D BoundingBoxLocal() const override;

    [[nodiscard]] Vector2D ClosestNormalLocal(
        const Vector2D& otherPoint) const override;

    [[nodiscard]] double SignedDistanceLocal(
        const Vector2D& otherPoint) const override;

    [[nodiscard]] SurfaceRayIntersection2 ClosestIntersectionLocal(
        const Ray2D& ray) const override;

    [[nodiscard]] Vector2D GradientLocal(const Vector2D& x) const;

    std::function<double(const Vector2D&)> m_func;
    BoundingBox2D m_domain;
    double m_resolution = 1e-3;
    double m_rayMarchingResolution = 1e-6;
    unsigned int m_maxNumberOfIterations = 5;
};

//! Shared pointer type for the CustomImplicitSurface2.
using CustomImplicitSurface2Ptr = std::shared_ptr<CustomImplicitSurface2>;

//!
//! \brief Front-end to create CustomImplicitSurface2 objects step by step.
//!
class CustomImplicitSurface2::Builder final
    : public SurfaceBuilderBase2<Builder>
{
 public:
    //! Returns builder with custom signed-distance function
    [[nodiscard]] Builder& WithSignedDistanceFunction(
        const std::function<double(const Vector2D&)>& func);

    //! Returns builder with domain.
    [[nodiscard]] Builder& WithDomain(const BoundingBox2D& domain);

    //! Returns builder with finite differencing resolution.
    [[nodiscard]] Builder& WithResolution(double resolution);

    //! Returns builder with ray marching resolution which determines the ray
    //! intersection quality.
    [[nodiscard]] Builder& WithRayMarchingResolution(
        double rayMarchingResolution);

    //! Returns builder with number of iterations for closest point/normal
    //! searches.
    [[nodiscard]] Builder& WithMaxNumberOfIterations(unsigned int numIter);

    //! Builds CustomImplicitSurface2.
    [[nodiscard]] CustomImplicitSurface2 Build() const;

    //! Builds shared pointer of CustomImplicitSurface2 instance.
    [[nodiscard]] CustomImplicitSurface2Ptr MakeShared() const;

 private:
    std::function<double(const Vector2D&)> m_func;
    BoundingBox2D m_domain;
    double m_resolution = 1e-3;
    double m_rayMarchingResolution = 1e-6;
    unsigned int m_maxNumberOfIterations = 5;
};
}  // namespace CubbyFlow

#endif