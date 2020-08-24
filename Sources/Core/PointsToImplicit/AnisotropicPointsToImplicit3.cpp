// This code is based on Jet framework.
// Copyright (c) 2018 Doyub Kim
// CubbyFlow is voxel-based fluid simulation engine for computer games.
// Copyright (c) 2020 CubbyFlow Team
// Core Part: Chris Ohk, Junwoo Hwang, Jihong Sin, Seungwoo Yoo
// AI Part: Dongheon Cho, Minseo Kim
// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Core/Math/SVD.hpp>
#include <Core/Matrix/Matrix3x3.hpp>
#include <Core/PointsToImplicit/AnisotropicPointsToImplicit3.hpp>
#include <Core/SPH/SPHSystemData3.hpp>
#include <Core/Searcher/PointKdTreeSearcher3.hpp>
#include <Core/Solver/LevelSet/FMMLevelSetSolver3.hpp>
#include <Core/Utils/Logging.hpp>

namespace CubbyFlow
{
inline double P(double distance)
{
    const double distanceSquared = distance * distance;

    if (distanceSquared >= 1.0)
    {
        return 0.0;
    }

    const double x = 1.0 - distanceSquared;
    return x * x * x;
}

inline double Wij(double distance, double r)
{
    if (distance < r)
    {
        return 1.0 - Cubic(distance / r);
    }

    return 0.0;
}

inline Matrix3x3D Vvt(const Vector3D& v)
{
    return Matrix3x3D(v.x * v.x, v.x * v.y, v.x * v.z, v.y * v.x, v.y * v.y,
                      v.y * v.z, v.z * v.x, v.z * v.y, v.z * v.z);
}

inline double W(const Vector3D& r, const Matrix3x3D& g, double gDet)
{
    static const double sigma = 315.0 / (64 * PI_DOUBLE);
    return sigma * gDet * P((g * r).Length());
}

AnisotropicPointsToImplicit3::AnisotropicPointsToImplicit3(
    double kernelRadius, double cutOffDensity, double positionSmoothingFactor,
    size_t minNumNeighbors, bool isOutputSDF)
    : m_kernelRadius(kernelRadius),
      m_cutOffDensity(cutOffDensity),
      m_positionSmoothingFactor(positionSmoothingFactor),
      m_minNumNeighbors(minNumNeighbors),
      m_isOutputSDF(isOutputSDF)
{
    // Do nothing
}

void AnisotropicPointsToImplicit3::Convert(
    const ConstArrayAccessor1<Vector3D>& points, ScalarGrid3* output) const
{
    if (output == nullptr)
    {
        CUBBYFLOW_WARN << "Null scalar grid output pointer provided.";
        return;
    }

    const auto res = output->Resolution();
    if (res.x * res.y * res.z == 0)
    {
        CUBBYFLOW_WARN << "Empty grid is provided.";
        return;
    }

    const auto bbox = output->BoundingBox();
    if (bbox.IsEmpty())
    {
        CUBBYFLOW_WARN << "Empty domain is provided.";
        return;
    }

    const double h = m_kernelRadius;
    const double invH = 1 / h;
    const double r = 2.0 * h;

    // Mean estimator for cov. mat.
    const auto meanNeighborSearcher =
        PointKdTreeSearcher3::Builder().MakeShared();
    meanNeighborSearcher->Build(points);

    CUBBYFLOW_INFO << "Built neighbor searcher.";

    SPHSystemData3 meanParticles;
    meanParticles.AddParticles(points);
    meanParticles.SetNeighborSearcher(meanNeighborSearcher);
    meanParticles.SetKernelRadius(r);

    // Compute G and xMean
    std::vector<Matrix3x3D> gs(points.size());
    Array1<Vector3D> xMeans(points.size());

    ParallelFor(ZERO_SIZE, points.size(), [&](size_t i) {
        const auto& x = points[i];

        // Compute xMean
        Vector3D xMean;
        double wSum = 0.0;
        size_t numNeighbors = 0;
        const auto getXMean = [&](size_t, const Vector3D& xj) {
            const double wj = Wij((x - xj).Length(), r);
            wSum += wj;
            xMean += wj * xj;
            ++numNeighbors;
        };
        meanNeighborSearcher->ForEachNearbyPoint(x, r, getXMean);

        assert(wSum > 0.0);
        xMean /= wSum;

        xMeans[i] = Lerp(x, xMean, m_positionSmoothingFactor);

        if (numNeighbors < m_minNumNeighbors)
        {
            const auto g = Matrix3x3D::MakeScaleMatrix(invH, invH, invH);
            gs[i] = g;
        }
        else
        {
            // Compute covariance matrix
            // We start with small scale matrix (h*h) in order to
            // prevent zero covariance matrix when points are all
            // perfectly lined up.
            auto cov = Matrix3x3D::MakeScaleMatrix(h * h, h * h, h * h);
            wSum = 0.0;
            const auto getCov = [&](size_t, const Vector3D& xj) {
                const double wj = Wij((xMean - xj).Length(), r);
                wSum += wj;
                cov += wj * Vvt(xj - xMean);
            };
            meanNeighborSearcher->ForEachNearbyPoint(x, r, getCov);

            cov /= wSum;

            // SVD
            Matrix3x3D u;
            Vector3D v;
            Matrix3x3D w;
            SVD(cov, u, v, w);

            // Take off the sign
            v.x = std::fabs(v.x);
            v.y = std::fabs(v.y);
            v.z = std::fabs(v.z);

            // Constrain Sigma
            const double maxSingularVal = v.Max();
            const double kr = 4.0;
            v.x = std::max(v.x, maxSingularVal / kr);
            v.y = std::max(v.y, maxSingularVal / kr);
            v.z = std::max(v.z, maxSingularVal / kr);

            const auto invSigma = Matrix3x3D::MakeScaleMatrix(1.0 / v);

            // Compute G
            // Volume preservation
            const double scale = std::pow(v.x * v.y * v.z, 1.0 / 3.0);
            const Matrix3x3D g = invH * scale * (w * invSigma * u.Transposed());
            gs[i] = g;
        }
    });

    CUBBYFLOW_INFO << "Computed G and means.";

    // SPH estimator
    meanParticles.SetKernelRadius(h);
    meanParticles.UpdateDensities();
    const auto d = meanParticles.GetDensities();
    const double m = meanParticles.GetMass();

    PointKdTreeSearcher3 meanNeighborSearcher3;
    meanNeighborSearcher3.Build(xMeans);

    // Compute SDF
    auto temp = output->Clone();
    temp->Fill([&](const Vector3D& x) {
        double sum = 0.0;
        meanNeighborSearcher3.ForEachNearbyPoint(
            x, r, [&](size_t i, const Vector3D& neighborPosition) {
                sum += m / d[i] *
                       W(neighborPosition - x, gs[i], gs[i].Determinant());
            });

        return m_cutOffDensity - sum;
    });

    CUBBYFLOW_INFO << "Computed SDF.";

    if (m_isOutputSDF)
    {
        FMMLevelSetSolver3 solver;
        solver.Reinitialize(*temp, std::numeric_limits<double>::max(), output);
    }
    else
    {
        temp->Swap(output);
    }

    CUBBYFLOW_INFO << "Done converting points to implicit surface.";
}
}  // namespace CubbyFlow