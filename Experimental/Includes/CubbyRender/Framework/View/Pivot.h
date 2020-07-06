/*************************************************************************
> File Name: Pivot.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The pivot class which contains properties of camera.
> Created Time: 2020/04/18
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_PIVOT_H
#define CUBBYFLOW_PIVOT_H

#include <Framework/View/Viewport.h>
#include <Framework/Utils/Prerequisites.h>
#include <Core/Vector/Vector3.h>
#include <Core/Vector/Vector4.h>
#include <Core/Matrix/Matrix4x4.h>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief The camera state class which contains properties of camera.
    //!
    struct Pivot
    {
    public:
        bool operator==(const Pivot& other) const;
        bool operator!=(const Pivot& other) const;

        Viewport viewport;

        Vector3F origin { 0.0f, 0.0f, 0.0f };
        Vector3F lookAt { 0.0f, 0.0f, 1.0f};
        Vector3F lookUp { 0.0f, 1.0f, 0.0f };

        float zFar = 100.0f;
        float zNear = 0.01f;
    protected:
    private:
    };
} 
}

#endif