/*************************************************************************
> File Name: BoundingBox3.h
> Project Name: CubbyFlow
> Author: Chan-Ho Chris Ohk
> Purpose: 3-D axis-aligned bounding box class.
> Created Time: 2017/03/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_BOUNDING_BOX3_H
#define CUBBYFLOW_BOUNDING_BOX3_H

#include <Core/BoundingBox/BoundingBox.h>
#include <Core/Ray/Ray3.h>
#include <Core/Vector/Vector3.h>

#include <limits>

namespace CubbyFlow
{
	//!
	//! \brief 3-D box-ray intersection result.
	//!
	//! \tparam T - The value type.
	//!
	template <typename T>
	struct BoundingBoxRayIntersection3
	{
		//! True if the box and ray intersects.
		bool isIntersecting = false;

		//! Distance to the first intersection point.
		T near = std::numeric_limits<T>::max();

		//! Distance to the second (and the last) intersection point.
		T far = std::numeric_limits<T>::max();
	};

	//!
	//! \brief 3-D axis-aligned bounding box class.
	//!
	//! \tparam T - Real number type.
	//!
	template <typename T>
	class BoundingBox<T, 3>
	{
	public:
		//! Lower corner of the bounding box.
		Vector3<T> lowerCorner;

		//! Upper corner of the bounding box.
		Vector3<T> upperCorner;

		//! Default constructor.
		BoundingBox();

		//! Constructs a box that tightly covers two points.
		BoundingBox(const Vector3<T>& point1, const Vector3<T>& point2);

		//! Constructs a box with other box instance.
		BoundingBox(const BoundingBox& other);
		
		//! Returns width of the box.
		T GetWidth() const;

		//! Returns height of the box.
		T GetHeight() const;

		//! Returns depth of the box.
		T GetDepth() const;

		//! Returns length of the box in given axis.
		T Length(size_t axis);

		//! Returns true of this box and other box overlaps.
		bool Overlaps(const BoundingBox& other) const;

		//! Returns true if the input vector is inside of this box.
		bool Contains(const Vector3<T>& point) const;

		//! Returns true if the input ray is intersecting with this box.
		bool Intersects(const Ray3<T>& ray) const;

		//! Returns intersection.isIntersecting = true if the input ray is
		//! intersecting with this box. If intersects, intersection.tNear is
		//! assigned with distant to the closest intersecting point, and
		//! intersection.tFar with furthest.
		BoundingBoxRayIntersection3<T> ClosestIntersection(const Ray3<T>& ray) const;

		//! Returns the mid-point of this box.
		Vector3<T> MidPoint() const;

		//! Returns diagonal length of this box.
		T DiagonalLength() const;

		//! Returns squared diagonal length of this box.
		T DiagonalLengthSquared() const;

		//! Resets this box to initial state (min=infinite, max=-infinite).
		void Reset();

		//! Merges this and other point.
		void Merge(const Vector3<T>& point);

		//! Merges this and other box.
		void Merge(const BoundingBox& other);

		//! Expands this box by given delta to all direction.
		//! If the width of the box was x, expand(y) will result a box with
		//! x+y+y width.
		void Expand(T delta);

		//! Returns corner position. Index starts from x-first order.
		Vector3<T> Corner(size_t idx) const;

		//! Returns the clamped point.
		Vector3<T> Clamp(const Vector3<T>& pt) const;

		//! Returns true if the box is empty.
		bool IsEmpty() const;
	};

	//! Type alias for 3-D BoundingBox.
	template <typename T> using BoundingBox3 = BoundingBox<T, 3>;

	//! Float-type 3-D BoundingBox.
	using BoundingBox3F = BoundingBox3<float>;

	//! Double-type 3-D BoundingBox.
	using BoundingBox3D = BoundingBox3<double>;

	//! Float-type 3-D box-ray intersection result.
	using BoundingBoxRayIntersection3F = BoundingBoxRayIntersection3<float>;

	//! Double-type 3-D box-ray intersection result.
	using BoundingBoxRayIntersection3D = BoundingBoxRayIntersection3<double>;
}

#include <Core/BoundingBox/BoundingBox3-Impl.h>

#endif