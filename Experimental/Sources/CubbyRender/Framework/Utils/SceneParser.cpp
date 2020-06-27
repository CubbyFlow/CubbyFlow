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