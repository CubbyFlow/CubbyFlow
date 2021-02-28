// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FLATBUFFERS_HELPER_HPP
#define CUBBYFLOW_FLATBUFFERS_HELPER_HPP

#include <Core/Matrix/Matrix.hpp>

#include <Flatbuffers/generated/BasicTypes_generated.h>

namespace CubbyFlow
{
inline fbs::Vector2UZ CubbyFlowToFlatbuffers(const Vector2UZ& vec)
{
    return fbs::Vector2UZ(vec.x, vec.y);
}

inline fbs::Vector3UZ CubbyFlowToFlatbuffers(const Vector3UZ& vec)
{
    return fbs::Vector3UZ(vec.x, vec.y, vec.z);
}

inline fbs::Vector2D CubbyFlowToFlatbuffers(const Vector2D& vec)
{
    return fbs::Vector2D(vec.x, vec.y);
}

inline fbs::Vector3D CubbyFlowToFlatbuffers(const Vector3D& vec)
{
    return fbs::Vector3D(vec.x, vec.y, vec.z);
}

inline Vector2UZ FlatbuffersToCubbyFlow(const fbs::Vector2UZ& vec)
{
    return Vector2UZ{ { vec.x(), vec.y() } };
}

inline Vector3UZ FlatbuffersToCubbyFlow(const fbs::Vector3UZ& vec)
{
    return Vector3UZ{ { vec.x(), vec.y(), vec.z() } };
}

inline Vector2D FlatbuffersToCubbyFlow(const fbs::Vector2D& vec)
{
    return Vector2D{ vec.x(), vec.y() };
}

inline Vector3D FlatbuffersToCubbyFlow(const fbs::Vector3D& vec)
{
    return Vector3D{ vec.x(), vec.y(), vec.z() };
}

template <typename GridType, typename FbsFactoryFunc, typename FbsGridType>
void SerializeGrid(flatbuffers::FlatBufferBuilder* builder,
                   const std::vector<GridType>& gridList, FbsFactoryFunc func,
                   std::vector<flatbuffers::Offset<FbsGridType>>* fbsGridList)
{
    for (const auto& grid : gridList)
    {
        auto type = builder->CreateString(grid->TypeName());

        std::vector<uint8_t> gridSerialized;
        grid->Serialize(&gridSerialized);

        auto fbsGrid = func(*builder, type,
                            builder->CreateVector(gridSerialized.data(),
                                                  gridSerialized.size()));
        fbsGridList->push_back(fbsGrid);
    }
}

template <typename FbsGridList, typename GridType, typename FactoryFunc>
void DeserializeGrid(FbsGridList* fbsGridList, FactoryFunc factoryFunc,
                     std::vector<GridType>* gridList)
{
    for (const auto grid : *fbsGridList)
    {
        auto type = grid->type()->c_str();

        std::vector<uint8_t> gridSerialized(grid->data()->begin(),
                                            grid->data()->end());

        auto newGrid = factoryFunc(type);
        newGrid->Deserialize(gridSerialized);

        gridList->push_back(newGrid);
    }
}
}  // namespace CubbyFlow

#endif