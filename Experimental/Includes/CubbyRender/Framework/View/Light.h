/*************************************************************************
> File Name: Light.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: The light class which have pos, dir, light color and etc..
> Created Time: 2020/06/27
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_LIGHT_H
#define CUBBYFLOW_LIGHT_H

#include <Framework/Utils/Prerequisites.h>
#include <Framework/View/OrthogonalCamera.h>
#include <Core/Vector/Vector3.h>
#include <memory>

namespace CubbyFlow {
namespace CubbyRender {
    
    //!
    //! \brief The camera class which view simulator scene
    //!
    class Light : public OrthogonalCamera
    {
    public:
        //! Default constructor.
        Light();

        //! constructor with pivot..
        Light(const Pivot& pivot);

        //! Default destructor.
        virtual ~Light();

        inline void setColor(const Vector3F& color);

        inline Vector3F getColor() const;

    protected:
    private:
        Vector3F _color;
    };

    using LightPtr = std::shared_ptr<Light>;
} 
}

#endif