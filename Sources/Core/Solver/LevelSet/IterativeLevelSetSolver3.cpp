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
#include <Core/Solver/LevelSet/IterativeLevelSetSolver3.hpp>
#include <Core/Utils/Logging.hpp>

namespace CubbyFlow
{
IterativeLevelSetSolver3::IterativeLevelSetSolver3()
{
    // Do nothing
}

IterativeLevelSetSolver3::~IterativeLevelSetSolver3()
{
    // Do nothing
}

void IterativeLevelSetSolver3::Reinitialize(const ScalarGrid3& inputSDF,
                                            double maxDistance,
                                            ScalarGrid3* outputSDF)
{
    const Size3 size = inputSDF.GetDataSize();
    const Vector3D gridSpacing = inputSDF.GridSpacing();

    if (!inputSDF.HasSameShape(*outputSDF))
    {
        throw std::invalid_argument(
            "inputSDF and outputSDF have not same shape.");
    }

    ArrayAccessor3<double> outputAcc = outputSDF->GetDataAccessor();

    const double dtau =
        PseudoTimeStep(inputSDF.GetConstDataAccessor(), gridSpacing);
    const unsigned int numberOfIterations =
        DistanceToNumberOfIterations(maxDistance, dtau);

    CopyRange3(inputSDF.GetConstDataAccessor(), size.x, size.y, size.z,
               &outputAcc);

    Array3<double> temp(size);
    ArrayAccessor3<double> tempAcc = temp.Accessor();

    CUBBYFLOW_INFO << "Reinitializing with pseudoTimeStep: " << dtau
                   << " numberOfIterations: " << numberOfIterations;

    for (unsigned int n = 0; n < numberOfIterations; ++n)
    {
        inputSDF.ParallelForEachDataPointIndex(
            [&](size_t i, size_t j, size_t k) {
                double s = Sign(outputAcc, gridSpacing, i, j, k);

                std::array<double, 2> dx, dy, dz;

                GetDerivatives(outputAcc, gridSpacing, i, j, k, &dx, &dy, &dz);

                // Explicit Euler step
                double val = outputAcc(i, j, k) -
                             dtau * std::max(s, 0.0) *
                                 (std::sqrt(Square(std::max(dx[0], 0.0)) +
                                            Square(std::min(dx[1], 0.0)) +
                                            Square(std::max(dy[0], 0.0)) +
                                            Square(std::min(dy[1], 0.0)) +
                                            Square(std::max(dz[0], 0.0)) +
                                            Square(std::min(dz[1], 0.0))) -
                                  1.0) -
                             dtau * std::min(s, 0.0) *
                                 (std::sqrt(Square(std::min(dx[0], 0.0)) +
                                            Square(std::max(dx[1], 0.0)) +
                                            Square(std::min(dy[0], 0.0)) +
                                            Square(std::max(dy[1], 0.0)) +
                                            Square(std::min(dz[0], 0.0)) +
                                            Square(std::max(dz[1], 0.0))) -
                                  1.0);
                tempAcc(i, j, k) = val;
            });

        std::swap(tempAcc, outputAcc);
    }

    auto outputSDFAcc = outputSDF->GetDataAccessor();
    CopyRange3(outputAcc, size.x, size.y, size.z, &outputSDFAcc);
}

void IterativeLevelSetSolver3::Extrapolate(const ScalarGrid3& input,
                                           const ScalarField3& sdf,
                                           double maxDistance,
                                           ScalarGrid3* output)
{
    if (!input.HasSameShape(*output))
    {
        throw std::invalid_argument(
            "inputSDF and outputSDF have not same shape.");
    }

    Array3<double> sdfGrid(input.GetDataSize());
    auto pos = input.GetDataPosition();
    sdfGrid.ParallelForEachIndex([&](size_t i, size_t j, size_t k) {
        sdfGrid(i, j, k) = sdf.Sample(pos(i, j, k));
    });

    Extrapolate(input.GetConstDataAccessor(), sdfGrid.ConstAccessor(),
                input.GridSpacing(), maxDistance, output->GetDataAccessor());
}

void IterativeLevelSetSolver3::Extrapolate(const CollocatedVectorGrid3& input,
                                           const ScalarField3& sdf,
                                           double maxDistance,
                                           CollocatedVectorGrid3* output)
{
    if (!input.HasSameShape(*output))
    {
        throw std::invalid_argument(
            "inputSDF and outputSDF have not same shape.");
    }

    Array3<double> sdfGrid(input.GetDataSize());
    auto pos = input.GetDataPosition();
    sdfGrid.ParallelForEachIndex([&](size_t i, size_t j, size_t k) {
        sdfGrid(i, j, k) = sdf.Sample(pos(i, j, k));
    });

    const Vector3D gridSpacing = input.GridSpacing();

    Array3<double> u(input.GetDataSize());
    Array3<double> u0(input.GetDataSize());
    Array3<double> v(input.GetDataSize());
    Array3<double> v0(input.GetDataSize());
    Array3<double> w(input.GetDataSize());
    Array3<double> w0(input.GetDataSize());

    input.ParallelForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        u(i, j, k) = input(i, j, k).x;
        v(i, j, k) = input(i, j, k).y;
        w(i, j, k) = input(i, j, k).z;
    });

    Extrapolate(u, sdfGrid.ConstAccessor(), gridSpacing, maxDistance, u0);
    Extrapolate(v, sdfGrid.ConstAccessor(), gridSpacing, maxDistance, v0);
    Extrapolate(w, sdfGrid.ConstAccessor(), gridSpacing, maxDistance, w0);

    output->ParallelForEachDataPointIndex([&](size_t i, size_t j, size_t k) {
        (*output)(i, j, k).x = u(i, j, k);
        (*output)(i, j, k).y = v(i, j, k);
        (*output)(i, j, k).z = w(i, j, k);
    });
}

