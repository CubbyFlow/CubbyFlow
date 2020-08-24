// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_GRID_FLUID_SOLVER3_HPP
#define CUBBYFLOW_GRID_FLUID_SOLVER3_HPP

#include <Core/Animation/PhysicsAnimation.hpp>
#include <Core/Collider/Collider3.hpp>
#include <Core/Emitter/GridEmitter3.hpp>
#include <Core/Grid/GridSystemData3.hpp>
#include <Core/Solver/Advection/AdvectionSolver3.hpp>
#include <Core/Solver/Grid/GridDiffusionSolver3.hpp>
#include <Core/Solver/Grid/GridPressureSolver3.hpp>

namespace CubbyFlow
{
//!
//! \brief Abstract base class for grid-based 3-D fluid solver.
//!
//! This is an abstract base class for grid-based 3-D fluid solver based on
//! Jos Stam's famous 1999 paper - "Stable Fluids". This solver takes fractional
//! step method as its foundation which is consisted of independent advection,
//! diffusion, external forces, and pressure projection steps. Each step is
//! configurable so that a custom step can be implemented. For example, if a
//! user wants to change the advection solver to her/his own implementation,
//! simply call GridFluidSolver3::setAdvectionSolver(newSolver).
//!
class GridFluidSolver3 : public PhysicsAnimation
{
 public:
    class Builder;

    //! Default constructor.
    GridFluidSolver3();

    //! Constructs solver with initial grid size.
    GridFluidSolver3(const Size3& resolution, const Vector3D& gridSpacing,
                     const Vector3D& gridOrigin);

    //! Default destructor.
    virtual ~GridFluidSolver3();

    //! Returns the gravity vector of the system.
    const Vector3D& GetGravity() const;

    //! Sets the gravity of the system.
    void SetGravity(const Vector3D& newGravity);

    //! Returns the viscosity coefficient.
    double GetViscosityCoefficient() const;

    //!
    //! \brief Sets the viscosity coefficient.
    //!
    //! This function sets the viscosity coefficient. Non-positive input will be
    //! clamped to zero.
    //!
    //! \param[in] newValue The new viscosity coefficient value.
    //!
    void SetViscosityCoefficient(double newValue);

    //!
    //! \brief Returns the CFL number from the current velocity field for given
    //! time interval.
    //!
    //! \param[in] timeIntervalInSeconds The time interval in seconds.
    //!
    double GetCFL(double timeIntervalInSeconds) const;

    //! Returns the max allowed CFL number.
    double GetMaxCFL() const;

    //! Sets the max allowed CFL number.
    void SetMaxCFL(double newCFL);

    //! Returns true if the solver is using compressed linear system.
    bool GetUseCompressedLinearSystem() const;

    //! Sets whether the solver should use compressed linear system.
    void SetUseCompressedLinearSystem(bool onoff);

    //! Returns the advection solver instance.
    const AdvectionSolver3Ptr& GetAdvectionSolver() const;

    //! Sets the advection solver.
    void SetAdvectionSolver(const AdvectionSolver3Ptr& newSolver);

    //! Returns the diffusion solver instance.
    const GridDiffusionSolver3Ptr& GetDiffusionSolver() const;

    //! Sets the diffusion solver.
    void SetDiffusionSolver(const GridDiffusionSolver3Ptr& newSolver);

    //! Returns the pressure solver instance.
    const GridPressureSolver3Ptr& GetPressureSolver() const;

    //! Sets the pressure solver.
    void SetPressureSolver(const GridPressureSolver3Ptr& newSolver);

    //! Returns the closed domain boundary flag.
    int GetClosedDomainBoundaryFlag() const;

    //! Sets the closed domain boundary flag.
    void SetClosedDomainBoundaryFlag(int flag);

    //!
    //! \brief Returns the grid system data.
    //!
    //! This function returns the grid system data. The grid system data stores
    //! the core fluid flow fields such as velocity. By default, the data
    //! instance has velocity field only.
    //!
    //! \see GridSystemData3
    //!
    const GridSystemData3Ptr& GetGridSystemData() const;

    //!
    //! \brief Resizes grid system data.
    //!
    //! This function resizes grid system data. You can also resize the grid by
    //! calling resize function directly from
    //! GridFluidSolver3::gridSystemData(), but this function provides a
    //! shortcut for the same operation.
    //!
    //! \param[in] newSize        The new size.
    //! \param[in] newGridSpacing The new grid spacing.
    //! \param[in] newGridOrigin  The new grid origin.
    //!
    void ResizeGrid(const Size3& newSize, const Vector3D& newGridSpacing,
                    const Vector3D& newGridOrigin) const;

