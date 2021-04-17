// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FACTORY_HPP
#define CUBBYFLOW_FACTORY_HPP

#include <Core/Grid/ScalarGrid.hpp>
#include <Core/Grid/VectorGrid.hpp>
#include <Core/Searcher/PointNeighborSearcher.hpp>

namespace CubbyFlow
{
class Factory
{
 public:
    static ScalarGrid2Ptr BuildScalarGrid2(const std::string& name);

    static ScalarGrid3Ptr BuildScalarGrid3(const std::string& name);

    static VectorGrid2Ptr BuildVectorGrid2(const std::string& name);

    static VectorGrid3Ptr BuildVectorGrid3(const std::string& name);

    static PointNeighborSearcher2Ptr BuildPointNeighborSearcher2(
        const std::string& name);

    static PointNeighborSearcher3Ptr BuildPointNeighborSearcher3(
        const std::string& name);
};
}  // namespace CubbyFlow

#endif