/*************************************************************************
> File Name: VectorCalculation-Impl.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Calculation utilities realted to vector. (ex. calculate normal of three vertices).
> Created Time: 2020/04/04
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_VECTORCALCULATION_IMPL_H
#define CUBBYFLOW_VECTORCALCULATION_IMPL_H

namespace CubbyFlow {
namespace CubbyRender {

    template <typename T, size_t N>
    inline bool checkTriangle(const Vector<T, N>& v1, const Vector<T, N>& v2, const Vector<T, N>& v3)
    {
        static_assert(1 <= N && N <= 4); //! 
        return (v2.x - v1.x) * (v3.y - v2.y) != (v3.x - v2.x) * (v2.y - v1.y);
    }

    template <typename T, size_t N>
    Vector<T, N> calculateNormal(const Vector<T, N>& v1, const Vector<T, N>& v2, const Vector<T, N>& v3)
    {
        static_assert(2 <= N && N <= 3);

        if (checkTriangle(v1, v2, v3))
            return Vector<T, N>();

        Vector<T, N> edge1 = v2 - v1;
        Vector<T, N> edge2 = v3 - v2;

        return edge1.Cross(edge2);
    }
}
}

#endif