// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_ARRAY_UTILS_HPP
#define CUBBYFLOW_ARRAY_UTILS_HPP

#include <Core/Array/ArrayAccessor2.hpp>
#include <Core/Array/ArrayAccessor3.hpp>

namespace CubbyFlow
{
//!
//! \brief Assigns \p value to 1-D array \p output with \p size.
//!
//! This function assigns \p value to 1-D array \p output with \p size. The
//! output array must support random access operator [].
//!
template <typename ArrayType, typename T>
void SetRange1(size_t size, const T& value, ArrayType* output);

//!
//! \brief Assigns \p value to 1-D array \p output from \p begin to \p end.
//!
//! This function assigns \p value to 1-D array \p output from \p begin to \p
//! end. The output array must support random access operator [].
//!
template <typename ArrayType, typename T>
void SetRange1(size_t begin, size_t end, const T& value, ArrayType* output);

//!
//! \brief Copies \p input array to \p output array with \p size.
//!
//! This function copies \p input array to \p output array with \p size. The
//! input and output array must support random access operator [].
//!
template <typename ArrayType1, typename ArrayType2>
void CopyRange1(const ArrayType1& input, size_t size, ArrayType2* output);

//!
//! \brief Copies \p input array to \p output array from \p begin to \p end.
//!
//! This function copies \p input array to \p output array from \p begin to
//! \p end. The input and output array must support random access operator [].
//!
template <typename ArrayType1, typename ArrayType2>
void CopyRange1(const ArrayType1& input, size_t begin, size_t end,
                ArrayType2* output);

//!
//! \brief Copies 2-D \p input array to \p output array with \p sizeX and
//! \p sizeY.
//!
//! This function copies 2-D \p input array to \p output array with \p sizeX and
//! \p sizeY. The input and output array must support 2-D random access operator
//! (i, j).
//!
template <typename ArrayType1, typename ArrayType2>
void CopyRange2(const ArrayType1& input, size_t sizeX, size_t sizeY,
                ArrayType2* output);

//!
//! \brief Copies 2-D \p input array to \p output array from
//! (\p beginX, \p beginY) to (\p endX, \p endY).
//!
//! This function copies 2-D \p input array to \p output array from
//! (\p beginX, \p beginY) to (\p endX, \p endY). The input and output array
//! must support 2-D random access operator (i, j).
//!
template <typename ArrayType1, typename ArrayType2>
void CopyRange2(const ArrayType1& input, size_t beginX, size_t endX,
                size_t beginY, size_t endY, ArrayType2* output);

//!
//! \brief Copies 3-D \p input array to \p output array with \p sizeX and
//! \p sizeY.
//!
//! This function copies 3-D \p input array to \p output array with \p sizeX and
//! \p sizeY. The input and output array must support 3-D random access operator
//! (i, j, k).
//!
template <typename ArrayType1, typename ArrayType2>
void CopyRange3(const ArrayType1& input, size_t sizeX, size_t sizeY,
                size_t sizeZ, ArrayType2* output);

//!
//! \brief Copies 3-D \p input array to \p output array from
//! (\p beginX, \p beginY, \p beginZ) to (\p endX, \p endY, \p endZ).
//!
//! This function copies 3-D \p input array to \p output array from
//! (\p beginX, \p beginY, \p beginZ) to (\p endX, \p endY, \p endZ). The input
//! and output array must support 3-D random access operator (i, j, k).
//!
template <typename ArrayType1, typename ArrayType2>
void CopyRange3(const ArrayType1& input, size_t beginX, size_t endX,
                size_t beginY, size_t endY, size_t beginZ, size_t endZ,
                ArrayType2* output);

//!
//! \brief Extrapolates 2-D input data from 'valid' (1) to 'invalid' (0) region.
//!
//! This function extrapolates 2-D input data from 'valid' (1) to 'invalid' (0)
//! region. It iterates multiple times to propagate the 'valid' values to nearby
//! 'invalid' region. The maximum distance of the propagation is equal to
//! numberOfIterations. The input parameters 'valid' and 'data' should be
//! collocated.
//!
//! \param input - data to extrapolate
//! \param valid - set 1 if valid, else 0.
//! \param numberOfIterations - number of iterations for propagation
//! \param output - extrapolated output
//!
template <typename T>
void ExtrapolateToRegion(const ConstArrayAccessor2<T>& input,
                         const ConstArrayAccessor2<char>& valid,
                         unsigned int numberOfIterations,
                         ArrayAccessor2<T> output);

//!
//! \brief Extrapolates 3-D input data from 'valid' (1) to 'invalid' (0) region.
//!
//! This function extrapolates 3-D input data from 'valid' (1) to 'invalid' (0)
//! region. It iterates multiple times to propagate the 'valid' values to nearby
//! 'invalid' region. The maximum distance of the propagation is equal to
//! numberOfIterations. The input parameters 'valid' and 'data' should be
//! collocated.
//!
//! \param input - data to extrapolate
//! \param valid - set 1 if valid, else 0.
//! \param numberOfIterations - number of iterations for propagation
//! \param output - extrapolated output
//!
template <typename T>
void ExtrapolateToRegion(const ConstArrayAccessor3<T>& input,
                         const ConstArrayAccessor3<char>& valid,
                         unsigned int numberOfIterations,
                         ArrayAccessor3<T> output);

//!
//! \brief Converts 2-D array to Comma Separated Value (CSV) stream.
//!
//! \param data - data to convert
//! \param stream - stream object to write CSV
//!
template <typename ArrayType>
void ConvertToCSV(const ArrayType& data, std::ostream* stream);
}  // namespace CubbyFlow

#include <Core/Array/ArrayUtils-Impl.hpp>

#endif