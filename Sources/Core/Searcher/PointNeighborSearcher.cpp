// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Searcher/PointNeighborSearcher.hpp>

#include <limits>

namespace CubbyFlow
{
template <size_t N>
void PointNeighborSearcher<N>::Build(
    const ConstArrayView1<Vector<double, N>>& points)
{
    Build(points, std::numeric_limits<double>::max());
}

template class PointNeighborSearcher<2>;

template class PointNeighborSearcher<3>;
}  // namespace CubbyFlow