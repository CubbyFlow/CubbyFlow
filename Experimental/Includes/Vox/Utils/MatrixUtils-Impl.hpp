/*************************************************************************
> File Name: MatrixUtils-Impl.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Calculation utilities realted to matrix. (ex. create perspective matrix).
> Created Time: 2021/01/10
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_MATRIX_UTILS_IMPL_HPP
#define CUBBYFLOW_VOX_MATRIX_UTILS_IMPL_HPP

#include <Core/Utils/Constants.hpp>

namespace Vox {

    template <typename T>
	CubbyFlow::Matrix4x4<T> Perspective(const T aspect, const T near, const T far, const T fov)
    {
        CubbyFlow::Matrix4x4<T> projection(0.0f);
        const float sx = static_cast<T>(std::tan(CubbyFlow::DegreesToRadians(fov) / 2.0f) * near);
        const float sy = sx / aspect;
        projection(0, 0) = 2.0 * near / sx;
        projection(1, 1) = 2.0 * near / sy;
        projection(2, 2) = -(near + far) / (far - near);
        projection(2, 3) = -(2.0 * far * near) / (far - near);
        projection(3, 2) = -static_cast<T>(1.0);
        
        return projection;
    }

    template <typename T>
	CubbyFlow::Matrix4x4<T> LookAt(const CubbyFlow::Vector3<T>& origin, const CubbyFlow::Vector3<T>& dir, const CubbyFlow::Vector3<T>& up)
    {
        const CubbyFlow::Vector3<T> right = (up.Cross(dir)).Normalized();
        const CubbyFlow::Vector3<T> cameraUp = (dir.Cross(right)).Normalized();

        CubbyFlow::Matrix4x4<T> view = { {right.x,        right.y,        right.z,        0.0f},
                                         {cameraUp.x,     cameraUp.y,     cameraUp.z,     0.0f},
                                         {dir.x,          dir.y,          dir.z,          0.0f},
                                         {0.0f,           0.0f,           0.0f,           1.0f} };

        CubbyFlow::Matrix4x4<T> translation = { {1.0f, 0.0f, 0.0f, -origin.x},
                                                {0.0f, 1.0f, 0.0f, -origin.y},
                                                {0.0f, 0.0f, 1.0f, -origin.z},
                                                {0.0f, 0.0f, 0.0f,    1.0f  } };

        return view * translation;
    }

}

#endif