// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_POINT_HASH_GRID_UTILS_HPP
#define CUBBYFLOW_POINT_HASH_GRID_UTILS_HPP

#include <Core/Matrix/Matrix.hpp>

namespace CubbyFlow
{
//!
//! \brief Hash grid common utilities for N-D point searchers.
//!
template <size_t N>
class PointHashGridUtils
{
 public:
    template <size_t M = N>
    static std::enable_if_t<M == 2, size_t> HashKey(
        const Vector<ssize_t, 2>& index, const Vector<ssize_t, 2>& resolution);

    template <size_t M = N>
    static std::enable_if_t<M == 3, size_t> HashKey(
        const Vector<ssize_t, 3>& index, const Vector<ssize_t, 3>& resolution);

    //!
    //! Returns the hash value for given N-D bucket index.
    //!
    //! \param[in]  bucketIndex The bucket index.
    //! \param[in]  resolution  The resolution.
    //!
    //! \return     The hash key from bucket index.
    //!
    static size_t GetHashKeyFromBucketIndex(
        const Vector<ssize_t, N>& bucketIndex,
        const Vector<ssize_t, N>& resolution);

    //!
    //! Gets the bucket index from a point.
    //!
    //! \param[in]  position The position of the point.
    //! \param[in]  gridSpacing The grid spacing.
    //!
    //! \return     The bucket index.
    //!
    static Vector<ssize_t, N> GetBucketIndex(const Vector<double, N>& position,
                                             double gridSpacing);

    static size_t GetHashKeyFromPosition(const Vector<double, N>& position,
                                         double gridSpacing,
                                         const Vector<ssize_t, N>& resolution);

    static void GetNearbyKeys(const Vector<double, N>& position,
                              double gridSpacing,
                              const Vector<ssize_t, N>& resolution,
                              size_t* nearbyKeys);
};

using PointHashGridUtils2 = PointHashGridUtils<2>;

using PointHashGridUtils3 = PointHashGridUtils<3>;
}  // namespace CubbyFlow

#endif