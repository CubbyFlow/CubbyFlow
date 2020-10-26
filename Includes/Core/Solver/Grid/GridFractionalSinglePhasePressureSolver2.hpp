// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FRACTIONAL_SINGLE_PHASE_PRESSURE_SOLVER2_HPP
#define CUBBYFLOW_FRACTIONAL_SINGLE_PHASE_PRESSURE_SOLVER2_HPP

#include <Core/FDM/FDMMGLinearSystem2.hpp>
#include <Core/Solver/FDM/FDMLinearSystemSolver2.hpp>
#include <Core/Solver/FDM/FDMMGSolver2.hpp>
#include <Core/Solver/Grid/GridPressureSolver2.hpp>

namespace CubbyFlow
{
//!
//! \brief 2-D fractional single-phase pressure solver.
//!
//! This class implements 2-D fractional (or variational) single-phase pressure
//! solver. It is called fractional because the solver encodes the boundaries
//! to the grid cells like anti-aliased pixels, meaning that a grid cell will
//! record the partially overlapping boundary as a fractional number.
//! Alternative approach is to represent boundaries like Lego blocks which is
//! the case for GridSinglePhasePressureSolver2.
//! In addition, this class solves single-phase flow, solving the pressure for
//! selected fluid region only and treat other area as an atmosphere region.
//! Thus, the pressure outside the fluid will be set to a constant value and
//! velocity field won't be altered. This solver also computes the fluid
//! boundary in fractional manner, meaning that the solver tries to capture the
//! sub-grid structures. This class uses ghost fluid method for such
//! calculation.
//!
//! \see Batty, Christopher, Florence Bertails, and Robert Bridson.
//!     "A fast variational framework for accurate solid-fluid coupling."
//!     ACM Transactions on Graphics (TOG). Vol. 26. No. 3. ACM, 2007.
//! \see Enright, Doug, et al. "Using the particle level set method and
//!     a second order accurate pressure boundary condition for free surface
//!     flows." ASME/JSME 2003 4th Joint Fluids Summer Engineering Conference.
//!     American Society of Mechanical Engineers, 2003.
//!
class GridFractionalSinglePhasePressureSolver2 final
    : public GridPressureSolver2
{
 public:
    //! Default constructor.
    GridFractionalSinglePhasePressureSolver2();

    //! Deleted copy constructor.
    GridFractionalSinglePhasePressureSolver2(
        const GridFractionalSinglePhasePressureSolver2&) = delete;

    //! Deleted move constructor.
    GridFractionalSinglePhasePressureSolver2(
        GridFractionalSinglePhasePressureSolver2&&) noexcept = delete;

    //! Default virtual destructor.
    ~GridFractionalSinglePhasePressureSolver2() override = default;

    //! Deleted copy assignment operator.
    GridFractionalSinglePhasePressureSolver2& operator=(
        const GridFractionalSinglePhasePressureSolver2&) = delete;

    //! Deleted move assignment operator.
    GridFractionalSinglePhasePressureSolver2& operator=(
        GridFractionalSinglePhasePressureSolver2&&) noexcept = delete;

    //!
    //! \brief Solves the pressure term and apply it to the velocity field.
    //!
    //! This function takes input velocity field and outputs pressure-applied
    //! velocity field. It also accepts extra arguments such as \p boundarySDF
    //! and \p fluidSDF that represent signed-distance representation of the
    //! boundary and fluid area. The negative region of \p boundarySDF means
    //! it is occupied by solid object. Also, the positive / negative area of
    //! the \p fluidSDF means it is occupied by fluid / atmosphere. If not
    //! specified, constant scalar field with std::numeric_limits<double>::max()
    //! will be used for \p boundarySDF meaning that no boundary at all.
    //! Similarly, a constant field with -std::numeric_limits<double>::max()
    //! will be used for \p fluidSDF which means it's fully occupied with fluid
    //! without any atmosphere.
    //!
    //! \param[in]     input                 The input velocity field.
    //! \param[in]     timeIntervalInSeconds The time interval for the sim.
    //! \param[in,out] output                The output velocity field.
    //! \param[in]     boundarySDF           The SDF of the boundary.
    //! \param[in]     boundaryVelocity      The velocity of the boundary.
    //! \param[in]     fluidSDF              The SDF of the fluid/atmosphere.
    //! \param[in]     useCompressed         True if it uses compressed system.
    //!
    void Solve(
        const FaceCenteredGrid2& input, double timeIntervalInSeconds,
        FaceCenteredGrid2* output,
        const ScalarField2& boundarySDF =
            ConstantScalarField2{ std::numeric_limits<double>::max() },
        const VectorField2& boundaryVelocity = ConstantVectorField2{ { 0, 0 } },
        const ScalarField2& fluidSDF =
            ConstantScalarField2{ -std::numeric_limits<double>::max() },
        bool useCompressed = false) override;

    //!
    //! \brief Returns the best boundary condition solver for this solver.
    //!
    //! This function returns the best boundary condition solver that works well
    //! with this pressure solver. Depending on the pressure solver
    //! implementation, different boundary condition solver might be used. For
    //! this particular class, an instance of
    //! GridFractionalBoundaryConditionSolver2 will be returned.
    //!
    [[nodiscard]] GridBoundaryConditionSolver2Ptr
    SuggestedBoundaryConditionSolver() const override;

    //! Returns the linear system solver.
    [[nodiscard]] const FDMLinearSystemSolver2Ptr& GetLinearSystemSolver()
        const;

    //! Sets the linear system solver.
    void SetLinearSystemSolver(const FDMLinearSystemSolver2Ptr& solver);

    //! Returns the pressure field.
    [[nodiscard]] const FDMVector2& GetPressure() const;

 private:
    void BuildWeights(const FaceCenteredGrid2& input,
                      const ScalarField2& boundarySDF,
                      const VectorField2& boundaryVelocity,
                      const ScalarField2& fluidSDF);

    void DecompressSolution();

    void BuildSystem(const FaceCenteredGrid2& input, bool useCompressed);

    void ApplyPressureGradient(const FaceCenteredGrid2& input,
                               FaceCenteredGrid2* output);

    FDMLinearSystem2 m_system;
    FDMCompressedLinearSystem2 m_compSystem;
    FDMLinearSystemSolver2Ptr m_systemSolver;

    FDMMGLinearSystem2 m_mgSystem;
    FDMMGSolver2Ptr m_mgSystemSolver;

    std::vector<Array2<double>> m_uWeights;
    std::vector<Array2<double>> m_vWeights;
    std::vector<Array2<double>> m_fluidSDF;

    std::function<Vector2D(const Vector2D&)> m_boundaryVel;
};

//! Shared pointer type for the GridFractionalSinglePhasePressureSolver2.
using GridFractionalSinglePhasePressureSolver2Ptr =
    std::shared_ptr<GridFractionalSinglePhasePressureSolver2>;
}  // namespace CubbyFlow

#endif