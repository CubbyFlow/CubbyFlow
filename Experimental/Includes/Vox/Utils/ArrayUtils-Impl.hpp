/*************************************************************************
> File Name: ArrayUtils-Impl.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: CubbyFlow array class util functions.
> Created Time: 2020/08/29
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_ARRAY_UTILS_IMPL_HPP
#define CUBBYFLOW_VOX_ARRAY_UTILS_IMPL_HPP

#include <iterator>
#include <numeric>

namespace Vox {

    namespace Detail
    {
        template <typename U, template<typename, size_t> class Storage, typename T, size_t N>
        inline void StorageCast(CubbyFlow::Array<U, N>& dest, const Storage<T, N>& src)
        {
            const size_t numElements = static_cast<size_t>(std::distance(src.begin(), src.end()));
            dest.Resize(src.size());
            CubbyFlow::ParallelFor(CubbyFlow::ZERO_SIZE, numElements, [&](size_t index){
                dest[index] = static_cast<U>(src[index]);
            });
        }
    }

    template <typename T>
    void IncreasingNumToArray(CubbyFlow::Array1<T>* dest, size_t num, T startNum)
    {
        dest->Resize(num);
        std::iota(dest->begin(), dest->end(), startNum);
    }

    template <typename U, typename T, size_t N>
    void ArrayCast(CubbyFlow::Array<U, N>& dest, const CubbyFlow::Array<T, N>& src)
    {
        Detail::StorageCast<U>(dest, src);
    }

    template <typename U, typename T, size_t N>
    void ArrayCast(CubbyFlow::Array<U, N>& dest, const CubbyFlow::ArrayAccessor<T, N>& src)
    {
        Detail::StorageCast<U>(dest, src);
    }

    template <typename U, typename T, size_t N>
    void ArrayCast(CubbyFlow::Array<U, N>& dest, const CubbyFlow::ConstArrayAccessor<T, N>& src)
    {
        Detail::StorageCast<U>(dest, src);
    }

};

#endif