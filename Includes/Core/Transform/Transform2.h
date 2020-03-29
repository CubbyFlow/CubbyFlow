/*************************************************************************
> File Name: Transform2.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Represents 2-D rigid body transform.
> Created Time: 2017/03/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_TRANSFORM2_H
#define CUBBYFLOW_TRANSFORM2_H

#include <Core/BoundingBox/BoundingBox2.hpp>
#include <Core/Ray/Ray2.hpp>
#include <Core/Vector/Vector2.h>

namespace CubbyFlow
{
	//!
	//! \brief Represents 2-D rigid body transform.
	//!
	class Transform2
	{
	public:
		//! Constructs identity transform.
		Transform2();

		//! Constructs a transform with translation and orientation.
		Transform2(const Vector2D& translation, double orientation);

		//! Returns the translation.
		const Vector2D& GetTranslation() const;

		//! Sets the translation.
		void SetTranslation(const Vector2D& translation);

		//! Returns the orientation in radians.
		double GetOrientation() const;

		//! Sets the orientation in radians.
		void SetOrientation(double orientation);

		//! Transforms a point in world coordinate to the local frame.
		Vector2D ToLocal(const Vector2D& pointInWorld) const;

		//! Transforms a direction in world coordinate to the local frame.
		Vector2D ToLocalDirection(const Vector2D& dirInWorld) const;

		//! Transforms a ray in world coordinate to the local frame.
		Ray2D ToLocal(const Ray2D& rayInWorld) const;

		//! Transforms a bounding box in world coordinate to the local frame.
		BoundingBox2D ToLocal(const BoundingBox2D& bboxInWorld) const;

		//! Transforms a point in local space to the world coordinate.
		Vector2D ToWorld(const Vector2D& pointInLocal) const;

		//! Transforms a direction in local space to the world coordinate.
		Vector2D ToWorldDirection(const Vector2D& dirInLocal) const;

		//! Transforms a ray in local space to the world coordinate.
		Ray2D ToWorld(const Ray2D& rayInLocal) const;

		//! Transforms a bounding box in local space to the world coordinate.
		BoundingBox2D ToWorld(const BoundingBox2D& bboxInLocal) const;

	private:
		Vector2D m_translation;
		double m_orientation = 0.0;
		double m_cosAngle = 1.0;
		double m_sinAngle = 0.0;
	};
}

#endif