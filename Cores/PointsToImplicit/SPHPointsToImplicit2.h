/*************************************************************************
> File Name: SPHPointsToImplicit2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D points-to-implicit converter based on standard SPH kernel.
> Created Time: 2017/11/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SPH_POINTS_TO_IMPLICIT2_H
#define CUBBYFLOW_SPH_POINTS_TO_IMPLICIT2_H

#include <PointsToImplicit/PointsToImplicit2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D points-to-implicit converter based on standard SPH kernel.
	//!
	//! \see Müller, Matthias, David Charypar, and Markus Gross.
	//!      "Particle-based fluid simulation for interactive applications."
	//!      Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
	//!      animation. Eurographics Association, 2003.
	//!
	class SPHPointsToImplicit2 final : public PointsToImplicit2
	{
	public:
		//! Constructs the converter with given kernel radius and cut-off density.
		SPHPointsToImplicit2(double kernelRadius = 1.0, double cutOffDensity = 0.5, bool isOutputSDF = true);

		//! Converts the given points to implicit surface scalar field.
		void Convert(const ConstArrayAccessor1<Vector2D>& points, ScalarGrid2* output) const override;

	private:
		double m_kernelRadius = 1.0;
		double m_cutOffDensity = 0.5;
		bool m_isOutputSDF = true;
	};

	//! Shared pointer type for SPHPointsToImplicit2 class.
	using SPHPointsToImplicit2Ptr = std::shared_ptr<SPHPointsToImplicit2>;
}

#endif