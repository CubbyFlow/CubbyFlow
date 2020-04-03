#include "pch.hpp"

#include <Core/Math/MathUtils.hpp>

using namespace CubbyFlow;

TEST(MathUtils, GetBarycentric)
{
    double x = 3.2;
    ssize_t i;
    double t;

    // Simplest case
    GetBarycentric(x, 0, 10, &i, &t);
    EXPECT_EQ(3, i);
    EXPECT_NEAR(0.2, t, 1e-9);

    // Zero range
    x = 0.7;
    GetBarycentric(x, 0, 0, &i, &t);
    EXPECT_EQ(0, i);
    EXPECT_NEAR(0.0, t, 1e-9);

    // Small range
    x = 0.7;
    GetBarycentric(x, 0, 1, &i, &t);
    EXPECT_EQ(0, i);
    EXPECT_NEAR(0.7, t, 1e-9);

    // Funky range
    x = 3.2;
    GetBarycentric(x, -10, 0, &i, &t);
    EXPECT_EQ(-7, i);
    EXPECT_NEAR(0.2, t, 1e-9);

    // Funky range 2
    x = 3.2;
    GetBarycentric(x, -3, 7, &i, &t);
    EXPECT_EQ(0, i);
    EXPECT_NEAR(0.2, t, 1e-9);

    // On-the-boarder
    x = 10.0;
    GetBarycentric(x, 0, 10, &i, &t);
    EXPECT_EQ(9, i);
    EXPECT_NEAR(1.0, t, 1e-9);

    // On-the-boarder 2
    x = 0.0;
    GetBarycentric(x, 0, 10, &i, &t);
    EXPECT_EQ(0, i);
    EXPECT_NEAR(0.0, t, 1e-9);

    // Out-of-bound
    x = 10.1;
    GetBarycentric(x, 0, 10, &i, &t);
    EXPECT_EQ(9, i);
    EXPECT_NEAR(1.0, t, 1e-9);

    // Out-of-bound 2
    x = -0.1;
    GetBarycentric(x, 0, 10, &i, &t);
    EXPECT_EQ(0, i);
    EXPECT_NEAR(0.0, t, 1e-9);
}

TEST(MathUtils, Lerp)
{
    float a = 0.f;
    float b = 1.f;

    float result = Lerp(a, b, 0.3f);
    EXPECT_FLOAT_EQ(0.3f, result);
}

TEST(MathUtils, CatmullRom)
{
    float a = 0.f;
    float b = 0.f;
    float c = 1.f;
    float d = 1.f;

    float result = CatmullRom(a, b, c, d, 0.5f);
    EXPECT_FLOAT_EQ(0.5f, result);
}

TEST(MathUtils, MonotonicCatmullRom)
{
    float a = 0.f;
    float b = 0.f;
    float c = 1.f;
    float d = 1.f;

    for (int i = 0; i <= 10; ++i)
    {
        float result = MonotonicCatmullRom(a, b, c, d, i * 0.1f);
        EXPECT_TRUE(result >= b && result <= c);

        if (i == 0)
        {
            EXPECT_FLOAT_EQ(b, result);
        }
        else if (i == 10)
        {
            EXPECT_FLOAT_EQ(c, result);
        }
    }

    a = 0.f;
    b = 1.f;
    c = 2.f;
    d = 3.f;

    for (int i = 0; i <= 10; ++i)
    {
        float result = MonotonicCatmullRom(a, b, c, d, i * 0.1f);
        EXPECT_TRUE(result >= b && result <= c);

        if (i == 0)
        {
            EXPECT_FLOAT_EQ(b, result);
        }
        else if (i == 10)
        {
            EXPECT_FLOAT_EQ(c, result);
        }
    }

    a = 0.f;
    b = 1.f;
    c = 2.f;
    d = 0.f;

    for (int i = 0; i <= 10; ++i)
    {
        float result = MonotonicCatmullRom(a, b, c, d, i * 0.1f);
        EXPECT_TRUE(result >= b && result <= c);

        if (i == 0)
        {
            EXPECT_FLOAT_EQ(b, result);
        }
        else if (i == 10)
        {
            EXPECT_FLOAT_EQ(c, result);
        }
    }

    a = 0.f;
    b = 2.f;
    c = 1.f;
    d = 3.f;

    for (int i = 0; i <= 10; ++i)
    {
        float result = MonotonicCatmullRom(a, b, c, d, i * 0.1f);
        EXPECT_TRUE(result >= c && result <= b);

        if (i == 0)
        {
            EXPECT_FLOAT_EQ(b, result);
        }
        else if (i == 10)
        {
            EXPECT_FLOAT_EQ(c, result);
        }
    }
}