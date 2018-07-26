/*************************************************************************
> File Name: Samplers.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Sampler functions for CubbyFlow.
> Created Time: 2017/06/21
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_SAMPLERS_H
#define CUBBYFLOW_SAMPLERS_H

#include <Core/Vector/Vector2.h>
#include <Core/Vector/Vector3.h>

namespace CubbyFlow
{
	//!
	//! \brief      Returns randomly sampled direction within a cone.
	//!
	//! For a given cone, defined by axis and angle, this function returns a sampled
	//! direction vector within the cone.
	//!
	//! \param[in]  u1    First random sample.
	//! \param[in]  u2    Second random sample.
	//! \param[in]  axis  The axis of the cone.
	//! \param[in]  angle The angle of the cone.
	//!
	//! \tparam     T     Real number type.
	//!
	//! \return     Sampled direction vector.
	//!
	template <typename T>
	inline Vector3<T> UniformSampleCone(T u1, T u2, const Vector3<T>& axis, T angle);

	//!
	//! \brief      Returns randomly sampled point within a unit hemisphere.
	//!
	//! For a given unit hemisphere, defined by center normal vector, this function
	//! returns a point within the hemisphere.
	//!
	//! \param[in]  u1      First random sample.
	//! \param[in]  u2      Second random sample.
	//! \param[in]  normal  The center normal of the hemisphere.
	//!
	//! \tparam     T       Real number type.
	//!
	//! \return     Sampled point.
	//!
	template <typename T>
	inline Vector3<T> UniformSampleHemisphere(T u1, T u2, const Vector3<T>& normal);

	//!
	//! \brief      Returns weighted sampled point on a hemisphere.
	//!
	//! For a given hemisphere, defined by center normal vector, this function
	//! returns a point on the hemisphere, where the probability is cosine-weighted.
	//!
	//! \param[in]  u1      First random sample.
	//! \param[in]  u2      Second random sample.
	//! \param[in]  normal  The center normal of the hemisphere.
	//!
	//! \tparam     T       Real number type.
	//!
	//! \return     Sampled point.
	//!
	template <typename T>
	inline Vector3<T> CosineWeightedSampleHemisphere(T u1, T u2, const Vector3<T>& normal);

	//!
	//! \brief      Returns randomly a point on a sphere.
	//!
	//! For a given sphere, defined by center normal vector, this function returns a
	//! point on the sphere.
	//!
	//! \param[in]  u1    First random sample.
	//! \param[in]  u2    Second random sample.
	//!
	//! \tparam     T     Real number type.
	//!
	//! \return     Sampled point.
	//!
	template <typename T>
	inline Vector3<T> UniformSampleSphere(T u1, T u2);

	//!
	//! \brief      Returns randomly a point on a disk.
	//!
	//! For a given disk, this function returns a point on the disk.
	//!
	//! \param[in]  u1    First random sample.
	//! \param[in]  u2    Second random sample.
	//!
	//! \tparam     T     Real number type.
	//!
	//! \return     Sampled point.
	//!
	template <typename T>
	inline Vector2<T> UniformSampleDisk(T u1, T u2);
}

#include <Core/Utils/Samplers-Impl.h>

#endif