/*************************************************************************
> File Name: SceneParser.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: parse scene for get camera, light, etc..
> Created Time: 2020/06/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Framework/Scene/SceneParser.h>
#include <Framework/Utils/GeometryCacheParser.h>
#include <Framework/Utils/Common.h>
#include <Framework/View/Camera.h>
#include <Framework/Renderable/PointsRenderable.h>
#include <Framework/Renderable/TriangleMeshRenderable.h>
#include <Framework/Buffer/Vertex.h>
#include <Core/Array/Array1.h>
#include <Core/Vector/Vector3.h>
#include <Core/Matrix/Matrix4x4.h>
#include <Core/Utils/Timer.h>
#include <pystring/pystring.h>
#include <fstream>
#include <cassert>
#include <iostream>

namespace CubbyFlow {
namespace CubbyRender {
    SceneParser::SceneParser()
    {
        //! Do nothing
    }

    SceneParser::SceneParser(const std::string& path)
    {   
        loadScene(path);
    }

    SceneParser::~SceneParser()
    {
        _metadata.clear();
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
    void SceneParser::onLoadSceneObject<PointsRenderable>(const nlohmann::json& json)
    {
        const std::string& name = json["name"].get<std::string>();
        const std::string& dir = json["dir"].get<std::string>();
        const std::string& format = json["format"].get<std::string>();
        const size_t numPointsCache = json["count"].get<size_t>();    

        std::string fileformat = pystring::os::path::join(dir, format);
        
        GeometryCacheParserPtr parser = std::make_shared<GeometryCacheParser>();
        parser->loadGeometryCache(fileformat, numPointsCache);
        
        const float scale = json["scale"].get<float>();
        const Vector3F translation(json["translation"][0].get<float>(), json["translation"][1].get<float>(), json["translation"][2].get<float>());

        const Matrix4x4F st = Matrix4x4F::MakeScaleMatrix(Vector3F(scale, scale, scale)) * Matrix4x4F::MakeTranslationMatrix(translation);

        for (size_t i = 0; i < parser->getNumberOfGeometryCache(); ++i)
        {
            auto vertices = parser->getVertexCache(i);
            const size_t numberOfFloats = VertexHelper::getNumberOfFloats(VertexFormat::Position3Normal3);
            const size_t numVertices = static_cast<size_t>(vertices.size() / numberOfFloats);
            ParallelFor(ZERO_SIZE, numVertices, [&](size_t index){
                const size_t baseIndex = index * numberOfFloats;
                Vector4F pos = st * Vector4F(vertices[baseIndex + 0], vertices[baseIndex + 1], vertices[baseIndex + 2], 1.0f);
                vertices[baseIndex]     = pos.x;
                vertices[baseIndex + 1] = pos.y;
                vertices[baseIndex + 2] = pos.z;
            });
        }
        
        const float radius = json["radius"].get<float>();

        _metadata.emplace(name + std::string("Cache"), parser);
        _metadata.emplace(name, std::make_shared<PointsRenderable>(parser->getVertexCache(0), Vector3F(221 / 255.0f, 145 / 255.0f, 48 / 255.0f), radius));
    }

    template <>
    void SceneParser::onLoadSceneObject<TriangleMeshRenderable>(const nlohmann::json& json)
    {
        const std::string& name = json["name"].get<std::string>();
        const std::string& dir = json["dir"].get<std::string>();
        const std::string& format = json["format"].get<std::string>();
        const size_t numPointsCache = json["count"].get<size_t>();    

        std::string fileformat = pystring::os::path::join(dir, format);
        
        GeometryCacheParserPtr parser = std::make_shared<GeometryCacheParser>();
        parser->loadGeometryCache(fileformat, numPointsCache);
        
        const float scale = json["scale"].get<float>();
        const Vector3F translation(json["translation"][0].get<float>(), json["translation"][1].get<float>(), json["translation"][2].get<float>());

        const Matrix4x4F st = Matrix4x4F::MakeScaleMatrix(Vector3F(scale, scale, scale)) * Matrix4x4F::MakeTranslationMatrix(translation);

        for (size_t i = 0; i < parser->getNumberOfGeometryCache(); ++i)
        {
            auto vertices = parser->getVertexCache(i);
            const size_t numberOfFloats = VertexHelper::getNumberOfFloats(VertexFormat::Position3Normal3);
            const size_t numVertices = static_cast<size_t>(vertices.size() / numberOfFloats);
            ParallelFor(ZERO_SIZE, numVertices, [&](size_t index){
                const size_t baseIndex = index * numberOfFloats;
                Vector4F pos = st * Vector4F(vertices[baseIndex + 0], vertices[baseIndex + 1], vertices[baseIndex + 2], 1.0f);
                vertices[baseIndex]     = pos.x;
                vertices[baseIndex + 1] = pos.y;
                vertices[baseIndex + 2] = pos.z;
            });
        }
        
        _metadata.emplace(name + std::string("Cache"), parser);
        _metadata.emplace(name, std::make_shared<TriangleMeshRenderable>(parser->getVertexCache(0), parser->getIndexCache(0)));
    }

    template <>
    void SceneParser::onLoadSceneObject<Camera>(const nlohmann::json& json)
    {
        Pivot pivot;

        Viewport viewport;
        viewport.leftTop = { json["viewport"][0].get<float>(),  json["viewport"][1].get<float>()};
        viewport.rightBottom = { json["viewport"][2].get<float>(),  json["viewport"][3].get<float>()};
        pivot.viewport = viewport;

        pivot.origin = {json["origin"][0].get<float>(), json["origin"][1].get<float>(), json["origin"][2].get<float>() };
        const Vector3F target = {json["target"][0].get<float>(), json["target"][1].get<float>(), json["target"][2].get<float>() };
        pivot.lookAt = (target - pivot.origin).Normalized();
        pivot.zNear = json["near"].get<float>();
        pivot.zFar = json["far"].get<float>();

        const std::string methodStr = json["method"].get<std::string>();
        const std::string name = json["name"].get<std::string>();
        if (methodStr == "perspective")
        {
            float fov = json["fov"].get<float>();
            _metadata.emplace(name, std::make_shared<PerspectiveCamera>(pivot, fov));
        }
        else if (methodStr == "orthogonal")
        {
            _metadata.emplace(name, std::make_shared<OrthogonalCamera>(pivot));
        }
        else 
        {
            CUBBYFLOW_ERROR << "Unknown camera projection method.";
            std::abort();
        }
    }
    template <>
    void SceneParser::onLoadSceneObject<Light>(const nlohmann::json& json)
    {
        Pivot pivot;

        Viewport viewport;
        viewport.leftTop = { json["viewport"][0].get<float>(),  json["viewport"][1].get<float>()};
        viewport.rightBottom = { json["viewport"][2].get<float>(),  json["viewport"][3].get<float>()};
        pivot.viewport = viewport;

        pivot.origin = {json["origin"][0].get<float>(), json["origin"][1].get<float>(), json["origin"][2].get<float>() };
        const Vector3F target = {json["target"][0].get<float>(), json["target"][1].get<float>(), json["target"][2].get<float>() };
        pivot.lookAt = (target - pivot.origin).Normalized();
        pivot.zNear = json["near"].get<float>();
        pivot.zFar = json["far"].get<float>();

        const std::string name = json["name"].get<std::string>();
        _metadata.emplace(name, std::make_shared<Light>(pivot));
    }
    template <>
    void SceneParser::onLoadSceneObject<Material>(const nlohmann::json& json)
    {
        UNUSED_VARIABLE(json);
    }

    void SceneParser::onLoadScene(const nlohmann::json& json)
    {
        ParallelFor(ZERO_SIZE, json.size(), [&](size_t index){
            auto& data = json[index];
            const std::string objectType = data["type"].get<std::string>();

            Timer timer;
            if (objectType == "points") onLoadSceneObject<PointsRenderable>(data);
            else if (objectType == "object") onLoadSceneObject<TriangleMeshRenderable>(data);
            else if (objectType == "camera") onLoadSceneObject<Camera>(data);
            else if (objectType == "light") onLoadSceneObject<Light>(data);
            else if (objectType == "material") onLoadSceneObject<Material>(data);
            //! else if (objectType == "render") parseSceneObject<RenderSetting>(data);
            else 
            {
                CUBBYFLOW_ERROR << "Unknown scene object type.";
                std::abort();
            }
            CUBBYFLOW_INFO << "Parse scene object with name [" << data["name"].get<std::string>() << "] took " << timer.DurationInSeconds() << " seconds";
        });
    }
}
}