/*************************************************************************
> File Name: VoxScene.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: parse scene for get camera, light, etc..
> Created Time: 2020/06/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/VoxScene.hpp>
#include <Vox/DebugUtils.hpp>
#include <Vox/PerspectiveCamera.hpp>
#include <Vox/GeometryCacheManager.hpp>
#include <Vox/GeometryCache.hpp>
#include <Core/Array/Array1.hpp>
#include <Core/Vector/Vector3.hpp>
#include <Core/Matrix/Matrix4x4.hpp>
#include <Core/Utils/Timer.hpp>
#include <pystring/pystring.h>
#include <fstream>
#include <cassert>
#include <iostream>

namespace Vox {
    VoxScene::VoxScene()
    {
        //! Do nothing
    }

    VoxScene::VoxScene(const Vox::Path& path)
    {
        LoadScene(path);
    }

    VoxScene::~VoxScene()
    {
        _metadata.clear();
    }

    std::shared_ptr<VoxScene> VoxScene::CreateScene(const Vox::Path& path)
    {
        const auto& result = FileSystem::FindPath(path);
        VoxAssert(result.IsNullPath() == false, CURRENT_SRC_PATH_TO_STR, "No Scene File with Path[" + path.ToString() + "]");
        return std::make_shared<VoxScene>(result);
    }

    void VoxScene::LoadScene(const Vox::Path& path)
    {
        std::ifstream sceneFile(path.ToString());

        VoxAssert(sceneFile.is_open(), CURRENT_SRC_PATH_TO_STR, "Cannot Open Scene File with Path [" + path.ToString() + "]");

        auto json = nlohmann::json::parse(sceneFile);
        OnLoadScene(json);
    }

    template <>
    void VoxScene::OnLoadSceneObject<PerspectiveCamera>(const nlohmann::json& json)
    {
        const CubbyFlow::Vector3F origin = {json["origin"][0].get<float>(), json["origin"][1].get<float>(), json["origin"][2].get<float>() };
        const CubbyFlow::Vector3F target = {json["target"][0].get<float>(), json["target"][1].get<float>(), json["target"][2].get<float>() };
        const float fov = json["fov"].get<float>();
        const float near = json["near"].get<float>();
        const float far  = json["far"].get<float>();
        //const bool controllable = json["controllable"].get<bool>();

        const std::string name = json["name"].get<std::string>();
        auto camera = std::make_shared<PerspectiveCamera>();

        camera->SetViewTransform(origin, target);
        camera->SetViewFrustum(fov, near, far);

        _metadata.emplace(name, camera);
    }

    template <>
    void VoxScene::OnLoadSceneObject<GeometryCacheManager>(const nlohmann::json& json)
    {
        const std::string name = json["name"].get<std::string>();
        const std::string format = json["format"].get<std::string>();
        const size_t count = json["count"].get<size_t>();
        const CubbyFlow::Vector3F translate = {json["translate"][0].get<float>(), json["translate"][1].get<float>(), json["translate"][2].get<float>() };
        const float scale = json["scale"].get<float>();

        auto manager = std::make_shared<GeometryCacheManager>(format, count);
        CubbyFlow::ParallelFor(CubbyFlow::ZERO_SIZE, manager->GetNumberOfCache(), [&](size_t index){
            const auto& cache = manager->GetGeometryCache(index);
            cache->TranslateCache(translate);
            cache->ScaleCache(scale);
        });

        _metadata.emplace(name, manager);
    }

    template <>
    void VoxScene::OnLoadSceneObject<GeometryCache>(const nlohmann::json& json)
    {
        const std::string name = json["name"].get<std::string>();
        const std::string format = json["format"].get<std::string>();
        const CubbyFlow::Vector3F translate = {json["translate"][0].get<float>(), json["translate"][1].get<float>(), json["translate"][2].get<float>() };
        const float scale = json["scale"].get<float>();

        auto object = std::make_shared<GeometryCache>(format);
        object->TranslateCache(translate);
        object->ScaleCache(scale);

        _metadata.emplace(name, object);
    }

    void VoxScene::OnLoadScene(const nlohmann::json& json)
    {
        for (const auto& data : json)
        {
            const std::string objectType = data["type"].get<std::string>();

            if (objectType == "camera") OnLoadSceneObject<PerspectiveCamera>(data);
            else if (objectType == "geometry_cache") OnLoadSceneObject<GeometryCacheManager>(data);
            else if (objectType == "static_object") OnLoadSceneObject<GeometryCache>(data);
            else VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown Scene Object Type [" + objectType + "]");
        };
    }
}