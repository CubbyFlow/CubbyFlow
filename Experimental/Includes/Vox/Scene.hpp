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

#include <Vox/GLTypes.hpp>
#include <memory>
#include <vector>

namespace Vox {
    class ParticleLoader;
    class FrameBuffer;

    /**
     * Represent one scene consituting rendering scenario.
     */
    class Scene 
    {
    public:
        Scene();
        ~Scene();
        void SetLoader(const std::shared_ptr<ParticleLoader>& loader);
        const std::shared_ptr<ParticleLoader>& GetLoader();

        //! Push Frame Buffer to the vector
        //! Binding will be occurred sequentially.
        void AddFrameBuffer(std::shared_ptr<FrameBuffer> fbo);
        //! Bind framebuffers sequentially.
        void BindNextFrameBuffer(GLenum target);
    protected:
    private:
        std::shared_ptr<ParticleLoader> _loader;
        std::vector<std::shared_ptr<FrameBuffer>> _fbos;
        std::vector<std::shared_ptr<FrameBuffer>>::iterator _fboIterator;
    };

};

#endif