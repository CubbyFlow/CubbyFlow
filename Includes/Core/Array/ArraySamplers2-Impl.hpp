// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_SAMPLERS2_IMPL_HPP
#define CUBBYFLOW_ARRAY_SAMPLERS2_IMPL_HPP

#include <Core/Math/MathUtils.h>

namespace CubbyFlow
{
template <typename T, typename R>
NearestArraySampler<T, R, 2>::NearestArraySampler(
    const ConstArrayAccessor2<T>& accessor, const Vector2<R>& gridSpacing,
    const Vector2<R>& gridOrigin)
{
    m_gridSpacing = gridSpacing;
    m_origin = gridOrigin;
    m_accessor = accessor;
}

template <typename T, typename R>
NearestArraySampler<T, R, 2>::NearestArraySampler(
    const NearestArraySampler& other)
{
    m_gridSpacing = other.m_gridSpacing;
    m_origin = other.m_origin;
    m_accessor = other.m_accessor;
}

template <typename T, typename R>
T NearestArraySampler<T, R, 2>::operator()(const Vector2<R>& pt) const
{
    ssize_t i, j;
    R fx, fy;

    assert(m_gridSpacing.x > std::numeric_limits<R>::epsilon());
    assert(m_gridSpacing.y > std::numeric_limits<R>::epsilon());

    const Vector2<R> normalizedX = (pt - m_origin) / m_gridSpacing;

    const ssize_t iSize = static_cast<ssize_t>(m_accessor.size().x);
    const ssize_t jSize = static_cast<ssize_t>(m_accessor.size().y);

    GetBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
    GetBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);

    i = std::min(static_cast<ssize_t>(i + fx + 0.5), iSize - 1);
    j = std::min(static_cast<ssize_t>(j + fy + 0.5), jSize - 1);

    return m_accessor(i, j);
}

template <typename T, typename R>
void NearestArraySampler<T, R, 2>::GetCoordinate(const Vector2<R>& pt,
                                                 Point2UI* index) const
{
    ssize_t i, j;
    R fx, fy;

    assert(m_gridSpacing.x > std::numeric_limits<R>::epsilon());
    assert(m_gridSpacing.y > std::numeric_limits<R>::epsilon());

    const Vector2<R> normalizedX = (pt - m_origin) / m_gridSpacing;

    const ssize_t iSize = static_cast<ssize_t>(m_accessor.size().x);
    const ssize_t jSize = static_cast<ssize_t>(m_accessor.size().y);

    GetBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
    GetBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);

    index->x = std::min(static_cast<ssize_t>(i + fx + 0.5), iSize - 1);
    index->y = std::min(static_cast<ssize_t>(j + fy + 0.5), jSize - 1);
}

template <typename T, typename R>
std::function<T(const Vector2<R>&)> NearestArraySampler<T, R, 2>::Functor()
    const
{
    NearestArraySampler sampler(*this);
    return std::bind(&NearestArraySampler::operator(), sampler,
                     std::placeholders::_1);
}

template <typename T, typename R>
LinearArraySampler<T, R, 2>::LinearArraySampler(
    const ConstArrayAccessor2<T>& accessor, const Vector2<R>& gridSpacing,
    const Vector2<R>& gridOrigin)
{
    m_gridSpacing = gridSpacing;
    m_invGridSpacing = static_cast<R>(1) / m_gridSpacing;
    m_origin = gridOrigin;
    m_accessor = accessor;
}

template <typename T, typename R>
LinearArraySampler<T, R, 2>::LinearArraySampler(const LinearArraySampler& other)
{
    m_gridSpacing = other.m_gridSpacing;
    m_invGridSpacing = other.m_invGridSpacing;
    m_origin = other.m_origin;
    m_accessor = other.m_accessor;
}

template <typename T, typename R>
T LinearArraySampler<T, R, 2>::operator()(const Vector2<R>& pt) const
{
    ssize_t i, j;
    R fx, fy;

    assert(m_gridSpacing.x > std::numeric_limits<R>::epsilon());
    assert(m_gridSpacing.y > std::numeric_limits<R>::epsilon());

    const Vector2<R> normalizedX = (pt - m_origin) / m_gridSpacing;

    const ssize_t iSize = static_cast<ssize_t>(m_accessor.size().x);
    const ssize_t jSize = static_cast<ssize_t>(m_accessor.size().y);

    GetBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
    GetBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);

    const ssize_t ip1 = std::min(i + 1, iSize - 1);
    const ssize_t jp1 = std::min(j + 1, jSize - 1);

    return BiLerp(m_accessor(i, j), m_accessor(ip1, j), m_accessor(i, jp1),
                  m_accessor(ip1, jp1), fx, fy);
}

