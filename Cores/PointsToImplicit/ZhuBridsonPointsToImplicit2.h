/*************************************************************************
> File Name: ZhuBridsonPointsToImplicit2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 2-D points-to-implicit converter based on Zhu and Bridson's method.
> Created Time: 2017/11/18
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ZHU_BRIDSON_POINTS_TO_IMPLICIT2_H
#define CUBBYFLOW_ZHU_BRIDSON_POINTS_TO_IMPLICIT2_H

#include <PointsToImplicit/PointsToImplicit2.h>

namespace CubbyFlow
{
	//!
	//! \brief 2-D points-to-implicit converter based on Zhu and Bridson's method.
	//!
	//! \see Zhu, Yongning, and Robert Bridson. "Animating sand as a fluid."
	//!      ACM Transactions on Graphics (TOG). Vol. 24. No. 3. ACM, 2005.
	//!
	class ZhuBridsonPointsToImplicit2 final : public PointsToImplicit2
	{
	public:
		//! Constructs the converter with given kernel radius and cut-off threshold.
		ZhuBridsonPointsToImplicit2(double kernelRadius = 1.0, double cutOffThreshold = 0.25, bool isOutputSDF = true);

		//! Converts the given points to implicit surface scalar field.
		void Convert(const ConstArrayAccessor1<Vector2D>& points, ScalarGrid2* output) const override;

	private:
		double m_kernelRadius = 1.0;
		double m_cutOffThreshold = 0.25;
		bool m_isOutputSDF = true;
	};

	//! Shared pointer type for ZhuBridsonPointsToImplicit2 class
	using ZhuBridsonPointsToImplicit2Ptr = std::shared_ptr<ZhuBridsonPointsToImplicit2>;
}

#endif