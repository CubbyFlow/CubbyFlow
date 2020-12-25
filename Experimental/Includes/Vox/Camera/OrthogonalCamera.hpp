/*************************************************************************
> File Name: OrthogonalCamera.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: camera class with orthogonal projection
> Created Time: 2020/12/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_ORTHOGONAL_CAMERA_HPP
#define CUBBYFLOW_VOX_ORTHOGONAL_CAMERA_HPP

#include <Vox/Camera/Camera.hpp>
#include <Vox/Scene/VoxSceneObject.hpp>
#include <Core/Matrix/Matrix4x4.hpp>
#include <Core/Vector/Vector3.hpp>

namespace Vox {

    class FrameContext;
    class Program;
    class Scene;

    /**
     * Camera class with orthogonal projection
     */
    class OrthogonalCamera : public Camera
    {
    public://! Set projection matrix related elements
        void SetViewFrustum(const float left, const float right, const float top, const float bottom);

    protected:
    private:
    };
};

#endif