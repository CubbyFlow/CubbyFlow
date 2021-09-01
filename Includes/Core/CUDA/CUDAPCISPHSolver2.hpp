// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_CUDA_PARTICLE_PCISPH_SOLVER2_HPP
#define CUBBYFLOW_CUDA_PARTICLE_PCISPH_SOLVER2_HPP

#ifdef CUBBYFLOW_USE_CUDA

#include <Core/CUDA/CUDASPHSolverBase2.hpp>

#include <memory>

namespace CubbyFlow
{
//!
//! \brief CUDA-based 2-D PCISPH solver.
//!
//! This class implements 2-D predictive-corrective SPH solver. The main
//! pressure solver is based on Solenthaler and Pajarola's 2009 SIGGRAPH paper.
//!
//! \see Solenthaler and Pajarola, Predictive-corrective incompressible SPH,
//!      ACM transactions on graphics (TOG). Vol. 28. No. 3. ACM, 2009.
//!
class CUDAPCISPHSolver2 : public CUDASPHSolverBase2
{
 public:
    class Builder;

    //! Constructs a solver with empty particle set.
    CUDAPCISPHSolver2();

    //! Constructs a solver with target density, spacing, and relative kernel
    //! radius.
    CUDAPCISPHSolver2(float targetDensity, float targetSpacing,
                      float relativeKernelRadius);

    //! Deleted copy constructor.
    CUDAPCISPHSolver2(const CUDAPCISPHSolver2&) = delete;

    //! Deleted move constructor.
    CUDAPCISPHSolver2(CUDAPCISPHSolver2&&) noexcept = delete;

    //! Default virtual destructor.
    ~CUDAPCISPHSolver2() override = default;

    //! Deleted copy assignment operator.
    CUDAPCISPHSolver2& operator=(const CUDAPCISPHSolver2&) = delete;

    //! Deleted move assignment operator.
    CUDAPCISPHSolver2& operator=(CUDAPCISPHSolver2&&) noexcept = delete;

    //! Returns max allowed density error ratio.
    [[nodiscard]] float MaxDensityErrorRatio() const;

    //!
    //! \brief Sets max allowed density error ratio.
    //!
    //! This function sets the max allowed density error ratio during the PCISPH
    //! iteration. Default is 0.01 (1%). The input value should be positive.
    //!
    void SetMaxDensityErrorRatio(float ratio);

    //! Returns max number of iterations.
    [[nodiscard]] unsigned int MaxNumberOfIterations() const;

    //!
    //! \brief Sets max number of PCISPH iterations.
    //!
    //! This function sets the max number of PCISPH iterations. Default is 5.
    //!
    void SetMaxNumberOfIterations(unsigned int n);

    //! Returns builder for CUDAPCISPHSolver2.
    static Builder GetBuilder();

 protected:
    //! Called to advance a single time-step.
    void OnAdvanceTimeStep(double timeStepInSeconds) override;

    CUDAArrayView1<float2> TempPositions();

    CUDAArrayView1<float2> TempVelocities();

    CUDAArrayView1<float> TempDensities();

    CUDAArrayView1<float2> PressureForces();

    CUDAArrayView1<float> DensityErrors();

 private:
    float ComputeDelta(float timeStepInSeconds);
    float ComputeBeta(float timeStepInSeconds);

    float m_maxDensityErrorRatio = 0.01f;
    unsigned int m_maxNumberOfIterations = 5;

    size_t m_tempPositionsIdx = 0;
    size_t m_tempVelocitiesIdx = 0;
    size_t m_tempDensitiesIdx = 0;
    size_t m_pressureForcesIdx = 0;
    size_t m_densityErrorsIdx = 0;
};

//! Shared pointer type for the CUDAPCISPHSolver2.
using CUDAPCISPHSolver2Ptr = std::shared_ptr<CUDAPCISPHSolver2>;

//!
//! \brief Front-end to create CUDAPCISPHSolver2 objects step by step.
//!
class CUDAPCISPHSolver2::Builder final
    : public CUDASPHSolverBuilderBase2<CUDAPCISPHSolver2::Builder>
{
 public:
    //! Builds CUDAPCISPHSolver2.
    [[nodiscard]] CUDAPCISPHSolver2 Build() const;

    //! Builds shared pointer of CUDAPCISPHSolver2 instance.
    [[nodiscard]] CUDAPCISPHSolver2Ptr MakeShared() const;
};
}  // namespace CubbyFlow

#endif

#endif