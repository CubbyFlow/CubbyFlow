// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_SAMPLERS_HPP
#define CUBBYFLOW_SAMPLERS_HPP

#include <Core/Matrix/Matrix.hpp>

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
Vector3<T> UniformSampleCone(T u1, T u2, const Vector3<T>& axis, T angle);

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
Vector3<T> UniformSampleHemisphere(T u1, T u2, const Vector3<T>& normal);

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
Vector3<T> CosineWeightedSampleHemisphere(T u1, T u2, const Vector3<T>& normal);

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
Vector3<T> UniformSampleSphere(T u1, T u2);

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
Vector2<T> UniformSampleDisk(T u1, T u2);
}  // namespace CubbyFlow

#include <Core/Utils/Samplers-Impl.hpp>

#endif