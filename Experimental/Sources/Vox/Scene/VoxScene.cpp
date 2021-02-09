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
#include <Vox/Core/CubeMap.hpp>
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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
        const auto& filename = FileSystem::FindPath(path);
        VoxAssert(filename.IsNullPath() == false, CURRENT_SRC_PATH_TO_STR, "No Scene File with Path[" + path.ToString() + "]");
        return std::make_shared<VoxScene>(filename);
    }

    void VoxScene::LoadScene(const Vox::Path& path)
    {
        pugi::xml_document document;
        
        //! Load XML scene file.
        auto result = document.load_file(path.ToCStr());
        VoxAssert(result, CURRENT_SRC_PATH_TO_STR, "Cannot Open Scene File with Path [" + path.ToString() + "]");

        std::cout << "Loading " << path.ToString() << " scene" << std::endl;

        //! Start parsing each scene objects from the scene file.
        OnLoadScene(document);
    }

    //! Camera loader
    template <>
    void VoxScene::OnLoadSceneObject<Camera>(const pugi::xml_node& node)
    {
        //! Get transform information of the camera.
        const auto& lookAt = node.child("transform").child("lookat");
        const CubbyFlow::Vector3F origin = Detail::ParseFromString(lookAt.attribute("origin").value());
        const CubbyFlow::Vector3F target = Detail::ParseFromString(lookAt.attribute("target").value());
        const CubbyFlow::Vector3F up = Detail::ParseFromString(lookAt.attribute("up").value());

        //! Get the type of camera. It would be perspective or orthogonal.
        const std::string type = node.attribute("type").value();

        //! Create camera with given camera type.
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

        //! Set transform to the camera.
        camera->SetViewTransform(origin, target, up);

        //! Push parsed camera instance to the unordered_map.
        _metadata.emplace(VoxStringID(node.attribute("name").value()), camera);
    }

    //! Shader program Loader
    template <>
    void VoxScene::OnLoadSceneObject<Program>(const pugi::xml_node& node)
    {
        //! At first, vs, fs and gs are initialized by zero.
        GLuint vs{ 0 }, fs{ 0 }, gs{ 0 };
        for (const auto& shader : node.child("shader"))
        {
            //! Parse shader type. It would be vs, fs or gs.
            const std::string type = shader.name();
            const auto& path = FileSystem::FindPath(shader.attribute("value").value()).ToString();
            
            //! Create shader resource with given shader type.
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
        
        //! Create program from generated shaders at above.
        auto program = std::make_shared<Program>(Renderer::CreateProgram(vs, gs, fs));

        //! After program generation, delete shaders because they are out of need anymore.
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        if (gs) glDeleteShader(gs);

        //! Get reference of the shader parameters.
        auto& shaderParams = program->GetParameters();

        //! Store uniform variables which will be passed at once in the render loop.
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

        //! Push parsed shader program instance to the unordered_map.
        _metadata.emplace(VoxStringID(node.attribute("name").value()), program);
    }

    //! Material Loader
    template <>
    void VoxScene::OnLoadSceneObject<Material>(const pugi::xml_node& node)
    {
        auto material = std::make_shared<Material>();

        //! Get already parsed shader program from this VoxScene instance.
        //! **The required program must precede material.**
        const auto& program = GetSceneObject<Program>(node.child("program").attribute("value").value());
        material->AttachProgramShader(program);

        //! Parse brdf material properties
        Material::BRDF brdf;
        brdf.albedo = Detail::ParseFromString(node.child("brdf").child("albedo").attribute("value").value());
        brdf.metallic = node.child("brdf").child("metallic").attribute("value").as_float();
        brdf.roughness = node.child("brdf").child("roughness").attribute("value").as_float();
        brdf.ao = node.child("brdf").child("ao").attribute("value").as_float();
        
        //! Send brdf properties to the material
        material->SetBRDF(brdf);

        //! Push parsed material to the unordered_map.
        _metadata.emplace(VoxStringID(node.attribute("name").value()), material);
    }

    //! Animation Loader
    template <>
    void VoxScene::OnLoadSceneObject<FluidBuffer>(const pugi::xml_node& node)
    {
        //! Parse fluid animation(which is sequential list of the obj files) path format and count of the obj files.
        //! path format looks like this : path%06d.obj 
        const std::string format = node.find_child_by_attribute("string", "name", "format").attribute("value").value();
        const int count = node.find_child_by_attribute("integer", "name", "count").attribute("value").as_int();

        //! Loading obj files from format and count.
        auto manager = std::make_shared<GeometryCacheManager>(format, count);

        //! Parse transform information of total animation bound
        const auto& transform = node.find_child_by_attribute("transform", "name", "toWorld");
        const CubbyFlow::Vector3F translate = Detail::ParseFromString(transform.child("translate").attribute("value").value());
        const CubbyFlow::Vector3F scale = Detail::ParseFromString(transform.child("scale").attribute("value").value());
        const CubbyFlow::Vector3F rotateAxis = Detail::ParseFromString(transform.child("rotate").attribute("axis").value());
        const float rotateDegree = transform.child("rotate").attribute("degree").as_float();

        //! Apply transformation to each object files.
        CubbyFlow::ParallelFor(CubbyFlow::ZERO_SIZE, manager->GetNumberOfCache(), [&](size_t index){
            const auto& cache = manager->GetGeometryCache(index);
            cache->TransformCache(translate, scale, rotateAxis, CubbyFlow::DegreesToRadians(rotateDegree));
            cache->InterleaveData(Vox::VertexFormat::Position3Normal3);
        });

        //! Get already parsed material instance from this VoxScene instance.
        //! **The required material must precede FluidAnim.**
        const std::string materialName = node.child("material").attribute("value").value();
        const auto& material = GetSceneObject<Material>(materialName);
        
        //! Create fluid buffer from the parsed material, program and geometry cache.
        auto fluidBuffer = std::make_shared<FluidBuffer>();
        fluidBuffer->AttachGeometryCacheManager(manager);
        fluidBuffer->AttachMaterial(material);

        //! Push parsed fluid animation to the unordered_map.
        _metadata.emplace(VoxStringID(node.attribute("name").value()), fluidBuffer);
    }

    //! Static Object Loader
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

        //! Push parsed static scene object to the unordered_map.
        // _metadata.emplace(VoxStringID(node.attribute("name").value()), manager);
    }

    //! Emitter Loader
    template <>
    void VoxScene::OnLoadSceneObject<Emitter>(const pugi::xml_node& node)
    {
        stbi_set_flip_vertically_on_load(true);

        //! Get hdr image filename
        const char* filename = node.find_child("hdr").attribute("value").value();

        //! Get floating point data pointer from the hdr image.
        int width, height, numChannel;
        float* data = stbi_loadf(filename, &width, &height, &numChannel, 0);

        //! Create texture with floating point format.
        GLuint hdrTexture = Renderer::CreateTexture(width, height, PixelFmt::PF_RGB16F, data);

        //! Deallocates the image data.
        stbi_image_free(data);

        //! Parse the baking resolution
        const auto& resolution = node.find_child("resolution");
        GLsizei resolutionX = resolution.attribute("x").as_uint();
        GLsizei resolutionY = resolution.attribute("y").as_uint();

        //! Baking cubemap from the hdr texture     
        std::shared_ptr<Emitter> cubemap = std::make_shared<CubeMap>(hdrTexture, resolutionX, resolutionY);

        //! Push parsed light emitter to the unordered_map.
         _metadata.emplace(VoxStringID(node.attribute("name").value()), cubemap);
    }

    void VoxScene::OnLoadScene(const pugi::xml_document& document)
    {
        //! Looping over scene nodes.
        for (const auto& data : document.child("scene"))
        {
            //! Detect the type of object by its name.
            const std::string objectType = data.name();

            std::cout << "Loading " << data.attribute("name").value();
            if (objectType == "sensor") OnLoadSceneObject<Camera>(data);
            else if (objectType == "material") OnLoadSceneObject<Material>(data);
            else if (objectType == "program") OnLoadSceneObject<Program>(data);
            else if (objectType == "anim") OnLoadSceneObject<FluidBuffer>(data);
            else if (objectType == "shape") OnLoadSceneObject<GeometryCache>(data);
            else if (objectType == "emitter") OnLoadSceneObject<Emitter>(data);
            else VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown Scene Object Type [" + objectType + "]");
            std::cout << " done" << std::endl;
        };
    }
}