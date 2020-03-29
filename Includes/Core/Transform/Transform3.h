/*************************************************************************
> File Name: Transform3.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Represents 3-D rigid body transform.
> Created Time: 2017/03/29
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_TRANSFORM3_H
#define CUBBYFLOW_TRANSFORM3_H

#include <Core/BoundingBox/BoundingBox3.hpp>
#include <Core/Math/Quaternion.hpp>
#include <Core/Ray/Ray3.hpp>
#include <Core/Vector/Vector3.h>

namespace CubbyFlow
{
	//!
	//! \brief Represents 3-D rigid body transform.
	//!
	class Transform3
	{
	public:
		//! Constructs identity transform.
		Transform3();

		//! Constructs a transform with translation and orientation.
		Transform3(const Vector3D& translation, const QuaternionD& orientation);

		//! Returns the translation.
		const Vector3D& GetTranslation() const;

		//! Sets the traslation.
		void SetTranslation(const Vector3D& translation);

		//! Returns the orientation.
		const QuaternionD& GetOrientation() const;

		//! Sets the orientation.
		void SetOrientation(const QuaternionD& orientation);

		//! Transforms a point in world coordinate to the local frame.
		Vector3D ToLocal(const Vector3D& pointInWorld) const;

		//! Transforms a direction in world coordinate to the local frame.
		Vector3D ToLocalDirection(const Vector3D& dirInWorld) const;

		//! Transforms a ray in world coordinate to the local frame.
		Ray3D ToLocal(const Ray3D& rayInWorld) const;

		//! Transforms a bounding box in world coordinate to the local frame.
		BoundingBox3D ToLocal(const BoundingBox3D& bboxInWorld) const;

		//! Transforms a point in local space to the world coordinate.
		Vector3D ToWorld(const Vector3D& pointInLocal) const;

		//! Transforms a direction in local space to the world coordinate.
		Vector3D ToWorldDirection(const Vector3D& dirInLocal) const;

		//! Transforms a ray in local space to the world coordinate.
		Ray3D ToWorld(const Ray3D& rayInLocal) const;

		//! Transforms a bounding box in local space to the world coordinate.
		BoundingBox3D ToWorld(const BoundingBox3D& bboxInLocal) const;

	private:
		Vector3D m_translation;
		QuaternionD m_orientation;
		Matrix3x3D m_orientationMat3;
		Matrix3x3D m_inverseOrientationMat3;
	};
}

#endif