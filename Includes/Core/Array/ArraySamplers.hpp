// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_SAMPLERS_HPP
#define CUBBYFLOW_ARRAY_SAMPLERS_HPP

namespace CubbyFlow
{
//!
//! \brief Generic N-D nearest array sampler class.
//!
//! \tparam T - The value type to sample.
//! \tparam R - The real number type.
//! \tparam N - Dimension.
//!
template <typename T, typename R, size_t N>
class NearestArraySampler final
{
 public:
    static_assert(N < 1 || N > 3,
                  "Not implemented - N should be either 1, 2 or 3.");
};

//!
//! \brief Generic N-D linear array sampler class.
//!
//! \tparam T - The value type to sample.
//! \tparam R - The real number type.
//! \tparam N - Dimension.
//!
template <typename T, typename R, size_t N>
class LinearArraySampler final
{
 public:
    static_assert(N < 1 || N > 3,
                  "Not implemented - N should be either 1, 2 or 3.");
};

//!
//! \brief Generic N-D cubic array sampler class.
//!
//! \tparam T - The value type to sample.
//! \tparam R - The real number type.
//! \tparam N - Dimension.
//!
template <typename T, typename R, size_t N>
class CubicArraySampler final
{
 public:
    static_assert(N < 1 || N > 3,
                  "Not implemented - N should be either 1, 2 or 3.");
};
}  // namespace CubbyFlow

#endif