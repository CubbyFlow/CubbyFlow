/*************************************************************************
> File Name: ArraySamplers3-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: 3-D nearest array sampler class.
> Created Time: 2017/05/19
> Copyright (c) 2018, Chan-Ho Chris Ohk
*************************************************************************/
#ifndef CUBBYFLOW_ARRAY_SAMPLERS3_IMPL_H
#define CUBBYFLOW_ARRAY_SAMPLERS3_IMPL_H

#include <Core/Math/MathUtils.h>

namespace CubbyFlow
{
	template <typename T, typename R>
	NearestArraySampler<T, R, 3>::NearestArraySampler(
		const ConstArrayAccessor3<T>& accessor,
		const Vector3<R>& gridSpacing,
		const Vector3<R>& gridOrigin)
	{
		m_gridSpacing = gridSpacing;
		m_origin = gridOrigin;
		m_accessor = accessor;
	}

	template <typename T, typename R>
	NearestArraySampler<T, R, 3>::NearestArraySampler(const NearestArraySampler& other)
	{
		m_gridSpacing = other.m_gridSpacing;
		m_origin = other.m_origin;
		m_accessor = other.m_accessor;
	}

	template <typename T, typename R>
	T NearestArraySampler<T, R, 3>::operator()(const Vector3<R>& pt) const
	{
		ssize_t i, j, k;
		R fx, fy, fz;

		assert(m_gridSpacing.x > std::numeric_limits<R>::epsilon());
		assert(m_gridSpacing.y > std::numeric_limits<R>::epsilon());
		assert(m_gridSpacing.z > std::numeric_limits<R>::epsilon());

		const Vector3<R> normalizedX = (pt - m_origin) / m_gridSpacing;

		const ssize_t iSize = static_cast<ssize_t>(m_accessor.size().x);
		const ssize_t jSize = static_cast<ssize_t>(m_accessor.size().y);
		const ssize_t kSize = static_cast<ssize_t>(m_accessor.size().z);

		GetBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
		GetBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);
		GetBarycentric(normalizedX.z, 0, kSize - 1, &k, &fz);

		i = std::min(static_cast<ssize_t>(i + fx + 0.5), iSize - 1);
		j = std::min(static_cast<ssize_t>(j + fy + 0.5), jSize - 1);
		k = std::min(static_cast<ssize_t>(k + fz + 0.5), kSize - 1);

