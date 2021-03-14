// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_SMOKE_SOLVER2_HPP
#define CUBBYFLOW_GRID_SMOKE_SOLVER2_HPP

#include <Core/Solver/Grid/GridFluidSolver2.hpp>

namespace CubbyFlow
{
//!
//! \brief      2-D grid-based smoke solver.
//!
//! This class extends GridFluidSolver2 to implement smoke simulation solver.
//! It adds smoke density and temperature fields to define the smoke and uses
//! buoyancy force to simulate hot rising smoke.
//!
//! \see Fedkiw, Ronald, Jos Stam, and Henrik Wann Jensen.
//!     "Visual simulation of smoke." Proceedings of the 28th annual conference
//!     on Computer graphics and interactive techniques. ACM, 2001.
//!
class GridSmokeSolver2 : public GridFluidSolver2
{
 public:
    class Builder;

    //! Default constructor.
    GridSmokeSolver2();

    //! Constructs solver with initial grid size.
    GridSmokeSolver2(const Vector2UZ& resolution, const Vector2D& gridSpacing,
                     const Vector2D& gridOrigin);

    //! Deleted copy constructor.
    GridSmokeSolver2(const GridSmokeSolver2&) = delete;

    //! Deleted move constructor.
    GridSmokeSolver2(GridSmokeSolver2&&) noexcept = delete;

    //! Default virtual destructor.
    ~GridSmokeSolver2() override = default;

    //! Deleted copy assignment operator.
    GridSmokeSolver2& operator=(const GridSmokeSolver2&) = delete;

    //! Deleted move assignment operator.
    GridSmokeSolver2& operator=(GridSmokeSolver2&&) noexcept = delete;

    //! Returns smoke diffusion coefficient.
    [[nodiscard]] double GetSmokeDiffusionCoefficient() const;

    //! Sets smoke diffusion coefficient.
    void SetSmokeDiffusionCoefficient(double newValue);

    //! Returns temperature diffusion coefficient.
    [[nodiscard]] double GetTemperatureDiffusionCoefficient() const;

    //! Sets temperature diffusion coefficient.
    void SetTemperatureDiffusionCoefficient(double newValue);

    //!
    //! \brief      Returns the buoyancy factor which will be multiplied to the
    //!     smoke density.
    //!
    //! This class computes buoyancy by looking up the value of smoke density
    //! and temperature, compare them to the average values, and apply
    //! multiplier factor to the diff between the value and the average. That
    //! multiplier is defined for each smoke density and temperature separately.
    //! For example, negative smoke density buoyancy factor means a heavier
    //! smoke should sink.
    //!
    //! \return     The buoyancy factor for the smoke density.
    //!
    [[nodiscard]] double GetBuoyancySmokeDensityFactor() const;

    //!
    //! \brief      Sets the buoyancy factor which will be multiplied to the
    //!     smoke density.
    //!
    //! This class computes buoyancy by looking up the value of smoke density
    //! and temperature, compare them to the average values, and apply
    //! multiplier factor to the diff between the value and the average. That
    //! multiplier is defined for each smoke density and temperature separately.
    //! For example, negative smoke density buoyancy factor means a heavier
    //! smoke should sink.
    //!
    //! \param newValue The new buoyancy factor for smoke density.
    //!
    void SetBuoyancySmokeDensityFactor(double newValue);

    //!
    //! \brief      Returns the buoyancy factor which will be multiplied to the
    //!     temperature.
    //!
    //! This class computes buoyancy by looking up the value of smoke density
    //! and temperature, compare them to the average values, and apply
    //! multiplier factor to the diff between the value and the average. That
    //! multiplier is defined for each smoke density and temperature separately.
    //! For example, negative smoke density buoyancy factor means a heavier
    //! smoke should sink.
    //!
    //! \return     The buoyancy factor for the temperature.
    //!
    [[nodiscard]] double GetBuoyancyTemperatureFactor() const;

    //!
    //! \brief      Sets the buoyancy factor which will be multiplied to the
    //!     temperature.
    //!
    //! This class computes buoyancy by looking up the value of smoke density
    //! and temperature, compare them to the average values, and apply
    //! multiplier factor to the diff between the value and the average. That
    //! multiplier is defined for each smoke density and temperature separately.
    //! For example, negative smoke density buoyancy factor means a heavier
    //! smoke should sink.
    //!
    //! \param newValue The new buoyancy factor for temperature.
    //!
    void SetBuoyancyTemperatureFactor(double newValue);

    //!
    //! \brief      Returns smoke decay factor.
    //!
    //! In addition to the diffusion, the smoke also can fade-out over time by
    //! setting the decay factor between 0 and 1.
    //!
    //! \return     The decay factor for smoke density.
    //!
    [[nodiscard]] double GetSmokeDecayFactor() const;

    //!
    //! \brief      Sets the smoke decay factor.
    //!
    //! In addition to the diffusion, the smoke also can fade-out over time by
    //! setting the decay factor between 0 and 1.
    //!
    //! \param[in]  newValue The new decay factor.
    //!
    void SetSmokeDecayFactor(double newValue);

    //!
    //! \brief      Returns temperature decay factor.
    //!
    //! In addition to the diffusion, the smoke also can fade-out over time by
    //! setting the decay factor between 0 and 1.
    //!
    //! \return     The decay factor for smoke temperature.
    //!
    [[nodiscard]] double GetTemperatureDecayFactor() const;

    //!
    //! \brief      Sets the temperature decay factor.
    //!
    //! In addition to the diffusion, the temperature also can fade-out over
    //! time by setting the decay factor between 0 and 1.
    //!
    //! \param[in]  newValue The new decay factor.
    //!
    void SetTemperatureDecayFactor(double newValue);

    //! Returns smoke density field.
    [[nodiscard]] ScalarGrid2Ptr GetSmokeDensity() const;

    //! Returns temperature field.
    [[nodiscard]] ScalarGrid2Ptr GetTemperature() const;

    //! Returns builder fox GridSmokeSolver2.
    [[nodiscard]] static Builder GetBuilder();

 protected:
    void OnEndAdvanceTimeStep(double timeIntervalInSeconds) override;

    void ComputeExternalForces(double timeIntervalInSeconds) override;

 private:
    void ComputeDiffusion(double timeIntervalInSeconds);

    void ComputeBuoyancyForce(double timeIntervalInSeconds);

    size_t m_smokeDensityDataID = 0;
    size_t m_temperatureDataID = 0;
    double m_smokeDiffusionCoefficient = 0.0;
    double m_temperatureDiffusionCoefficient = 0.0;
    double m_buoyancySmokeDensityFactor = -0.000625;
    double m_buoyancyTemperatureFactor = 5.0;
    double m_smokeDecayFactor = 0.001;
    double m_temperatureDecayFactor = 0.001;
};

//! Shared pointer type for the GridSmokeSolver2.
using GridSmokeSolver2Ptr = std::shared_ptr<GridSmokeSolver2>;

//!
//! \brief Front-end to create GridSmokeSolver2 objects step by step.
//!
class GridSmokeSolver2::Builder final
    : public GridFluidSolverBuilderBase2<Builder>
{
 public:
    //! Builds GridSmokeSolver2.
    [[nodiscard]] GridSmokeSolver2 Build() const;

    //! Builds shared pointer of GridSmokeSolver2 instance.
    [[nodiscard]] GridSmokeSolver2Ptr MakeShared() const;
};
}  // namespace CubbyFlow

#endif