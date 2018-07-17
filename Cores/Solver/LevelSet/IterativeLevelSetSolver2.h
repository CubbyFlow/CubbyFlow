/*************************************************************************
> File Name: IterativeLevelSetSolver2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Abstract base class for 2-D PDE-based iterative level set solver.
> Created Time: 2017/08/30
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ITERATIVE_LEVEL_SET_SOLVER2_H
#define CUBBYFLOW_ITERATIVE_LEVEL_SET_SOLVER2_H

#include <Solver/LevelSet/LevelSetSolver2.h>

namespace CubbyFlow
{
	//!
	//! \brief Abstract base class for 2-D PDE-based iterative level set solver.
	//!
	//! This class provides infrastructure for 2-D PDE-based iterative level set
	//! solver. Internally, the class implements upwind-style wave propagation and
	//! the inheriting classes must provide a way to compute the derivatives for
	//! given grid points.
	//!
	//! \see Osher, Stanley, and Ronald Fedkiw. Level set methods and dynamic
	//!     implicit surfaces. Vol. 153. Springer Science & Business Media, 2006.
	//!
	class IterativeLevelSetSolver2 : public LevelSetSolver2
	{
	public:
		//! Default constructor.
		IterativeLevelSetSolver2();

		//! Default destructor.
		virtual ~IterativeLevelSetSolver2();

		//!
		//! Reinitializes given scalar field to signed-distance field.
		//!
		//! \param inputSDF Input signed-distance field which can be distorted.
		//! \param maxDistance Max range of reinitialization.
		//! \param outputSDF Output signed-distance field.
		//!
		void Reinitialize(
			const ScalarGrid2& inputSDF,
			double maxDistance,
			ScalarGrid2* outputSDF) override;

		//!
		//! Extrapolates given scalar field from negative to positive SDF region.
		//!
		//! \param input Input scalar field to be extrapolated.
		//! \param sdf Reference signed-distance field.
		//! \param maxDistance Max range of extrapolation.
		//! \param output Output scalar field.
		//!
		void Extrapolate(
			const ScalarGrid2& input,
			const ScalarField2& sdf,
			double maxDistance,
			ScalarGrid2* output) override;

		//!
		//! Extrapolates given collocated vector field from negative to positive SDF
		//! region.
		//!
		//! \param input Input collocated vector field to be extrapolated.
		//! \param sdf Reference signed-distance field.
		//! \param maxDistance Max range of extrapolation.
		//! \param output Output collocated vector field.
		//!
		void Extrapolate(
			const CollocatedVectorGrid2& input,
			const ScalarField2& sdf,
			double maxDistance,
			CollocatedVectorGrid2* output) override;

		//!
		//! Extrapolates given face-centered vector field from negative to positive
		//! SDF region.
		//!
		//! \param input Input face-centered field to be extrapolated.
		//! \param sdf Reference signed-distance field.
		//! \param maxDistance Max range of extrapolation.
		//! \param output Output face-centered vector field.
		//!
		void Extrapolate(
			const FaceCenteredGrid2& input,
			const ScalarField2& sdf,
			double maxDistance,
			FaceCenteredGrid2* output) override;

		//! Returns the maximum CFL limit.
		double GetMaxCFL() const;

		//!
		//! \brief Sets the maximum CFL limit.
		//!
		//! This function sets the maximum CFL limit for the internal upwind-style
		//! PDE calculation. The negative input will be clamped to 0.
		//!
		void SetMaxCFL(double newMaxCfl);

	protected:
		//! Computes the derivatives for given grid point.
		virtual void GetDerivatives(
			ConstArrayAccessor2<double> grid,
			const Vector2D& gridSpacing,
			size_t i, size_t j,
			std::array<double, 2>* dx, std::array<double, 2>* dy) const = 0;

	private:
		double m_maxCFL = 0.5;

		void Extrapolate(
			const ConstArrayAccessor2<double>& input,
			const ConstArrayAccessor2<double>& sdf,
			const Vector2D& gridSpacing,
			double maxDistance,
			ArrayAccessor2<double> output);

		static unsigned int DistanceToNumberOfIterations(double distance, double dtau);

		static double Sign(
			const ConstArrayAccessor2<double>& sdf,
			const Vector2D& gridSpacing,
			size_t i, size_t j);

		double PseudoTimeStep(
			ConstArrayAccessor2<double> sdf,
			const Vector2D& gridSpacing) const;
	};

	using IterativeLevelSetSolver2Ptr = std::shared_ptr<IterativeLevelSetSolver2>;
}

#endif