    //!
    //! \brief Returns the resolution of the grid system data.
    //!
    //! This function returns the resolution of the grid system data. This is
    //! equivalent to calling gridSystemData()->resolution(), but provides a
    //! shortcut.
    //!
    Size3 GetResolution() const;

    //!
    //! \brief Returns the grid spacing of the grid system data.
    //!
    //! This function returns the resolution of the grid system data. This is
    //! equivalent to calling gridSystemData()->gridSpacing(), but provides a
    //! shortcut.
    //!
    Vector3D GetGridSpacing() const;

    //!
    //! \brief Returns the origin of the grid system data.
    //!
    //! This function returns the resolution of the grid system data. This is
    //! equivalent to calling gridSystemData()->origin(), but provides a
    //! shortcut.
    //!
    Vector3D GetGridOrigin() const;

    //!
    //! \brief Returns the velocity field.
    //!
    //! This function returns the velocity field from the grid system data.
    //! It is just a shortcut to the most commonly accessed data chunk.
    //!
    const FaceCenteredGrid3Ptr& GetVelocity() const;

    //! Returns the collider.
    const Collider3Ptr& GetCollider() const;

    //! Sets the collider.
    void SetCollider(const Collider3Ptr& newCollider);

    //! Returns the emitter.
    const GridEmitter3Ptr& GetEmitter() const;

    //! Sets the emitter.
    void SetEmitter(const GridEmitter3Ptr& newEmitter);

    //! Returns builder fox GridFluidSolver3.
    static Builder GetBuilder();

 protected:
    //! Called when it needs to setup initial condition.
    void OnInitialize() override;

    //! Called when advancing a single time-step.
    void OnAdvanceTimeStep(double timeIntervalInSeconds) override;

    //!
    //! \brief Returns the required sub-time-steps for given time interval.
    //!
    //! This function returns the required sub-time-steps for given time
    //! interval based on the max allowed CFL number. If the time interval is
    //! too large so that it makes the CFL number greater than the max value,
    //! This function will return a number that is greater than 1.
    //!
    //! \see GridFluidSolver3::GetMaxCFL
    //!
    unsigned int GetNumberOfSubTimeSteps(
        double timeIntervalInSeconds) const override;

    //! Called at the beginning of a time-step.
    virtual void OnBeginAdvanceTimeStep(double timeIntervalInSeconds);

    //! Called at the end of a time-step.
    virtual void OnEndAdvanceTimeStep(double timeIntervalInSeconds);

    //!
    //! \brief Computes the external force terms.
    //!
    //! This function computes the external force applied for given time
    //! interval. By default, it only computes the gravity.
    //!
    //! \see GridFluidSolver3::ComputeGravity
    //!
    virtual void ComputeExternalForces(double timeIntervalInSeconds);

    //! Computes the viscosity term using the diffusion solver.
    virtual void ComputeViscosity(double timeIntervalInSeconds);

    //! Computes the pressure term using the pressure solver.
    virtual void ComputePressure(double timeIntervalInSeconds);

    //! Computes the advection term using the advection solver.
    virtual void ComputeAdvection(double timeIntervalInSeconds);

    //!
    //! \brief Returns the signed-distance representation of the fluid.
    //!
    //! This function returns the signed-distance representation of the fluid.
    //! Positive sign area is considered to be atmosphere and won't be included
    //! for computing the dynamics. By default, this will return constant scalar
    //! field of -std::numeric_limits<double>::max(), meaning that
    //! the entire volume is occupied with fluid.
    //!
    virtual ScalarField3Ptr GetFluidSDF() const;

    //! Computes the gravity term.
    void ComputeGravity(double timeIntervalInSeconds);

    //!
    //! \brief Applies the boundary condition to the velocity field.
    //!
    //! This function applies the boundary condition to the velocity field by
    //! constraining the flow based on the boundary condition solver.
    //!
    void ApplyBoundaryCondition() const;

    //! Extrapolates given field into the collider-occupied region.
    void ExtrapolateIntoCollider(ScalarGrid3* grid);

    //! Extrapolates given field into the collider-occupied region.
    void ExtrapolateIntoCollider(CollocatedVectorGrid3* grid);

    //! Extrapolates given field into the collider-occupied region.
    void ExtrapolateIntoCollider(FaceCenteredGrid3* grid);

    //! Returns the signed-distance field representation of the collider.
    ScalarField3Ptr GetColliderSDF() const;

    //! Returns the velocity field of the collider.
    VectorField3Ptr GetColliderVelocityField() const;

