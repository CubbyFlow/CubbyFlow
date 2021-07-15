// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_SPH_SYSTEM_DATA3_HPP
#define CUBBYFLOW_CUDA_SPH_SYSTEM_DATA3_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/CUDA/CUDAParticleSystemData3.hpp>
#include <Core/Utils/Constants.hpp>

namespace CubbyFlow
{
class CUDASPHSystemData3 : public CUDAParticleSystemData3
{
 public:
    //! Constructs empty SPH system.
    CUDASPHSystemData3();

    //! Constructs SPH system data with given number of particles.
    explicit CUDASPHSystemData3(size_t numberOfParticles);

    //! Default virtual destructor.
    ~CUDASPHSystemData3() override = default;

    //! Copy constructor.
    CUDASPHSystemData3(const CUDASPHSystemData3& other);

    //! Move constructor.
    CUDASPHSystemData3(CUDASPHSystemData3&& other) noexcept;

    //! Copy assignment operator.
    CUDASPHSystemData3& operator=(const CUDASPHSystemData3& other);

    //! Move assignment operator.
    CUDASPHSystemData3& operator=(CUDASPHSystemData3&& other) noexcept;

    //! Returns the density array view (immutable).
    ConstCUDAArrayView1<float> Densities() const;

    //! Returns the density array view (mutable).
    CUDAArrayView1<float> Densities();

    //! Returns the pressure array view (immutable).
    ConstCUDAArrayView1<float> Pressures() const;

    //! Returns the pressure array view (mutable).
    CUDAArrayView1<float> Pressures();

    //!
    //! \brief Updates the density array with the latest particle positions.
    //!
    //! This function updates the density array by recalculating each particle's
    //! latest nearby particles' position.
    //!
    //! \warning You must update the neighbor searcher
    //! (CUDASPHSystemData::BuildNeighborSearcher) before calling this function.
    //!
    void UpdateDensities();

    //! Returns the target density of this particle system.
    float TargetDensity() const;

    //! Sets the target density of this particle system.
    void SetTargetDensity(float targetDensity);

    //! Returns the target particle spacing in meters.
    float TargetSpacing() const;

    //!
    //! \brief Sets the target particle spacing in meters.
    //!
    //! Once this function is called, hash grid and density should be
    //! updated using UpdateHashGrid() and UpdateDensities()).
    //!
    void SetTargetSpacing(float spacing);

    //!
    //! \brief Returns the relative kernel radius.
    //!
    //! Returns the relative kernel radius compared to the target particle
    //! spacing (i.e. kernel radius / target spacing).
    //!
    float RelativeKernelRadius() const;

    //!
    //! \brief Sets the relative kernel radius.
    //!
    //! Sets the relative kernel radius compared to the target particle
    //! spacing (i.e. kernel radius / target spacing).
    //! Once this function is called, hash grid and density should
    //! be updated using UpdateHashGrid() and UpdateDensities()).
    //!
    void SetRelativeKernelRadius(float relativeRadius);

    //! Returns the kernel radius in meters unit.
    float KernelRadius() const;

    //!
    //! \brief Sets the absolute kernel radius.
    //!
    //! Sets the absolute kernel radius compared to the target particle
    //! spacing (i.e. relative kernel radius * target spacing).
    //! Once this function is called, hash grid and density should
    //! be updated using UpdateHashGrid() and UpdateDensities()).
    //!
    void SetKernelRadius(float kernelRadius);

    //! Returns the mass of the particles.
    float Mass() const;

    //! Builds neighbor searcher with kernel radius.
    void BuildNeighborSearcher();

    //! Builds neighbor lists with kernel radius and update densities.
    void BuildNeighborListsAndUpdateDensities();

    //! Copies from other SPH system data.
    void Set(const CUDASPHSystemData3& other);

 private:
    //! Target density of this particle system in kg/m^2.
    float m_targetDensity = WATER_DENSITY_FLOAT;

    //! Target spacing of this particle system in meters.
    float m_targetSpacing = 0.1f;

    //! Relative radius of SPH kernel.
    //! SPH kernel radius divided by target spacing.
    float m_kernelRadiusOverTargetSpacing = 1.8f;

    //! SPH kernel radius in meters.
    float m_kernelRadius = 0.1f;

    float m_mass = 1e-3f;

    size_t m_pressureIdx = 0;

    size_t m_densityIdx = 0;

    //! Computes the mass based on the target density and spacing.
    void ComputeMass();
};

//! Shared pointer for the CUDASPHSystemData3 type.
using CUDASPHSystemData3Ptr = std::shared_ptr<CUDASPHSystemData3>;

}  // namespace CubbyFlow

#endif

#endif