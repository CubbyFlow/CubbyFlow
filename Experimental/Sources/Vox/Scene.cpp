/*************************************************************************
> File Name: Scene.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Scene class contains whole application objects, such as Loader, Material, Mesh, etc..
> Created Time: 2020/07/27
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Scene.hpp>
#include <Vox/ParticleLoader.hpp>

namespace Vox {

    void Scene::SetLoader(const std::shared_ptr<ParticleLoader>& loader)
    {
        _loader = loader;
    }

    const std::shared_ptr<ParticleLoader>& Scene::GetLoader()
    {
        return _loader;
    }

};