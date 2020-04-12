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

#include <Framework/Utils/Prerequisites.h>
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

        //! Add texture to this material instance with its slot id.
        void addTexture(unsigned int slotID, TexturePtr texture);

        //! Set shader instance to this material.
        void setShader(ShaderPtr shader);

        //! Return shader instance of this material.
        ShaderPtr getShader();

        //! bind this material to renderer.
        void bind(RendererPtr renderer);

        //! unbind this material from renderer.
        void unbind(RendererPtr renderer);

        //! release shader and textures.
        void release();
        
     protected:
     private:
        ShaderPtr _shader;
        std::vector<std::pair<unsigned int, TexturePtr>> _textures;
    };

    using MaterialPtr = std::shared_ptr<Material>;

} 
} 

#endif 