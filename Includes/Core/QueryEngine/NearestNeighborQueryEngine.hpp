// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_NEAREST_NEIGHBOR_QUERY_ENGINE_HPP
#define CUBBYFLOW_NEAREST_NEIGHBOR_QUERY_ENGINE_HPP

#include <Core/Matrix/Matrix.hpp>

#include <functional>
#include <limits>

namespace CubbyFlow
{
//! N-D nearest neighbor query result.
template <typename T, size_t N>
struct NearestNeighborQueryResult
{
    const T* item = nullptr;
    double distance = std::numeric_limits<double>::max();
};

//! 2-D nearest neighbor query result.
template <typename T>
using NearestNeighborQueryResult2 = NearestNeighborQueryResult<T, 2>;

//! 3-D nearest neighbor query result.
template <typename T>
using NearestNeighborQueryResult3 = NearestNeighborQueryResult<T, 3>;

//! N-D nearest neighbor distance measure function.
template <typename T, size_t N>
using NearestNeighborDistanceFunc =
    std::function<double(const T&, const Vector<double, N>&)>;

//! 2-D nearest neighbor distance measure function.
template <typename T>
using NearestNeighborDistanceFunc2 = NearestNeighborDistanceFunc<T, 2>;

//! 3-D nearest neighbor distance measure function.
template <typename T>
using NearestNeighborDistanceFunc3 = NearestNeighborDistanceFunc<T, 3>;

//! Abstract base class for N-D nearest neighbor query engine.
template <typename T, size_t N>
class NearestNeighborQueryEngine
{
 public:
    //! Default constructor.
    NearestNeighborQueryEngine() = default;

    //! Default virtual destructor.
    virtual ~NearestNeighborQueryEngine() = default;

    //! Default copy constructor.
    NearestNeighborQueryEngine(const NearestNeighborQueryEngine& other) =
        default;

    //! Default move constructor.
    NearestNeighborQueryEngine(NearestNeighborQueryEngine&& other) noexcept =
        default;

    //! Default copy assignment operator.
    NearestNeighborQueryEngine& operator=(
        const NearestNeighborQueryEngine& other) = default;

    //! Default move assignment operator.
    NearestNeighborQueryEngine& operator=(
        NearestNeighborQueryEngine&& other) noexcept = default;

    //! Returns the nearest neighbor for given point and distance measure
    //! function.
    [[nodiscard]] virtual NearestNeighborQueryResult<T, N> Nearest(
        const Vector<double, N>& pt,
        const NearestNeighborDistanceFunc<T, N>& distanceFunc) const = 0;
};

//! Abstract base class for 2-D nearest neighbor query engine.
template <typename T>
using NearestNeighborQueryEngine2 = NearestNeighborQueryEngine<T, 2>;

//! Abstract base class for 3-D nearest neighbor query engine.
template <typename T>
using NearestNeighborQueryEngine3 = NearestNeighborQueryEngine<T, 3>;
}  // namespace CubbyFlow

#endif