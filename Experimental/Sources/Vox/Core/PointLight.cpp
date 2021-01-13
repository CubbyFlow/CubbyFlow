/*************************************************************************
> File Name: PointLight.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Base emitter class
> Created Time: 2021/01/06
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/PointLight.hpp>

namespace Vox
{
   void PointLight::OnUploadProgram(ShaderParameters& params)
   {
      params.SetParameter("light.position", _position);
      params.SetParameter("light.color", _color);
   }

};  // namespace Vox