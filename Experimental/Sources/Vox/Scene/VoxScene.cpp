/*************************************************************************
> File Name: VoxScene.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: parse scene for get camera, light, etc..
> Created Time: 2020/06/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Scene/VoxScene.hpp>
#include <Vox/Utils/DebugUtils.hpp>
#include <Vox/Utils/StringID.hpp>
#include <Vox/Core/Program.hpp>
#include <Vox/Core/PointLight.hpp>
#include <Vox/Core/FluidRenderable.hpp>
#include <Vox/Core/StaticRenderable.hpp>
#include <Vox/Core/Texture.hpp>
#include <Vox/Core/Material.hpp>
#include <Vox/Core/TransparentMaterial.hpp>
#include <Vox/Core/BRDFMaterial.hpp>
#include <Vox/Utils/FileSystem.hpp>
#include <Vox/Camera/PerspectiveCamera.hpp>
#include <fstream>
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
        const auto& filename = FileSystem::FindPath(path);
        VoxAssert(filename.IsNullPath() == false, CURRENT_SRC_PATH_TO_STR, "No Scene File with Path[" + path.ToString() + "]");
        return std::make_shared<VoxScene>(filename);
    }

    void VoxScene::DeallocateObject(const std::string& objName)
    {
        unsigned int key = VoxStringID(objName);
        auto iter = _metadata.find(key);
        VoxAssert(iter != _metadata.end(), CURRENT_SRC_PATH_TO_STR, "Failed to find [" + objName + "] from the scene file");

        _metadata.erase(iter);
    }

    void VoxScene::LoadScene(const Vox::Path& path)
    {
        //! Load XML scene file.
        auto result = _document.load_file(path.ToCStr());
        VoxAssert(result, CURRENT_SRC_PATH_TO_STR, "Cannot Open Scene File with Path [" + path.ToString() + "]");

        std::cout << "Loading " << path.ToString() << " scene" << std::endl;

        //! Start parsing each scene objects from the scene file.
        //! Looping over scene nodes.
        for (const auto& data : _document.child("scene"))
        {
            std::shared_ptr<VoxSceneObject> instance;

            //! Detect the type of object by its name.
            const std::string objectType = data.name();

            std::cout << "Loading " << data.attribute("name").value();

            if (objectType == "sensor") instance = std::make_shared<PerspectiveCamera>();
            else if (objectType == "brdf") instance = std::make_shared<BRDFMaterial>();
            else if (objectType == "transparent") instance = std::make_shared<TransparentMaterial>();
            else if (objectType == "program") instance = std::make_shared<Program>();
            else if (objectType == "anim") instance = std::make_shared<FluidRenderable>();
            else if (objectType == "shape") instance = std::make_shared<StaticRenderable>();
            else if (objectType == "light") instance = std::make_shared<PointLight>();
            else if (objectType == "texture") instance = std::make_shared<Texture>();
            else VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown Scene Object Type [" + objectType + "]");

            instance->LoadXMLNode(this, data);
            _metadata.emplace(VoxStringID(data.attribute("name").value()), instance);

            std::cout << " done" << std::endl;
        };
    }

    void VoxScene::WriteScene(const Vox::Path& path)
    {
        for (auto& dataPair : _xmlCache)
        {
            auto iter = _metadata.find(dataPair.first);

            if (iter == _metadata.end())
                continue;

            std::shared_ptr<VoxSceneObject> instance = iter->second;
            instance->WriteXMLNode(dataPair.second);
        };

        _document.save_file(path.ToCStr());
    }
}