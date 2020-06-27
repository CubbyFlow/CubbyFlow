/*************************************************************************
> File Name: SceneParser-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: parse scene for get camera, light, etc..
> Created Time: 2020/06/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_SCENEPARSER_IMPL_H
#define CUBBYFLOW_SCENEPARSER_IMPL_H

#include <Framework/View/PerspectiveCamera.h>
#include <Framework/View/OrthogonalCamera.h>
#include <Framework/View/Light.h>
#include <Framework/View/Pivot.h>
#include <Framework/View/Viewport.h>
#include <Framework/Renderable/PointsRenderable.h>
#include <Framework/Renderable/TriangleMeshRenderable.h>
#include <Framework/Utils/Common.h>
#include <Core/Vector/Vector2.h>
#include <Core/Vector/Vector3.h>
#include <cstring>
#include <cassert>

namespace CubbyFlow {
namespace CubbyRender {

    template <typename SceneObject>
    std::shared_ptr<SceneObject> SceneParser::getSceneObject(const std::string& objName)
    {
        auto iter = _metadata.find(objName);
        if (iter == _metadata.end())
        {
            CUBBYFLOW_ERROR << "Failed to find [" << objName << "] from the scene file.";
            return nullptr;
        }
        const std::vector<unsigned char>& byteArray = *iter;
        const size_t byteLength = byteArray.size();

        std::shared_ptr<SceneObject> instance = std::make_shared<SceneObject>();
        std::memcpy(static_cast<void*>(instance.get()), static_cast<void*>(byteArray.data()), byteLength);

        return instance;
    }

    template <typename SceneObject>
    void SceneParser::parseSceneObject(const nlohmann::json& json)
    {
        static_assert("No implementation exists.");
    }

    

    template <typename SceneObject>
    void SceneParser::onLoadSceneObject(const std::string& name,  SceneObject object)
    {
        std::vector<unsigned char> byteArray(sizeof(object));
        std::memcpy(static_cast<void*>(byteArray.data()), static_cast<void*>(&object), sizeof(object));
        _metadata.emplace(name, byteArray);
    }
} 
}

#endif