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

#include <Framework/View/Camera.h>
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

    template <>
    void SceneParser::parseSceneObject<PointsRenderable>(const nlohmann::json& json)
    {
        const std::string& dir = json["dir"].get<std::string>();
        const std::string& format = json["format"].get<std::string>();
        const int numPointsCache = json["count"].get<int>();
    }
    template <>
    void SceneParser::parseSceneObject<TriangleMeshRenderable>(const nlohmann::json& json)
    {
        const std::string& objPath = json["path"].get<std::string>();
        const float scale = json["scale"].get<float>();
        const Vector3F translation = { json["translation"][0].get<float>(), json["translation"][1].get<float>(), json["translation"][2].get<float>() };
    }
    template <>
    void SceneParser::parseSceneObject<Camera>(const nlohmann::json& json)
    {
        Viewport viewport;
        viewport.leftTop = { json["viewport"][0].get<float>(), json["viewport"][1].get<float>() };
        viewport.rightBottom = { json["viewport"][2].get<float>(), json["viewport"][3].get<float>() };

        Pivot pivot;
        pivot.origin = {json["origin"][0].get<float>(), json["origin"][1].get<float>(), json["origin"][2].get<float>() };
        pivot.lookAt = {json["target"][0].get<float>(), json["target"][1].get<float>(), json["target"][2].get<float>() };

        float zNear = json["near"].get<float>();
        float zFar = json["far"].get<float>();
        float fovy = json["fovy"].get<float>();

        const std::string methodStr = json["method"].get<std::string>();
        ProjectionMethod method;
        if (methodStr == "perspective") method = ProjectionMethod::PERSPECTIVE;
        else if (methodStr == "orthogonal") method = ProjectionMethod::ORTHOGONAL;
        else 
        {
            CUBBYFLOW_ERROR << "Unknown camera projection method.";
            std::abort();
        }
    }
    template <>
    void SceneParser::parseSceneObject<Light>(const nlohmann::json& json)
    {
        
    }
    //! template <>
    //! void SceneParser::parseSceneObject<RenderSetting>(const nlohmann::json& json)
    //! {
    //!     
    //! }
} 
}

#endif