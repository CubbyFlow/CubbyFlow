#include "pch.hpp"

#include <Core/Matrix/Matrix.hpp>

using namespace CubbyFlow;

TEST(VectorN, Constructors)
{
    VectorND empty;
    EXPECT_EQ(0u, empty.GetRows());

    VectorND vec({ 1.0, 4.0, 3.0, -5.0, 2.0 });

    EXPECT_EQ(5u, vec.GetRows());
    EXPECT_EQ(1.0, vec[0]);
    EXPECT_EQ(4.0, vec[1]);
    EXPECT_EQ(3.0, vec[2]);
    EXPECT_EQ(-5.0, vec[3]);
    EXPECT_EQ(2.0, vec[4]);

    VectorND vec2(vec);

    EXPECT_EQ(5u, vec2.GetRows());
    EXPECT_EQ(1.0, vec2[0]);
    EXPECT_EQ(4.0, vec2[1]);
    EXPECT_EQ(3.0, vec2[2]);
    EXPECT_EQ(-5.0, vec2[3]);
    EXPECT_EQ(2.0, vec2[4]);

    VectorND vec3(7, 3.14);
    EXPECT_EQ(7u, vec3.GetRows());
    for (int i = 0; i < 7; ++i)
    {
        EXPECT_EQ(3.14, vec3[i]);
    }
}

TEST(VectorN, BasicSetters)
{
    VectorND vec;

    vec.Resize(8, 3.0);
    EXPECT_EQ(8u, vec.GetRows());
    for (int i = 0; i < 8; ++i)
    {
        EXPECT_EQ(3.0, vec[i]);
    }

    vec = { 1.0, 4.0, 3.0, -5.0, 2.0 };

    EXPECT_EQ(5u, vec.GetRows());
    EXPECT_EQ(1.0, vec[0]);
    EXPECT_EQ(4.0, vec[1]);
    EXPECT_EQ(3.0, vec[2]);
    EXPECT_EQ(-5.0, vec[3]);
    EXPECT_EQ(2.0, vec[4]);

    VectorND vec2(3, 4.0);

    vec2 = vec;
    EXPECT_EQ(5u, vec2.GetRows());
    EXPECT_EQ(1.0, vec2[0]);
    EXPECT_EQ(4.0, vec2[1]);
    EXPECT_EQ(3.0, vec2[2]);
    EXPECT_EQ(-5.0, vec2[3]);
    EXPECT_EQ(2.0, vec2[4]);

    VectorND vec3(7, 3.14);
    vec2.Swap(vec3);

    for (int i = 0; i < 7; ++i)
    {
        EXPECT_EQ(3.14, vec2[i]);
    }
    EXPECT_EQ(5u, vec3.GetRows());
    EXPECT_EQ(1.0, vec3[0]);
    EXPECT_EQ(4.0, vec3[1]);
    EXPECT_EQ(3.0, vec3[2]);
    EXPECT_EQ(-5.0, vec3[3]);
    EXPECT_EQ(2.0, vec3[4]);

    vec3.Fill(0.0);
    for (size_t i = 0; i < vec3.GetRows(); ++i)
    {
        EXPECT_EQ(0.0, vec3[i]);
    }

    vec3.Fill(vec);
    vec3.Normalize();
    double denom = std::sqrt(55.0);
    EXPECT_EQ(1.0 / denom, vec3[0]);
    EXPECT_EQ(4.0 / denom, vec3[1]);
    EXPECT_EQ(3.0 / denom, vec3[2]);
    EXPECT_EQ(-5.0 / denom, vec3[3]);
    EXPECT_EQ(2.0 / denom, vec3[4]);
}

TEST(VectorN, BasicGetters)
{
    VectorND vecA = { +3.0, -1.0, +2.0, 5.0 };

    EXPECT_EQ(4u, vecA.GetRows());

    const double* data = vecA.data();
    EXPECT_EQ(3.0, data[0]);
    EXPECT_EQ(-1.0, data[1]);
    EXPECT_EQ(2.0, data[2]);
    EXPECT_EQ(5.0, data[3]);

    double* data2 = vecA.data();
    data2[0] = 6.0;
    data2[1] = 2.5;
    data2[2] = -9.0;
    data2[3] = 8.0;
    EXPECT_EQ(+6.0, vecA[0]);
    EXPECT_EQ(+2.5, vecA[1]);
    EXPECT_EQ(-9.0, vecA[2]);
    EXPECT_EQ(+8.0, vecA[3]);

    const auto iter = vecA.begin();
    EXPECT_EQ(+6.0, iter[0]);
    EXPECT_EQ(+2.5, iter[1]);
    EXPECT_EQ(-9.0, iter[2]);
    EXPECT_EQ(+8.0, iter[3]);

    vecA = { +3.0, -1.0, +2.0, 5.0 };
    auto iter2 = vecA.begin();
    iter2[0] = 6.0;
    iter2[1] = 2.5;
    iter2[2] = -9.0;
    iter2[3] = 8.0;
    EXPECT_EQ(+6.0, iter2[0]);
    EXPECT_EQ(+2.5, iter2[1]);
    EXPECT_EQ(-9.0, iter2[2]);
    EXPECT_EQ(+8.0, iter2[3]);

    auto d = vecA.end() - vecA.begin();
    EXPECT_EQ(4, d);

    EXPECT_EQ(7.5, vecA.Sum());
    EXPECT_EQ(7.5 / 4.0, vecA.Avg());
    EXPECT_EQ(-9.0, vecA.Min());
    EXPECT_EQ(8.0, vecA.Max());
    EXPECT_EQ(2.5, vecA.AbsMin());
    EXPECT_EQ(-9.0, vecA.AbsMax());
    EXPECT_EQ(2u, vecA.DominantAxis());
    EXPECT_EQ(1u, vecA.SubdominantAxis());

    auto vecB = vecA;
    VectorND vecC = vecB.Normalized();
    vecA.Normalize();
    for (size_t i = 0; i < vecA.GetRows(); ++i)
    {
        EXPECT_EQ(vecA[i], vecC[i]);
    }

    vecA[0] = 6.0;
    vecA[1] = 2.5;
    vecA[2] = -9.0;
    vecA[3] = 8.0;
    double lenSqr = vecA.LengthSquared();
    EXPECT_EQ(187.25, lenSqr);

    double len = vecA.Length();
    EXPECT_EQ(std::sqrt(187.25), len);

    vecA = { +3.0, -1.0, +2.0, 5.0 };
    vecB = { +6.0, +2.5, -9.0, 8.0 };
    double distSq = vecA.DistanceSquaredTo(vecB);
    EXPECT_EQ(151.25, distSq);

    double dist = vecA.DistanceTo(vecB);
    EXPECT_EQ(std::sqrt(151.25), dist);

    VectorNF vecD = vecA.CastTo<float>();
    EXPECT_EQ(+3.f, vecD[0]);
    EXPECT_EQ(-1.f, vecD[1]);
    EXPECT_EQ(+2.f, vecD[2]);
    EXPECT_EQ(+5.f, vecD[3]);

    EXPECT_NE(vecA, vecB);
    vecB = vecA;
    EXPECT_EQ(vecA, vecB);

    vecB[0] += 1e-8;
    vecB[1] -= 1e-8;
    vecB[2] += 1e-8;
    vecB[3] -= 1e-8;
    EXPECT_NE(vecA, vecB);
    EXPECT_TRUE(vecA.IsSimilar(vecB, 1e-7));
}

