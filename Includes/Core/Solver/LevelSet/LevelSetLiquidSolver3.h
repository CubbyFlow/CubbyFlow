/*************************************************************************
> File Name: LevelSetLiquidSolver3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Level set based 3-D liquid solver.
> Created Time: 2017/09/06
> Copyright (c) 2018, Dongmin Kim
*************************************************************************/
#ifndef CUBBYFLOW_LEVEL_SET_LIQUID_SOLVER3_H
#define CUBBYFLOW_LEVEL_SET_LIQUID_SOLVER3_H

#include <Core/Solver/Grid/GridFluidSolver3.h>
#include <Core/Solver/LevelSet/LevelSetSolver3.h>

namespace CubbyFlow
{
	//!
	//! \brief      Level set based 3-D liquid solver.
	//!
	//! This class implements level set-based 3-D liquid solver. It defines the
	//! surface of the liquid using signed-distance field and use stable fluids
	//! framework to compute the forces.
	//!
	//! \see Enright, Douglas, Stephen Marschner, and Ronald Fedkiw.
	//!     "Animation and rendering of complex water surfaces." ACM Transactions on
	//!     Graphics (TOG). Vol. 21. No. 3. ACM, 2002.
	//!
	class LevelSetLiquidSolver3 : public GridFluidSolver3
	{
	public:
		class Builder;

		//! Default constructor.
		LevelSetLiquidSolver3();

		//! Constructs solver with initial grid size.
		LevelSetLiquidSolver3(
			const Size3& resolution,
			const Vector3D& gridSpacing,
			const Vector3D& gridOrigin);

		//! Destructor.
		virtual ~LevelSetLiquidSolver3();

		//! Returns signed-distance field.
		ScalarGrid3Ptr GetSignedDistanceField() const;

		//! Returns the level set solver.
		LevelSetSolver3Ptr GetLevelSetSolver() const;

		//! Sets the level set solver.
		void SetLevelSetSolver(const LevelSetSolver3Ptr& newSolver);

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
		//! 34, no. 1 (3005): 81-97.
		//!
		void SetIsGlobalCompensationEnabled(bool isEnabled);

		//!
		//! \brief Returns liquid volume measured by smeared Heaviside function.
		//!
		//! This function measures the liquid volume (area in 3-D) using smeared
		//! Heaviside function. Thus, the estimated volume is an approximated
		//! quantity.
		//!
		double ComputeVolume() const;

		//! Returns builder fox LevelSetLiquidSolver3.
		static Builder GetBuilder();

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
		//! LevelSetLiquidSolver3::signedDistanceField().
		//!
		ScalarField3Ptr GetFluidSDF() const override;

	private:
		size_t m_signedDistanceFieldId;
		LevelSetSolver3Ptr m_levelSetSolver;
		double m_minReinitializeDistance = 10.0;
		bool m_isGlobalCompensationEnabled = false;
		double m_lastKnownVolume = 0.0;

		void Reinitialize(double currentCFL);

		void ExtrapolateVelocityToAir(double currentCFL);

		void AddVolume(double volDiff);
	};

	//! Shared pointer type for the LevelSetLiquidSolver3.
	using LevelSetLiquidSolver3Ptr = std::shared_ptr<LevelSetLiquidSolver3>;

	//!
	//! \brief Front-end to create LevelSetLiquidSolver3 objects step by step.
	//!
	class LevelSetLiquidSolver3::Builder final : public GridFluidSolverBuilderBase3<LevelSetLiquidSolver3::Builder>
	{
	public:
		//! Builds LevelSetLiquidSolver3.
		LevelSetLiquidSolver3 Build() const;

		//! Builds shared pointer of LevelSetLiquidSolver3 instance.
		LevelSetLiquidSolver3Ptr MakeShared() const;
	};
}

#endif