/*************************************************************************
> File Name: VectorUtils-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Calculation utilities realted to vector. (ex. calculate normal of three vertices).
> Created Time: 2020/04/04
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VOX_VECTOR_UTILS_IMPL_HPP
#define CUBBYFLOW_VOX_VECTOR_UTILS_IMPL_HPP

#include <sstream>

using namespace CubbyFlow;
namespace Vox {

    template <typename T, size_t N>
    bool CheckTriangle(const Vector<T, N>& v1, const Vector<T, N>& v2, const Vector<T, N>& v3)
    {
        static_assert(2 <= N && N <= 4); //! At compile time, make sure the dimension of the vertex is in between 2 and 4.
        return (v2.x - v1.x) * (v3.y - v2.y) == (v3.x - v2.x) * (v2.y - v1.y);
    }

    template <typename T, size_t N>
    Vector<T, N> CalculateNormal(const Vector<T, N>& v1, const Vector<T, N>& v2, const Vector<T, N>& v3)
    {
        static_assert(2 <= N && N <= 3);

        if (!CheckTriangle(v1, v2, v3))
            return Vector<T, N>();

        Vector<T, N> edge1 = v2 - v1;
        Vector<T, N> edge2 = v3 - v2;

        return edge1.Cross(edge2).Normalized();
    }

    template <typename T, size_t N>
    CubbyFlow::Vector<T, N> ParseFromString(const std::string& val)
    {
        std::istringstream isstr(val);

        CubbyFlow::Vector<T, N> ret;
        for (size_t i = 0; i < N; ++i)
            isstr >> ret.At(i);

        return ret;
    }
}

#endif