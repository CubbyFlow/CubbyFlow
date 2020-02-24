/*************************************************************************
> File Name: Common.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: Useful macros and functions collection.
> Created Time: 2020/02/15
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#ifndef CUBBYFLOW_COMMON_H
#define CUBBYFLOW_COMMON_H

//! Jet public headers
#include <Core/Utils/Constants.h>
#include <Core/Utils/Macros.h>
#include <Core/Utils/Logging.h>
#include <Core/Vector/Vector.h>
#include <cstdint>

namespace CubbyFlow
{
    template <typename T>
    using Vector1 = Vector<T, 1>;

    template <typename T>
    using Vector2 = Vector<T, 2>;

    template <typename T>
    using Vector3 = Vector<T, 3>;

    template <typename T>
    using Vector4 = Vector<T, 4>;

    using Vector1B = Vector1<int8_t>;
    using Vector1UB = Vector1<uint8_t>;
    using Vector1S = Vector1<int16_t>;
    using Vector1US = Vector1<uint16_t>;
    using Vector1I = Vector1<int32_t>;
    using Vector1UI = Vector1<uint32_t>;
    using Vector1L = Vector1<int64_t>;
    using Vector1UL = Vector1<uint64_t>;
    using Vector1F = Vector1<float>;
    using Vector1D = Vector1<double>;
    using Vector1Z = Vector1<ssize_t>;
    using Vector1UZ = Vector1<size_t>;

    using Vector2B = Vector2<int8_t>;
    using Vector2UB = Vector2<uint8_t>;
    using Vector2S = Vector2<int16_t>;
    using Vector2US = Vector2<uint16_t>;
    using Vector2I = Vector2<int32_t>;
    using Vector2UI = Vector2<uint32_t>;
    using Vector2L = Vector2<int64_t>;
    using Vector2UL = Vector2<uint64_t>;
    using Vector2F = Vector2<float>;
    using Vector2D = Vector2<double>;
    using Vector2Z = Vector2<ssize_t>;
    using Vector2UZ = Vector2<size_t>;

    using Vector3B = Vector3<int8_t>;
    using Vector3UB = Vector3<uint8_t>;
    using Vector3S = Vector3<int16_t>;
    using Vector3US = Vector3<uint16_t>;
    using Vector3I = Vector3<int32_t>;
    using Vector3UI = Vector3<uint32_t>;
    using Vector3L = Vector3<int64_t>;
    using Vector3UL = Vector3<uint64_t>;
    using Vector3F = Vector3<float>;
    using Vector3D = Vector3<double>;
    using Vector3Z = Vector3<ssize_t>;
    using Vector3UZ = Vector3<size_t>;

    using Vector4B = Vector4<int8_t>;
    using Vector4UB = Vector4<uint8_t>;
    using Vector4S = Vector4<int16_t>;
    using Vector4US = Vector4<uint16_t>;
    using Vector4I = Vector4<int32_t>;
    using Vector4UI = Vector4<uint32_t>;
    using Vector4L = Vector4<int64_t>;
    using Vector4UL = Vector4<uint64_t>;
    using Vector4F = Vector4<float>;
    using Vector4D = Vector4<double>;
    using Vector4Z = Vector4<ssize_t>;
    using Vector4UZ = Vector4<size_t>;
}

#endif