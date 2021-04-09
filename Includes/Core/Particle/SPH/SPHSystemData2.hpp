// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SPH_SYSTEM_DATA2_HPP
#define CUBBYFLOW_SPH_SYSTEM_DATA2_HPP

#include <Core/Particle/ParticleSystemData.hpp>

namespace CubbyFlow
{
//!
//! \brief      2-D SPH particle system data.
//!
//! This class extends ParticleSystemData2 to specialize the data model for SPH.
//! It includes density and pressure array as a default particle attribute, and
//! it also contains SPH utilities such as interpolation operator.
//!
class SPHSystemData2 : public ParticleSystemData2
{
 public:
    //! Constructs empty SPH system.
    SPHSystemData2();

    //! Constructs SPH system data with given number of particles.
    explicit SPHSystemData2(size_t numberOfParticles);

    //! Copy constructor.
    SPHSystemData2(const SPHSystemData2& other);

    //! Default move constructor.
    SPHSystemData2(SPHSystemData2&&) noexcept = default;

    //! Default virtual destructor.
    ~SPHSystemData2() override = default;

    //! Copy assignment operator.
    SPHSystemData2& operator=(const SPHSystemData2& other);

    //! Default move assignment operator.
    SPHSystemData2& operator=(SPHSystemData2&&) noexcept = default;

    //!
    //! \brief      Sets the radius.
    //!
    //! Sets the radius of the particle system. The radius will be interpreted
    //! as target spacing.
    //!
    void SetRadius(double newRadius) override;

    //!
    //! \brief      Sets the mass of a particle.
    //!
    //! Setting the mass of a particle will change the target density.
    //!
    //! \param[in]  newMass The new mass.
    //!
    void SetMass(double newMass) override;

    //! Returns the density array view (immutable).
    [[nodiscard]] ConstArrayView1<double> Densities() const;

    //! Returns the density array view (mutable).
    [[nodiscard]] ArrayView1<double> Densities();

    //! Returns the pressure array view (immutable).
    [[nodiscard]] ConstArrayView1<double> Pressures() const;

    //! Returns the pressure array view (mutable).
    [[nodiscard]] ArrayView1<double> Pressures();

    //!
    //! \brief Updates the density array with the latest particle positions.
    //!
    //! This function updates the density array by recalculating each particle's
    //! latest nearby particles' position.
    //!
    //! \warning You must update the neighbor searcher
    //! (SPHSystemData2::BuildNeighborSearcher) before calling this function.
    //!
    void UpdateDensities();

    //! Sets the target density of this particle system.
    void SetTargetDensity(double targetDensity);

    //! Returns the target density of this particle system.
    [[nodiscard]] double GetTargetDensity() const;

    //!
    //! \brief Sets the target particle spacing in meters.
    //!
    //! Once this function is called, hash grid and density should be
    //! updated using UpdateHashGrid() and UpdateDensities).
    //!
    void SetTargetSpacing(double spacing);

    //! Returns the target particle spacing in meters.
    [[nodiscard]] double GetTargetSpacing() const;

    //!
    //! \brief Sets the relative kernel radius.
    //!
    //! Sets the relative kernel radius compared to the target particle
    //! spacing (i.e. kernel radius / target spacing).
    //! Once this function is called, hash grid and density should
    //! be updated using UpdateHashGrid() and UpdateDensities).
    //!
    void SetRelativeKernelRadius(double relativeRadius);

    //!
    //! \brief Returns the relative kernel radius.
    //!
    //! Returns the relative kernel radius compared to the target particle
    //! spacing (i.e. kernel radius / target spacing).
    //!
    [[nodiscard]] double GetRelativeKernelRadius() const;

    //!
    //! \brief Sets the absolute kernel radius.
    //!
    //! Sets the absolute kernel radius compared to the target particle
    //! spacing (i.e. relative kernel radius * target spacing).
    //! Once this function is called, hash grid and density should
    //! be updated using UpdateHashGrid() and UpdateDensities).
    //!
    void SetKernelRadius(double kernelRadius);

    //! Returns the kernel radius in meters unit.
    [[nodiscard]] double GetKernelRadius() const;

    //! Returns sum of kernel function evaluation for each nearby particle.
    [[nodiscard]] double SumOfKernelNearby(const Vector2D& origin) const;

    //!
    //! \brief Returns interpolated value at given origin point.
    //!
    //! Returns interpolated scalar data from the given position using
    //! standard SPH weighted average. The data array should match the
    //! particle layout. For example, density or pressure arrays can be
    //! used.
    //!
    //! \warning You must update the neighbor searcher
    //! (SPHSystemData2::BuildNeighborSearcher) before calling this function.
    //!
    [[nodiscard]] double Interpolate(
        const Vector2D& origin, const ConstArrayView1<double>& values) const;

    //!
    //! \brief Returns interpolated vector value at given origin point.
    //!
    //! Returns interpolated vector data from the given position using
    //! standard SPH weighted average. The data array should match the
    //! particle layout. For example, velocity or acceleration arrays can be
    //! used.
    //!
    //! \warning You must update the neighbor searcher
    //! (SPHSystemData2::BuildNeighborSearcher) before calling this function.
    //!
    [[nodiscard]] Vector2D Interpolate(
        const Vector2D& origin, const ConstArrayView1<Vector2D>& values) const;

    //!
    //! Returns the gradient of the given values at i-th particle.
    //!
    //! \warning You must update the neighbor lists
    //! (SPHSystemData2::BuildNeighborLists) before calling this function.
    //!
    [[nodiscard]] Vector2D GradientAt(
        size_t i, const ConstArrayView1<double>& values) const;

    //!
    //! Returns the Laplacian of the given values at i-th particle.
    //!
    //! \warning You must update the neighbor lists
    //! (SPHSystemData2::BuildNeighborLists) before calling this function.
    //!
    [[nodiscard]] double LaplacianAt(
        size_t i, const ConstArrayView1<double>& values) const;

    //!
    //! Returns the Laplacian of the given values at i-th particle.
    //!
    //! \warning You must update the neighbor lists
    //! (SPHSystemData2::BuildNeighborLists) before calling this function.
    //!
    [[nodiscard]] Vector2D LaplacianAt(
        size_t i, const ConstArrayView1<Vector2D>& values) const;

    //! Builds neighbor searcher with kernel radius.
    void BuildNeighborSearcher();

    //! Builds neighbor lists with kernel radius.
    void BuildNeighborLists();

    //! Serializes this SPH system data to the buffer.
    void Serialize(std::vector<uint8_t>* buffer) const override;

    //! Deserializes this SPH system data from the buffer.
    void Deserialize(const std::vector<uint8_t>& buffer) override;

    //! Copies from other SPH system data.
    void Set(const SPHSystemData2& other);

 private:
    //! Target density of this particle system in kg/m^2.
    double m_targetDensity = WATER_DENSITY;

    //! Target spacing of this particle system in meters.
    double m_targetSpacing = 0.1;

    //! Relative radius of SPH kernel.
    //! SPH kernel radius divided by target spacing.
    double m_kernelRadiusOverTargetSpacing = 1.8;

    //! SPH kernel radius in meters.
    double m_kernelRadius = 1.0;

    size_t m_pressureIdx = 0;

    size_t m_densityIdx = 0;

    //! Computes the mass based on the target density and spacing.
    void ComputeMass();
};

//! Shared pointer for the SPHSystemData2 type.
using SPHSystemData2Ptr = std::shared_ptr<SPHSystemData2>;
}  // namespace CubbyFlow

#endif