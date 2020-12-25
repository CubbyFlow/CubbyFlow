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
#include <Vox/Core/Renderer.hpp>
#include <Vox/Core/Program.hpp>
#include <Vox/Core/Emitter.hpp>
#include <Vox/Core/Material.hpp>
#include <Vox/Utils/MathUtils.hpp>
#include <Vox/Utils/FileSystem.hpp>
#include <Vox/Camera/PerspectiveCamera.hpp>
#include <Vox/Camera/OrthographicCamera.hpp>
#include <Vox/Scene/GeometryCacheManager.hpp>
#include <Vox/Scene/GeometryCache.hpp>
#include <Core/Array/Array1.hpp>
#include <Core/Vector/Vector3.hpp>
#include <Core/Matrix/Matrix4x4.hpp>
#include <Core/Utils/Timer.hpp>
#include <pystring/pystring.h>
#include <glad/glad.h>
#include <fstream>
#include <sstream>

namespace Vox {

    namespace Detail
    {
        CubbyFlow::Vector3F ParseFromString(const std::string& val)
        {
            std::istringstream isstr(val);
            CubbyFlow::Vector3F ret;

            isstr >> ret.x >> ret.y >> ret.z;
            return ret;
        }
    }

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
        pugi::xml_document document;
        
        auto result = document.load_file(path.ToCStr());
        VoxAssert(result, CURRENT_SRC_PATH_TO_STR, "Cannot Open Scene File with Path [" + path.ToString() + "]");

        OnLoadScene(document);
    }

    template <>
    void VoxScene::OnLoadSceneObject<Camera>(const pugi::xml_node& node)
    {
        const auto& transform = node.child("transform");
        const CubbyFlow::Vector3F origin = Detail::ParseFromString(transform.attribute("origin").value());
        const CubbyFlow::Vector3F target = Detail::ParseFromString(transform.attribute("target").value());
        const CubbyFlow::Vector3F up = Detail::ParseFromString(transform.attribute("up").value());

        const std::string type = node.attribute("type").value();
        std::shared_ptr<Camera> camera;
        if (type == "perspective")
        {
            std::shared_ptr<PerspectiveCamera> temp = std::make_shared<PerspectiveCamera>();
            const float fov = node.find_child_by_attribute("float", "name", "fov").attribute("value").as_float();
            const float far = node.find_child_by_attribute("float", "name", "focusDistance").attribute("value").as_float();
            temp->SetViewFrustum(fov, far);
            camera = temp;
        }
        else if (type == "orthographic")
        {
            std::shared_ptr<OrthographicCamera> temp = std::make_shared<OrthographicCamera>();
            camera = temp;
        }
        else
        {
            VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown Camera Type [" + type + "]");
        }

        camera->SetViewTransform(origin, target, up);
        _metadata.emplace(VoxStringID(node.attribute("name").value()), camera);
    }

    template <>
    void VoxScene::OnLoadSceneObject<Material>(const pugi::xml_node& node)
    {
        UNUSED_VARIABLE(node);
        // GLuint vs = Renderer::CreateShaderFromFile(json["vs"].get<std::string>(), GL_VERTEX_SHADER);
        // GLuint fs = Renderer::CreateShaderFromFile(json["fs"].get<std::string>(), GL_FRAGMENT_SHADER);

        // auto program = std::make_shared<Program>(Renderer::CreateProgram(vs, 0, fs));

        // const std::string name = json["name"].get<std::string>();
        // auto material = std::make_shared<Material>();
        // material->AttachProgramShader(program);

        // _metadata.emplace(VoxStringID(node.attribute("name").value()), material);
    }

    template <>
    void VoxScene::OnLoadSceneObject<GeometryCacheManager>(const pugi::xml_node& node)
    {
        const std::string format = node.find_child_by_attribute("string", "name", "format").attribute("value").value();
        const int count = node.find_child_by_attribute("integer", "name", "count").attribute("value").as_int();
        auto manager = std::make_shared<GeometryCacheManager>(format, count);

        const auto& transform = node.find_child_by_attribute("transform", "name", "toWorld");
        const CubbyFlow::Vector3F translate = Detail::ParseFromString(transform.child("translate").attribute("value").value());
        const CubbyFlow::Vector3F scale = Detail::ParseFromString(transform.child("scale").attribute("value").value());
        const CubbyFlow::Vector3F rotateAxis = Detail::ParseFromString(transform.child("rotate").attribute("axis").value());
        const float rotateDegree = transform.child("rotate").attribute("degree").as_float();

        CubbyFlow::ParallelFor(CubbyFlow::ZERO_SIZE, manager->GetNumberOfCache(), [&](size_t index){
            const auto& cache = manager->GetGeometryCache(index);
            cache->TransformCache(translate, scale, rotateAxis, DegreeToRadian(rotateDegree));
        });

        _metadata.emplace(VoxStringID(node.attribute("name").value()), manager);
    }

    template <>
    void VoxScene::OnLoadSceneObject<GeometryCache>(const pugi::xml_node& node)
    {
        UNUSED_VARIABLE(node);
        // const std::string name = json["name"].get<std::string>();
        // const std::string format = json["format"].get<std::string>();
        // const CubbyFlow::Vector3F translate = {json["translate"][0].get<float>(), json["translate"][1].get<float>(), json["translate"][2].get<float>() };
        // const float scale = json["scale"].get<float>();

        // auto object = std::make_shared<GeometryCache>(format);
        // object->TranslateCache(translate);
        // object->ScaleCache(scale);

        // _metadata.emplace(VoxStringID(node.attribute("name").value()), manager);
    }

    template <>
    void VoxScene::OnLoadSceneObject<Emitter>(const pugi::xml_node& node)
    {
        UNUSED_VARIABLE(node);

        // _metadata.emplace(VoxStringID(node.attribute("name").value()), manager);
    }

    void VoxScene::OnLoadScene(const pugi::xml_document& document)
    {
        for (const auto& data : document.child("scene"))
        {
            const std::string objectType = data.name();

            if (objectType == "sensor") OnLoadSceneObject<Camera>(data);
            else if (objectType == "material") OnLoadSceneObject<Material>(data);
            else if (objectType == "anim") OnLoadSceneObject<GeometryCacheManager>(data);
            else if (objectType == "shape") OnLoadSceneObject<GeometryCache>(data);
            else if (objectType == "emitter") OnLoadSceneObject<Emitter>(data);
            else VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown Scene Object Type [" + objectType + "]");
        };
    }
}