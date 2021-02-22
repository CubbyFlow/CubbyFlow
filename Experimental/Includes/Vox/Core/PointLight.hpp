/*************************************************************************
> File Name: PointLight.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: point light emitter class
> Created Time: 2021/01/06
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_POINT_LIGHT_HPP
#define CUBBYFLOW_VOX_POINT_LIGHT_HPP

#include <Vox/Scene/VoxSceneObject.hpp>
#include <Core/Vector/Vector3.hpp>
#include <Core/Array/Array1.hpp>

namespace Vox
{
    class Program;

   /**
    * Point light class
    */
   class PointLight : public VoxSceneObject
   {
    public:
       PointLight() = default;
        
       //! Add point light to list
       void AddLight(const CubbyFlow::Vector3F& position, const CubbyFlow::Vector3F& color);

       //! Upload all added light imformations to given program shader.
       void UploadToProgram(const std::shared_ptr<Program>& program);
    private:
      CubbyFlow::Array1<CubbyFlow::Vector3F> _positions;
      CubbyFlow::Array1<CubbyFlow::Vector3F> _colors;
      int _numLights { 0 };
   };

};  // namespace Vox

#endif