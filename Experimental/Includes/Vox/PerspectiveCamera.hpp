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

#include <Vox/VoxSceneObject.hpp>
#include <Core/Matrix/Matrix4x4.h>
#include <Core/Vector/Vector3.h>

namespace Vox {

    class FrameContext;
    class Program;
    class Scene;

    /**
     * Camera class with perspective projection
     */
    class PerspectiveCamera : public VoxSceneObject
    {
    public:
        //! Set view matrix related elements such as origin, directoion
        void SetViewTransform(const CubbyFlow::Vector3F& origin, const CubbyFlow::Vector3F& point);

        //! Set projection matrix related elements such as field of view
        void SetViewFrustum(const float fov, const float near, const float far);

        //! Set Window screen aspect ratio
        void SetAspectRatio(const float aspectRatio);

        //! Update viewProjection with new view matrix and new projection matrix.
        void UpdateMatrix();

        //! Rotate camera with orbit
        void OrbitRotation(const CubbyFlow::Vector3F& focusPoint, float yaw, float pitch, float distance);

        //! Send modified view projection matrix 
        void SendViewProjection(const std::shared_ptr<Program>& program);

        //! Returns View Projection Matrix.
        CubbyFlow::Matrix4x4F GetViewProjectionMatrix() const;

        //! Returns the camera origin position.
        inline CubbyFlow::Vector3F GetCameraOrigin() const
        {
            return _origin;
        }

        //! Returns the camera normalized. direction vector.
        inline CubbyFlow::Vector3F GetCameraDirection() const
        {
            return _dir;
        }

    protected:
    private:
        CubbyFlow::Matrix4x4F _viewProjection = CubbyFlow::Matrix4x4F::MakeIdentity();
        CubbyFlow::Vector3F _origin { 0.0f, 0.0f, 0.0f };
        CubbyFlow::Vector3F _dir { 0.0f, 0.0f, -1.0f } ; //! Directon vector (MUST BE NORMALIZED)
        float _fov { 45.0f };
        float _aspectRatio { 1.0f };
        float _near { 0.01f };
        float _far { 10000.0f };
    };

};

#endif