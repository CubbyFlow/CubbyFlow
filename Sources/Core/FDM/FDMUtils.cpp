// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/FDM/FDMUtils.hpp>
#include <Core/Math/MathUtils.hpp>

#include <array>

namespace CubbyFlow
{
Vector2D Gradient2(const ConstArrayView2<double>& data,
                   const Vector2D& gridSpacing, size_t i, size_t j)
{
    const Vector2UZ ds = data.Size();

    assert(i < ds.x && j < ds.y);

    const double left = data((i > 0) ? i - 1 : i, j);
    const double right = data((i + 1 < ds.x) ? i + 1 : i, j);
    const double down = data(i, (j > 0) ? j - 1 : j);
    const double up = data(i, (j + 1 < ds.y) ? j + 1 : j);

    return 0.5 * ElemDiv(Vector2D{ right - left, up - down }, gridSpacing);
}

std::array<Vector2D, 2> Gradient2(const ConstArrayView2<Vector2D>& data,
                                  const Vector2D& gridSpacing, size_t i,
                                  size_t j)
{
    const Vector2UZ ds = data.Size();

    assert(i < ds.x && j < ds.y);

    const Vector2D left = data((i > 0) ? i - 1 : i, j);
    const Vector2D right = data((i + 1 < ds.x) ? i + 1 : i, j);
    const Vector2D down = data(i, (j > 0) ? j - 1 : j);
    const Vector2D up = data(i, (j + 1 < ds.y) ? j + 1 : j);

    std::array<Vector2D, 2> result;
    result[0] =
        0.5 * ElemDiv(Vector2D{ right.x - left.x, up.x - down.x }, gridSpacing);
    result[1] =
        0.5 * ElemDiv(Vector2D{ right.y - left.y, up.y - down.y }, gridSpacing);

    return result;
}

Vector3D Gradient3(const ConstArrayView3<double>& data,
                   const Vector3D& gridSpacing, size_t i, size_t j, size_t k)
{
    const Vector3UZ ds = data.Size();

    assert(i < ds.x && j < ds.y && k < ds.z);

    const double left = data((i > 0) ? i - 1 : i, j, k);
    const double right = data((i + 1 < ds.x) ? i + 1 : i, j, k);
    const double down = data(i, (j > 0) ? j - 1 : j, k);
    const double up = data(i, (j + 1 < ds.y) ? j + 1 : j, k);
    const double back = data(i, j, (k > 0) ? k - 1 : k);
    const double front = data(i, j, (k + 1 < ds.z) ? k + 1 : k);

    return 0.5 * ElemDiv(Vector3D{ right - left, up - down, front - back },
                         gridSpacing);
}

std::array<Vector3D, 3> Gradient3(const ConstArrayView3<Vector3D>& data,
                                  const Vector3D& gridSpacing, size_t i,
                                  size_t j, size_t k)
{
    const Vector3UZ ds = data.Size();

    assert(i < ds.x && j < ds.y && k < ds.z);

    const Vector3D left = data((i > 0) ? i - 1 : i, j, k);
    const Vector3D right = data((i + 1 < ds.x) ? i + 1 : i, j, k);
    const Vector3D down = data(i, (j > 0) ? j - 1 : j, k);
    const Vector3D up = data(i, (j + 1 < ds.y) ? j + 1 : j, k);
    const Vector3D back = data(i, j, (k > 0) ? k - 1 : k);
    const Vector3D front = data(i, j, (k + 1 < ds.z) ? k + 1 : k);

    std::array<Vector3D, 3> result;
    result[0] = 0.5 * ElemDiv(Vector3D{ right.x - left.x, up.x - down.x,
                                        front.x - back.x },
                              gridSpacing);
    result[1] = 0.5 * ElemDiv(Vector3D{ right.y - left.y, up.y - down.y,
                                        front.y - back.y },
                              gridSpacing);
    result[2] = 0.5 * ElemDiv(Vector3D{ right.z - left.z, up.z - down.z,
                                        front.z - back.z },
                              gridSpacing);

    return result;
}

double Laplacian2(const ConstArrayView2<double>& data,
                  const Vector2D& gridSpacing, size_t i, size_t j)
{
    const double center = data(i, j);
    const Vector2UZ ds = data.Size();

    assert(i < ds.x && j < ds.y);

    double dLeft = 0.0, dRight = 0.0, dDown = 0.0, dUp = 0.0;

    if (i > 0)
    {
        dLeft = center - data(i - 1, j);
    }
    if (i + 1 < ds.x)
    {
        dRight = data(i + 1, j) - center;
    }

    if (j > 0)
    {
        dDown = center - data(i, j - 1);
    }
    if (j + 1 < ds.y)
    {
        dUp = data(i, j + 1) - center;
    }

    return (dRight - dLeft) / Square(gridSpacing.x) +
           (dUp - dDown) / Square(gridSpacing.y);
}

Vector2D Laplacian2(const ConstArrayView2<Vector2D>& data,
                    const Vector2D& gridSpacing, size_t i, size_t j)
{
    const Vector2D center = data(i, j);
    const Vector2UZ ds = data.Size();

    assert(i < ds.x && j < ds.y);

    Vector2D dLeft, dRight, dDown, dUp;

    if (i > 0)
    {
        dLeft = center - data(i - 1, j);
    }
    if (i + 1 < ds.x)
    {
        dRight = data(i + 1, j) - center;
    }

    if (j > 0)
    {
        dDown = center - data(i, j - 1);
    }
    if (j + 1 < ds.y)
    {
        dUp = data(i, j + 1) - center;
    }

    return (dRight - dLeft) / Square(gridSpacing.x) +
           (dUp - dDown) / Square(gridSpacing.y);
}

double Laplacian3(const ConstArrayView3<double>& data,
                  const Vector3D& gridSpacing, size_t i, size_t j, size_t k)
{
    const double center = data(i, j, k);
    const Vector3UZ ds = data.Size();

    assert(i < ds.x && j < ds.y && k < ds.z);

    double dLeft = 0.0, dRight = 0.0, dDown = 0.0, dUp = 0.0, dback = 0.0,
           dFront = 0.0;

    if (i > 0)
    {
        dLeft = center - data(i - 1, j, k);
    }
    if (i + 1 < ds.x)
    {
        dRight = data(i + 1, j, k) - center;
    }

    if (j > 0)
    {
        dDown = center - data(i, j - 1, k);
    }
    if (j + 1 < ds.y)
    {
        dUp = data(i, j + 1, k) - center;
    }

    if (k > 0)
    {
        dback = center - data(i, j, k - 1);
    }
    if (k + 1 < ds.z)
    {
        dFront = data(i, j, k + 1) - center;
    }

    return (dRight - dLeft) / Square(gridSpacing.x) +
           (dUp - dDown) / Square(gridSpacing.y) +
           (dFront - dback) / Square(gridSpacing.z);
}

Vector3D Laplacian3(const ConstArrayView3<Vector3D>& data,
                    const Vector3D& gridSpacing, size_t i, size_t j, size_t k)
{
    const Vector3D center = data(i, j, k);
    const Vector3UZ ds = data.Size();

    assert(i < ds.x && j < ds.y && k < ds.z);

    Vector3D dLeft, dRight, dDown, dUp, dBack, dFront;

    if (i > 0)
    {
        dLeft = center - data(i - 1, j, k);
    }
    if (i + 1 < ds.x)
    {
        dRight = data(i + 1, j, k) - center;
    }

    if (j > 0)
    {
        dDown = center - data(i, j - 1, k);
    }
    if (j + 1 < ds.y)
    {
        dUp = data(i, j + 1, k) - center;
    }

    if (k > 0)
    {
        dBack = center - data(i, j, k - 1);
    }
    if (k + 1 < ds.z)
    {
        dFront = data(i, j, k + 1) - center;
    }

    return (dRight - dLeft) / Square(gridSpacing.x) +
           (dUp - dDown) / Square(gridSpacing.y) +
           (dFront - dBack) / Square(gridSpacing.z);
}

double Divergence2(const ConstArrayView2<Vector2D>& data,
                   const Vector2D& gridSpacing, size_t i, size_t j)
{
    const Vector2UZ& ds = data.Size();

    assert(i < ds.x && j < ds.y);

    const double left = data((i > 0) ? i - 1 : i, j).x;
    const double right = data((i + 1 < ds.x) ? i + 1 : i, j).x;
    const double down = data(i, (j > 0) ? j - 1 : j).y;
    const double up = data(i, (j + 1 < ds.y) ? j + 1 : j).y;

    return 0.5 * (right - left) / gridSpacing.x +
           0.5 * (up - down) / gridSpacing.y;
}

double Divergence3(const ConstArrayView3<Vector3D>& data,
                   const Vector3D& gridSpacing, size_t i, size_t j, size_t k)
{
    const Vector3UZ ds = data.Size();

    assert(i < ds.x && j < ds.y && k < ds.z);

    const double left = data((i > 0) ? i - 1 : i, j, k).x;
    const double right = data((i + 1 < ds.x) ? i + 1 : i, j, k).x;
    const double down = data(i, (j > 0) ? j - 1 : j, k).y;
    const double up = data(i, (j + 1 < ds.y) ? j + 1 : j, k).y;
    const double back = data(i, j, (k > 0) ? k - 1 : k).z;
    const double front = data(i, j, (k + 1 < ds.z) ? k + 1 : k).z;

    return 0.5 * (right - left) / gridSpacing.x +
           0.5 * (up - down) / gridSpacing.y +
           0.5 * (front - back) / gridSpacing.z;
}

double Curl2(const ConstArrayView2<Vector2D>& data, const Vector2D& gridSpacing,
             size_t i, size_t j)
{
    const Vector2UZ ds = data.Size();

    assert(i < ds.x && j < ds.y);

    const Vector2D left = data((i > 0) ? i - 1 : i, j);
    const Vector2D right = data((i + 1 < ds.x) ? i + 1 : i, j);
    const Vector2D bottom = data(i, (j > 0) ? j - 1 : j);
    const Vector2D top = data(i, (j + 1 < ds.y) ? j + 1 : j);

    const double Fx_ym = bottom.x;
    const double Fx_yp = top.x;

    const double Fy_xm = left.y;
    const double Fy_xp = right.y;

    return 0.5 * (Fy_xp - Fy_xm) / gridSpacing.x -
           0.5 * (Fx_yp - Fx_ym) / gridSpacing.y;
}

Vector3D Curl3(const ConstArrayView3<Vector3D>& data,
               const Vector3D& gridSpacing, size_t i, size_t j, size_t k)
{
    const Vector3UZ ds = data.Size();

    assert(i < ds.x && j < ds.y && k < ds.z);

    const Vector3D left = data((i > 0) ? i - 1 : i, j, k);
    const Vector3D right = data((i + 1 < ds.x) ? i + 1 : i, j, k);
    const Vector3D down = data(i, (j > 0) ? j - 1 : j, k);
    const Vector3D up = data(i, (j + 1 < ds.y) ? j + 1 : j, k);
    const Vector3D back = data(i, j, (k > 0) ? k - 1 : k);
    const Vector3D front = data(i, j, (k + 1 < ds.z) ? k + 1 : k);

    const double Fx_ym = down.x;
    const double Fx_yp = up.x;
    const double Fx_zm = back.x;
    const double Fx_zp = front.x;

    const double Fy_xm = left.y;
    const double Fy_xp = right.y;
    const double Fy_zm = back.y;
    const double Fy_zp = front.y;

    const double Fz_xm = left.z;
    const double Fz_xp = right.z;
    const double Fz_ym = down.z;
    const double Fz_yp = up.z;

    return Vector3D{ 0.5 * (Fz_yp - Fz_ym) / gridSpacing.y -
                         0.5 * (Fy_zp - Fy_zm) / gridSpacing.z,
                     0.5 * (Fx_zp - Fx_zm) / gridSpacing.z -
                         0.5 * (Fz_xp - Fz_xm) / gridSpacing.x,
                     0.5 * (Fy_xp - Fy_xm) / gridSpacing.x -
                         0.5 * (Fx_yp - Fx_ym) / gridSpacing.y };
}
}  // namespace CubbyFlow