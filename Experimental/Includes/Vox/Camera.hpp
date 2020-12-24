/*************************************************************************
> File Name: Camera.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: base camera class
> Created Time: 2020/12/22
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_CAMERA_HPP
#define CUBBYFLOW_VOX_CAMERA_HPP

#include <Core/Matrix/Matrix4x4.hpp>
#include <Core/Vector/Vector3.hpp>
#include <Vox/VoxSceneObject.hpp>

namespace Vox
{
class FrameContext;
class Program;
class Scene;

/**
 * Base camera class
 */
class Camera : public VoxSceneObject
{
 public:
    Camera() = default;

    //! Set view matrix related elements such as origin, directoion
    void SetViewTransform(const CubbyFlow::Vector3F& origin,
                          const CubbyFlow::Vector3F& point);

    //! Update viewProjection with new view matrix and new projection matrix.
    virtual void UpdateMatrix() {};

    //! Rotate camera with orbit
    void OrbitRotation(const CubbyFlow::Vector3F& focusPoint, float yaw,
                       float pitch, float distance);

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
    CubbyFlow::Matrix4x4F _viewProjection = CubbyFlow::Matrix4x4F::MakeIdentity();
    CubbyFlow::Vector3F _origin { 0.0f, 0.0f, 0.0f };
    CubbyFlow::Vector3F _dir { 0.0f, 0.0f, -1.0f };  //! Directon vector (MUST BE NORMALIZED)
 private:
};

};  // namespace Vox

#endif