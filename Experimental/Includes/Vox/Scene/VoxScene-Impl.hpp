/*************************************************************************
> File Name: VoxScene-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: parse scene for get camera, light, etc..
> Created Time: 2020/06/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_SCENE_IMPL_HPP
#define CUBBYFLOW_VOX_SCENE_IMPL_HPP

#include <Vox/Utils/DebugUtils.hpp>
#include <Vox/Utils/FileSystem.hpp>
#include <Core/Vector/Vector2.hpp>
#include <Core/Vector/Vector3.hpp>
#include <cstring>
#include <cassert>

namespace Vox {

    template <typename Type>
    std::shared_ptr<Type> VoxScene::GetSceneObject(const std::string& objName)
    {
        auto iter = _metadata.find(objName);
        VoxAssert(iter != _metadata.end(), CURRENT_SRC_PATH_TO_STR, "Failed to find [" + objName + "] from the scene file");

        std::shared_ptr<VoxSceneObject> obj = iter->second;

        std::shared_ptr<Type> instance = std::dynamic_pointer_cast<Type>(obj);
        VoxAssert(instance != nullptr, CURRENT_SRC_PATH_TO_STR, "Cannot down-casting scene object with name [" + objName + "]");

        return instance;
    }

    template <typename Type>
    void VoxScene::OnLoadSceneObject(const nlohmann::json& json)
    {
        static_assert("No implementation exists.");
    }
} 

#endif