TEST(VectorN, BinaryOperators)
{
    VectorND vecA = { +3.0, -1.0, +2.0, 5.0 };
    VectorND vecB = { +6.0, +2.5, -9.0, 8.0 };
    VectorND vecC = vecA + vecB;

    EXPECT_EQ(+9.0, vecC[0]);
    EXPECT_EQ(+1.5, vecC[1]);
    EXPECT_EQ(-7.0, vecC[2]);
    EXPECT_EQ(13.0, vecC[3]);

    vecC = vecA + 3.0;
    EXPECT_EQ(+6.0, vecC[0]);
    EXPECT_EQ(+2.0, vecC[1]);
    EXPECT_EQ(+5.0, vecC[2]);
    EXPECT_EQ(+8.0, vecC[3]);

    vecC = 2.0 + vecA;
    EXPECT_EQ(+5.0, vecC[0]);
    EXPECT_EQ(+1.0, vecC[1]);
    EXPECT_EQ(+4.0, vecC[2]);
    EXPECT_EQ(+7.0, vecC[3]);

    vecC = vecA - vecB;
    EXPECT_EQ(-3.0, vecC[0]);
    EXPECT_EQ(-3.5, vecC[1]);
    EXPECT_EQ(11.0, vecC[2]);
    EXPECT_EQ(-3.0, vecC[3]);

    vecC = 6.0 - vecA;
    EXPECT_EQ(+3.0, vecC[0]);
    EXPECT_EQ(+7.0, vecC[1]);
    EXPECT_EQ(+4.0, vecC[2]);
    EXPECT_EQ(+1.0, vecC[3]);

    vecC = vecA - 4.0;
    EXPECT_EQ(-1.0, vecC[0]);
    EXPECT_EQ(-5.0, vecC[1]);
    EXPECT_EQ(-2.0, vecC[2]);
    EXPECT_EQ(+1.0, vecC[3]);

    vecC = ElemMul(vecA, vecB);
    EXPECT_EQ(18.0, vecC[0]);
    EXPECT_EQ(-2.5, vecC[1]);
    EXPECT_EQ(-18.0, vecC[2]);
    EXPECT_EQ(40.0, vecC[3]);

    vecC = vecA * 2.0;
    EXPECT_EQ(+6.0, vecC[0]);
    EXPECT_EQ(-2.0, vecC[1]);
    EXPECT_EQ(+4.0, vecC[2]);
    EXPECT_EQ(10.0, vecC[3]);

    vecC = ElemDiv(vecA, vecB);
    EXPECT_EQ(+0.5, vecC[0]);
    EXPECT_EQ(-0.4, vecC[1]);
    EXPECT_EQ(-2.0 / 9.0, vecC[2]);
    EXPECT_EQ(0.625, vecC[3]);

    vecC = vecA / 0.5;
    EXPECT_EQ(+6.0, vecC[0]);
    EXPECT_EQ(-2.0, vecC[1]);
    EXPECT_EQ(+4.0, vecC[2]);
    EXPECT_EQ(10.0, vecC[3]);

    vecC = 2.0 / vecA;
    EXPECT_EQ(+2.0 / 3.0, vecC[0]);
    EXPECT_EQ(-2.0, vecC[1]);
    EXPECT_EQ(+1.0, vecC[2]);
    EXPECT_EQ(+0.4, vecC[3]);

    vecC = 3.0 / (0.5 * vecA + 2.0 * vecB);
    EXPECT_EQ(3.0 / 13.5, vecC[0]);
    EXPECT_EQ(2.0 / 3.0, vecC[1]);
    EXPECT_EQ(3.0 / -17.0, vecC[2]);
    EXPECT_EQ(3.0 / 18.5, vecC[3]);
}