 private:
    Vector3D m_gravity = Vector3D(0.0, -9.8, 0.0);
    double m_viscosityCoefficient = 0.0;
    double m_maxCFL = 5.0;
    bool m_useCompressedLinearSys = false;
    int m_closedDomainBoundaryFlag = DIRECTION_ALL;

    GridSystemData3Ptr m_grids;
    Collider3Ptr m_collider;
    GridEmitter3Ptr m_emitter;

    AdvectionSolver3Ptr m_advectionSolver;
    GridDiffusionSolver3Ptr m_diffusionSolver;
    GridPressureSolver3Ptr m_pressureSolver;
    GridBoundaryConditionSolver3Ptr m_boundaryConditionSolver;

    void BeginAdvanceTimeStep(double timeIntervalInSeconds);

    void EndAdvanceTimeStep(double timeIntervalInSeconds);

    void UpdateCollider(double timeIntervalInSeconds) const;

    void UpdateEmitter(double timeIntervalInSeconds) const;
};

//! Shared pointer type for the GridFluidSolver3.
using GridFluidSolver3Ptr = std::shared_ptr<GridFluidSolver3>;

//!
//! \brief Base class for grid-based fluid solver builder.
//!
template <typename DerivedBuilder>
class GridFluidSolverBuilderBase3
{
 public:
    //! Returns builder with grid resolution.
    DerivedBuilder& WithResolution(const Size3& resolution);

    //! Returns builder with grid spacing.
    DerivedBuilder& WithGridSpacing(const Vector3D& gridSpacing);

    //! Returns builder with grid spacing.
    DerivedBuilder& WithGridSpacing(double gridSpacing);

    //!
    //! \brief Returns builder with domain size in x-direction.
    //!
    //! To build a solver, one can use either grid spacing directly or domain
    //! size in x-direction to set the final grid spacing.
    //!
    DerivedBuilder& WithDomainSizeX(double domainSizeX);

    //! Returns builder with grid origin
    DerivedBuilder& WithOrigin(const Vector3D& gridOrigin);

 protected:
    Size3 m_resolution{ 1, 1, 1 };
    Vector3D m_gridSpacing{ 1, 1, 1 };
    Vector3D m_gridOrigin{ 0, 0, 0 };
    double m_domainSizeX = 1.0;
    bool m_useDomainSize = false;

    Vector3D GetGridSpacing() const;
};

template <typename T>
T& GridFluidSolverBuilderBase3<T>::WithResolution(const Size3& resolution)
{
    m_resolution = resolution;
    return static_cast<T&>(*this);
}

template <typename T>
T& GridFluidSolverBuilderBase3<T>::WithGridSpacing(const Vector3D& gridSpacing)
{
    m_gridSpacing = gridSpacing;
    m_useDomainSize = false;
    return static_cast<T&>(*this);
}

template <typename T>
T& GridFluidSolverBuilderBase3<T>::WithGridSpacing(double gridSpacing)
{
    m_gridSpacing.x = gridSpacing;
    m_gridSpacing.y = gridSpacing;
    m_gridSpacing.z = gridSpacing;
    m_useDomainSize = false;
    return static_cast<T&>(*this);
}

template <typename T>
T& GridFluidSolverBuilderBase3<T>::WithDomainSizeX(double domainSizeX)
{
    m_domainSizeX = domainSizeX;
    m_useDomainSize = true;
    return static_cast<T&>(*this);
}

template <typename T>
T& GridFluidSolverBuilderBase3<T>::WithOrigin(const Vector3D& gridOrigin)
{
    m_gridOrigin = gridOrigin;
    return static_cast<T&>(*this);
}

template <typename T>
Vector3D GridFluidSolverBuilderBase3<T>::GetGridSpacing() const
{
    Vector3D gridSpacing = m_gridSpacing;

    if (m_useDomainSize)
    {
        gridSpacing.Set(m_domainSizeX / static_cast<double>(m_resolution.x));
    }

    return gridSpacing;
}

//!
//! \brief Front-end to create GridFluidSolver3 objects step by step.
//!
class GridFluidSolver3::Builder final
    : public GridFluidSolverBuilderBase3<GridFluidSolver3::Builder>
{
 public:
    //! Builds GridFluidSolver3.
    GridFluidSolver3 Build() const;

    //! Builds shared pointer of GridFluidSolver3 instance.
    GridFluidSolver3Ptr MakeShared() const
    {
        return std::make_shared<GridFluidSolver3>(
            m_resolution, GetGridSpacing(), m_gridOrigin);
    }
};
}  // namespace CubbyFlow

#endif