template <typename T, typename R>
void LinearArraySampler<T, R, 2>::GetCoordinatesAndWeights(
    const Vector2<R>& pt, std::array<Point2UI, 4>* indices,
    std::array<R, 4>* weights) const
{
    ssize_t i, j;
    R fx, fy;

    assert(m_gridSpacing.x > std::numeric_limits<R>::epsilon());
    assert(m_gridSpacing.y > std::numeric_limits<R>::epsilon());

    const Vector2<R> normalizedX = (pt - m_origin) / m_gridSpacing;

    const ssize_t iSize = static_cast<ssize_t>(m_accessor.size().x);
    const ssize_t jSize = static_cast<ssize_t>(m_accessor.size().y);

    GetBarycentric(normalizedX.x, 0, iSize, &i, &fx);
    GetBarycentric(normalizedX.y, 0, jSize, &j, &fy);

    const ssize_t ip1 = std::min(i + 1, iSize - 1);
    const ssize_t jp1 = std::min(j + 1, jSize - 1);

    (*indices)[0] = Point2UI(i, j);
    (*indices)[1] = Point2UI(ip1, j);
    (*indices)[2] = Point2UI(i, jp1);
    (*indices)[3] = Point2UI(ip1, jp1);

    (*weights)[0] = (1 - fx) * (1 - fy);
    (*weights)[1] = fx * (1 - fy);
    (*weights)[2] = (1 - fx) * fy;
    (*weights)[3] = fx * fy;
}

template <typename T, typename R>
void LinearArraySampler<T, R, 2>::GetCoordinatesAndGradientWeights(
    const Vector2<R>& x, std::array<Point2UI, 4>* indices,
    std::array<Vector2<R>, 4>* weights) const
{
    ssize_t i, j;
    R fx, fy;

    assert(m_gridSpacing.x > 0.0);
    assert(m_gridSpacing.y > 0.0);

    const Vector2<R> normalizedX = (x - m_origin) * m_invGridSpacing;

    const ssize_t iSize = static_cast<ssize_t>(m_accessor.size().x);
    const ssize_t jSize = static_cast<ssize_t>(m_accessor.size().y);

    GetBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
    GetBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);

    const ssize_t ip1 = std::min(i + 1, iSize - 1);
    const ssize_t jp1 = std::min(j + 1, jSize - 1);

    (*indices)[0] = Point2UI(i, j);
    (*indices)[1] = Point2UI(ip1, j);
    (*indices)[2] = Point2UI(i, jp1);
    (*indices)[3] = Point2UI(ip1, jp1);

    (*weights)[0] = Vector2<R>(fy * m_invGridSpacing.x - m_invGridSpacing.x,
                               fx * m_invGridSpacing.y - m_invGridSpacing.y);
    (*weights)[1] = Vector2<R>(-fy * m_invGridSpacing.x + m_invGridSpacing.x,
                               -fx * m_invGridSpacing.y);
    (*weights)[2] = Vector2<R>(-fy * m_invGridSpacing.x,
                               -fx * m_invGridSpacing.y + m_invGridSpacing.y);
    (*weights)[3] =
        Vector2<R>(fy * m_invGridSpacing.x, fx * m_invGridSpacing.y);
}

template <typename T, typename R>
std::function<T(const Vector2<R>&)> LinearArraySampler<T, R, 2>::Functor() const
{
    LinearArraySampler sampler(*this);
    return std::bind(&LinearArraySampler::operator(), sampler,
                     std::placeholders::_1);
}

template <typename T, typename R>
CubicArraySampler<T, R, 2>::CubicArraySampler(
    const ConstArrayAccessor2<T>& accessor, const Vector2<R>& gridSpacing,
    const Vector2<R>& gridOrigin)
{
    m_gridSpacing = gridSpacing;
    m_origin = gridOrigin;
    m_accessor = accessor;
}

template <typename T, typename R>
CubicArraySampler<T, R, 2>::CubicArraySampler(const CubicArraySampler& other)
{
    m_gridSpacing = other.m_gridSpacing;
    m_origin = other.m_origin;
    m_accessor = other.m_accessor;
}

template <typename T, typename R>
T CubicArraySampler<T, R, 2>::operator()(const Vector2<R>& pt) const
{
    ssize_t i, j;
    R fx, fy;

    assert(m_gridSpacing.x > std::numeric_limits<R>::epsilon());
    assert(m_gridSpacing.y > std::numeric_limits<R>::epsilon());

    const Vector2<R> normalizedX = (pt - m_origin) / m_gridSpacing;

    const ssize_t iSize = static_cast<ssize_t>(m_accessor.size().x);
    const ssize_t jSize = static_cast<ssize_t>(m_accessor.size().y);

    GetBarycentric(normalizedX.x, 0, iSize - 1, &i, &fx);
    GetBarycentric(normalizedX.y, 0, jSize - 1, &j, &fy);

    const ssize_t is[4] = { std::max(i - 1, ZERO_SSIZE), i,
                            std::min(i + 1, iSize - 1),
                            std::min(i + 2, iSize - 1) };
    const ssize_t js[4] = { std::max(j - 1, ZERO_SSIZE), j,
                            std::min(j + 1, jSize - 1),
                            std::min(j + 2, jSize - 1) };

    // Calculate in i direction first
    T values[4];

    for (int n = 0; n < 4; ++n)
    {
        values[n] = MonotonicCatmullRom(
            m_accessor(is[0], js[n]), m_accessor(is[1], js[n]),
            m_accessor(is[2], js[n]), m_accessor(is[3], js[n]), fx);
    }

    return MonotonicCatmullRom(values[0], values[1], values[2], values[3], fy);
}

template <typename T, typename R>
std::function<T(const Vector2<R>&)> CubicArraySampler<T, R, 2>::Functor() const
{
    CubicArraySampler sampler(*this);
    return std::bind(&CubicArraySampler::operator(), sampler,
                     std::placeholders::_1);
}
}  // namespace CubbyFlow

#endif