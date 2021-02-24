/*************************************************************************
> File Name: StaticRenderable.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The static object class
> Created Time: 2021/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/StaticRenderable.hpp>
#include <Vox/Core/Material.hpp>
#include <Vox/Core/Program.hpp>
#include <Vox/Mesh/Mesh.hpp>
#include <Vox/Utils/VectorUtils.hpp>
#include <Vox/Scene/VoxScene.hpp>
#include <Vox/Scene/GeometryCache.hpp>

namespace Vox {

    StaticRenderable::StaticRenderable()
    {
        //! Do nothing
    }

    StaticRenderable::~StaticRenderable()
    {
        //! Do nothing
    }

    void StaticRenderable::DrawRenderableObject(const std::shared_ptr<FrameContext>& ctx)
    {
        //! Configure render settings before rendering.
        ConfigureRenderSettings(ctx);

        //! Render the mesh.
        for (const auto& mesh : _meshes)
        {
            mesh->DrawMesh(ctx);
        }
    }

    void StaticRenderable::ConfigureRenderSettings(const std::shared_ptr<FrameContext>& ctx)
    {
        //! Bind material to the context.
        _material->BindMaterial(ctx);

        auto& program = _material->GetProgram();
        if (program->HasUniformVariable("camera.model"))
            program->GetParameters().SetParameter("camera.model", _modelMatrix);
    }

    void StaticRenderable::LoadXMLNode(VoxScene* scene, const pugi::xml_node& node)
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
        const auto& material = scene->GetSceneObject<Material>(materialName);

        //! Create fluid buffer from the parsed material, program and geometry cache.
        AttachMaterial(material);

        //! Parse transform information of total animation bound
        const auto& transform = node.find_child_by_attribute("transform", "name", "toWorld");
        const CubbyFlow::Vector3F translate = ParseFromString<float, 3>(transform.child("translate").attribute("value").value());
        const CubbyFlow::Vector3F scale = ParseFromString<float, 3>(transform.child("scale").attribute("value").value());
        const CubbyFlow::Vector3F rotateAxis = ParseFromString<float, 3>(transform.child("rotate").attribute("axis").value());
        const float rotateDegree = transform.child("rotate").attribute("degree").as_float();

        //! Multiply Scale * Ratation * Model matrices and pass.
        SetModelMatrix(CubbyFlow::Matrix4x4F::MakeTranslationMatrix(translate) *
                       CubbyFlow::Matrix4x4F::MakeRotationMatrix(rotateAxis, CubbyFlow::DegreesToRadians(rotateDegree)) *
                       CubbyFlow::Matrix4x4F::MakeScaleMatrix(scale));

        //! Add mesh to the static object
        AddGeometryMesh(mesh);
    }

    void StaticRenderable::WriteXMLNode(pugi::xml_node& node)
    {
        UNUSED_VARIABLE(node);
    }
};