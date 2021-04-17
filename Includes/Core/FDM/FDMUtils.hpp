// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef CUBBYFLOW_FDM_UTILS_HPP
#define CUBBYFLOW_FDM_UTILS_HPP

#include <Core/Array/ArrayView.hpp>
#include <Core/Matrix/Matrix.hpp>

namespace CubbyFlow
{
//! \brief Returns 2-D gradient vector from given 2-D scalar grid-like array
//!        \p data, \p gridSpacing, and array index (\p i, \p j).
Vector2D Gradient2(const ConstArrayView2<double>& data,
                   const Vector2D& gridSpacing, size_t i, size_t j);

//! \brief Returns 2-D gradient vectors from given 2-D vector grid-like array
//!        \p data, \p gridSpacing, and array index (\p i, \p j).
std::array<Vector2D, 2> Gradient2(const ConstArrayView2<Vector2D>& data,
                                  const Vector2D& gridSpacing, size_t i,
                                  size_t j);

//! \brief Returns 3-D gradient vector from given 3-D scalar grid-like array
//!        \p data, \p gridSpacing, and array index (\p i, \p j, \p k).
Vector3D Gradient3(const ConstArrayView3<double>& data,
                   const Vector3D& gridSpacing, size_t i, size_t j, size_t k);

//! \brief Returns 3-D gradient vectors from given 3-D vector grid-like array
//!        \p data, \p gridSpacing, and array index (\p i, \p j, \p k).
std::array<Vector3D, 3> Gradient3(const ConstArrayView3<Vector3D>& data,
                                  const Vector3D& gridSpacing, size_t i,
                                  size_t j, size_t k);

//! \brief Returns Laplacian value from given 2-D scalar grid-like array
//!        \p data, \p gridSpacing, and array index (\p i, \p j).
double Laplacian2(const ConstArrayView2<double>& data,
                  const Vector2D& gridSpacing, size_t i, size_t j);

//! \brief Returns 2-D Laplacian vectors from given 2-D vector grid-like array
//!        \p data, \p gridSpacing, and array index (\p i, \p j).
Vector2D Laplacian2(const ConstArrayView2<Vector2D>& data,
                    const Vector2D& gridSpacing, size_t i, size_t j);

//! \brief Returns Laplacian value from given 3-D scalar grid-like array
//!        \p data, \p gridSpacing, and array index (\p i, \p j, \p k).
double Laplacian3(const ConstArrayView3<double>& data,
                  const Vector3D& gridSpacing, size_t i, size_t j, size_t k);

//! \brief Returns 3-D Laplacian vectors from given 3-D vector grid-like array
//!        \p data, \p gridSpacing, and array index (\p i, \p j, \p k).
Vector3D Laplacian3(const ConstArrayView3<Vector3D>& data,
                    const Vector3D& gridSpacing, size_t i, size_t j, size_t k);

//! \brief Returns divergence value from given 2-D vector grid-like array
//!        \p data, \p gridSpacing, and array index (\p i, \p j).
double Divergence2(const ConstArrayView2<Vector2D>& data,
                   const Vector2D& gridSpacing, size_t i, size_t j);

//! \brief Returns diverence value from given 3-D vector grid-like array
//!        \p data, \p gridSpacing, and array index (\p i, \p j, \p k).
double Divergence3(const ConstArrayView3<Vector3D>& data,
                   const Vector3D& gridSpacing, size_t i, size_t j, size_t k);

//! \brief Returns curl value from given 2-D vector grid-like array
//!        \p data, \p gridSpacing, and array index (\p i, \p j).
double Curl2(const ConstArrayView2<Vector2D>& data, const Vector2D& gridSpacing,
             size_t i, size_t j);

//! \brief Returns curl value from given 3-D vector grid-like array
//!        \p data, \p gridSpacing, and array index (\p i, \p j, \p k).
Vector3D Curl3(const ConstArrayView3<Vector3D>& data,
               const Vector3D& gridSpacing, size_t i, size_t j, size_t k);

template <size_t N>
struct GetFDMUtils
{
    // Do nothing
};

template <>
struct GetFDMUtils<2>
{
    static Vector2D Gradient(const ConstArrayView2<double>& data,
                             const Vector2D& gridSpacing, const Vector2UZ& idx)
    {
        return Gradient2(data, gridSpacing, idx.x, idx.y);
    }

    static double Laplacian(const ConstArrayView2<double>& data,
                            const Vector2D& gridSpacing, const Vector2UZ& idx)
    {
        return Laplacian2(data, gridSpacing, idx.x, idx.y);
    }

    static double Divergence(const ConstArrayView2<Vector2D>& data,
                             const Vector2D& gridSpacing, const Vector2UZ& idx)
    {
        return Divergence2(data, gridSpacing, idx.x, idx.y);
    }

    static double Curl(const ConstArrayView2<Vector2D>& data,
                       const Vector2D& gridSpacing, const Vector2UZ& idx)
    {
        return Curl2(data, gridSpacing, idx.x, idx.y);
    }
};

template <>
struct GetFDMUtils<3>
{
    static Vector3D Gradient(const ConstArrayView3<double>& data,
                             const Vector3D& gridSpacing, const Vector3UZ& idx)
    {
        return Gradient3(data, gridSpacing, idx.x, idx.y, idx.z);
    }

    static double Laplacian(const ConstArrayView3<double>& data,
                            const Vector3D& gridSpacing, const Vector3UZ& idx)
    {
        return Laplacian3(data, gridSpacing, idx.x, idx.y, idx.z);
    }

    static double Divergence(const ConstArrayView3<Vector3D>& data,
                             const Vector3D& gridSpacing, const Vector3UZ& idx)
    {
        return Divergence3(data, gridSpacing, idx.x, idx.y, idx.z);
    }

    static Vector3D Curl(const ConstArrayView3<Vector3D>& data,
                         const Vector3D& gridSpacing, const Vector3UZ& idx)
    {
        return Curl3(data, gridSpacing, idx.x, idx.y, idx.z);
    }
};
}  // namespace CubbyFlow

#endif