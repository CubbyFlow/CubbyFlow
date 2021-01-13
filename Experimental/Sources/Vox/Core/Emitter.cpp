/*************************************************************************
> File Name: Emitter.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Base emitter class
> Created Time: 2021/01/06
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Core/Emitter.hpp>
#include <Core/Utils/Macros.hpp>

namespace Vox
{
   void Emitter::UploadToProgram(const std::shared_ptr<Program>& program)
   {
      auto& params = program->GetParameters();

      OnUploadProgram(params);
   }

   void Emitter::OnUploadProgram(ShaderParameters& params)
   {
      UNUSED_VARIABLE(params);
   }

};  // namespace Vox