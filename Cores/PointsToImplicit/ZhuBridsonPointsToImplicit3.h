/*************************************************************************
> File Name: ZhuBridsonPointsToImplicit3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D points-to-implicit converter based on Zhu and Bridson's method.
> Created Time: 2017/11/18
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ZHU_BRIDSON_POINTS_TO_IMPLICIT3_H
#define CUBBYFLOW_ZHU_BRIDSON_POINTS_TO_IMPLICIT3_H

#include <PointsToImplicit/PointsToImplicit3.h>

namespace CubbyFlow
{
	//!
	//! \brief 3-D points-to-implicit converter based on Zhu and Bridson's method.
	//!
	//! \see Zhu, Yongning, and Robert Bridson. "Animating sand as a fluid."
	//!      ACM Transactions on Graphics (TOG). Vol. 24. No. 3. ACM, 2005.
	//!
	class ZhuBridsonPointsToImplicit3 final : public PointsToImplicit3
	{
	public:
		//! Constructs the converter with given kernel radius and cut-off threshold.
		ZhuBridsonPointsToImplicit3(double kernelRadius = 1.0, double cutOffThreshold = 0.25, bool isOutputSDF = true);

		//! Converts the given points to implicit surface scalar field.
		void Convert(const ConstArrayAccessor1<Vector3D>& points, ScalarGrid3* output) const override;

	private:
		double m_kernelRadius = 1.0;
		double m_cutOffThreshold = 0.25;
		bool m_isOutputSDF = true;
	};

	//! Shared pointer type for ZhuBridsonPointsToImplicit3 class
	using ZhuBridsonPointsToImplicit3Ptr = std::shared_ptr<ZhuBridsonPointsToImplicit3>;
}

#endif