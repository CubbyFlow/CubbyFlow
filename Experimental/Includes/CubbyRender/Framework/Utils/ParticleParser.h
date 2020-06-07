/*************************************************************************
> File Name: ParticleParser.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Fluid data parser (pos, xyz)
> Created Time: 2020/06/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_PARTICLE_PARSER_H
#define CUBBYFLOW_PARTICLE_PARSER_H

#include <Framework/Utils/Prerequisites.h>
#include <Core/Array/Array1.h>
#include <Core/Array/ArrayAccessor1.h>
#include <Core/Vector/Vector3.h>
#include <string>
#include <vector>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    class ParticleParser final
    {
    public:
        //! Default Constructor.
        ParticleParser();

        //! Default Destructor.
        ~ParticleParser();

        //! load multiple fluid files from given directory.
        bool loadParticleFiles(const std::wstring& dir, const std::wstring& format);

        //! return const reference of particle vector of one state.
        ArrayAccessor1<Vector3F> getParticles(size_t index);

        //! return the number of particle state(number of particle files)
        size_t getNumberOfParticleState() const;
    protected:
        //! load particles from pos file.
        void loadPosFile(const std::wstring& path);
        //! load particles from xyz file.
        void loadXyzFile(const std::wstring& path);
    private:
        std::vector<Array1<Vector3F>> _particleStates;
    };

    using ParticleParserPtr = std::shared_ptr<ParticleParser>;
};
};


#endif