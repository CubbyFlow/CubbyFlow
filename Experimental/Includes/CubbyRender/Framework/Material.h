/*************************************************************************
> File Name: Material.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: the abstract base class for Material/
> Created Time: 2020/02/24
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifndef CUBBYFLOW_MATERIAL_H
#define CUBBYFLOW_MATERIAL_H

#include <Framework/Prerequisites.h>
#include <memory>
#include <vector>

namespace CubbyFlow {
namespace CubbyRender {

    class Material
    {
    public:
        //! Default constructor.
        Material();

        //! Constructor with shader
        Material(ShaderPtr shader);

        //! Destructor.
        virtual ~Material();

        ShaderPtr getShader();

        void destroy();

     protected:

     private:
        ShaderPtr _shader;
    };

    using MaterialPtr = std::shared_ptr<Material>;

} 
} 

#endif 