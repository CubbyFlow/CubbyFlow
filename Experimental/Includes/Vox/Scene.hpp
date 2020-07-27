/*************************************************************************
> File Name: Scene.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Scene class contains whole application objects, such as Loader, Material, Mesh, etc..
> Created Time: 2020/07/27
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_SCENE_HPP
#define CUBBYFLOW_VOX_SCENE_HPP

#include <memory>

namespace Vox {
    class ParticleLoader;

    /**
     * Represent one scene consituting rendering scenario.
     */
    class Scene 
    {
    public:
        void SetLoader(const std::shared_ptr<ParticleLoader>& loader);
        const std::shared_ptr<ParticleLoader>& GetLoader();
    protected:
    private:
        std::shared_ptr<ParticleLoader> _loader;
    };

};

#endif