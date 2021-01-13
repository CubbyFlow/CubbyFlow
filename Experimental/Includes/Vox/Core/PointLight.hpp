/*************************************************************************
> File Name: PointLight.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Base emitter class
> Created Time: 2021/01/06
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_POINT_LIGHT_HPP
#define CUBBYFLOW_VOX_POINT_LIGHT_HPP

#include <Vox/Core/Emitter.hpp>
#include <Core/Vector/Vector3.hpp>

namespace Vox
{
   /**
    * Point light class
    */
   class PointLight : public Emitter
   {
    public:
       PointLight() = default;
        
      inline void SetPosition(const CubbyFlow::Vector3F& position)
      {
         _position = position;
      }
      
      inline void SetColor(const CubbyFlow::Vector3F& color)
      {
         _color = color;
      }

    protected:
      void OnUploadProgram(ShaderParameters& params) override;
    private:
      CubbyFlow::Vector3F _position;
      CubbyFlow::Vector3F _color;
   };

};  // namespace Vox

#endif