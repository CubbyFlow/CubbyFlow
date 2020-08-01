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
#include <Vox/FileSystem.hpp>
#include <Vox/DebugUtils.hpp>
#include <Vox/FrameBuffer.hpp>
#include <Vox/ParticleLoader.hpp>
#include <glad/glad.h>

namespace Vox {

    Scene::Scene() 
    {

    }

    Scene::~Scene()
    {

    }

    void Scene::SetLoader(const std::shared_ptr<ParticleLoader>& loader)
    {
        _loader = loader;
    }

    const std::shared_ptr<ParticleLoader>& Scene::GetLoader()
    {
        return _loader;
    }

	void Scene::AddFrameBuffer(std::shared_ptr<FrameBuffer> fbo)
	{
		_fbos.push_back(fbo);
        _fboIterator = _fbos.begin();
	}

	void Scene::BindNextFrameBuffer(GLenum target)
	{
		VoxAssert(_fbos.size() != 0, CURRENT_SRC_PATH_TO_STR, "At least one frame buffer object must exist");

		auto& fbo = *_fboIterator;
		fbo->BindFrameBuffer(target);

		if (_fboIterator + 1 == _fbos.end())
		{
			_fboIterator = _fbos.begin();
		}
	}
};