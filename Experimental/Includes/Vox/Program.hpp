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

#include <Vox/GLTypes.hpp>
#include <string>
#include <unordered_map>

namespace Vox {

    /**
     * OpenGL Program wrapper class 
     */
    class Program 
    {
    public:
        //! Default Constructor
        Program(GLuint program);
        //! Default Destructor
        ~Program();

        //! Use this program in current context.
        void UseProgram() const;

        template <typename UniformType>
        void SendUniformVariable(const std::string& name, UniformType&& var);

    protected:
    private:
        std::unordered_map<std::string, GLint> _uniformCache;
        GLuint _program { 0 };
    };

};

#include <Vox/Program-Impl.hpp>
#endif