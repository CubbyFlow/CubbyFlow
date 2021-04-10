// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Solver/Grid/GridForwardEulerDiffusionSolver2.hpp>
#include <Core/Utils/LevelSetUtils.hpp>

namespace CubbyFlow
{
static const char FLUID = 0;
static const char AIR = 1;
static const char BOUNDARY = 2;

template <typename T>
T Laplacian(const ConstArrayView2<T>& data, const Array2<char>& marker,
            const Vector2D& gridSpacing, size_t i, size_t j)
{
    const T center = data(i, j);
    const Vector2UZ& ds = data.Size();

    assert(i < ds.x && j < ds.y);

    T dLeft = T{};
    T dRight = T{};
    T dDown = T{};
    T dUp = T{};

    if (i > 0 && marker(i - 1, j) == FLUID)
    {
        dLeft = center - data(i - 1, j);
    }
    if (i + 1 < ds.x && marker(i + 1, j) == FLUID)
    {
        dRight = data(i + 1, j) - center;
    }

    if (j > 0 && marker(i, j - 1) == FLUID)
    {
        dDown = center - data(i, j - 1);
    }
    if (j + 1 < ds.y && marker(i, j + 1) == FLUID)
    {
        dUp = data(i, j + 1) - center;
    }

    return (dRight - dLeft) / Square(gridSpacing.x) +
           (dUp - dDown) / Square(gridSpacing.y);
}

void GridForwardEulerDiffusionSolver2::Solve(const ScalarGrid2& source,
                                             double diffusionCoefficient,
                                             double timeIntervalInSeconds,
                                             ScalarGrid2* dest,
                                             const ScalarField2& boundarySDF,
                                             const ScalarField2& fluidSDF)
{
    ConstArrayView2<double> src = source.DataView();
    Vector2D h = source.GridSpacing();
    const auto pos = Unroll2(source.DataPosition());

    BuildMarkers(source.Resolution(), pos, boundarySDF, fluidSDF);

    source.ParallelForEachDataPointIndex([&](size_t i, size_t j) {
        if (m_markers(i, j) == FLUID)
        {
            (*dest)(i, j) =
                source(i, j) + diffusionCoefficient * timeIntervalInSeconds *
                                   Laplacian(src, m_markers, h, i, j);
        }
        else
        {
            (*dest)(i, j) = source(i, j);
        }
    });
}

void GridForwardEulerDiffusionSolver2::Solve(
    const CollocatedVectorGrid2& source, double diffusionCoefficient,
    double timeIntervalInSeconds, CollocatedVectorGrid2* dest,
    const ScalarField2& boundarySDF, const ScalarField2& fluidSDF)
{
    ConstArrayView2<Vector2<double>> src = source.DataView();
    Vector2D h = source.GridSpacing();
    const auto pos = Unroll2(source.DataPosition());

    BuildMarkers(source.Resolution(), pos, boundarySDF, fluidSDF);

    source.ParallelForEachDataPointIndex([&](size_t i, size_t j) {
        if (m_markers(i, j) == FLUID)
        {
            (*dest)(i, j) = src(i, j) + diffusionCoefficient *
                                            timeIntervalInSeconds *
                                            Laplacian(src, m_markers, h, i, j);
        }
        else
        {
            (*dest)(i, j) = src(i, j);
        }
    });
}

void GridForwardEulerDiffusionSolver2::Solve(const FaceCenteredGrid2& source,
                                             double diffusionCoefficient,
                                             double timeIntervalInSeconds,
                                             FaceCenteredGrid2* dest,
                                             const ScalarField2& boundarySDF,
                                             const ScalarField2& fluidSDF)
{
    ConstArrayView2<double> uSrc = source.UView();
    ConstArrayView2<double> vSrc = source.VView();
    ArrayView2<double> u = dest->UView();
    ArrayView2<double> v = dest->VView();
    const auto uPos = Unroll2(source.UPosition());
    const auto vPos = Unroll2(source.VPosition());
    Vector2D h = source.GridSpacing();

    BuildMarkers(source.USize(), uPos, boundarySDF, fluidSDF);

    source.ParallelForEachUIndex([&](size_t i, size_t j) {
        if (m_markers(i, j) == FLUID)
        {
            u(i, j) = uSrc(i, j) + diffusionCoefficient *
                                       timeIntervalInSeconds *
                                       Laplacian(uSrc, m_markers, h, i, j);
        }
        else
        {
            u(i, j) = uSrc(i, j);
        }
    });

    BuildMarkers(source.VSize(), vPos, boundarySDF, fluidSDF);

    source.ParallelForEachVIndex([&](size_t i, size_t j) {
        if (m_markers(i, j) == FLUID)
        {
            v(i, j) = vSrc(i, j) + diffusionCoefficient *
                                       timeIntervalInSeconds *
                                       Laplacian(vSrc, m_markers, h, i, j);
        }
        else
        {
            v(i, j) = vSrc(i, j);
        }
    });
}

void GridForwardEulerDiffusionSolver2::BuildMarkers(
    const Vector2UZ& size, const std::function<Vector2D(size_t, size_t)>& pos,
    const ScalarField2& boundarySDF, const ScalarField2& fluidSDF)
{
    m_markers.Resize(size);

    ForEachIndex(m_markers.Size(), [&](size_t i, size_t j) {
        if (IsInsideSDF(boundarySDF.Sample(pos(i, j))))
        {
            m_markers(i, j) = BOUNDARY;
        }
        else if (IsInsideSDF(fluidSDF.Sample(pos(i, j))))
        {
            m_markers(i, j) = FLUID;
        }
        else
        {
            m_markers(i, j) = AIR;
        }
    });
}
}  // namespace CubbyFlow