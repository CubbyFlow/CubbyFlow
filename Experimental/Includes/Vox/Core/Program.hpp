/*************************************************************************
> File Name: Program.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Program wrapper class
> Created Time: 2020/08/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_PROGRAM_HPP
#define CUBBYFLOW_VOX_PROGRAM_HPP

#include <Vox/Core/NonCopyable.hpp>
#include <Vox/Core/ShaderParameters.hpp>
#include <Vox/Utils/GLTypes.hpp>
#include <Vox/Scene/VoxSceneObject.hpp>
#include <string>
#include <unordered_map>

namespace Vox {

    class FrameContext;

    /**
     * OpenGL Program wrapper class 
     */
    class Program : public VoxSceneObject
    {
    public:
        //! Default Constructor
        Program(GLuint program);
        //! Default Destructor
        ~Program();

        //! Use this program in current context.
        void BindProgram(const std::shared_ptr<FrameContext>& ctx);

        //! get immutable shader parameters which will be sent to program.
        const ShaderParameters& GetParameters() const;

        //! get mutable shader parameters which will be send to program.
        ShaderParameters& GetParameters();

        //! Returns whether if this object have uniform variable with name.
        bool HasUniformVariable(const std::string& name);

    private:
        //! return shader uniform location id.
        //!
        //! if given parameter does not exist in _locationCache, cache it.
        //! because of communication between cpu and gpu cause bottleneck,
        //! use unordered_map for caching.
        //!
        //! \param name of the shader uniform want to know
        //! \return shader uniform location.
        GLint GetUniformLocation(const std::string& name);

        void SendParametersToGPU();

        std::unordered_map<std::string, GLuint> _locationCache;
        ShaderParameters _parameters;
        GLuint _program { 0 };
    };

};

#endif