/*************************************************************************
> File Name: PerspectiveCamera.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: amera class with perspective projection
> Created Time: 2020/08/02
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_PERSPECTIVE_CAMERA_HPP
#define CUBBYFLOW_VOX_PERSPECTIVE_CAMERA_HPP

#include <Core/Matrix/Matrix4x4.h>
#include <Core/Vector/Vector3.h>

namespace Vox {

    class FrameContext;
    
    /**
     * Camera class with perspective projection
     */
    class PerspectiveCamera 
    {
    public:
        //! Set view matrix related elements such as origin, directoion
        void SetViewElements(const CubbyFlow::Vector3F& origin, const CubbyFlow::Vector3F& point);

        //! Set projection matrix related elements such as field of view, screen aspect ratio.
        void SetPerspectiveElements(const float fov, const float aspectRatio, const float near, const float far);

        //! Update viewProjection with new view matrix and new projection matrix.
        void UpdateMatrix();

        //! Returns View Projection Matrix.
        CubbyFlow::Matrix4x4F GetViewProjectionMatrix() const;

    protected:
    private:
        CubbyFlow::Matrix4x4F _viewProjection;
        CubbyFlow::Vector3F _origin { 0.0f, 0.0f, 0.0f };
        CubbyFlow::Vector3F _dir { 0.0f, 0.0f, -1.0f } ; //! Directon vector (MUST BE NORMALIZED)
        float _fov { 45.0f };
        float _aspectRatio { 1.0f };
        float _near { 0.01f };
        float _far { 10000.0f };
    };

};

#endif