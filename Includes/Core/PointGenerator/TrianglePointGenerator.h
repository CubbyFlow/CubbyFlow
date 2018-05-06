/*************************************************************************
> File Name: TrianglePointGenerator.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Right triangle point generator.
> Created Time: 2017/06/03
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_TRIANGLE_POINT_GENERATOR_H
#define CUBBYFLOW_TRIANGLE_POINT_GENERATOR_H

#include <Core/PointGenerator/PointGenerator2.h>

namespace CubbyFlow
{
	//!
	//! \brief Right triangle point generator.
	//!
	class TrianglePointGenerator final : public PointGenerator2
	{
	public:
		//!
		//! \brief Invokes \p callback function for each right triangle points
		//! inside \p boundingBox.
		//!
		//! This function iterates every right triangle points inside \p boundingBox
		//! where \p spacing is the size of the right triangle structure.
		//!
		void ForEachPoint(const BoundingBox2D& boundingBox, double spacing,
			const std::function<bool(const Vector2D&)>& callback) const override;
	};

	using TrianglePointGeneratorPtr = std::shared_ptr<TrianglePointGenerator>;
}

#endif