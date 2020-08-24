#include "pch.hpp"

#include <Core/SPH/SPHStdKernel2.hpp>

using namespace CubbyFlow;

TEST(SPHStdKernel2, Constructors)
{
    SPHStdKernel2 kernel;
    EXPECT_DOUBLE_EQ(0.0, kernel.h);

    SPHStdKernel2 kernel2(3.0);
    EXPECT_DOUBLE_EQ(3.0, kernel2.h);
}

TEST(SPHStdKernel2, KernelFunction)
{
    SPHStdKernel2 kernel(10.0);

    double prevValue = kernel(0.0);

    for (int i = 1; i <= 10; ++i)
    {
        double value = kernel(static_cast<double>(i));
        EXPECT_LT(value, prevValue);
    }
}

TEST(SPHStdKernel2, FirstDerivative)
{
    const SPHStdKernel2 kernel{ 10.0 };

    const double value0 = kernel.FirstDerivative(0.0);
    const double value1 = kernel.FirstDerivative(5.0);
    const double value2 = kernel.FirstDerivative(10.0);
    EXPECT_DOUBLE_EQ(0.0, value0);
    EXPECT_DOUBLE_EQ(0.0, value2);

    // Compare with finite difference
    const double e = 0.001;
    const double fdm = (kernel(5.0 + e) - kernel(5.0 - e)) / (2.0 * e);
    EXPECT_NEAR(fdm, value1, 1e-10);
}

TEST(SPHStdKernel2, SecondDerivative)
{
    const SPHStdKernel2 kernel{ 10.0 };

    const double value0 = kernel.SecondDerivative(0.0);
    const double value1 = kernel.SecondDerivative(5.0);
    const double value2 = kernel.SecondDerivative(10.0);

    const double h4 = pow(10.0, 4.0);
    EXPECT_DOUBLE_EQ(-24.0 / (PI_DOUBLE * h4), value0);
    EXPECT_DOUBLE_EQ(0.0, value2);

    // Compare with finite difference
    const double e = 0.001;
    const double fdm =
        (kernel(5.0 + e) - 2.0 * kernel(5.0) + kernel(5.0 - e)) / (e * e);
    EXPECT_NEAR(fdm, value1, 1e-10);
}

TEST(SPHStdKernel2, Gradient)
{
    SPHStdKernel2 kernel(10.0);

    Vector2D value0 = kernel.Gradient(0.0, Vector2D(1, 0));
    EXPECT_DOUBLE_EQ(0.0, value0.x);
    EXPECT_DOUBLE_EQ(0.0, value0.y);

    Vector2D value1 = kernel.Gradient(5.0, Vector2D(0, 1));
    EXPECT_DOUBLE_EQ(0.0, value1.x);
    EXPECT_LT(0.0, value1.y);

    Vector2D value2 = kernel.Gradient(Vector2D(0, 5));
    EXPECT_EQ(value1, value2);
}

TEST(SPHSpikyKernel2, Constructors)
{
    SPHSpikyKernel2 kernel;
    EXPECT_DOUBLE_EQ(0.0, kernel.h);

    SPHSpikyKernel2 kernel2(3.0);
    EXPECT_DOUBLE_EQ(3.0, kernel2.h);
}

TEST(SPHSpikyKernel2, KernelFunction)
{
    SPHSpikyKernel2 kernel(10.0);

    double prevValue = kernel(0.0);

    for (int i = 1; i <= 10; ++i)
    {
        double value = kernel(static_cast<double>(i));
        EXPECT_LT(value, prevValue);
    }
}

TEST(SPHSpikyKernel2, FirstDerivative)
{
    const SPHSpikyKernel2 kernel{ 10.0 };

    const double value0 = kernel.FirstDerivative(0.0);
    const double value1 = kernel.FirstDerivative(5.0);
    const double value2 = kernel.FirstDerivative(10.0);
    EXPECT_LT(value0, value1);
    EXPECT_LT(value1, value2);

    // Compare with finite difference
    const double e = 0.001;
    const double fdm = (kernel(5.0 + e) - kernel(5.0 - e)) / (2.0 * e);
    EXPECT_NEAR(fdm, value1, 1e-10);
}

TEST(SPHSpikyKernel2, SecondDerivative)
{
    const SPHSpikyKernel2 kernel{ 10.0 };

    const double value0 = kernel.SecondDerivative(0.0);
    const double value1 = kernel.SecondDerivative(5.0);
    const double value2 = kernel.SecondDerivative(10.0);

    const double h4 = pow(10.0, 4.0);
    EXPECT_DOUBLE_EQ(60.0 / (PI_DOUBLE * h4), value0);
    EXPECT_DOUBLE_EQ(0.0, value2);

    // Compare with finite difference
    const double e = 0.001;
    const double fdm =
        (kernel(5.0 + e) - 2.0 * kernel(5.0) + kernel(5.0 - e)) / (e * e);
    EXPECT_NEAR(fdm, value1, 1e-10);
}

TEST(SPHSpikyKernel2, Gradient)
{
    SPHSpikyKernel2 kernel(10.0);

    Vector2D value0 = kernel.Gradient(0.0, Vector2D(1, 0));
    EXPECT_LT(0.0, value0.x);
    EXPECT_DOUBLE_EQ(0.0, value0.y);

    Vector2D value1 = kernel.Gradient(5.0, Vector2D(0, 1));
    EXPECT_DOUBLE_EQ(0.0, value1.x);
    EXPECT_LT(0.0, value1.y);

    Vector2D value2 = kernel.Gradient(Vector2D(0, 5));
    EXPECT_EQ(value1, value2);
}