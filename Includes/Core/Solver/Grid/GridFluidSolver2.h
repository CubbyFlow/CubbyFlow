/*************************************************************************
> File Name: GridFluidSolver2.h
> Project Name: CubbyFlow
> Author: Dongmin Kim
> Purpose: Abstract base class for grid-based 2-D fluid solver.
> Created Time: 2017/08/10
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_GRID_FLUID_SOLVER2_H
#define CUBBYFLOW_GRID_FLUID_SOLVER2_H

#include <Core/Animation/PhysicsAnimation.h>
#include <Core/Collider/Collider2.h>
#include <Core/Emitter/GridEmitter2.h>
#include <Core/Grid/GridSystemData2.h>
#include <Core/Solver/Advection/AdvectionSolver2.h>
#include <Core/Solver/Grid/GridDiffusionSolver2.h>
#include <Core/Solver/Grid/GridPressureSolver2.h>

namespace CubbyFlow
{
	//!
	//! \brief Abstract base class for grid-based 2-D fluid solver.
	//!
	//! This is an abstract base class for grid-based 2-D fluid solver based on
	//! Jos Stam's famous 1999 paper - "Stable Fluids". This solver takes fractional
	//! step method as its foundation which is consisted of independent advection,
	//! diffusion, external forces, and pressure projection steps. Each step is
	//! configurable so that a custom step can be implemented. For example, if a
	//! user wants to change the advection solver to her/his own implementation,
	//! simply call GridFluidSolver2::SetAdvectionSolver(newSolver).
	//!
	class GridFluidSolver2 : public PhysicsAnimation
	{
	public:
		class Builder;

		//! Default constructor.
		GridFluidSolver2();

		//! Constructs solver with initial grid size.
		GridFluidSolver2(
			const Size2& resolution,
			const Vector2D& gridSpacing,
			const Vector2D& gridOrigin);

		//! Default destructor.
		virtual ~GridFluidSolver2();

		//! Returns the gravity vector of the system.
		const Vector2D& GetGravity() const;

		//! Sets the gravity of the system.
		void SetGravity(const Vector2D& newGravity);

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
		//! \brief Returns the CFL number from the current velocity field for given time interval.
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
		const AdvectionSolver2Ptr& GetAdvectionSolver() const;

		//! Sets the advection solver.
		void SetAdvectionSolver(const AdvectionSolver2Ptr& newSolver);

		//! Returns the diffusion solver instance.
		const GridDiffusionSolver2Ptr& GetDiffusionSolver() const;

		//! Sets the diffusion solver.
		void SetDiffusionSolver(const GridDiffusionSolver2Ptr& newSolver);

		//! Returns the pressure solver instance.
		const GridPressureSolver2Ptr& GetPressureSolver() const;

		//! Sets the pressure solver.
		void SetPressureSolver(const GridPressureSolver2Ptr& newSolver);

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
		//! \see GridSystemData2
		//!
		const GridSystemData2Ptr& GetGridSystemData() const;

		//!
		//! \brief Resizes grid system data.
		//!
		//! This function resizes grid system data. You can also resize the grid by
		//! calling resize function directly from
		//! GridFluidSolver2::GetGridSystemData(), but this function provides a
		//! shortcut for the same operation.
		//!
		//! \param[in] newSize        The new size.
		//! \param[in] newGridSpacing The new grid spacing.
		//! \param[in] newGridOrigin  The new grid origin.
		//!
		void ResizeGrid(
			const Size2& newSize,
			const Vector2D& newGridSpacing,
			const Vector2D& newGridOrigin) const;

		//!
		//! \brief Returns the resolution of the grid system data.
		//!
		//! This function returns the resolution of the grid system data. This is
		//! equivalent to calling GetGridSystemData()->Resolution(), but provides a
		//! shortcut.
		//!
		Size2 GetResolution() const;

		//!
		//! \brief Returns the grid spacing of the grid system data.
		//!
		//! This function returns the resolution of the grid system data. This is
		//! equivalent to calling GetGridSystemData()->GridSpacing(), but provides a
		//! shortcut.
		//!
		Vector2D GetGridSpacing() const;

		//!
		//! \brief Returns the origin of the grid system data.
		//!
		//! This function returns the resolution of the grid system data. This is
		//! equivalent to calling GetGridSystemData()->Origin(), but provides a
		//! shortcut.
		//!
		Vector2D GetGridOrigin() const;

		//!
		//! \brief Returns the velocity field.
		//!
		//! This function returns the velocity field from the grid system data.
		//! It is just a shortcut to the most commonly accessed data chunk.
		//!
		const FaceCenteredGrid2Ptr& GetVelocity() const;

		//! Returns the collider.
		const Collider2Ptr& GetCollider() const;

		//! Sets the collider.
		void SetCollider(const Collider2Ptr& newCollider);

		//! Returns the emitter.
		const GridEmitter2Ptr& GetEmitter() const;

		//! Sets the emitter.
		void SetEmitter(const GridEmitter2Ptr& newEmitter);

		//! Returns builder fox GridFluidSolver2.
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
		//! \see GridFluidSolver2::GetMaxCFL
		//!
		unsigned int GetNumberOfSubTimeSteps(double timeIntervalInSeconds) const override;

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
		//! \see GridFluidSolver2::ComputeGravity
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
		virtual ScalarField2Ptr GetFluidSDF() const;

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
		void ExtrapolateIntoCollider(ScalarGrid2* grid);

		//! Extrapolates given field into the collider-occupied region.
		void ExtrapolateIntoCollider(CollocatedVectorGrid2* grid);

		//! Extrapolates given field into the collider-occupied region.
		void ExtrapolateIntoCollider(FaceCenteredGrid2* grid);

		//! Returns the signed-distance field representation of the collider.
		ScalarField2Ptr GetColliderSDF() const;

		//! Returns the velocity field of the collider.
		VectorField2Ptr GetColliderVelocityField() const;

	private:
		Vector2D m_gravity = Vector2D(0.0, -9.8);
		double m_viscosityCoefficient = 0.0;
		double m_maxCFL = 5.0;
		bool m_useCompressedLinearSys = false;
		int m_closedDomainBoundaryFlag = DIRECTION_ALL;

		GridSystemData2Ptr m_grids;
		Collider2Ptr m_collider;
		GridEmitter2Ptr m_emitter;

		AdvectionSolver2Ptr m_advectionSolver;
		GridDiffusionSolver2Ptr m_diffusionSolver;
		GridPressureSolver2Ptr m_pressureSolver;
		GridBoundaryConditionSolver2Ptr m_boundaryConditionSolver;

		void BeginAdvanceTimeStep(double timeIntervalInSeconds);

		void EndAdvanceTimeStep(double timeIntervalInSeconds);

		void UpdateCollider(double timeIntervalInSeconds) const;

		void UpdateEmitter(double timeIntervalInSeconds) const;
	};

	//! Shared pointer type for the GridFluidSolver2.
	using GridFluidSolver2Ptr = std::shared_ptr<GridFluidSolver2>;
	
	//!
	//! \brief Base class for grid-based fluid solver builder.
	//!
	template <typename DerivedBuilder>
	class GridFluidSolverBuilderBase2
	{
	public:
		//! Returns builder with grid resolution.
		DerivedBuilder& WithResolution(const Size2& resolution);

		//! Returns builder with grid spacing.
		DerivedBuilder& WithGridSpacing(const Vector2D& gridSpacing);

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
		DerivedBuilder& WithOrigin(const Vector2D& gridOrigin);

	protected:
		Size2 m_resolution{ 1, 1 };
		Vector2D m_gridSpacing{ 1, 1 };
		Vector2D m_gridOrigin{ 0, 0 };
		double m_domainSizeX = 1.0;
		bool m_useDomainSize = false;

		Vector2D GetGridSpacing() const;
	};

	template <typename T>
	T& GridFluidSolverBuilderBase2<T>::WithResolution(const Size2& resolution)
	{
		m_resolution = resolution;
		return static_cast<T&>(*this);
	}

	template <typename T>
	T& GridFluidSolverBuilderBase2<T>::WithGridSpacing(const Vector2D& gridSpacing)
	{
		m_gridSpacing = gridSpacing;
		m_useDomainSize = false;
		return static_cast<T&>(*this);
	}

	template <typename T>
	T& GridFluidSolverBuilderBase2<T>::WithGridSpacing(double gridSpacing)
	{
		m_gridSpacing.x = gridSpacing;
		m_gridSpacing.y = gridSpacing;
		m_useDomainSize = false;
		return static_cast<T&>(*this);
	}

	template <typename T>
	T& GridFluidSolverBuilderBase2<T>::WithDomainSizeX(double domainSizeX)
	{
		m_domainSizeX = domainSizeX;
		m_useDomainSize = true;
		return static_cast<T&>(*this);
	}

	template <typename T>
	T& GridFluidSolverBuilderBase2<T>::WithOrigin(const Vector2D& gridOrigin)
	{
		m_gridOrigin = gridOrigin;
		return static_cast<T&>(*this);
	}

	template <typename T>
	Vector2D GridFluidSolverBuilderBase2<T>::GetGridSpacing() const
	{
		Vector2D gridSpacing = m_gridSpacing;
		
		if (m_useDomainSize)
		{
			gridSpacing.Set(m_domainSizeX / static_cast<double>(m_resolution.x));
		}

		return gridSpacing;
	}

	//!
	//! \brief Front-end to create GridFluidSolver2 objects step by step.
	//!
	class GridFluidSolver2::Builder final : public GridFluidSolverBuilderBase2<GridFluidSolver2::Builder>
	{
	public:
		//! Builds GridFluidSolver2.
		GridFluidSolver2 Build() const;

		//! Builds shared pointer of GridFluidSolver2 instance.
		GridFluidSolver2Ptr MakeShared() const
		{
			return std::make_shared<GridFluidSolver2>(m_resolution, GetGridSpacing(), m_gridOrigin);
		}
	};
}

#endif