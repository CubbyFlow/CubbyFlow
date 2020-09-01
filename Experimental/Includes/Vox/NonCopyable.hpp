/*************************************************************************
> File Name: NonCopyable.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: C++ NonCopyable interface
> Created Time: 2020/09/01
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_NONCOPYABLE_HPP
#define CUBBYFLOW_VOX_NONCOPYABLE_HPP

namespace Vox {

    class NonCopyable 
    {
    public:
        //! Default Constructor
        NonCopyable() = default;
        //! Default Destructor
        virtual ~NonCopyable() = default;
        //! Delete move operator and copy operator.
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
        NonCopyable(NonCopyable&&) = delete;
        NonCopyable& operator=(NonCopyable&&) = delete;
    };
};

#endif