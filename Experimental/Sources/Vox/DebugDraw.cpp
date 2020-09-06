/*************************************************************************
> File Name: DebugDraw.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The debug drawing class for visualizing horizontal grid, x,y,z axis, bounding boxes.
> Created Time: 2020/08/08
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/DebugDraw.hpp>
#include <Vox/ShaderPreset.hpp>
#include <Vox/Renderer.hpp>
#include <Vox/Mesh.hpp>
#include <Vox/RenderableObject.hpp>
#include <Vox/FrameContext.hpp>
#include <Vox/Material.hpp>
#include <Vox/Program.hpp>
#include <glad/glad.h>
#include <cstring>
#include <numeric>
#include <cstdlib>

namespace Vox {

    DebugDraw::DebugDraw()
    {
        //! Do nothing.
    }

    DebugDraw::~DebugDraw()
    {
        //! Do nothing.
    }

    void DebugDraw::Initialize(const std::shared_ptr<FrameContext>& ctx)
    {
        GLuint vs = Renderer::CreateShaderFromSource(kDebugDrawShaders[0], GL_VERTEX_SHADER);
        GLuint fs = Renderer::CreateShaderFromSource(kDebugDrawShaders[1], GL_FRAGMENT_SHADER);
        _debugShader = ctx->CreateProgram("P_DebugDraw", Renderer::CreateProgram(vs, 0, fs));
        glDeleteShader(vs);
        glDeleteShader(fs);

        _renderable.reset(new RenderableObject());

        auto material = std::make_shared<Material>();
        material->AttachProgramShader(_debugShader);
        _renderable->AttachMaterial(material);
    }

    void DebugDraw::AddFloorGrid(const CubbyFlow::Point2I numCell, float cellSize, const CubbyFlow::Vector3F color)
    {
        MeshShape shape;

        const size_t nrPoints = (numCell.x + numCell.y + 2) * 2;
        shape.positions.Resize(nrPoints);
        shape.indices.Resize(nrPoints);

        //! Fill the indices from 0 to nrPoints - 1.
        std::iota(shape.indices.begin(), shape.indices.end(), 0);

        //! assume cell outter lines' width is almost equal to 0.
        const float horizontalLength = numCell.x * cellSize;
        const float verticalLength = numCell.y * cellSize;
        const CubbyFlow::Vector2F minCorner = { -numCell.x * cellSize * 0.5f, -numCell.y * cellSize * 0.5f };

        //! Add vertical lines.
        for (int x = 0; x < numCell.x + 1; ++x)
        {
            shape.positions[x * 2] = {minCorner.x + x * cellSize, 0.0f, minCorner.y};
            shape.positions[x * 2 + 1] = {minCorner.x + x * cellSize, 0.0f, minCorner.y + verticalLength};
        }

        //! Add horizontal lines.
        for (int y = 0, baseIdx = (numCell.x + 1) * 2; y < numCell.y + 1; ++y)
        {
            shape.positions[baseIdx + y * 2] = {minCorner.x, 0.0f, minCorner.y + y * cellSize};
            shape.positions[baseIdx + y * 2 + 1] = {minCorner.x + horizontalLength, 0.0f, minCorner.y + y * cellSize};
        }

        _renderable->AddGeometryMesh(std::make_shared<Mesh>(shape, VertexFormat::Position3, false));
    }

    void DebugDraw::AddCartesianCoordinateAxis(const float axisXLength, const float axisYLength, const float axisZLength)
    {
        MeshShape shape;
        shape.interleaved = {
            0.0f, 0.0f, 0.0f, axisXLength, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, axisYLength, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, axisZLength
        };

        shape.indices.Resize(shape.interleaved.size());
        std::iota(shape.indices.begin(), shape.indices.end(), 0);

        _renderable->AddGeometryMesh(std::make_shared<Mesh>(shape, VertexFormat::Position3, true));
    }

    void DebugDraw::AddBoundingBox(const CubbyFlow::BoundingBox3F& boundingBox, const CubbyFlow::Vector3F color)
    {
        MeshShape shape;
        const auto& bbMin = boundingBox.lowerCorner, bbMax = boundingBox.upperCorner;
        shape.interleaved = {
            bbMin.x, bbMin.y, bbMin.z, bbMax.x, bbMin.y, bbMin.z,
            bbMin.x, bbMin.y, bbMax.z, bbMax.x, bbMin.y, bbMax.z,
            bbMin.x, bbMax.y, bbMin.z, bbMax.x, bbMax.y, bbMin.z,
            bbMin.x, bbMax.y, bbMax.z, bbMax.x, bbMax.y, bbMax.z,
            bbMin.x, bbMin.y, bbMin.z, bbMin.x, bbMin.y, bbMax.z,
            bbMax.x, bbMin.y, bbMin.z, bbMax.x, bbMin.y, bbMax.z,
            bbMin.x, bbMax.y, bbMin.z, bbMin.x, bbMax.y, bbMax.z,
            bbMax.x, bbMax.y, bbMin.z, bbMax.x, bbMax.y, bbMax.z,
            bbMin.x, bbMin.y, bbMin.z, bbMin.x, bbMax.y, bbMin.z,
            bbMax.x, bbMin.y, bbMin.z, bbMax.x, bbMax.y, bbMin.z,
            bbMin.x, bbMin.y, bbMax.z, bbMin.x, bbMax.y, bbMax.z,
            bbMax.x, bbMin.y, bbMax.z, bbMax.x, bbMax.y, bbMax.z,
        };

         shape.indices.Resize(shape.interleaved.size());
        std::iota(shape.indices.begin(), shape.indices.end(), 0);

        _renderable->AddGeometryMesh(std::make_shared<Mesh>(shape, VertexFormat::Position3, true));
    }

    void DebugDraw::ClearVAO()
    {
        _renderable->ClearGeometryMesh();
    }

    void DebugDraw::DrawFrame(const std::shared_ptr<FrameContext>& ctx) const
    {
        _renderable->DrawRenderableObject(ctx);
    }

};