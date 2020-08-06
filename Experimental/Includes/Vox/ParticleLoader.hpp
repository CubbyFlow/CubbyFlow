/*************************************************************************
> File Name: ParticleLoader.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Fluid data parser (pos, xyz)
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_PARTICLE_LOADER_HPP
#define CUBBYFLOW_VOX_PARTICLE_LOADER_HPP

#include <Core/Array/Array1.h>
#include <Core/Vector/Vector3.h>
#include <string>
#include <vector>

namespace Vox {
    class ParticleLoader
    {
    public:
        //! Default Constructor.
        ParticleLoader();

        //! Default Destructor.
        ~ParticleLoader();

        //! load multiple fluid files from given directory.
        void LoadParticles(const std::string& format, size_t count);

        //! return the number of particle state(number of particle files)
        size_t GetNumberOfFrame() const;

        //! return the number of bytes in the frmae with index
        size_t GetNumberOfBytes(size_t frameIndex) const;

        //! copy the particle data with frame index to destination memory
        void CopyParticleData(void* dst, size_t frameIndex) const;

        std::tuple<CubbyFlow::Vector3F, CubbyFlow::Vector3F> GetBoundingBox() const;

    protected:
        //! load geometry cache from pos file.
        void LoadPosFile(const std::string& path);
        //! load geometry cache from xyz file.
        void LoadXyzFile(const std::string& path);
    private:
        void RenewBoundingBox(float x, float y, float z);

        std::vector<CubbyFlow::Array1<float>> _frames;
        CubbyFlow::Vector3F _bbMin;
        CubbyFlow::Vector3F _bbMax;
    };
};

#endif