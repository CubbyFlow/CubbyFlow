/*************************************************************************
> File Name: ParticleBuffer.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The buffer class which store buffer desc and own raw pointer optionally.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_PARTICLE_BUFFER_HPP
#define CUBBYFLOW_VOX_PARTICLE_BUFFER_HPP

#include <Vox/RoundRobinAsyncBuffer.hpp>
#include <memory>
#include <vector>

namespace Vox {

    /**
     * Buffer wrapper class for particles inherited by RoundRobinAsyncBuffer.
     * Each vertex array have only one vertex buffer object.
     * vertex buffer have one position attribute.
     **/
    class ParticleBuffer : public RoundRobinAsyncBuffer
    {
    public:
        //! Default Constructor.
        ParticleBuffer();
        //! Constructor with number of the buffers.
        ParticleBuffer(const size_t numBuffer);
        //! Default destructor.
        ~ParticleBuffer();
    
    protected:
        void OnAsyncBufferTransfer(const std::shared_ptr<GeometryCacheManager>& cacheManager) override;
        void OnDrawFrame(const std::shared_ptr<FrameContext>& ctx) override;
    private:
        std::vector<GLuint> _vbos;
        size_t _numVertices { 0 };
    };
};

#endif