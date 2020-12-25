/*************************************************************************
> File Name: ArrayUtils.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: CubbyFlow array class util functions.
> Created Time: 2020/08/29
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_ARRAY_UTILS_HPP
#define CUBBYFLOW_VOX_ARRAY_UTILS_HPP

#include <Core/Array/Array1.hpp>
#include <Core/Array/Array2.hpp>
#include <Core/Array/Array3.hpp>
#include <Core/Array/ArrayAccessor1.hpp>
#include <Core/Array/ArrayAccessor2.hpp>
#include <Core/Array/ArrayAccessor3.hpp>

namespace Vox {

    template <typename U, typename T, size_t N>
    void ArrayCast(CubbyFlow::Array<U, N>& dest, const CubbyFlow::Array<T, N>& src);
    
    template <typename U, typename T, size_t N>
    void ArrayCast(CubbyFlow::Array<U, N>& dest, const CubbyFlow::ArrayAccessor<T, N>& src);

    template <typename U, typename T, size_t N>
    void ArrayCast(CubbyFlow::Array<U, N>& dest, const CubbyFlow::ConstArrayAccessor<T, N>& src);

};

#include <Vox/ArrayUtils-Impl.hpp>
#endif