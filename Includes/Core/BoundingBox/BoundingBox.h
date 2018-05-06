/*************************************************************************
> File Name: BoundingBox.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: Generic N-D axis-aligned bounding box class.
> Created Time: 2017/03/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_BOUNDING_BOX_H
#define CUBBYFLOW_BOUNDING_BOX_H

#include <Core/Vector/Vector.h>

namespace CubbyFlow
{
	//!
	//! \brief Generic N-D axis-aligned bounding box class.
	//!
	//! \tparam T - Real number type.
	//! \tparam N - Dimension.
	//!
	template <typename T, size_t N>
	class BoundingBox
	{
	public:
		static_assert(N > 0, "Size of static-sized box should be greater than zero.");

		using VectorType = Vector<T, N>;

		//! Lower corner of the bounding box.
		VectorType lowerCorner;

		//! Upper corner of the bounding box.
		VectorType upperCorner;

		//! Default constructor.
		BoundingBox();

		//! Constructs a box that tightly covers two points.
		BoundingBox(const VectorType& point1, const VectorType& point2);

		//! Constructs a box with other box instance.
		BoundingBox(const BoundingBox& other);

		//! Returns true of this box and other box overlaps.
		bool Overlaps(const BoundingBox& other) const;

		//! Returns true if the input point is inside of this box.
		bool Contains(const VectorType& point) const;

		//! Returns the mid-point of this box.
		VectorType MidPoint() const;

		//! Returns diagonal length of this box.
		T DiagonalLength() const;

		//! Returns squared diagonal length of this box.
		T DiagonalLengthSquared() const;
		
		//! Resets this box to initial state (min=infinite, max=-infinite).
		void Reset();

		//! Merges this and other point.
		void Merge(const VectorType& point);

		//! Merges this and other boxes.
		void Merge(const BoundingBox& other);

		//! Expands this box by given delta to all direction.
		//! If the width of the box was x, expand(y) will result a box with
		//! x+y+y width.
		void Expand(T delta);
	};
}

#include <Core/BoundingBox/BoundingBox-Impl.h>

#endif