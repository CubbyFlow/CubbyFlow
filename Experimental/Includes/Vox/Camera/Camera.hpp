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
#include <Vox/Scene/VoxSceneObject.hpp>

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

    //! Set view matrix related elements such as origin, directoion and up vectors.
    void SetViewTransform(const CubbyFlow::Vector3F& origin,
                          const CubbyFlow::Vector3F& point,
                          const CubbyFlow::Vector3F& up);

    //! Set camera origin.
    inline void SetOrigin(const CubbyFlow::Vector3F& origin)
    {
        _origin = origin;
    }

    //! Set camera direction.
    inline void SetDirection(const CubbyFlow::Vector3F& direction)
    {
        _dir = direction.Normalized();
    }

    //! Set camera up vector.
    inline void SetUp(const CubbyFlow::Vector3F& up)
    {
        _up = up;
    }

    //! Update viewProjection with new view matrix and new projection matrix.
    virtual void UpdateMatrix() {};

    //! Returns View Matrix.
    CubbyFlow::Matrix4x4F Camera::GetViewMatrix() const;

    //! Returns Projection Matrix.
    CubbyFlow::Matrix4x4F Camera::GetProjectionMatrix() const;

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

    //! Returns the camera up vector
    inline CubbyFlow::Vector3F GetCameraUp() const
    {
        return _up;
    }

    //! Upload uniform variables to given program.
    void UploadToProgram(const std::shared_ptr<Program>& program);

    //! Load scene object attributes from the xml node
    virtual void LoadXMLNode(VoxScene* scene, const pugi::xml_node& node) override = 0;
    //! Write this scene object attributes to the given documents.
    virtual void WriteXMLNode(pugi::xml_node& node) override = 0;
 protected:
    CubbyFlow::Matrix4x4F _view = CubbyFlow::Matrix4x4F::MakeIdentity();
    CubbyFlow::Matrix4x4F _projection = CubbyFlow::Matrix4x4F::MakeIdentity();
    CubbyFlow::Vector3F _origin { 0.0f, 0.0f, 0.0f };
    CubbyFlow::Vector3F _dir { 0.0f, 0.0f, -1.0f };  //! Directon vector (MUST BE NORMALIZED)
    CubbyFlow::Vector3F _up { 0.0f, 1.0f, 0.0f };
 private:
};

};  // namespace Vox

#endif