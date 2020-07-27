/*************************************************************************
> File Name: SimulationBuffer.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The buffer class which store buffer desc and own raw pointer optionally.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_SIMULATION_BUFFER_HPP
#define CUBBYFLOW_VOX_SIMULATION_BUFFER_HPP

#include <Vox/GLTypes.hpp>
#include <memory>
#include <vector>

typedef struct __GLsync *GLsync;

namespace Vox {
    class Scene;
    /**
     * Buffer for simulating fluid particles which need huge effort for optimizing data transfer performance.
     * Implemented with multiple buffer technique (round-robin)
     **/
    class SimulationBuffer
    {
    public:
        //! Default Constructor.
        SimulationBuffer();
        //! Constructor with number of the buffers.
        SimulationBuffer(const size_t numBuffer);
        //! Default destructor.
        ~SimulationBuffer();
    
        //! Draw one frame of the particles data.
        void DrawFrame(const std::shared_ptr<Scene>& scn);
        //! advance frame index by one.
        void AdvanceFrame();

        static const size_t kMaxBufferSize = 0x400000; //! == 4194304
        static const size_t kDefaultNumBuffer = 3;
    protected:
    private:
        std::vector<GLuint> _buffers;
        std::vector<GLsync> _fences; 
        size_t _frameIndex { 0 } ;
    };
};

#endif