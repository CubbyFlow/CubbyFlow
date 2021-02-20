/*************************************************************************
> File Name: FluidRenderable.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The buffer class which store buffer desc and own raw pointer optionally.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/FluidRenderable.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Mesh/Mesh.hpp>
#include <Vox/Scene/GeometryCacheManager.hpp>
#include <Vox/Scene/GeometryCache.hpp>
#include <Vox/Core/Material.hpp>
#include <Vox/Core/Vertex.hpp>
#include <glad/glad.h>

using namespace CubbyFlow;

namespace Vox {

    FluidRenderable::FluidRenderable(const std::shared_ptr<GeometryCacheManager>& manager, const size_t numBuffer)
    {
        SetGeometryCacheManager(manager);
        Resize(numBuffer);
    }   

    FluidRenderable::~FluidRenderable()
    {
        CleanUp();
    }

    void FluidRenderable::Resize(const size_t numBuffer)
    {
        _meshes.Resize(numBuffer, std::make_shared<Mesh>());
        if (_numBuffer <= numBuffer) //! In case new number of the buffer is larger than original.
        {
            for (size_t i = _numBuffer; i < numBuffer; ++i)
            {
                _meshes[i]->SetBufferUsage(GL_STREAM_DRAW);
                //! generate mesh with given vertex format and pre-allocates 2096kB for each buffer(vertex, index buffer)
                _meshes[i]->GenerateEmptyMesh(_cacheManager->GetVertexFormat(), kMaxBufferSize, kMaxBufferSize);
            }
            glBindVertexArray(0);
        }
        _fences.Resize(numBuffer);
        _numBuffer = numBuffer;
    }

    bool FluidRenderable::CheckFence(GLuint64 timeout) 
    {
        const size_t bufferNum = _frameIndex % _numBuffer;
        const GLsync& fence = _fences[bufferNum];

        if (fence)
        {
            GLenum result = glClientWaitSync(fence, 0, timeout); //! CPU side synchronization
            return result == GL_ALREADY_SIGNALED || result == GL_CONDITION_SATISFIED;
        }
        return true;
    }

    void FluidRenderable::AsyncBufferTransfer()
    {
        const size_t bufferNum = _frameIndex % _numBuffer;

        //! Take Modulo with geometry cache size for repeating transfer.
        const size_t index = _frameIndex % _cacheManager->GetNumberOfCache();
        const auto& shape = _cacheManager->GetGeometryCache(index)->GetShape(0);

        //! Transfer the vertices data
        _meshes[bufferNum]->AsyncTransfer(shape, shape.interleaved.size() * sizeof(float));

        //! Transfer the indices data
        _meshes[bufferNum]->AsyncTransfer(shape, shape.indices.size() * sizeof(unsigned int), true);
    }

    void FluidRenderable::ConfigureRenderSettings(const std::shared_ptr<FrameContext>& ctx)
    {
        //! Bind material to the context.
        _material->BindMaterial(ctx);

        //! Send model matrix to the program
        _material->GetProgram()->GetParameters().SetParameter("camera.model", _modelMatrix);
    }

    void FluidRenderable::DrawRenderableObject(const std::shared_ptr<FrameContext>& ctx)
    {
        //! Configure render settings before rendering.
        ConfigureRenderSettings(ctx);

        //! Render the mesh.
        const size_t bufferNum = _frameIndex % _numBuffer;
        _meshes[bufferNum]->DrawMesh(ctx);
        
        //! Make fence to draw call.
        _fences[bufferNum] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0); 
    }

    void FluidRenderable::AdvanceFrame()
    {
        ++_frameIndex; //! Advance frame index;
    }

    void FluidRenderable::SetGeometryCacheManager(const std::shared_ptr<GeometryCacheManager>& manager)
    {
        if (_cacheManager && _cacheManager->GetVertexFormat() != manager->GetVertexFormat())
        {
            CleanUp();
            Resize();
        }

        _cacheManager = manager;
    }

    void FluidRenderable::CleanUp()
    {
        for (const auto& fence : _fences)
        {
            //! Wait whole fences finished
            glClientWaitSync(fence, 0, 1000000);
            glDeleteSync(fence);
        }
        _fences.Clear();
        _meshes.Clear();
    }
};