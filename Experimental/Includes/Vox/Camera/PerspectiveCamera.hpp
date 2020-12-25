/*************************************************************************
> File Name: PerspectiveCamera.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: camera class with perspective projection
> Created Time: 2020/08/02
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_PERSPECTIVE_CAMERA_HPP
#define CUBBYFLOW_VOX_PERSPECTIVE_CAMERA_HPP

#include <Vox/Camera/Camera.hpp>
#include <Vox/Scene/VoxSceneObject.hpp>
#include <Core/Matrix/Matrix4x4.hpp>
#include <Core/Vector/Vector3.hpp>

namespace Vox {

    class FrameContext;
    class Program;
    class Scene;

    /**
     * Camera class with perspective projection
     */
    class PerspectiveCamera : public Camera
    {
    public://! Set projection matrix related elements such as field of view
        void SetViewFrustum(const float fov, const float far);

        //! Set Window screen aspect ratio
        void SetAspectRatio(const float aspectRatio);
        
        //! Update viewProjection with new view matrix and new projection matrix.
        void UpdateMatrix();
    protected:
    private:
        float _fov { 45.0f };
        float _aspectRatio { 1.0f };
        float _near { 0.01f };
        float _far { 10000.0f };
    };

    using Light = PerspectiveCamera;
};

#endif