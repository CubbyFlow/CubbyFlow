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
#include <Vox/FrameContext.hpp>
#include <Vox/Program.hpp>
#include <glad/glad.h>
#include <cstring>
#include <cstdlib>

#include <iostream>
namespace Vox {

    namespace Detail {
        GLuint GenerateVAO(const void* data, const size_t numBytes)
        {
            GLuint vao, vbo;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, numBytes, data, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
            glBindVertexArray(0);
            return vao;
        }
    }

    DebugDraw::DebugDraw()
    {
    }

    DebugDraw::~DebugDraw()
    {
        for (auto renderable : _renderables)
            glDeleteVertexArrays(1, &(renderable.vao));
    }

    void DebugDraw::Initialize(const std::shared_ptr<FrameContext>& ctx)
    {
        GLuint vs = Renderer::CreateShaderFromSource(kDebugDrawShaders[0], GL_VERTEX_SHADER);
        GLuint fs = Renderer::CreateShaderFromSource(kDebugDrawShaders[1], GL_FRAGMENT_SHADER);
        ctx->CreateProgram("P_DebugDraw", Renderer::CreateProgram(vs, 0, fs));
        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    void DebugDraw::AddFloorGrid(const CubbyFlow::Point2I numCell, float cellSize, const CubbyFlow::Vector3F color)
    {
        std::vector<CubbyFlow::Vector3F> positions((numCell.x + numCell.y + 2) * 2);

        //! assume cell outter lines' width is almost equal to 0.
        const float horizontalLength = numCell.x * cellSize;
        const float verticalLength = numCell.y * cellSize;
        const CubbyFlow::Vector2F minCorner = { -numCell.x * cellSize * 0.5f, -numCell.y * cellSize * 0.5f };

        //! Add vertical lines.
        for (int x = 0; x < numCell.x + 1; ++x)
        {
            positions[x * 2] = {minCorner.x + x * cellSize, 0.0f, minCorner.y};
            positions[x * 2 + 1] = {minCorner.x + x * cellSize, 0.0f, minCorner.y + verticalLength};
        }

        //! Add horizontal lines.
        for (int y = 0, baseIdx = (numCell.x + 1) * 2; y < numCell.y + 1; ++y)
        {
            positions[baseIdx + y * 2] = {minCorner.x, 0.0f, minCorner.y + y * cellSize};
            positions[baseIdx + y * 2 + 1] = {minCorner.x + horizontalLength, 0.0f, minCorner.y + y * cellSize};
        }

        const size_t numBytes = positions.size() * sizeof(CubbyFlow::Vector3F);
        const size_t numVertices = numBytes / (sizeof(float) * 3);
        _renderables.push_back({ color, numVertices, Detail::GenerateVAO(static_cast<void*>(positions.data()), numBytes) });
    }

    void DebugDraw::AddCartesianCoordinateAxis(const float axisXLength, const float axisYLength, const float axisZLength)
    {
        const float axisX[] = {
            0.0f, 0.0f, 0.0f, axisXLength, 0.0f, 0.0f
        };
        const float axisY[] = {
            0.0f, 0.0f, 0.0f, 0.0f, axisYLength, 0.0f
        };
        const float axisZ[] = {
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, axisZLength
        };

        _renderables.push_back({ CubbyFlow::Vector3F(1.0f, 0.0f, 0.0f), size_t(2), Detail::GenerateVAO(static_cast<const void*>(axisX), sizeof(axisX)) });
        _renderables.push_back({ CubbyFlow::Vector3F(0.0f, 1.0f, 0.0f), size_t(2), Detail::GenerateVAO(static_cast<const void*>(axisY), sizeof(axisY)) });
        _renderables.push_back({ CubbyFlow::Vector3F(0.0f, 0.0f, 1.0f), size_t(2), Detail::GenerateVAO(static_cast<const void*>(axisZ), sizeof(axisZ)) });
    }

    void DebugDraw::AddBoundingBox(const CubbyFlow::BoundingBox3F& boundingBox, const CubbyFlow::Vector3F color)
    {
        const auto& bbMin = boundingBox.lowerCorner, bbMax = boundingBox.upperCorner;
        const float positions[] = {
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

         const size_t numBytes = sizeof(positions);
         const size_t numVertices = numBytes / (sizeof(float) * 3);
        _renderables.push_back({ color, numVertices, Detail::GenerateVAO(static_cast<const void*>(positions), numBytes) });
    }

    void DebugDraw::ClearVAO()
    {
        for (auto renderable : _renderables)
            glDeleteVertexArrays(1, &(renderable.vao));
        _renderables.clear();
    }

    void DebugDraw::DrawFrame(const std::shared_ptr<FrameContext>& ctx) const
    {
        auto program = ctx->GetProgram("DebugDraw");
        program->BindProgram(ctx->GetContextScene());
        for (const auto& renderable : _renderables)
        {
            auto& params = program->GetParameters();
            params.SetParameter("Color", renderable.color);

            glBindVertexArray(renderable.vao);
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(renderable.numVertices));
        }
        glBindVertexArray(0);
    }

};