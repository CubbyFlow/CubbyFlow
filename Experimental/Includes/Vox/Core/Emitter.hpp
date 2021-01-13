/*************************************************************************
> File Name: Emitter.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Base emitter class
> Created Time: 2020/12/25
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_EMITTER_HPP
#define CUBBYFLOW_VOX_EMITTER_HPP

#include <Vox/Scene/VoxSceneObject.hpp>
#include <Vox/Core/Program.hpp>

namespace Vox
{
   /**
    * Base Emitter class
    */
   class Emitter : public VoxSceneObject
   {
    public:
       Emitter() = default;
        
       void UploadToProgram(const std::shared_ptr<Program>& program);
    protected:
       virtual void OnUploadProgram(ShaderParameters& params);
    private:
   };

};  // namespace Vox

#endif