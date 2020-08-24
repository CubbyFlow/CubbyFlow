// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SPH_SOLVER3_HPP
#define CUBBYFLOW_SPH_SOLVER3_HPP

#include <Core/SPH/SPHSystemData3.hpp>
#include <Core/Solver/Particle/ParticleSystemSolver3.hpp>

namespace CubbyFlow
{
//!
//! \brief 3-D SPH solver.
//!
//! This class implements 3-D SPH solver. The main pressure solver is based on
//! equation-of-state (EOS).
//!
//! \see M{\"u}ller et al., Particle-based fluid simulation for interactive
//!      applications, SCA 2003.
//! \see M. Becker and M. Teschner, Weakly compressible SPH for free surface
//!      flows, SCA 2007.
//! \see Adams and Wicke, Meshless approximation methods and applications in
//!      physics based modeling and animation, Eurographics tutorials 2009.
//!
class SPHSolver3 : public ParticleSystemSolver3
{
 public:
    class Builder;

    //! Constructs a solver with empty particle set.
    SPHSolver3();

    //! Constructs a solver with target density, spacing, and relative kernel
    //! radius.
    SPHSolver3(double targetDensity, double targetSpacing,
               double relativeKernelRadius);

    virtual ~SPHSolver3();

    //! Returns the exponent part of the equation-of-state.
    double GetEosExponent() const;

    //!
    //! \brief Sets the exponent part of the equation-of-state.
    //!
    //! This function sets the exponent part of the equation-of-state.
    //! The value must be greater than 1.0, and smaller inputs will be clamped.
    //! Default is 7.
    //!
    void SetEosExponent(double newEosExponent);

    //! Returns the negative pressure scale.
    double GetNegativePressureScale() const;

    //!
    //! \brief Sets the negative pressure scale.
    //!
    //! This function sets the negative pressure scale. By setting the number
    //! between 0 and 1, the solver will scale the effect of negative pressure
    //! which can prevent the clumping of the particles near the surface. Input
    //! value outside 0 and 1 will be clamped within the range. Default is 0.
    //!
    void SetNegativePressureScale(double newNegativePressureScale);

    //! Returns the viscosity coefficient.
    double GetViscosityCoefficient() const;

    //! Sets the viscosity coefficient.
    void SetViscosityCoefficient(double newViscosityCoefficient);

    //! Returns the pseudo viscosity coefficient.
    double GetPseudoViscosityCoefficient() const;

    //!
    //! \brief Sets the pseudo viscosity coefficient.
    //!
    //! This function sets the pseudo viscosity coefficient which applies
    //! additional pseudo-physical damping to the system. Default is 10.
    //!
    void SetPseudoViscosityCoefficient(double newPseudoViscosityCoefficient);

    //! Returns the speed of sound.
    double GetSpeedOfSound() const;

    //!
    //! \brief Sets the speed of sound.
    //!
    //! This function sets the speed of sound which affects the stiffness of the
    //! EOS and the time-step size. Higher value will make EOS stiffer and the
    //! time-step smaller. The input value must be higher than 0.0.
    //!
    void SetSpeedOfSound(double newSpeedOfSound);

    //!
    //! \brief Multiplier that scales the max allowed time-step.
    //!
    //! This function returns the multiplier that scales the max allowed
    //! time-step. When the scale is 1.0, the time-step is bounded by the speed
    //! of sound and max acceleration.
    //!
    double GetTimeStepLimitScale() const;

    //!
    //! \brief Sets the multiplier that scales the max allowed time-step.
    //!
    //! This function sets the multiplier that scales the max allowed
    //! time-step. When the scale is 1.0, the time-step is bounded by the speed
    //! of sound and max acceleration.
    //!
    void SetTimeStepLimitScale(double newScale);

    //! Returns the SPH system data.
    SPHSystemData3Ptr GetSPHSystemData() const;

    //! Returns builder fox SPHSolver3.
    static Builder GetBuilder();

 protected:
    //! Returns the number of sub-time-steps.
    unsigned int GetNumberOfSubTimeSteps(
        double timeIntervalInSeconds) const override;

