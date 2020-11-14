// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_NEAREST_NEIGHBOR_QUERY_ENGINE2_HPP
#define CUBBYFLOW_NEAREST_NEIGHBOR_QUERY_ENGINE2_HPP

#include <Core/Vector/Vector2.hpp>

namespace CubbyFlow
{
//! Nearest neighbor query result.
template <typename T>
struct NearestNeighborQueryResult2
{
    const T* item = nullptr;
    double distance = std::numeric_limits<double>::max();
};

//! Nearest neighbor distance measure function.
template <typename T>
using NearestNeighborDistanceFunc2 =
    std::function<double(const T&, const Vector2D&)>;

//! Abstract base class for 2-D nearest neighbor query engine.
template <typename T>
class NearestNeighborQueryEngine2
{
 public:
    //! Default constructor.
    NearestNeighborQueryEngine2() = default;

    //! Default copy constructor.
    NearestNeighborQueryEngine2(const NearestNeighborQueryEngine2&) = default;

    //! Default move constructor.
    NearestNeighborQueryEngine2(NearestNeighborQueryEngine2&&) noexcept =
        default;

    //! Default virtual destructor.
    virtual ~NearestNeighborQueryEngine2() = default;

    //! Default copy assignment operator.
    NearestNeighborQueryEngine2& operator=(const NearestNeighborQueryEngine2&) =
        default;

    //! Default move assignment operator.
    NearestNeighborQueryEngine2& operator=(
        NearestNeighborQueryEngine2&&) noexcept = default;

    //! Returns the nearest neighbor for given point and distance measure
    //! function.
    [[nodiscard]] virtual NearestNeighborQueryResult2<T> GetNearestNeighbor(
        const Vector2D& pt,
        const NearestNeighborDistanceFunc2<T>& distanceFunc) const = 0;
};
}  // namespace CubbyFlow

#endif