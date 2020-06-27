/*************************************************************************
> File Name: Camera.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The camera class which view simulator scene
> Created Time: 2020/04/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#include <Framework/View/Camera.h>
#include <Framework/Utils/Common.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    Camera::Camera()
    {
        //! Do nothing
    }

    Camera::Camera(const Pivot& pivot)
        : _pivot(pivot)
    {
        //! Do nothing
    }

    Camera::~Camera()
    {
        //! Do nothing
    }

    Matrix4x4F Camera::getViewMatrix() const
    {
        //! http://www.songho.ca/opengl/gl_transform.html#modelview
        const auto& lookAt = _pivot.lookAt;
        const auto& lookUp = _pivot.lookUp;
        const auto& origin = _pivot.origin;

        Vector3F at = (-lookAt).Normalized();
        Vector3F up = lookUp;
        Vector3F right = up.Cross(at).Normalized();
        up = at.Cross(right);

        Matrix4x4F view = {{right.x,           up.x,           at.x,               0.0f},
                           {right.y,           up.y,           at.y,               0.0f},
                           {right.z,           up.z,           at.z,               0.0f},
                           {right.Dot(origin), up.Dot(origin), (-at).Dot(origin),  1.0f}};

        return view;
    }

    Matrix4x4F Camera::getProjectionMatrix() const
    {
        Matrix4x4F projection(0.0f);
        if (_method == ProjectionMethod::PERSPECTIVE)
        {
            //! http://www.songho.ca/opengl/gl_projectionmatrix.html
            //! https://github.com/g-truc/glm/blob/master/glm/ext/matrix_clip_space.inl
            const float aspect = static_cast<float>(_viewport.getWidth()) / _viewport.getHeight();
            const float fovTan = 1.0f / std::tan((PI_FLOAT * _fovy / 180.0f) / 2.0f);

		    projection(0, 0) = fovTan / aspect;
		    projection(1, 1) = fovTan;
		    projection(2, 2) = -(_zFar - _zNear) / (_zFar + _zNear);
		    projection(2, 3) = -1.0f;
		    projection(3, 2) = -2.0f * _zFar * _zNear / (_zFar - _zNear);
        }
        else if (_method == ProjectionMethod::ORTHOGONAL)
        {
            //! Do nothing.
        }
        else
        {
            CUBBYFLOW_ERROR << "Unknown Camera Projection Method";
            std::abort();
        }
        return projection;
    }
    //! several setter and getter below.
    void Camera::setProjectionMethod(ProjectionMethod method)
    {
        _method = method;
    }
    void Camera::setViewport(Viewport viewport)
    {
        _viewport = viewport;
    }
    void Camera::setNearFar(float near, float far)
    {
        _zNear = near;
        _zFar = far;
    }
    void Camera::setFovy(float fovy)
    {
        _fovy = fovy;
    }
    ProjectionMethod Camera::getProjectionMethod() const
    {
        return _method;
    }
    Pivot& Camera::getPivot()
    {
        return _pivot;
    }
    Viewport& Camera::getViewport()
    {
        return _viewport;
    }
    const Pivot& Camera::getPivot() const
    {
        return _pivot;
    }
    const Viewport& Camera::getViewport() const
    {
        return _viewport;
    }
    float Camera::getNear() const
    {
        return _zNear;
    }
    float Camera::getFar() const
    {
        return _zFar;
    }
    float Camera::getFovy() const
    {
        return _fovy;
    }
} 
}