void IterativeLevelSetSolver3::Extrapolate(const FaceCenteredGrid3& input,
                                           const ScalarField3& sdf,
                                           double maxDistance,
                                           FaceCenteredGrid3* output)
{
    if (!input.HasSameShape(*output))
    {
        throw std::invalid_argument(
            "inputSDF and outputSDF have not same shape.");
    }

    const Vector3D gridSpacing = input.GridSpacing();

    auto u = input.GetUConstAccessor();
    auto uPos = input.GetUPosition();
    Array3<double> sdfAtU(u.size());
    input.ParallelForEachUIndex([&](size_t i, size_t j, size_t k) {
        sdfAtU(i, j, k) = sdf.Sample(uPos(i, j, k));
    });

    Extrapolate(u, sdfAtU, gridSpacing, maxDistance, output->GetUAccessor());

    auto v = input.GetVConstAccessor();
    auto vPos = input.GetVPosition();
    Array3<double> sdfAtV(v.size());
    input.ParallelForEachVIndex([&](size_t i, size_t j, size_t k) {
        sdfAtV(i, j, k) = sdf.Sample(vPos(i, j, k));
    });

    Extrapolate(v, sdfAtV, gridSpacing, maxDistance, output->GetVAccessor());

    auto w = input.GetWConstAccessor();
    auto wPos = input.GetWPosition();
    Array3<double> sdfAtW(w.size());
    input.ParallelForEachWIndex([&](size_t i, size_t j, size_t k) {
        sdfAtW(i, j, k) = sdf.Sample(wPos(i, j, k));
    });

    Extrapolate(w, sdfAtW, gridSpacing, maxDistance, output->GetWAccessor());
}

void IterativeLevelSetSolver3::Extrapolate(
    const ConstArrayAccessor3<double>& input,
    const ConstArrayAccessor3<double>& sdf, const Vector3D& gridSpacing,
    double maxDistance, ArrayAccessor3<double> output)
{
    const Size3 size = input.size();

    ArrayAccessor3<double> outputAcc = output;

    const double dtau = PseudoTimeStep(sdf, gridSpacing);
    const unsigned int numberOfIterations =
        DistanceToNumberOfIterations(maxDistance, dtau);

    CopyRange3(input, size.x, size.y, size.z, &outputAcc);

    Array3<double> temp(size);
    ArrayAccessor3<double> tempAcc = temp.Accessor();

    for (unsigned int n = 0; n < numberOfIterations; ++n)
    {
        ParallelFor(
            ZERO_SIZE, size.x, ZERO_SIZE, size.y, ZERO_SIZE, size.z,
            [&](size_t i, size_t j, size_t k) {
                if (sdf(i, j, k) >= 0)
                {
                    std::array<double, 2> dx, dy, dz;
                    Vector3D grad = Gradient3(sdf, gridSpacing, i, j, k);

                    GetDerivatives(outputAcc, gridSpacing, i, j, k, &dx, &dy,
                                   &dz);

                    tempAcc(i, j, k) = outputAcc(i, j, k) -
                                       dtau * (std::max(grad.x, 0.0) * dx[0] +
                                               std::min(grad.x, 0.0) * dx[1] +
                                               std::max(grad.y, 0.0) * dy[0] +
                                               std::min(grad.y, 0.0) * dy[1] +
                                               std::max(grad.z, 0.0) * dz[0] +
                                               std::min(grad.z, 0.0) * dz[1]);
                }
                else
                {
                    tempAcc(i, j, k) = outputAcc(i, j, k);
                }
            });

        std::swap(tempAcc, outputAcc);
    }

    CopyRange3(outputAcc, size.x, size.y, size.z, &output);
}

double IterativeLevelSetSolver3::GetMaxCFL() const
{
    return m_maxCFL;
}

void IterativeLevelSetSolver3::SetMaxCFL(double newMaxCFL)
{
    m_maxCFL = std::max(newMaxCFL, 0.0);
}

unsigned int IterativeLevelSetSolver3::DistanceToNumberOfIterations(
    double distance, double dtau)
{
    return static_cast<unsigned int>(std::ceil(distance / dtau));
}

double IterativeLevelSetSolver3::Sign(const ConstArrayAccessor3<double>& sdf,
                                      const Vector3D& gridSpacing, size_t i,
                                      size_t j, size_t k)
{
    double d = sdf(i, j, k);
    double e = std::min({ gridSpacing.x, gridSpacing.y, gridSpacing.z });
    return d / std::sqrt(d * d + e * e);
}

double IterativeLevelSetSolver3::PseudoTimeStep(
    ConstArrayAccessor3<double> sdf, const Vector3D& gridSpacing) const
{
    const Size3 size = sdf.size();

    const double h = std::max({ gridSpacing.x, gridSpacing.y, gridSpacing.z });

    double maxS = -std::numeric_limits<double>::max();
    double dtau = m_maxCFL * h;

    for (size_t k = 0; k < size.z; ++k)
    {
        for (size_t j = 0; j < size.y; ++j)
        {
            for (size_t i = 0; i < size.x; ++i)
            {
                double s = Sign(sdf, gridSpacing, i, j, k);
                maxS = std::max(s, maxS);
            }
        }
    }

    while (dtau * maxS / h > m_maxCFL)
    {
        dtau *= 0.5;
    }

    return dtau;
}
}  // namespace CubbyFlow