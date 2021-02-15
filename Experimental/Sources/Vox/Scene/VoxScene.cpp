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
#include <Vox/Core/FluidRenderable.hpp>
#include <Vox/Core/StaticRenderable.hpp>
#include <Vox/Core/Texture.hpp>
#include <Vox/Core/Material.hpp>
#include <Vox/Core/TransparentMaterial.hpp>
#include <Vox/Core/BRDFMaterial.hpp>
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

    void VoxScene::DeallocateObject(const std::string& objName)
    {
        unsigned int key = VoxStringID(objName);
        auto iter = _metadata.find(key);
        VoxAssert(iter != _metadata.end(), CURRENT_SRC_PATH_TO_STR, "Failed to find [" + objName + "] from the scene file");

        _metadata.erase(iter);
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
        //! Parse the type of the material
        const std::string materialType = node.attribute("type").value();
        
        //! Get already parsed shader program from this VoxScene instance.
        //! **The required program must precede material.**
        const auto& program = GetSceneObject<Program>(node.child("program").attribute("value").value());

        std::shared_ptr<Material> material;
        if (materialType == "brdf")
        {
            auto temp = std::make_shared<BRDFMaterial>();
            temp->AttachProgramShader(program);

            //! Parse brdf material properties
            BRDFMaterial::BRDF brdf;
            brdf.albedo = Detail::ParseFromString(node.child("brdf").child("albedo").attribute("value").value());
            brdf.metallic = node.child("brdf").child("metallic").attribute("value").as_float();
            brdf.roughness = node.child("brdf").child("roughness").attribute("value").as_float();
            brdf.ao = node.child("brdf").child("ao").attribute("value").as_float();

            //! Send brdf properties to the material
            temp->SetBRDF(brdf);

            material = temp;
        }
        else if (materialType == "envmapping")
        {
            auto temp = std::make_shared<TransparentMaterial>();
            temp->AttachProgramShader(program);

            //! Parse refraction ratio
            const float refractionRatio = node.child("refraction").attribute("value").as_float();
            temp->SetRefractionRatio(refractionRatio);

            material = temp;
        }
        else
        {
            VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown material type [" + materialType + "]");
        }

        

        //! Push parsed material to the unordered_map.
        _metadata.emplace(VoxStringID(node.attribute("name").value()), material);
    }

    //! Animation Loader
    template <>
    void VoxScene::OnLoadSceneObject<FluidRenderable>(const pugi::xml_node& node)
    {
        //! Parse fluid animation(which is sequential list of the obj files) path format and count of the obj files.
        //! path format looks like this : path%06d.obj 
        const std::string format = node.find_child_by_attribute("string", "name", "format").attribute("value").value();
        const int count = node.find_child_by_attribute("integer", "name", "count").attribute("value").as_int();

        //! Loading obj files from format and count.
        auto manager = std::make_shared<GeometryCacheManager>(format, count);

        //! interleaving each geometry cache vertex data.
        CubbyFlow::ParallelFor(CubbyFlow::ZERO_SIZE, manager->GetNumberOfCache(), [&](size_t index){
            const auto& cache = manager->GetGeometryCache(index);
            cache->InterleaveData(Vox::VertexFormat::Position3Normal3);
        });

        //! Get already parsed material instance from this VoxScene instance.
        //! **The required material must precede FluidAnim.**
        const std::string materialName = node.child("material").attribute("value").value();
        const auto& material = GetSceneObject<Material>(materialName);
        
        //! Create fluid buffer from the parsed material, program and geometry cache.
        auto fluidBuffer = std::make_shared<FluidRenderable>();
        fluidBuffer->AttachGeometryCacheManager(manager);
        fluidBuffer->AttachMaterial(material);

        //! Parse transform information of total animation bound
        const auto& transform = node.find_child_by_attribute("transform", "name", "toWorld");
        const CubbyFlow::Vector3F translate = Detail::ParseFromString(transform.child("translate").attribute("value").value());
        const CubbyFlow::Vector3F scale = Detail::ParseFromString(transform.child("scale").attribute("value").value());
        const CubbyFlow::Vector3F rotateAxis = Detail::ParseFromString(transform.child("rotate").attribute("axis").value());
        const float rotateDegree = transform.child("rotate").attribute("degree").as_float();

        //! Multiply Scale * Ratation * Model matrices and pass.
        fluidBuffer->SetModelMatrix(CubbyFlow::Matrix4x4F::MakeScaleMatrix(scale) *
                                    CubbyFlow::Matrix4x4F::MakeRotationMatrix(rotateAxis, CubbyFlow::DegreesToRadians(rotateDegree)) *
                                    CubbyFlow::Matrix4x4F::MakeTranslationMatrix(translate));

        //! Push parsed fluid animation to the unordered_map.
        _metadata.emplace(VoxStringID(node.attribute("name").value()), fluidBuffer);
    }

    //! Static Object Loader
    template <>
    void VoxScene::OnLoadSceneObject<StaticRenderable>(const pugi::xml_node& node)
    {
        //! Parse fluid animation(which is sequential list of the obj files) path format and count of the obj files.
        //! path format looks like this : path%06d.obj 
        const std::string filename = node.find_child_by_attribute("string", "name", "filename").attribute("value").value();

        //! Loading obj files from format and count.
        auto cache = std::make_shared<GeometryCache>(FileSystem::FindPath(filename).ToString());

        //! interleaving the geometry cache vertex data.
        cache->InterleaveData(Vox::VertexFormat::Position3Normal3);

        //! Generate the mesh from the geometry cache.
        auto mesh = std::make_shared<Mesh>();
        mesh->GenerateMeshObject(cache->GetShape(0), VertexFormat::Position3Normal3, true);

        //! Get already parsed material instance from this VoxScene instance.
        //! **The required material must precede FluidAnim.**
        const std::string materialName = node.child("material").attribute("value").value();
        const auto& material = GetSceneObject<Material>(materialName);

        //! Create fluid buffer from the parsed material, program and geometry cache.
        auto staticObject = std::make_shared<StaticRenderable>();
        staticObject->AttachMaterial(material);

        //! Parse transform information of total animation bound
        const auto& transform = node.find_child_by_attribute("transform", "name", "toWorld");
        const CubbyFlow::Vector3F translate = Detail::ParseFromString(transform.child("translate").attribute("value").value());
        const CubbyFlow::Vector3F scale = Detail::ParseFromString(transform.child("scale").attribute("value").value());
        const CubbyFlow::Vector3F rotateAxis = Detail::ParseFromString(transform.child("rotate").attribute("axis").value());
        const float rotateDegree = transform.child("rotate").attribute("degree").as_float();

        //! Multiply Scale * Ratation * Model matrices and pass.
        staticObject->SetModelMatrix(CubbyFlow::Matrix4x4F::MakeTranslationMatrix(translate) *
            CubbyFlow::Matrix4x4F::MakeRotationMatrix(rotateAxis, CubbyFlow::DegreesToRadians(rotateDegree)) *
            CubbyFlow::Matrix4x4F::MakeScaleMatrix(scale));

        //! Add mesh to the static object
        staticObject->AddGeometryMesh(mesh);

        //! Push parsed fluid animation to the unordered_map.
        _metadata.emplace(VoxStringID(node.attribute("name").value()), staticObject);
    }

    //! Emitter Loader
    template <>
    void VoxScene::OnLoadSceneObject<Emitter>(const pugi::xml_node& node)
    {
        UNUSED_VARIABLE(node);
        //! Push parsed light emitter to the unordered_map.
        // _metadata.emplace(VoxStringID(node.attribute("name").value()), std::make_shared<Texture>(GL_TEXTURE_2D, textureID));
    }

    //! Texture Loader
    template <>
    void VoxScene::OnLoadSceneObject<Texture>(const pugi::xml_node& node)
    {
        stbi_set_flip_vertically_on_load(true);

        //! Get texture image filename
        const std::string filename = FileSystem::FindPath(node.attribute("filename").value()).ToString();

        //! Get texture image type
        const std::string type = node.attribute("type").value();

        int width, height, numChannel;
        GLuint textureID;
        if (type == "hdr")
        {
            //! Get floating point data pointer from the hdr image.
            float* data = stbi_loadf(filename.c_str(), &width, &height, &numChannel, 0);
            //! Create texture with floating point format.
            textureID = Renderer::CreateTexture(width, height, PixelFmt::PF_RGB32F, data);
            //! Deallocates the image data.
            stbi_image_free(data);
        }
        else
        {
            //! Get data pointer from the hdr image.
            unsigned char* data = stbi_load(filename.c_str(), &width, &height, &numChannel, 0);
            //! Create texture with floating point format.
            textureID = Renderer::CreateTexture(width, height, PixelFmt::PF_RGB8, data);
            //! Deallocates the image data.
            stbi_image_free(data);
        }

        //! Push parsed light emitter to the unordered_map.
        _metadata.emplace(VoxStringID(node.attribute("name").value()), std::make_shared<Texture>(GL_TEXTURE_2D, textureID));
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
            else if (objectType == "anim") OnLoadSceneObject<FluidRenderable>(data);
            else if (objectType == "shape") OnLoadSceneObject<StaticRenderable>(data);
            else if (objectType == "emitter") OnLoadSceneObject<Emitter>(data);
            else if (objectType == "texture") OnLoadSceneObject<Texture>(data);
            else VoxAssert(false, CURRENT_SRC_PATH_TO_STR, "Unknown Scene Object Type [" + objectType + "]");
            std::cout << " done" << std::endl;
        };
    }
}