    //! Accumulates the force to the forces array in the particle system.
    void AccumulateForces(double timeStepInSeconds) override;

    //! Performs pre-processing step before the simulation.
    void OnBeginAdvanceTimeStep(double timeStepInSeconds) override;

    //! Performs post-processing step before the simulation.
    void OnEndAdvanceTimeStep(double timeStepInSeconds) override;

    //! Accumulates the non-pressure forces to the forces array in the particle
    //! system.
    virtual void AccumulateNonPressureForces(double timeStepInSeconds);

    //! Accumulates the pressure force to the forces array in the particle
    //! system.
    virtual void AccumulatePressureForce(double timeStepInSeconds);

    //! Computes the pressure.
    void ComputePressure();

    //! Accumulates the pressure force to the given \p pressureForces array.
    void AccumulatePressureForce(const ConstArrayAccessor1<Vector3D>& positions,
                                 const ConstArrayAccessor1<double>& densities,
                                 const ConstArrayAccessor1<double>& pressures,
                                 ArrayAccessor1<Vector3D> pressureForces);

    //! Accumulates the viscosity force to the forces array in the particle
    //! system.
    void AccumulateViscosityForce();

    //! Computes pseudo viscosity.
    void ComputePseudoViscosity(double timeStepInSeconds);

 private:
    //! Exponent component of equation-of-state (or Tait's equation).
    double m_eosExponent = 7.0;

    //! Negative pressure scaling factor.
    //! Zero means clamping. One means do nothing.
    double m_negativePressureScale = 0.0;

    //! Viscosity coefficient.
    double m_viscosityCoefficient = 0.01;

    //! Pseudo-viscosity coefficient velocity filtering.
    //! This is a minimum "safety-net" for SPH solver which is quite
    //! sensitive to the parameters.
    double m_pseudoViscosityCoefficient = 10.0;

    //! Speed of sound in medium to determine the stiffness of the system.
    //! Ideally, it should be the actual speed of sound in the fluid, but in
    //! practice, use lower value to trace-off performance and compressibility.
    double m_speedOfSound = 100.0;

    //! Scales the max allowed time-step.
    double m_timeStepLimitScale = 1.0;
};

//! Shared pointer type for the SPHSolver3.
using SPHSolver3Ptr = std::shared_ptr<SPHSolver3>;

//!
//! \brief Base class for SPH-based fluid solver builder.
//!
template <typename DerivedBuilder>
class SPHSolverBuilderBase3
{
 public:
    //! Returns builder with target density.
    DerivedBuilder& WithTargetDensity(double targetDensity);

    //! Returns builder with target spacing.
    DerivedBuilder& WithTargetSpacing(double targetSpacing);

    //! Returns builder with relative kernel radius.
    DerivedBuilder& WithRelativeKernelRadius(double relativeKernelRadius);

 protected:
    double m_targetDensity = WATER_DENSITY;
    double m_targetSpacing = 0.1;
    double m_relativeKernelRadius = 1.8;
};

template <typename T>
T& SPHSolverBuilderBase3<T>::WithTargetDensity(double targetDensity)
{
    m_targetDensity = targetDensity;
    return static_cast<T&>(*this);
}

template <typename T>
T& SPHSolverBuilderBase3<T>::WithTargetSpacing(double targetSpacing)
{
    m_targetSpacing = targetSpacing;
    return static_cast<T&>(*this);
}

template <typename T>
T& SPHSolverBuilderBase3<T>::WithRelativeKernelRadius(
    double relativeKernelRadius)
{
    m_relativeKernelRadius = relativeKernelRadius;
    return static_cast<T&>(*this);
}

//!
//! \brief Front-end to create SPHSolver3 objects step by step.
//!
class SPHSolver3::Builder final
    : public SPHSolverBuilderBase3<SPHSolver3::Builder>
{
 public:
    //! Builds SPHSolver3.
    SPHSolver3 Build() const;

    //! Builds shared pointer of SPHSolver3 instance.
    SPHSolver3Ptr MakeShared() const;
};
}  // namespace CubbyFlow

#endif