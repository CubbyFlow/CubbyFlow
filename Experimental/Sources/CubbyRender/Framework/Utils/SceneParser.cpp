/*************************************************************************
> File Name: SceneParser.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: parse scene for get camera, light, etc..
> Created Time: 2020/06/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Utils/SceneParser.h>
#include <Framework/Utils/Common.h>
#include <Framework/View/Camera.h>
#include <Framework/Renderable/PointsRenderable.h>
#include <Framework/Renderable/TriangleMeshRenderable.h>
#include <fstream>
#include <cassert>

namespace CubbyFlow {
namespace CubbyRender {
    SceneParser::SceneParser()
    {
        //! Do nothing
    }

    SceneParser::~SceneParser()
    {
        //! Do nothing
    }

    void SceneParser::writeScene(const std::string& path)
    {
        std::ofstream sceneFile(path);

        if (sceneFile.is_open())
        {
            
        }
        else
        {
            CUBBYFLOW_ERROR << "Cannot create scene file [" << path << "]";
            std::abort();
        }
    }

    void SceneParser::loadScene(const std::string& path)
    {
        std::ifstream sceneFile(path);

        if (sceneFile.is_open())
        {
            auto json = nlohmann::json::parse(sceneFile);
            onLoadScene(json);
        }
        else
        {
            CUBBYFLOW_ERROR << "Cannot load scene file [" << path << "]";
            std::abort();
        }
    }

    template <>
    void SceneParser::parseSceneObject<PointsRenderable>(const nlohmann::json& json)
    {
        UNUSED_VARIABLE(json);
        //! const std::string& dir = json["dir"].get<std::string>();
        //! const std::string& format = json["format"].get<std::string>();
        //! const int numPointsCache = json["count"].get<int>();
        //! onLoadSceneObject(camera);
    }
    template <>
    void SceneParser::parseSceneObject<TriangleMeshRenderable>(const nlohmann::json& json)
    {
        UNUSED_VARIABLE(json);
        //! const std::string& objPath = json["path"].get<std::string>();
        //! const float scale = json["scale"].get<float>();
        //! const Vector3F translation = { json["translation"][0].get<float>(), json["translation"][1].get<float>(), json["translation"][2].get<float>() };
        //! onLoadSceneObject(camera);
    }
    template <>
    void SceneParser::parseSceneObject<Camera>(const nlohmann::json& json)
    {
        Pivot pivot;

        Viewport viewport;
        viewport.leftTop = { json["viewport"][0].get<float>(),  json["viewport"][1].get<float>()};
        viewport.rightBottom = { json["viewport"][2].get<float>(),  json["viewport"][3].get<float>()};
        pivot.viewport = viewport;

        pivot.origin = {json["origin"][0].get<float>(), json["origin"][1].get<float>(), json["origin"][2].get<float>() };
        pivot.lookAt = {json["target"][0].get<float>(), json["target"][1].get<float>(), json["target"][2].get<float>() };
        pivot.zNear = json["near"].get<float>();
        pivot.zFar = json["far"].get<float>();

        const std::string methodStr = json["method"].get<std::string>();
        const std::string name = json["name"].get<std::string>();
        if (methodStr == "perspective")
        {
            float fov = json["fov"].get<float>();
            PerspectiveCamera camera(pivot, fov);
            onLoadSceneObject(name, camera);
        }
        else if (methodStr == "orthogonal")
        {
            OrthogonalCamera camera(pivot);
            onLoadSceneObject(name, camera);
        }
        else 
        {
            CUBBYFLOW_ERROR << "Unknown camera projection method.";
            std::abort();
        }
    }
    template <>
    void SceneParser::parseSceneObject<Light>(const nlohmann::json& json)
    {
        Pivot pivot;

        Viewport viewport;
        viewport.leftTop = { json["viewport"][0].get<float>(),  json["viewport"][1].get<float>()};
        viewport.rightBottom = { json["viewport"][2].get<float>(),  json["viewport"][3].get<float>()};
        pivot.viewport = viewport;

        pivot.origin = {json["origin"][0].get<float>(), json["origin"][1].get<float>(), json["origin"][2].get<float>() };
        pivot.lookAt = {json["target"][0].get<float>(), json["target"][1].get<float>(), json["target"][2].get<float>() };
        pivot.zNear = json["near"].get<float>();
        pivot.zFar = json["far"].get<float>();

        Light light(pivot);
        const std::string name = json["name"].get<std::string>();
        onLoadSceneObject(name, light);
    }
    
    void SceneParser::onLoadScene(const nlohmann::json& json)
    {
        for (auto& data : json)
        {
            const std::string objectType = data["type"].get<std::string>();

            if (objectType == "points") parseSceneObject<PointsRenderable>(data);
            else if (objectType == "object") parseSceneObject<TriangleMeshRenderable>(data);
            else if (objectType == "camera") parseSceneObject<Camera>(data);
            else if (objectType == "light") parseSceneObject<Light>(data);
            //! else if (objectType == "render") parseSceneObject<RenderSetting>(data);
            else 
            {
                CUBBYFLOW_ERROR << "Unknown scene object type.";
                std::abort();
            }
        }
    }
}
}