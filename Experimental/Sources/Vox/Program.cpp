/*************************************************************************
> File Name: Program.cpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: OpenGL Program wrapper class
> Created Time: 2020/08/03
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/Program.hpp>
#include <glad/glad.h>

namespace Vox {

    Program::Program(GLuint program)
    {
        _program = program;
    }

    Program::~Program()
    {
        if(_program) glDeleteProgram(_program);
    }

    void Program::UseProgram() const
    {
        glUseProgram(_program);
    }
    
};