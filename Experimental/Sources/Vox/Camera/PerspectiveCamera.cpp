/*************************************************************************
> File Name: PerspectiveCamera.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: amera class with perspective projection
> Created Time: 2020/08/02
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Camera/PerspectiveCamera.hpp>
#include <Vox/Core/FrameContext.hpp>
#include <Vox/Core/Program.hpp>
#include <Vox/Scene/VoxScene.hpp>
#include <Vox/Utils/VectorUtils.hpp>
#include <Vox/Utils/MatrixUtils.hpp>
#include <Core/Math/Quaternion.hpp>

using namespace CubbyFlow;
namespace Vox {
    void PerspectiveCamera::SetViewFrustum(const float fov, const float far)
    {
        _fov = fov;
        _far = far;
    }

    void PerspectiveCamera::SetAspectRatio(const float aspectRatio)
    {
        _aspectRatio = aspectRatio;
    }

    void PerspectiveCamera::UpdateMatrix()
    {
        _view = LookAt(_origin, _dir, _up);
        _projection = Perspective(_aspectRatio, _near, _far, _fov);
    }

    void PerspectiveCamera::LoadXMLNode(VoxScene* scene, const pugi::xml_node& node)
    {
        UNUSED_VARIABLE(scene);
        //! Get transform information of the camera.
        const auto& lookAt = node.child("transform").child("lookat");
        const CubbyFlow::Vector3F origin = ParseFromString<float, 3>(lookAt.attribute("origin").value());
        const CubbyFlow::Vector3F target = ParseFromString<float, 3>(lookAt.attribute("target").value());
        const CubbyFlow::Vector3F up = ParseFromString<float, 3>(lookAt.attribute("up").value());

        //! Create camera with given camera type.
        const float fov = node.find_child_by_attribute("float", "name", "fov").attribute("value").as_float();
        const float far = node.find_child_by_attribute("float", "name", "focusDistance").attribute("value").as_float();
        SetViewFrustum(fov, far);

        //! Set transform to the camera.
        SetViewTransform(origin, target, up);
    }

    void PerspectiveCamera::WriteXMLNode(pugi::xml_node& node)
    {
        UNUSED_VARIABLE(node);
    }
};
