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
#include <Vox/Core/FluidBuffer.hpp>
#include <Vox/Core/Material.hpp>
#include <Core/Math/MathUtils.hpp>
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
#include <iostream>

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

        std::cout << "Loading " << path.ToString() << " scene" << std::endl;
        OnLoadScene(document);
    }

    //! Camera loader
    template <>
    void VoxScene::OnLoadSceneObject<Camera>(const pugi::xml_node& node)
    {
        std::cout << "Loading " << node.attribute("name").value();

        const auto& lookAt = node.child("transform").child("lookat");
        const CubbyFlow::Vector3F origin = Detail::ParseFromString(lookAt.attribute("origin").value());
        const CubbyFlow::Vector3F target = Detail::ParseFromString(lookAt.attribute("target").value());
        const CubbyFlow::Vector3F up = Detail::ParseFromString(lookAt.attribute("up").value());

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

        std::cout << " done" << std::endl;
    }

    //! Shader program Loader
    template <>
    void VoxScene::OnLoadSceneObject<Program>(const pugi::xml_node& node)
    {
        std::cout << "Loading " << node.attribute("name").value();

        GLuint vs{ 0 }, fs{ 0 }, gs{ 0 };
        for (const auto& shader : node.child("shader"))
        {
            const std::string type = shader.name();
            const auto& path = FileSystem::FindPath(shader.attribute("value").value()).ToString();
            
            if (type == "vs")
            {
                vs = Renderer::CreateShaderFromFile(path, GL_VERTEX_SHADER);
            }
            else if (type == "fs")
            {
                fs = Renderer::CreateShaderFromFile(path, GL_FRAGMENT_SHADER);
            }
            else if (type == "gs")
            {
                gs = Renderer::CreateShaderFromFile(path, GL_GEOMETRY_SHADER);
            }
            else
            {
                VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown shader type : " + type);
            }
        }
        
        auto program = std::make_shared<Program>(Renderer::CreateProgram(vs, gs, fs));
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        if (gs) glDeleteShader(gs);

        auto& shaderParams = program->GetParameters();
        for (const auto& uniform : node.child("uniform"))
        {
            const std::string name = uniform.attribute("name").value();
            const std::string type = uniform.name();

            const auto& valueNode = uniform.attribute("value");
            if (type == "integer")
            {
                shaderParams.SetParameter(name, valueNode.as_int());
            }
            else if (type == "float")
            {
                shaderParams.SetParameter(name, valueNode.as_float());
            }
            else if (type == "rgb" || type == "xyz")
            {
                shaderParams.SetParameter(name, Detail::ParseFromString(valueNode.value()));
            }
            else
            {
                VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown shader uniform variable type : " + type);
            }
        }

        _metadata.emplace(VoxStringID(node.attribute("name").value()), program);
        std::cout << " done" << std::endl;
    }

    //! Material Loader
    template <>
    void VoxScene::OnLoadSceneObject<Material>(const pugi::xml_node& node)
    {
        std::cout << "Loading " << node.attribute("name").value();

        auto material = std::make_shared<Material>();

        const auto& program = GetSceneObject<Program>(node.child("program").attribute("value").value());
        material->AttachProgramShader(program);

        Material::BRDF brdf;
        brdf.albedo = Detail::ParseFromString(node.child("brdf").child("albedo").attribute("value").value());
        brdf.metallic = node.child("brdf").child("metallic").attribute("value").as_float();
        brdf.roughness = node.child("brdf").child("roughness").attribute("value").as_float();
        brdf.ao = node.child("brdf").child("ao").attribute("value").as_float();
        
        material->SetBRDF(brdf);

        _metadata.emplace(VoxStringID(node.attribute("name").value()), material);
        std::cout << " done" << std::endl;
    }

    //! Animation Loader
    template <>
    void VoxScene::OnLoadSceneObject<FluidBuffer>(const pugi::xml_node& node)
    {
        std::cout << "Loading " << node.attribute("name").value();

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
            cache->TransformCache(translate, scale, rotateAxis, CubbyFlow::DegreesToRadians(rotateDegree));
            cache->InterleaveData(Vox::VertexFormat::Position3Normal3);
        });

        const std::string materialName = node.child("material").attribute("value").value();
        const auto& material = GetSceneObject<Material>(materialName);
        
        auto fluidBuffer = std::make_shared<FluidBuffer>();
        fluidBuffer->AttachGeometryCacheManager(manager);
        fluidBuffer->AttachMaterial(material);

        _metadata.emplace(VoxStringID(node.attribute("name").value()), fluidBuffer);

        std::cout << " done" << std::endl;
    }

    //! Static Object Loader
    template <>
    void VoxScene::OnLoadSceneObject<GeometryCache>(const pugi::xml_node& node)
    {
        std::cout << "Loading " << node.attribute("name").value();

        UNUSED_VARIABLE(node);
        // const std::string name = json["name"].get<std::string>();
        // const std::string format = json["format"].get<std::string>();
        // const CubbyFlow::Vector3F translate = {json["translate"][0].get<float>(), json["translate"][1].get<float>(), json["translate"][2].get<float>() };
        // const float scale = json["scale"].get<float>();

        // auto object = std::make_shared<GeometryCache>(format);
        // object->TranslateCache(translate);
        // object->ScaleCache(scale);

        // _metadata.emplace(VoxStringID(node.attribute("name").value()), manager);

        std::cout << " done" << std::endl;
    }

    //! Emitter Loader
    template <>
    void VoxScene::OnLoadSceneObject<Emitter>(const pugi::xml_node& node)
    {
        std::cout << "Loading " << node.attribute("name").value();
        UNUSED_VARIABLE(node);

        // _metadata.emplace(VoxStringID(node.attribute("name").value()), manager);
        std::cout << " done" << std::endl;
    }

    void VoxScene::OnLoadScene(const pugi::xml_document& document)
    {
        for (const auto& data : document.child("scene"))
        {
            const std::string objectType = data.name();

            if (objectType == "sensor") OnLoadSceneObject<Camera>(data);
            else if (objectType == "material") OnLoadSceneObject<Material>(data);
            else if (objectType == "program") OnLoadSceneObject<Program>(data);
            else if (objectType == "anim") OnLoadSceneObject<FluidBuffer>(data);
            else if (objectType == "shape") OnLoadSceneObject<GeometryCache>(data);
            else if (objectType == "emitter") OnLoadSceneObject<Emitter>(data);
            else VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown Scene Object Type [" + objectType + "]");
        };
    }
}