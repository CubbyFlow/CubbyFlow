// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_SAMPLERS1_IMPL_HPP
#define CUBBYFLOW_ARRAY_SAMPLERS1_IMPL_HPP

#include <Core/Math/MathUtils.hpp>

namespace CubbyFlow
{
template <typename T, typename R>
NearestArraySampler<T, R, 1>::NearestArraySampler(
    const ConstArrayAccessor1<T>& accessor, R gridSpacing, R gridOrigin)
{
    m_gridSpacing = gridSpacing;
    m_origin = gridOrigin;
    m_accessor = accessor;
}

template <typename T, typename R>
NearestArraySampler<T, R, 1>::NearestArraySampler(
    const NearestArraySampler& other)
{
    m_gridSpacing = other.m_gridSpacing;
    m_origin = other.m_origin;
    m_accessor = other.m_accessor;
}

template <typename T, typename R>
T NearestArraySampler<T, R, 1>::operator()(R pt) const
{
    ssize_t i;
    R fx;

    assert(m_gridSpacing > std::numeric_limits<R>::epsilon());

    const R normalizedX = (pt - m_origin) / m_gridSpacing;

    const ssize_t iSize = static_cast<ssize_t>(m_accessor.size());

    GetBarycentric(normalizedX, 0, iSize - 1, &i, &fx);

    i = std::min(static_cast<ssize_t>(i + fx + 0.5), iSize - 1);

    return m_accessor[i];
}

template <typename T, typename R>
void NearestArraySampler<T, R, 1>::GetCoordinate(R pt, size_t* index) const
{
    ssize_t i;
    R fx;

    assert(m_gridSpacing > std::numeric_limits<R>::epsilon());

    const R normalizedX = (pt - m_origin) / m_gridSpacing;

    const ssize_t iSize = static_cast<ssize_t>(m_accessor.size());

    GetBarycentric(normalizedX, 0, iSize - 1, &i, &fx);

    *index = std::min(static_cast<ssize_t>(i + fx + 0.5), iSize - 1);
}

template <typename T, typename R>
std::function<T(R)> NearestArraySampler<T, R, 1>::Functor() const
{
    NearestArraySampler sampler(*this);
    return std::bind(&NearestArraySampler::operator(), sampler,
                     std::placeholders::_1);
}

template <typename T, typename R>
LinearArraySampler<T, R, 1>::LinearArraySampler(
    const ConstArrayAccessor1<T>& accessor, R gridSpacing, R gridOrigin)
{
    m_gridSpacing = gridSpacing;
    m_origin = gridOrigin;
    m_accessor = accessor;
}

template <typename T, typename R>
LinearArraySampler<T, R, 1>::LinearArraySampler(const LinearArraySampler& other)
{
    m_gridSpacing = other.m_gridSpacing;
    m_origin = other.m_origin;
    m_accessor = other.m_accessor;
}

template <typename T, typename R>
T LinearArraySampler<T, R, 1>::operator()(R pt) const
{
    ssize_t i;
    R fx;

    assert(m_gridSpacing > std::numeric_limits<R>::epsilon());

    const R normalizedX = (pt - m_origin) / m_gridSpacing;

    const ssize_t iSize = static_cast<ssize_t>(m_accessor.size());

    GetBarycentric(normalizedX, 0, iSize - 1, &i, &fx);

    const ssize_t ip1 = std::min(i + 1, iSize - 1);

    return Lerp(m_accessor[i], m_accessor[ip1], fx);
}

template <typename T, typename R>
void LinearArraySampler<T, R, 1>::GetCoordinatesAndWeights(R pt, size_t* i0,
                                                           size_t* i1,
                                                           T* weight0,
                                                           T* weight1) const
{
    ssize_t i;
    R fx;

    assert(m_gridSpacing > std::numeric_limits<R>::epsilon());

    const R normalizedX = (pt - m_origin) / m_gridSpacing;

    const ssize_t iSize = static_cast<ssize_t>(m_accessor.size());

    GetBarycentric(normalizedX, 0, iSize - 1, &i, &fx);

    const ssize_t ip1 = std::min(i + 1, iSize - 1);

    *i0 = i;
    *i1 = ip1;
    *weight0 = 1 - fx;
    *weight1 = fx;
}

template <typename T, typename R>
std::function<T(R)> LinearArraySampler<T, R, 1>::Functor() const
{
    LinearArraySampler sampler(*this);
    return std::bind(&LinearArraySampler::operator(), sampler,
                     std::placeholders::_1);
}

template <typename T, typename R>
CubicArraySampler<T, R, 1>::CubicArraySampler(
    const ConstArrayAccessor1<T>& accessor, R gridSpacing, R gridOrigin)
{
    m_gridSpacing = gridSpacing;
    m_origin = gridOrigin;
    m_accessor = accessor;
}

template <typename T, typename R>
CubicArraySampler<T, R, 1>::CubicArraySampler(const CubicArraySampler& other)
{
    m_gridSpacing = other.m_gridSpacing;
    m_origin = other.m_origin;
    m_accessor = other.m_accessor;
}

template <typename T, typename R>
T CubicArraySampler<T, R, 1>::operator()(R x) const
{
    ssize_t i;
    R fx;

    assert(m_gridSpacing > std::numeric_limits<R>::epsilon());

    const R normalizedX = (x - m_origin) / m_gridSpacing;

    const ssize_t iSize = static_cast<ssize_t>(m_accessor.size());

    GetBarycentric(normalizedX, 0, iSize - 1, &i, &fx);

    const ssize_t im1 = std::max(i - 1, ZERO_SSIZE);
    const ssize_t ip1 = std::min(i + 1, iSize - 1);
    const ssize_t ip2 = std::min(i + 2, iSize - 1);

    return MonotonicCatmullRom(m_accessor[im1], m_accessor[i], m_accessor[ip1],
                               m_accessor[ip2], fx);
}

template <typename T, typename R>
std::function<T(R)> CubicArraySampler<T, R, 1>::Functor() const
{
    CubicArraySampler sampler(*this);
    return std::bind(&CubicArraySampler::operator(), sampler,
                     std::placeholders::_1);
}
}  // namespace CubbyFlow

#endif