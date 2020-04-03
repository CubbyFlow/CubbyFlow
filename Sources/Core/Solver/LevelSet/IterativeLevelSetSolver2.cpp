// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Array/ArrayUtils.hpp>
#include <Core/FDM/FDMUtils.hpp>
#include <Core/Solver/LevelSet/IterativeLevelSetSolver2.hpp>
#include <Core/Utils/Logging.hpp>

namespace CubbyFlow
{
IterativeLevelSetSolver2::IterativeLevelSetSolver2()
{
    // Do nothing
}

IterativeLevelSetSolver2::~IterativeLevelSetSolver2()
{
    // Do nothing
}

void IterativeLevelSetSolver2::Reinitialize(const ScalarGrid2& inputSDF,
                                            double maxDistance,
                                            ScalarGrid2* outputSDF)
{
    const Size2 size = inputSDF.GetDataSize();
    const Vector2D gridSpacing = inputSDF.GridSpacing();

    if (!inputSDF.HasSameShape(*outputSDF))
    {
        throw std::invalid_argument(
            "inputSDF and outputSDF have not same shape.");
    }

    ArrayAccessor2<double> outputAcc = outputSDF->GetDataAccessor();

    const double dtau =
        PseudoTimeStep(inputSDF.GetConstDataAccessor(), gridSpacing);
    const unsigned int numberOfIterations =
        DistanceToNumberOfIterations(maxDistance, dtau);

    CopyRange2(inputSDF.GetConstDataAccessor(), size.x, size.y, &outputAcc);

    Array2<double> temp(size);
    ArrayAccessor2<double> tempAcc = temp.Accessor();

    CUBBYFLOW_INFO << "Reinitializing with pseudoTimeStep: " << dtau
                   << " numberOfIterations: " << numberOfIterations;

    for (unsigned int n = 0; n < numberOfIterations; ++n)
    {
        inputSDF.ParallelForEachDataPointIndex([&](size_t i, size_t j) {
            double s = Sign(outputAcc, gridSpacing, i, j);

            std::array<double, 2> dx, dy;

            GetDerivatives(outputAcc, gridSpacing, i, j, &dx, &dy);

            // Explicit Euler step
            double val = outputAcc(i, j) -
                         dtau * std::max(s, 0.0) *
                             (std::sqrt(Square(std::max(dx[0], 0.0)) +
                                        Square(std::min(dx[1], 0.0)) +
                                        Square(std::max(dy[0], 0.0)) +
                                        Square(std::min(dy[1], 0.0))) -
                              1.0) -
                         dtau * std::min(s, 0.0) *
                             (std::sqrt(Square(std::min(dx[0], 0.0)) +
                                        Square(std::max(dx[1], 0.0)) +
                                        Square(std::min(dy[0], 0.0)) +
                                        Square(std::max(dy[1], 0.0))) -
                              1.0);
            tempAcc(i, j) = val;
        });

        std::swap(tempAcc, outputAcc);
    }

    auto outputSDFAcc = outputSDF->GetDataAccessor();
    CopyRange2(outputAcc, size.x, size.y, &outputSDFAcc);
}

void IterativeLevelSetSolver2::Extrapolate(const ScalarGrid2& input,
                                           const ScalarField2& sdf,
                                           double maxDistance,
                                           ScalarGrid2* output)
{
    if (!input.HasSameShape(*output))
    {
        throw std::invalid_argument(
            "inputSDF and outputSDF have not same shape.");
    }

    Array2<double> sdfGrid(input.GetDataSize());
    auto pos = input.GetDataPosition();
    sdfGrid.ParallelForEachIndex(
        [&](size_t i, size_t j) { sdfGrid(i, j) = sdf.Sample(pos(i, j)); });

    Extrapolate(input.GetConstDataAccessor(), sdfGrid.ConstAccessor(),
                input.GridSpacing(), maxDistance, output->GetDataAccessor());
}

void IterativeLevelSetSolver2::Extrapolate(const CollocatedVectorGrid2& input,
                                           const ScalarField2& sdf,
                                           double maxDistance,
                                           CollocatedVectorGrid2* output)
{
    if (!input.HasSameShape(*output))
    {
        throw std::invalid_argument(
            "inputSDF and outputSDF have not same shape.");
    }

    Array2<double> sdfGrid(input.GetDataSize());
    auto pos = input.GetDataPosition();
    sdfGrid.ParallelForEachIndex(
        [&](size_t i, size_t j) { sdfGrid(i, j) = sdf.Sample(pos(i, j)); });

    const Vector2D gridSpacing = input.GridSpacing();

    Array2<double> u(input.GetDataSize());
    Array2<double> u0(input.GetDataSize());
    Array2<double> v(input.GetDataSize());
    Array2<double> v0(input.GetDataSize());

    input.ParallelForEachDataPointIndex([&](size_t i, size_t j) {
        u(i, j) = input(i, j).x;
        v(i, j) = input(i, j).y;
    });

    Extrapolate(u, sdfGrid.ConstAccessor(), gridSpacing, maxDistance, u0);
    Extrapolate(v, sdfGrid.ConstAccessor(), gridSpacing, maxDistance, v0);

    output->ParallelForEachDataPointIndex([&](size_t i, size_t j) {
        (*output)(i, j).x = u(i, j);
        (*output)(i, j).y = v(i, j);
    });
}

void IterativeLevelSetSolver2::Extrapolate(const FaceCenteredGrid2& input,
                                           const ScalarField2& sdf,
                                           double maxDistance,
                                           FaceCenteredGrid2* output)
{
    if (!input.HasSameShape(*output))
    {
        throw std::invalid_argument(
            "inputSDF and outputSDF have not same shape.");
    }

    const Vector2D gridSpacing = input.GridSpacing();

    auto u = input.GetUConstAccessor();
    auto uPos = input.GetUPosition();
    Array2<double> sdfAtU(u.size());
    input.ParallelForEachUIndex(
        [&](size_t i, size_t j) { sdfAtU(i, j) = sdf.Sample(uPos(i, j)); });

    Extrapolate(u, sdfAtU, gridSpacing, maxDistance, output->GetUAccessor());

    auto v = input.GetVConstAccessor();
    auto vPos = input.GetVPosition();
    Array2<double> sdfAtV(v.size());
    input.ParallelForEachVIndex(
        [&](size_t i, size_t j) { sdfAtV(i, j) = sdf.Sample(vPos(i, j)); });

    Extrapolate(v, sdfAtV, gridSpacing, maxDistance, output->GetVAccessor());
}

void IterativeLevelSetSolver2::Extrapolate(
    const ConstArrayAccessor2<double>& input,
    const ConstArrayAccessor2<double>& sdf, const Vector2D& gridSpacing,
    double maxDistance, ArrayAccessor2<double> output)
{
    const Size2 size = input.size();

    ArrayAccessor2<double> outputAcc = output;

    const double dtau = PseudoTimeStep(sdf, gridSpacing);
    const unsigned int numberOfIterations =
        DistanceToNumberOfIterations(maxDistance, dtau);

    CopyRange2(input, size.x, size.y, &outputAcc);

    Array2<double> temp(size);
    ArrayAccessor2<double> tempAcc = temp.Accessor();

    for (unsigned int n = 0; n < numberOfIterations; ++n)
    {
        ParallelFor(
            ZERO_SIZE, size.x, ZERO_SIZE, size.y, [&](size_t i, size_t j) {
                if (sdf(i, j) >= 0)
                {
                    std::array<double, 2> dx, dy;
                    Vector2D grad = Gradient2(sdf, gridSpacing, i, j);

                    GetDerivatives(outputAcc, gridSpacing, i, j, &dx, &dy);

                    tempAcc(i, j) = outputAcc(i, j) -
                                    dtau * (std::max(grad.x, 0.0) * dx[0] +
                                            std::min(grad.x, 0.0) * dx[1] +
                                            std::max(grad.y, 0.0) * dy[0] +
                                            std::min(grad.y, 0.0) * dy[1]);
                }
                else
                {
                    tempAcc(i, j) = outputAcc(i, j);
                }
            });

        std::swap(tempAcc, outputAcc);
    }

    CopyRange2(outputAcc, size.x, size.y, &output);
}

double IterativeLevelSetSolver2::GetMaxCFL() const
{
    return m_maxCFL;
}

void IterativeLevelSetSolver2::SetMaxCFL(double newMaxCFL)
{
    m_maxCFL = std::max(newMaxCFL, 0.0);
}

unsigned int IterativeLevelSetSolver2::DistanceToNumberOfIterations(
    double distance, double dtau)
{
    return static_cast<unsigned int>(std::ceil(distance / dtau));
}

double IterativeLevelSetSolver2::Sign(const ConstArrayAccessor2<double>& sdf,
                                      const Vector2D& gridSpacing, size_t i,
                                      size_t j)
{
    double d = sdf(i, j);
    double e = std::min(gridSpacing.x, gridSpacing.y);
    return d / std::sqrt(d * d + e * e);
}

double IterativeLevelSetSolver2::PseudoTimeStep(
    ConstArrayAccessor2<double> sdf, const Vector2D& gridSpacing) const
{
    const Size2 size = sdf.size();

    const double h = std::max(gridSpacing.x, gridSpacing.y);

    double maxS = -std::numeric_limits<double>::max();
    double dtau = m_maxCFL * h;

    for (size_t j = 0; j < size.y; ++j)
    {
        for (size_t i = 0; i < size.x; ++i)
        {
            double s = Sign(sdf, gridSpacing, i, j);
            maxS = std::max(s, maxS);
        }
    }

    while (dtau * maxS / h > m_maxCFL)
    {
        dtau *= 0.5;
    }

    return dtau;
}
}  // namespace CubbyFlow