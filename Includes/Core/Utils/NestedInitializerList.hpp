// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.
//
// This code is adopted from https://github.com/christophercrouzet/m3ta
//
// The MIT License (MIT)
//
// Copyright (c) 2015 Christopher Crouzet
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef CUBBYFLOW_NESTED_INITIALIZER_LIST_HPP
#define CUBBYFLOW_NESTED_INITIALIZER_LIST_HPP

#include <initializer_list>

namespace CubbyFlow
{
template <typename T, size_t N>
struct NestedInitializerLists
{
    using Type =
        std::initializer_list<typename NestedInitializerLists<T, N - 1>::Type>;
};

template <typename T>
struct NestedInitializerLists<T, 0>
{
    using Type = T;
};

template <typename T, size_t N>
using NestedInitializerListsT = typename NestedInitializerLists<T, N>::Type;
}  // namespace CubbyFlow

#endif