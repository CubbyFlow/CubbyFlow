// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_LEVEL_SET_LIQUID_SOLVER2_HPP
#define CUBBYFLOW_LEVEL_SET_LIQUID_SOLVER2_HPP

#include <Core/Solver/Grid/GridFluidSolver2.hpp>
#include <Core/Solver/LevelSet/LevelSetSolver2.hpp>

namespace CubbyFlow
{
//!
//! \brief      Level set based 2-D liquid solver.
//!
//! This class implements level set-based 2-D liquid solver. It defines the
//! surface of the liquid using signed-distance field and use stable fluids
//! framework to compute the forces.
//!
//! \see Enright, Douglas, Stephen Marschner, and Ronald Fedkiw.
//!     "Animation and rendering of complex water surfaces." ACM Transactions on
//!     Graphics (TOG). Vol. 21. No. 3. ACM, 2002.
//!
class LevelSetLiquidSolver2 : public GridFluidSolver2
{
 public:
    class Builder;

    //! Default constructor.
    LevelSetLiquidSolver2();

    //! Constructs solver with initial grid size.
    LevelSetLiquidSolver2(const Vector2UZ& resolution,
                          const Vector2D& gridSpacing,
                          const Vector2D& gridOrigin);

    //! Deleted copy constructor.
    LevelSetLiquidSolver2(const LevelSetLiquidSolver2&) = delete;

    //! Deleted move constructor.
    LevelSetLiquidSolver2(LevelSetLiquidSolver2&&) noexcept = delete;

    //! Default virtual destructor.
    ~LevelSetLiquidSolver2() override = default;

    //! Deleted copy assignment operator.
    LevelSetLiquidSolver2& operator=(const LevelSetLiquidSolver2&) = delete;

    //! Deleted move assignment operator.
    LevelSetLiquidSolver2& operator=(LevelSetLiquidSolver2&&) noexcept = delete;

    //! Returns signed-distance field.
    [[nodiscard]] ScalarGrid2Ptr GetSignedDistanceField() const;

    //! Returns the level set solver.
    [[nodiscard]] LevelSetSolver2Ptr GetLevelSetSolver() const;

    //! Sets the level set solver.
    void SetLevelSetSolver(const LevelSetSolver2Ptr& newSolver);

    //! Sets minimum reinitialization distance.
    void SetMinReinitializeDistance(double distance);

    //!
    //! \brief Enables (or disables) global compensation feature flag.
    //!
    //! When \p isEnabled is true, the global compensation feature is enabled.
    //! The global compensation measures the volume at the beginning and the end
    //! of the time-step and adds the volume change back to the level-set field
    //! by globally shifting the front.
    //!
    //! \see Song, Oh-Young, Hyuncheol Shin, and Hyeong-Seok Ko.
    //! "Stable but non-dissipative water." ACM Transactions on Graphics (TOG)
    //! 24, no. 1 (2005): 81-97.
    //!
    void SetIsGlobalCompensationEnabled(bool isEnabled);

    //!
    //! \brief Returns liquid volume measured by smeared Heaviside function.
    //!
    //! This function measures the liquid volume (area in 2-D) using smeared
    //! Heaviside function. Thus, the estimated volume is an approximated
    //! quantity.
    //!
    [[nodiscard]] double ComputeVolume() const;

    //! Returns builder fox LevelSetLiquidSolver2.
    [[nodiscard]] static Builder GetBuilder();

 protected:
    //! Called at the beginning of the time-step.
    void OnBeginAdvanceTimeStep(double timeIntervalInSeconds) override;

    //! Called at the end of the time-step.
    void OnEndAdvanceTimeStep(double timeIntervalInSeconds) override;

    //! Customizes advection step.
    void ComputeAdvection(double timeIntervalInSeconds) override;

    //!
    //! \brief Returns fluid region as a signed-distance field.
    //!
    //! This function returns fluid region as a signed-distance field. For this
    //! particular class, it returns the same field as the function
    //! LevelSetLiquidSolver2::signedDistanceField().
    //!
    [[nodiscard]] ScalarField2Ptr GetFluidSDF() const override;

 private:
    void Reinitialize(double currentCFL);

    void ExtrapolateVelocityToAir(double currentCFL);

    void AddVolume(double volDiff);

    size_t m_signedDistanceFieldId;
    LevelSetSolver2Ptr m_levelSetSolver;
    double m_minReinitializeDistance = 10.0;
    bool m_isGlobalCompensationEnabled = false;
    double m_lastKnownVolume = 0.0;
};

//! Shared pointer type for the LevelSetLiquidSolver2.
using LevelSetLiquidSolver2Ptr = std::shared_ptr<LevelSetLiquidSolver2>;

//!
//! \brief Front-end to create LevelSetLiquidSolver2 objects step by step.
//!
class LevelSetLiquidSolver2::Builder final
    : public GridFluidSolverBuilderBase2<Builder>
{
 public:
    //! Builds LevelSetLiquidSolver2.
    [[nodiscard]] LevelSetLiquidSolver2 Build() const;

    //! Builds shared pointer of LevelSetLiquidSolver2 instance.
    [[nodiscard]] LevelSetLiquidSolver2Ptr MakeShared() const;
};
}  // namespace CubbyFlow

#endif