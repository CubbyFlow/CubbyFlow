// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_PIC_SOLVER3_HPP
#define CUBBYFLOW_PIC_SOLVER3_HPP

#include <Core/Emitter/ParticleEmitter3.hpp>
#include <Core/Particle/ParticleSystemData.hpp>
#include <Core/Solver/Grid/GridFluidSolver3.hpp>

namespace CubbyFlow
{
//!
//! \brief 3-D Particle-in-Cell (PIC) implementation.
//!
//! This class implements 3-D Particle-in-Cell (PIC) method by inheriting
//! GridFluidSolver3. Since it is a grid-particle hybrid method, the solver
//! also has a particle system to track fluid particles.
//!
//! \see Zhu, Yongning, and Robert Bridson. "Animating sand as a fluid."
//!     ACM Transactions on Graphics (TOG). Vol. 34. No. 3. ACM, 3005.
//!
class PICSolver3 : public GridFluidSolver3
{
 public:
    class Builder;

    //! Default constructor.
    PICSolver3();

    //! Constructs solver with initial grid size.
    PICSolver3(const Vector3UZ& resolution, const Vector3D& gridSpacing,
               const Vector3D& gridOrigin);

    //! Deleted copy constructor.
    PICSolver3(const PICSolver3&) = delete;

    //! Deleted move constructor.
    PICSolver3(PICSolver3&&) noexcept = delete;

    //! Default virtual destructor.
    ~PICSolver3() override = default;

    //! Deleted copy assignment operator.
    PICSolver3& operator=(const PICSolver3&) = delete;

    //! Deleted move assignment operator.
    PICSolver3& operator=(PICSolver3&&) noexcept = delete;

    //! Returns the signed-distance field of particles.
    [[nodiscard]] ScalarGrid3Ptr GetSignedDistanceField() const;

    //! Returns the particle system data.
    [[nodiscard]] const ParticleSystemData3Ptr& GetParticleSystemData() const;

    //! Returns the particle emitter.
    [[nodiscard]] const ParticleEmitter3Ptr& GetParticleEmitter() const;

    //! Sets the particle emitter.
    void SetParticleEmitter(const ParticleEmitter3Ptr& newEmitter);

    //! Returns builder fox PICSolver3.
    [[nodiscard]] static Builder GetBuilder();

 protected:
    //! Initializes the simulator.
    void OnInitialize() override;

    //! Invoked before a simulation time-step begins.
    void OnBeginAdvanceTimeStep(double timeIntervalInSeconds) override;

    //! Computes the advection term of the fluid solver.
    void ComputeAdvection(double timeIntervalInSeconds) override;

    //! Returns the signed-distance field of the fluid.
    [[nodiscard]] ScalarField3Ptr GetFluidSDF() const override;

    //! Transfers velocity field from particles to grids.
    virtual void TransferFromParticlesToGrids();

    //! Transfers velocity field from grids to particles.
    virtual void TransferFromGridsToParticles();

    //! Moves particles.
    virtual void MoveParticles(double timeIntervalInSeconds);

    Array3<char> m_uMarkers;
    Array3<char> m_vMarkers;
    Array3<char> m_wMarkers;

 private:
    void ExtrapolateVelocityToAir();

    void BuildSignedDistanceField();

    void UpdateParticleEmitter(double timeIntervalInSeconds) const;

    size_t m_signedDistanceFieldID;
    ParticleSystemData3Ptr m_particles;
    ParticleEmitter3Ptr m_particleEmitter;
};

//! Shared pointer type for the PICSolver3.
using PICSolver3Ptr = std::shared_ptr<PICSolver3>;

//!
//! \brief Front-end to create PICSolver3 objects step by step.
//!
class PICSolver3::Builder final : public GridFluidSolverBuilderBase3<Builder>
{
 public:
    //! Builds PICSolver3.
    [[nodiscard]] PICSolver3 Build() const;

    //! Builds shared pointer of PICSolver3 instance.
    [[nodiscard]] PICSolver3Ptr MakeShared() const;
};
}  // namespace CubbyFlow

#endif