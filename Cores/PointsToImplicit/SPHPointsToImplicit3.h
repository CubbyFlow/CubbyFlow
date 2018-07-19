/*************************************************************************
> File Name: SPHPointsToImplicit3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D points-to-implicit converter based on standard SPH kernel.
> Created Time: 2017/11/18
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SPH_POINTS_TO_IMPLICIT3_H
#define CUBBYFLOW_SPH_POINTS_TO_IMPLICIT3_H

#include <PointsToImplicit/PointsToImplicit3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D points-to-implicit converter based on standard SPH kernel.
	//!
	//! \see Müller, Matthias, David Charypar, and Markus Gross.
	//!      "Particle-based fluid simulation for interactive applications."
	//!      Proceedings of the 2003 ACM SIGGRAPH/Eurographics symposium on Computer
	//!      animation. Eurographics Association, 2003.
	//!
	class SPHPointsToImplicit3 final : public PointsToImplicit3
	{
	public:
		//! Constructs the converter with given kernel radius and cut-off density.
		SPHPointsToImplicit3(double kernelRadius = 1.0, double cutOffDensity = 0.5, bool isOutputSDF = true);

		//! Converts the given points to implicit surface scalar field.
		void Convert(const ConstArrayAccessor1<Vector3D>& points, ScalarGrid3* output) const override;

	private:
		double m_kernelRadius = 1.0;
		double m_cutOffDensity = 0.5;
		bool m_isOutputSDF = true;
	};

	//! Shared pointer type for SPHPointsToImplicit3 class.
	using SPHPointsToImplicit3Ptr = std::shared_ptr<SPHPointsToImplicit3>;
}

#endif