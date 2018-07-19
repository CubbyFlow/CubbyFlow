/*************************************************************************
> File Name: SphericalPointsToImplicit2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D points-to-implicit converter based on simple sphere model.
> Created Time: 2017/11/16
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SPHERICAL_POINTS_TO_IMPLICIT2_H
#define CUBBYFLOW_SPHERICAL_POINTS_TO_IMPLICIT2_H

#include <PointsToImplicit/PointsToImplicit2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D points-to-implicit converter based on simple sphere model.
	//!
	class SphericalPointsToImplicit2 final : public PointsToImplicit2
	{
	public:
		//! Constructs the converter with given sphere radius.
		SphericalPointsToImplicit2(double radius = 1.0, bool isOutputSdf = true);

		//! Converts the given points to implicit surface scalar field.
		void Convert(const ConstArrayAccessor1<Vector2D>& points, ScalarGrid2* output) const override;

	private:
		double m_radius = 1.0;
		bool m_isOutputSDF = true;
	};

	//! Shared pointer type for SphericalPointsToImplicit2.
	using SphericalPointsToImplicit2Ptr = std::shared_ptr<SphericalPointsToImplicit2>;
}

#endif