		return m_accessor(i, j, k);
	}

	template <typename T, typename R>
	void NearestArraySampler<T, R, 3>::GetCoordinate(const Vector3<R>& pt, Point3UI* index) const
	{
		ssize_t i, j, k;
		R fx, fy, fz;

		assert(m_gridSpacing.x > std::numeric_limits<R>::epsilon());
		assert(m_gridSpacing.y > std::numeric_limits<R>::epsilon());
		assert(m_gridSpacing.z > std::numeric_limits<R>::epsilon());

		const Vector3<R> normalizedX = (pt - m_origin) / m_gridSpacing;

		const ssize_t iSize = static_cast<ssize_t>(m_accessor.size().x);
		const ssize_t jSize = static_cast<ssize_t>(m_accessor.size().y);
		const ssize_t kSize = static_cast<ssize_t>(m_accessor.size().z);

		GetBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
		GetBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);
		GetBarycentric(normalizedX.z, 0, kSize - 1, &k, &fz);

		index->x = std::min(static_cast<ssize_t>(i + fx + 0.5), iSize - 1);
		index->y = std::min(static_cast<ssize_t>(j + fy + 0.5), jSize - 1);
		index->z = std::min(static_cast<ssize_t>(k + fz + 0.5), kSize - 1);
	}

	template <typename T, typename R>
	std::function<T(const Vector3<R>&)> NearestArraySampler<T, R, 3>::Functor() const
	{
		NearestArraySampler sampler(*this);
		return std::bind(&NearestArraySampler::operator(), sampler, std::placeholders::_1);
	}

	template <typename T, typename R>
	LinearArraySampler<T, R, 3>::LinearArraySampler(
		const ConstArrayAccessor3<T>& accessor,
		const Vector3<R>& gridSpacing,
		const Vector3<R>& gridOrigin)
	{
		m_gridSpacing = gridSpacing;
		m_invGridSpacing = static_cast<R>(1) / m_gridSpacing;
		m_origin = gridOrigin;
		m_accessor = accessor;
	}
	
	template <typename T, typename R>
	LinearArraySampler<T, R, 3>::LinearArraySampler(const LinearArraySampler& other)
	{
		m_gridSpacing = other.m_gridSpacing;
		m_invGridSpacing = other.m_invGridSpacing;
		m_origin = other.m_origin;
		m_accessor = other.m_accessor;
	}

	template <typename T, typename R>
	T LinearArraySampler<T, R, 3>::operator()(const Vector3<R>& pt) const
	{
		ssize_t i, j, k;
		R fx, fy, fz;

		assert(m_gridSpacing.x > std::numeric_limits<R>::epsilon());
		assert(m_gridSpacing.y > std::numeric_limits<R>::epsilon());
		assert(m_gridSpacing.z > std::numeric_limits<R>::epsilon());

		const Vector3<R> normalizedX = (pt - m_origin) / m_gridSpacing;

		const ssize_t iSize = static_cast<ssize_t>(m_accessor.size().x);
		const ssize_t jSize = static_cast<ssize_t>(m_accessor.size().y);
		const ssize_t kSize = static_cast<ssize_t>(m_accessor.size().z);

		GetBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
		GetBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);
		GetBarycentric(normalizedX.z, 0, kSize - 1, &k, &fz);

		const ssize_t ip1 = std::min(i + 1, iSize - 1);
		const ssize_t jp1 = std::min(j + 1, jSize - 1);
		const ssize_t kp1 = std::min(k + 1, kSize - 1);

		return TriLerp(
			m_accessor(i, j, k), m_accessor(ip1, j, k),
			m_accessor(i, jp1, k), m_accessor(ip1, jp1, k),
			m_accessor(i, j, kp1), m_accessor(ip1, j, kp1),
			m_accessor(i, jp1, kp1), m_accessor(ip1, jp1, kp1),
			fx, fy, fz);
	}

	template <typename T, typename R>
	void LinearArraySampler<T, R, 3>::GetCoordinatesAndWeights(
		const Vector3<R>& pt,
		std::array<Point3UI, 8>* indices,
		std::array<R, 8>* weights) const
	{
		ssize_t i, j, k;
		R fx, fy, fz;

		assert(m_gridSpacing.x > std::numeric_limits<R>::epsilon());
		assert(m_gridSpacing.y > std::numeric_limits<R>::epsilon());
		assert(m_gridSpacing.z > std::numeric_limits<R>::epsilon());

		const Vector3<R> normalizedX = (pt - m_origin) / m_gridSpacing;

		const ssize_t iSize = static_cast<ssize_t>(m_accessor.size().x);
		const ssize_t jSize = static_cast<ssize_t>(m_accessor.size().y);
		const ssize_t kSize = static_cast<ssize_t>(m_accessor.size().z);

		GetBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
		GetBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);
		GetBarycentric(normalizedX.z, 0, kSize - 1, &k, &fz);

		const ssize_t ip1 = std::min(i + 1, iSize - 1);
		const ssize_t jp1 = std::min(j + 1, jSize - 1);
		const ssize_t kp1 = std::min(k + 1, kSize - 1);

		(*indices)[0] = Point3UI(i, j, k);
		(*indices)[1] = Point3UI(ip1, j, k);
		(*indices)[2] = Point3UI(i, jp1, k);
		(*indices)[3] = Point3UI(ip1, jp1, k);
		(*indices)[4] = Point3UI(i, j, kp1);
		(*indices)[5] = Point3UI(ip1, j, kp1);
		(*indices)[6] = Point3UI(i, jp1, kp1);
		(*indices)[7] = Point3UI(ip1, jp1, kp1);

		(*weights)[0] = (1 - fx) * (1 - fy) * (1 - fz);
		(*weights)[1] = fx * (1 - fy) * (1 - fz);
		(*weights)[2] = (1 - fx) * fy * (1 - fz);
		(*weights)[3] = fx * fy * (1 - fz);
		(*weights)[4] = (1 - fx) * (1 - fy) * fz;
		(*weights)[5] = fx * (1 - fy) * fz;
		(*weights)[6] = (1 - fx) * fy * fz;
		(*weights)[7] = fx * fy * fz;
	}

	template <typename T, typename R>
	void LinearArraySampler<T, R, 3>::GetCoordinatesAndGradientWeights(
		const Vector3<R>& x,
		std::array<Point3UI, 8>* indices,
		std::array<Vector3<R>, 8>* weights) const
	{
		ssize_t i, j, k;
		R fx, fy, fz;

		assert(m_gridSpacing.x > 0.0);
		assert(m_gridSpacing.y > 0.0);
		assert(m_gridSpacing.z > 0.0);

		const Vector3<R> normalizedX = (x - m_origin) / m_gridSpacing;

		const ssize_t iSize = static_cast<ssize_t>(m_accessor.size().x);
		const ssize_t jSize = static_cast<ssize_t>(m_accessor.size().y);
		const ssize_t kSize = static_cast<ssize_t>(m_accessor.size().z);

		GetBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
		GetBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);
		GetBarycentric(normalizedX.z, 0, kSize - 1, &k, &fz);

		const ssize_t ip1 = std::min(i + 1, iSize - 1);
		const ssize_t jp1 = std::min(j + 1, jSize - 1);
		const ssize_t kp1 = std::min(k + 1, kSize - 1);

		(*indices)[0] = Point3UI(i, j, k);
		(*indices)[1] = Point3UI(ip1, j, k);
		(*indices)[2] = Point3UI(i, jp1, k);
		(*indices)[3] = Point3UI(ip1, jp1, k);
		(*indices)[4] = Point3UI(i, j, kp1);
		(*indices)[5] = Point3UI(ip1, j, kp1);
		(*indices)[6] = Point3UI(i, jp1, kp1);
		(*indices)[7] = Point3UI(ip1, jp1, kp1);

		(*weights)[0] = Vector3<R>(-m_invGridSpacing.x * (1 - fy) * (1 - fz), -m_invGridSpacing.y * (1 - fx) * (1 - fz), -m_invGridSpacing.z * (1 - fx) * (1 - fy));
		(*weights)[1] = Vector3<R>(m_invGridSpacing.x * (1 - fy) * (1 - fz), fx * (-m_invGridSpacing.y) * (1 - fz), fx * (1 - fy) * (-m_invGridSpacing.z));
		(*weights)[2] = Vector3<R>((-m_invGridSpacing.x) * fy * (1 - fz), (1 - fx) * m_invGridSpacing.y * (1 - fz), (1 - fx) * fy * (-m_invGridSpacing.z));
		(*weights)[3] = Vector3<R>(m_invGridSpacing.x * fy * (1 - fz), fx * m_invGridSpacing.y * (1 - fz), fx * fy * (-m_invGridSpacing.z));
		(*weights)[4] = Vector3<R>((-m_invGridSpacing.x) * (1 - fy) * fz, (1 - fx) * (-m_invGridSpacing.y) * fz, (1 - fx) * (1 - fy) * m_invGridSpacing.z);
		(*weights)[5] = Vector3<R>(m_invGridSpacing.x * (1 - fy) * fz, fx * (-m_invGridSpacing.y) * fz, fx * (1 - fy) * m_invGridSpacing.z);
		(*weights)[6] = Vector3<R>((-m_invGridSpacing.x) * fy * fz, (1 - fx) * m_invGridSpacing.y * fz, (1 - fx) * fy * m_invGridSpacing.z);
		(*weights)[7] = Vector3<R>(m_invGridSpacing.x * fy * fz, fx * m_invGridSpacing.y * fz, fx * fy * m_invGridSpacing.z);
	}

	template <typename T, typename R>
	std::function<T(const Vector3<R>&)> LinearArraySampler<T, R, 3>::Functor() const
	{
		LinearArraySampler sampler(*this);
		return std::bind(&LinearArraySampler::operator(), sampler, std::placeholders::_1);
	}
	
	template <typename T, typename R>
	CubicArraySampler<T, R, 3>::CubicArraySampler(
		const ConstArrayAccessor3<T>& accessor,
		const Vector3<R>& gridSpacing,
		const Vector3<R>& gridOrigin)
	{
		m_gridSpacing = gridSpacing;
		m_origin = gridOrigin;
		m_accessor = accessor;
	}
	
	template <typename T, typename R>
	CubicArraySampler<T, R, 3>::CubicArraySampler(const CubicArraySampler& other)
	{
		m_gridSpacing = other.m_gridSpacing;
		m_origin = other.m_origin;
		m_accessor = other.m_accessor;
	}

	template <typename T, typename R>
	T CubicArraySampler<T, R, 3>::operator()(const Vector3<R>& pt) const
	{
		ssize_t i, j, k;
		R fx, fy, fz;

		assert(m_gridSpacing.x > std::numeric_limits<R>::epsilon());
		assert(m_gridSpacing.y > std::numeric_limits<R>::epsilon());
		assert(m_gridSpacing.z > std::numeric_limits<R>::epsilon());

		const Vector3<R> normalizedX = (pt - m_origin) / m_gridSpacing;

		const ssize_t iSize = static_cast<ssize_t>(m_accessor.size().x);
		const ssize_t jSize = static_cast<ssize_t>(m_accessor.size().y);
		const ssize_t kSize = static_cast<ssize_t>(m_accessor.size().z);

		GetBarycentric(normalizedX.x, 0, iSize, &i, &fx);
		GetBarycentric(normalizedX.y, 0, jSize, &j, &fy);
		GetBarycentric(normalizedX.z, 0, kSize, &k, &fz);

		const ssize_t is[4] = { std::max(i - 1, ZERO_SSIZE), i, std::min(i + 1, iSize - 1), std::min(i + 2, iSize - 1) };
		const ssize_t js[4] = { std::max(j - 1, ZERO_SSIZE), j, std::min(j + 1, jSize - 1), std::min(j + 2, jSize - 1) };
		const ssize_t ks[4] = { std::max(k - 1, ZERO_SSIZE), k, std::min(k + 1, kSize - 1), std::min(k + 2, kSize - 1) };

		T kValues[4];

		for (int kk = 0; kk < 4; ++kk)
		{
			T jValues[4];

			for (int jj = 0; jj < 4; ++jj)
			{
				jValues[jj] = MonotonicCatmullRom(
					m_accessor(is[0], js[jj], ks[kk]),
					m_accessor(is[1], js[jj], ks[kk]),
					m_accessor(is[2], js[jj], ks[kk]),
					m_accessor(is[3], js[jj], ks[kk]),
					fx);
			}

			kValues[kk] = MonotonicCatmullRom(jValues[0], jValues[1], jValues[2], jValues[3], fy);
		}

		return MonotonicCatmullRom(kValues[0], kValues[1], kValues[2], kValues[3], fz);
	}

	template <typename T, typename R>
	std::function<T(const Vector3<R>&)> CubicArraySampler<T, R, 3>::Functor() const
	{
		CubicArraySampler sampler(*this);
		return std::bind(&CubicArraySampler::operator(), sampler, std::placeholders::_1);
	}
}

#endif