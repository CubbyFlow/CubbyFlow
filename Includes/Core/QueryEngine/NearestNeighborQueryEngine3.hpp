// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_NEAREST_NEIGHBOR_QUERY_ENGINE3_HPP
#define CUBBYFLOW_NEAREST_NEIGHBOR_QUERY_ENGINE3_HPP

#include <Core/Vector/Vector3.hpp>

namespace CubbyFlow
{
//! Nearest neighbor query result.
template <typename T>
struct NearestNeighborQueryResult3
{
    const T* item = nullptr;
    double distance = std::numeric_limits<double>::max();
};

//! Nearest neighbor distance measure function.
template <typename T>
using NearestNeighborDistanceFunc3 =
    std::function<double(const T&, const Vector3D&)>;

//! Abstract base class for 3-D nearest neighbor query engine.
template <typename T>
class NearestNeighborQueryEngine3
{
 public:
    //! Returns the nearest neighbor for given point and distance measure
    //! function.
    virtual NearestNeighborQueryResult3<T> GetNearestNeighbor(
        const Vector3D& pt,
        const NearestNeighborDistanceFunc3<T>& distanceFunc) const = 0;
};
}  // namespace CubbyFlow

#endif