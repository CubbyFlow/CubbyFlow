#include "pch.hpp"

#include <Core/Math/SVD.hpp>

using namespace CubbyFlow;

TEST(SVD, Float)
{
    MatrixMxNF a{ { 0, 1 }, { 1, 1 }, { 1, 0 } };

    MatrixMxNF u, v;
    VectorNF w;

    SVD(a, u, w, v);

    MatrixMxNF w2(2, 2);
    w2(0, 0) = w[0];
    w2(1, 1) = w[1];

    MatrixMxNF aApprox = u * w2 * v.Transposed();
    EXPECT_TRUE(a.IsSimilar(aApprox, 1e-6));
}

TEST(SVD, Double)
{
    MatrixMxND a{ { 0, 1 }, { 1, 1 }, { 1, 0 } };

    MatrixMxND u, v;
    VectorND w;

    SVD(a, u, w, v);

    MatrixMxND w2(2, 2);
    w2(0, 0) = w[0];
    w2(1, 1) = w[1];

    MatrixMxND aApprox = u * w2 * v.Transposed();
    EXPECT_TRUE(a.IsSimilar(aApprox, 1e-12));
}