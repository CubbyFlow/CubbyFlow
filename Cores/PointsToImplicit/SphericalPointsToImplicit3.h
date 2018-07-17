/*************************************************************************
> File Name: SphericalPointsToImplicit3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D points-to-implicit converter based on simple sphere model.
> Created Time: 2017/11/17
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SPHERICAL_POINTS_TO_IMPLICIT3_H
#define CUBBYFLOW_SPHERICAL_POINTS_TO_IMPLICIT3_H

#include <PointsToImplicit/PointsToImplicit3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D points-to-implicit converter based on simple sphere model.
	//!
	class SphericalPointsToImplicit3 final : public PointsToImplicit3
	{
	public:
		//! Constructs the converter with given sphere radius.
		SphericalPointsToImplicit3(double radius = 1.0, bool isOutputSDF = true);

		//! Converts the given points to implicit surface scalar field.
		void Convert(const ConstArrayAccessor1<Vector3D>& points, ScalarGrid3* output) const override;

	private:
		double m_radius = 1.0;
		bool m_isOutputSDF = true;
	};

	//! Shared pointer type for SphericalPointsToImplicit3.
	using SphericalPointsToImplicit3Ptr = std::shared_ptr<SphericalPointsToImplicit3>;
}

#endif